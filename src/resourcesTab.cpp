#include "resourcesTab.h"
#include <QVBoxLayout>
#include <QQmlContext>

// Init Resource Tab
ResourcesTab::ResourcesTab(QWidget *parent) : QWidget(parent) {
  // backend data source
  monitor = new ResourceMonitor(this);

  // CPU graph
  cpuGraph->setReziseMode(QQuickWidget::SizeRootObjectToView);
  cpuGraph->rootContext()->setContextProperty("resourceMonitor", resourceMonitor);
  cpuGraph->setSource(QUrl("qrc:/qml/cpuGraph.qml"));

  // load resource tab layout
  setResourcesLayout();
}

// destructor
ResourcesTab::~ResourcesTab(){}

// Set resources tab layout
void ResourcesTab::setResourcesLayout() {
  // layout
  QVBoxLayout *resourceslayout = new QVBoxLayout(this);

  // CPU group box
  cpuBox = new QGroupBox("CPU");
  cpuBox->setCheckable(true);
  cpuBox->setChecked(true);

  // Memory/Swap group box
  memSwapBox = new QGroupBox("Memory and Swap");
  memSwapBox->setCheckable(true);
  memSwapBox->setChecked(false);

  // Network group box
  QGroupBox *networkBox = newQGroupBox("Network");
  networkBox->setCheckable(true);
  networkBox->setChecked(false);

  // Disk group box
  QGroupBox *diskBox = newQGroupBox("Disk");
  diskBox->setCheckable(true);
  diskBox->setChecked(false);


  // CPU layout
  QVBoxLayout *cpuLayout = new QVBoxLayout(this);
  cpuLayout->addWidget(cpuGraph); // add graph widget to CPU layout
  cpuBox->setLayout(cpuLayout);  // set CPU box layout
 

  // add group boxes to resources tab main layout
  layout->addWidget(cpuBox);       
  layout->addWidget(memSwapBox);
  layout->addWidget(networkBox);
  layout->addWidget(diskBox);

  setLayout(layout);
}
