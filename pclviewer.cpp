#include "pclviewer.h"
#include "ui_pclviewer.h"

#include <QtConcurrent/QtConcurrent>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#if VTK_MAJOR_VERSION > 8
#include <vtkGenericOpenGLRenderWindow.h>
#endif

PCLViewer::PCLViewer(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::PCLViewer)
{
  ui->setupUi(this);
  this->setWindowTitle("PCL viewer");

  // Setup the cloud pointer
  cloud.reset(new PointCloudT);
  // The number of points in the cloud
  cloud->resize(200);

  // Fill the cloud with some points
  for (auto& point : *cloud)
  {
    point.x = 1024 * rand() / (RAND_MAX + 1.0f);
    point.y = 1024 * rand() / (RAND_MAX + 1.0f);
    point.z = 1024 * rand() / (RAND_MAX + 1.0f);

    point.r = 128;
    point.g = 128;
    point.b = 128;
  }

  // Set up the QVTK window
#if VTK_MAJOR_VERSION > 8
  auto renderer = vtkSmartPointer<vtkRenderer>::New();
  auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  viewer.reset(new pcl::visualization::PCLVisualizer(
      renderer, renderWindow, "viewer", false));
  ui->qvtkWidget->setRenderWindow(viewer->getRenderWindow());
  viewer->setupInteractor(ui->qvtkWidget->interactor(),
                          ui->qvtkWidget->renderWindow());
#else
  viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
  ui->qvtkWidget->SetRenderWindow(viewer->getRenderWindow());
  viewer->setupInteractor(ui->qvtkWidget->GetInteractor(),
                          ui->qvtkWidget->GetRenderWindow());
#endif

  // // Connect "random" button and the function
  // connect(ui->pushButton_random,
  //         SIGNAL(clicked()),
  //         this,
  //         SLOT(randomButtonPressed()));

  // // Connect R,G,B sliders and their functions
  // connect(ui->horizontalSlider_R,
  //         SIGNAL(valueChanged(int)),
  //         this,
  //         SLOT(redSliderValueChanged(int)));
  // connect(ui->horizontalSlider_G,
  //         SIGNAL(valueChanged(int)),
  //         this,
  //         SLOT(greenSliderValueChanged(int)));
  // connect(ui->horizontalSlider_B,
  //         SIGNAL(valueChanged(int)),
  //         this,
  //         SLOT(blueSliderValueChanged(int)));
  // connect(ui->horizontalSlider_R,
  //         SIGNAL(sliderReleased()),
  //         this,
  //         SLOT(RGBsliderReleased()));
  // connect(ui->horizontalSlider_G,
  //         SIGNAL(sliderReleased()),
  //         this,
  //         SLOT(RGBsliderReleased()));
  // connect(ui->horizontalSlider_B,
  //         SIGNAL(sliderReleased()),
  //         this,
  //         SLOT(RGBsliderReleased()));

  // // Connect point size slider
  // connect(ui->horizontalSlider_p,
  //         SIGNAL(valueChanged(int)),
  //         this,
  //         SLOT(pSliderValueChanged(int)));

  viewer->addPointCloud(cloud, "cloud");
  // pSliderValueChanged(2);
  viewer->resetCamera();
  viewer->setPointCloudRenderingProperties(
      pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "cloud");

  refresh_view();
}

// void PCLViewer::randomButtonPressed()
// {
//   printf("Random button was pressed\n");

//   // Set the new color
//   for (auto& point : *cloud)
//   {
//     point.r = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
//     point.g = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
//     point.b = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
//   }

//   viewer->updatePointCloud(cloud, "cloud");
//   refresh_view();
// }

// void PCLViewer::RGBsliderReleased()
// {
//   // Set the new color
//   for (auto& point : *cloud)
//   {
//     point.r = red;
//     point.g = green;
//     point.b = blue;
//   }
//   viewer->updatePointCloud(cloud, "cloud");
//   refresh_view();
// }

// void PCLViewer::pSliderValueChanged(int value)
// {
//   viewer->setPointCloudRenderingProperties(
//       pcl::visualization::PCL_VISUALIZER_POINT_SIZE, value, "cloud");
//   refresh_view();
// }

void PCLViewer::refresh_view()
{
#if VTK_MAJOR_VERSION > 8
  ui->qvtkWidget->renderWindow()->Render();
#else
  ui->qvtkWidget->update();
#endif
}

// void PCLViewer::redSliderValueChanged(int value)
// {
//   red = value;
//   printf("redSliderValueChanged: [%d|%d|%d]\n", red, green, blue);
// }

// void PCLViewer::greenSliderValueChanged(int value)
// {
//   green = value;
//   printf("greenSliderValueChanged: [%d|%d|%d]\n", red, green, blue);
// }

// void PCLViewer::blueSliderValueChanged(int value)
// {
//   blue = value;
//   printf("blueSliderValueChanged: [%d|%d|%d]\n", red, green, blue);
// }

PCLViewer::~PCLViewer() { delete ui; }

void PCLViewer::on_openFolderPushButton_clicked()
{
  QSettings settings(QSettings::Scope::UserScope);

  auto dir =
      QFileDialog::getExistingDirectory(this,
                                        "选择日志所在的文件夹",
                                        settings.value("openLogDir").toString(),
                                        QFileDialog::Option::ShowDirsOnly);
  if (dir.isEmpty())
  {
    return;
  }
  setProperty("dir", dir);
  qInfo() << "selected folder: " << dir;

  settings.setValue("openLogDir", dir);

  auto logs = QDir(dir).entryList({"*.log"});
  qInfo() << "find logs: " << logs;

  ui->fileComboBox->addItems(logs);
  ui->fileComboBox->setCurrentIndex(0);
}

void PCLViewer::on_fileComboBox_currentTextChanged(const QString& filename)
{
  using ReadFileResult = std::pair<bool, QString>;
  auto watcher = QSharedPointer<QFutureWatcher<ReadFileResult>>::create();
  auto future = QtConcurrent::run(
      [=]() -> ReadFileResult
      {
        const auto& dir = property("dir").toString();
        if (dir.isEmpty())
        {
          qCritical() << "this is a bugs, property(dir) empty";
          return {false, "this is a bug, property(dir) empty"};
        }
        QFile fin(QStringLiteral("%1/%2").arg(dir, filename));
        if (!fin.open(QFile::OpenModeFlag::ReadOnly))
        {
          qWarning() << QStringLiteral("read error: %1 -> %2")
                            .arg(fin.fileName(), fin.errorString());
          return {false, fin.errorString()};
        }
        QTextStream stream(&fin);
        auto data = stream.readAll();
        qInfo() << QStringLiteral("open %1 bytes in file: %2")
                       .arg(data.size())
                       .arg(filename);
        return {true, data};
      });
  watcher->setFuture(future);

  ui->plainTextEdit->setPlaceholderText("reading, please wait..");
  connect(watcher.get(),
          &QFutureWatcher<ReadFileResult>::finished,
          this,
          [=]()
          {
            auto [success, data] = watcher->result();
            qInfo() << QStringLiteral("plainTextEdit received %1 bytes")
                           .arg(data.size());
            if (!success)
            {
              ui->plainTextEdit->setPlainText(nullptr);
              ui->plainTextEdit->setPlaceholderText(data);
            }
            else if (data.isEmpty())
            {
              ui->plainTextEdit->setPlainText(nullptr);
              ui->plainTextEdit->setPlaceholderText("this file is empty..");
            }
            else
            {
              ui->plainTextEdit->setPlainText(data);
            }
          });
}
