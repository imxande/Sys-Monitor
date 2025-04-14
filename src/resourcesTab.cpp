#include "resourcesTab.h"
#include <QVBoxLayout>
#include <QQmlContext>
#include <QUrl>
#include <QDebug>

// Init Resource Tab
ResourcesTab::ResourcesTab(QWidget *parent) : QWidget(parent) {
  // backend data source
  monitor = new ResourceMonitor(this);

  // CPU graph
  cpuGraph = new QQuickWidget(this);
  cpuGraph->setResizeMode(QQuickWidget::SizeRootObjectToView);
  cpuGraph->rootContext()->setContextProperty("resourceMonitor", monitor);
  QUrl qmlURL("qrc:/qml/cpuGraph.qml");
  cpuGraph->setSource(qmlURL);

  // sanity check
  if (cpuGraph->status() != QQuickWidget::Ready) {
    qDebug() << "QML failed to load!" << cpuGraph->errors();
  }

  // load resource tab layout
  setResourcesLayout();
}

// destructor
ResourcesTab::~ResourcesTab(){}

// Set resources tab layout
void ResourcesTab::setResourcesLayout() {
  // layout
  QVBoxLayout *resourcesLayout = new QVBoxLayout();

  // CPU group box
  cpuBox = new QGroupBox("CPU");
  cpuBox->setCheckable(true);
  cpuBox->setChecked(true);

  // Memory/Swap group box
  memSwapBox = new QGroupBox("Memory and Swap");
  memSwapBox->setCheckable(true);
  memSwapBox->setChecked(false);

  // Network group box
  QGroupBox *networkBox = new QGroupBox("Network");
  networkBox->setCheckable(true);
  networkBox->setChecked(false);

  // Disk group box
  QGroupBox *diskBox = new QGroupBox("Disk");
  diskBox->setCheckable(true);
  diskBox->setChecked(false);


  // CPU layout
  QVBoxLayout *cpuLayout = new QVBoxLayout(this);
  cpuLayout->addWidget(cpuGraph); // add graph widget to CPU layout
  cpuBox->setLayout(cpuLayout);  // set CPU box layout
 

  // add group boxes to resources tab main layout
  resourcesLayout->addWidget(cpuBox);       
  resourcesLayout->addWidget(memSwapBox);
  resourcesLayout->addWidget(networkBox);
  resourcesLayout->addWidget(diskBox);

  setLayout(resourcesLayout);
}
