#include "MainWindow.h"
#include <QScreen>

// init Main Window
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // load main window config
  configUI();
}

// destructor
MainWindow::~MainWindow(){};

// UI configurations
void MainWindow::configUI() {
  // OS screen resolution
  QScreen *screen = QGuiApplication::primaryScreen();

  // load OS screen size as the window size
  if (screen) {
    QSize resolution = screen->geometry().size();
    int w = resolution.width();
    int h = resolution.height();
    resize(w, h);
  }

  // name of window
  setWindowTitle("System Monitor");
}
