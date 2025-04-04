#include "mainWindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
  // create app
  QApplication app(argc, argv);

  // create main window
  MainWindow monitor;
  monitor.show();

  return app.exec();
}
