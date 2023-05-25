#pragma once

#include <iostream>

// Qt
#include <QMainWindow>

// Point Cloud Library
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

namespace Ui
{
class PCLViewer;
}

class PCLViewer : public QMainWindow
{
  Q_OBJECT

 public:
  explicit PCLViewer(QWidget *parent = 0);
  virtual ~PCLViewer();

 private slots:
  void on_openFolderPushButton_clicked();

     void on_fileComboBox_currentTextChanged(const QString &arg1);

 protected:
  void refresh_view();

  pcl::visualization::PCLVisualizer::Ptr viewer;
  PointCloudT::Ptr cloud;
  Ui::PCLViewer *ui;
};
