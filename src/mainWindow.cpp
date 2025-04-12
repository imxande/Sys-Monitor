#include "fileSystemsTab.h"
#include "mainWindow.h"
#include "processesTab.h"
#include "resourcesTab.h"
#include <QScreen>
#include <QTabWidget>
#include <QVBoxLayout>

// init Main Window
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  // load main window config
  configUI();

  // set layout
  setLayout();
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

// Layout setup
void MainWindow::setLayout() {
  // create central widget
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  // create and set main layout
  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

  // create tabs
  QTabWidget *tabs = new QTabWidget(centralWidget);
  QWidget *processTab = new ProcessesTab(this);
  QWidget *resourcesTab = new ResourcesTab(this);
  QWidget *fileSystemsTab = new FileSystemsTab(this);

  // add each tab widget
  tabs->addTab(processTab, "Processes");
  tabs->addTab(resourcesTab, "Resources");
  tabs->addTab(fileSystemsTab, "File Systems");

  // add tabs to main layout
  mainLayout->addWidget(tabs);
}
