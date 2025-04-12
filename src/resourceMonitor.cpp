#include "resourceMonitor.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QString>

// Init resource monitor
ResourceMonitor::ResourceMonitor(QObject *parent) : QObject(parent) {
  // create timer
  updateTimer = new QTimer(this);
  
  // connect timeout signal to approptiate slot
  connect(updateTimer, &QTimer::timeout, this, );

  // start timer for 1s intervals
  updateTimer->start(1000);
 }

// destructor
ResourceMonitor::~ResourceMonitor(){}

// Get CPU usage
void ResourceMonitor::getCpuUsage() {
  return cpuUsage;
}

// Update CPU usage
void ResourceMonitor::updateCpuUsage() {
  // read CPU usage stats
  // calculate actual CPU percentage
  // update cpuUsage member
  // emit signal if value changed
}

// Read CPU time stats
QPair<qulonglong, qulonglong> ResourceMonitor::readCpuUsage() {
  // placeholders
  QFile statFile("/proc/stat");
  QTextStream in(&statFile);

  if (!statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
   qDebug() << "Error reading " + statFile.fileName(); 
    return {};
  }

  // read first line here of proc/stat
  QString line = in.readLine();
  
}
