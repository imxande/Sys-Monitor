#include "fileSystemInfo.h"
#include "mainWindow.h"
#include "processInfo.h"
#include <QApplication>
#include <QMetaType>

int main(int argc, char *argv[]) {
  /*
   * Register ProcessInfo struct in order for the Type
   * to be used in queued signal slots connection.
   */
  qRegisterMetaType<ProcessInfo>("ProcessInfo");
  qRegisterMetaType<QList<ProcessInfo>>("QList<ProcessInfo>");

  // create app
  QApplication app(argc, argv);

  // create main window
  MainWindow monitor;
  monitor.show();

  return app.exec();
}
