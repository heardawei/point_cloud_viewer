#include <QApplication>
#include <QMainWindow>

#include "pclviewer.h"

int main (int argc, char *argv[])
{
  QApplication a (argc, argv);
  QCoreApplication::setOrganizationName("alibaba-inc");
  QCoreApplication::setApplicationName(argv[0]);
  PCLViewer w;
  w.show ();
  return a.exec ();
}
