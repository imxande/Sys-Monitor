#include "resourcesTab.h"
#include <QDebug>
#include <QFrame>
#include <QLabel>
#include <QQmlContext>
#include <QToolButton>
#include <QUrl>
#include <QVBoxLayout>

// Init Resource Tab
ResourcesTab::ResourcesTab(QWidget *parent) : QWidget(parent) {
  // backend data source
  monitor = new ResourceMonitor(this);

  // CPU graph
  cpuGraph = new QQuickWidget(this);
  cpuGraph->setResizeMode(QQuickWidget::SizeRootObjectToView);
  cpuGraph->rootContext()->setContextProperty("resourceMonitor", monitor);
  cpuGraph->setSource(QUrl("qrc:/qml/cpuGraph.qml"));

  // Memory and Swap graph
  memGraph = new QQuickWidget(this); 
  memGraph->setResizeMode(QQuickWidget::SizeRootObjectToView);
  memGraph->rootContext()->setContextProperty("resourceMonitor", monitor);
  memGraph->setSource(QUrl("qrc:/qml/memGraph.qml"));

  // Network graph
  networkGraph = new QQuickWidget(this);
  networkGraph->setResizeMode(QQuickWidget::SizeRootObjectToView);
  networkGraph->rootContext()->setContextProperty("resourceMonitor", monitor);
  networkGraph->setSource(QUrl("qrc:qml/networkGraph.qml"));

  // Disk graph
  diskGraph = new QQuickWidget(this);
  diskGraph->setResizeMode(QQuickWidget::SizeRootObjectToView);
  diskGraph->rootContext()->setContextProperty("resourceMonitor", monitor);
  diskGraph->setSource(QUrl("qrc:qml/diskGraph.qml"));



  // sanity check
  if (cpuGraph->status() != QQuickWidget::Ready) {
    qDebug() << "QML failed to load!" << cpuGraph->errors();
  } else if (memGraph->status() != QQuickWidget::Ready) {
    qDebug() << "Memory and Swap QML failed to load!" << memGraph->errors();
  }  else if (networkGraph->status() != QQuickWidget::Ready) {
    qDebug() << "Network QML failed to load!" << networkGraph->errors();
  }  else if (diskGraph->status() != QQuickWidget::Ready) {
    qDebug() << "Disk QML failed to load!" << diskGraph->errors();
  } 

  // load resource tab layout
  setResourcesLayout();
}

// destructor
ResourcesTab::~ResourcesTab() {}

// Set resources tab layout
void ResourcesTab::setResourcesLayout() {
  // placeholder
  QVBoxLayout *resourcesLayout = new QVBoxLayout;

  // Toogle buttons
  resourcesLayout->addWidget(createSection("CPU"));
  resourcesLayout->addWidget(createSection("Memory and Swap"));
  resourcesLayout->addWidget(createSection("Network"));
  resourcesLayout->addWidget(createSection("Disk"));

  // set layout
  setLayout(resourcesLayout);
}

// Creates collapsible section
QWidget *ResourcesTab::createSection(const QString &title) {
  // placeholder
  QWidget *section = new QWidget(this); // section widget

  // toggle button
  QToolButton *toggleButton = new QToolButton(section);
  toggleButton->setText(title);
  toggleButton->setCheckable(true);
  toggleButton->setChecked(true);
  toggleButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
  toggleButton->setArrowType(Qt::DownArrow);

  // collapsible content
  QFrame *content = new QFrame(section);
  content->setFrameShape(QFrame::StyledPanel);
  content->setVisible(true);

  // content layout
  QVBoxLayout *contentLayout = new QVBoxLayout(content);
  // set graphs
  if (title == "CPU") {
    contentLayout->addWidget(cpuGraph);
  } else if (title == "Memory and Swap") {
    contentLayout->addWidget(memGraph);
  } else if (title == "Network") {
    contentLayout->addWidget(networkGraph);
  } else {
    contentLayout->addWidget(diskGraph);
  }


  // collapse logic
  connect(toggleButton, &QToolButton::toggled, [=](bool checked) {
    content->setVisible(checked);
    toggleButton->setArrowType(checked ? Qt::DownArrow : Qt::RightArrow);
  });

  // section layout
  QVBoxLayout *sectionLayout = new QVBoxLayout(section);
  sectionLayout->setContentsMargins(0, 0, 0, 0);
  sectionLayout->addWidget(toggleButton);
  sectionLayout->addWidget(content);

  return section;
}
