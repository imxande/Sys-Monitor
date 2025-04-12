#include "resourceMonitor.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>

// Init resource monitor
ResourceMonitor::ResourceMonitor(QObject *parent) : QObject(parent) {
  // create timer
  updateTimer = new QTimer(this);

  // connect timeout signal to approptiate slot
  connect(updateTimer, &QTimer::timeout, this,
          &ResourceMonitor::updateCpuUsage);

  // start timer for 1s intervals
  updateTimer->start(1000);

  // sanity check
  readCpuUsage();
}

// destructor
ResourceMonitor::~ResourceMonitor() {}

// Get CPU usage
float ResourceMonitor::getCpuUsage() { return cpuUsage; }

// Update CPU usage
void ResourceMonitor::updateCpuUsage() {
  // read CPU usage stats
  // calculate actual CPU percentage
  // update cpuUsage member
  // emit signal if value changed
  qDebug() << "Hello from updateCpuUsage";
}

// Read CPU time stats
QPair<qulonglong, qulonglong> ResourceMonitor::readCpuUsage() {
  // placeholders
  QFile statFile("/proc/stat");
  QTextStream in(&statFile);
  QStringList tokens;
  qulonglong total;
  qulonglong idleTotal;

  if (!statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Error reading " + statFile.fileName();
    return {};
  }

  // read from proc/stat
  QString line = in.readLine();

  if (!line.isEmpty()) {
    tokens = line.split(" ", Qt::SkipEmptyParts);
    if (tokens.size() >= 11) {
        // usage stats
        qulonglong user = tokens[1].toULongLong();
        qulonglong nice = tokens[2].toULongLong();
        qulonglong system = tokens[3].toULongLong();
        qulonglong idle = tokens[4].toULongLong();
        qulonglong iowait = tokens[5].toULongLong();
        qulonglong irq = tokens[6].toULongLong();
        qulonglong softirq = tokens[7].toULongLong();
        qulonglong steal = tokens[8].toULongLong();
        // calculate total
        total = user + nice + system + idle + iowait + irq + softirq + steal;

        // calculate idleTotal
        idleTotal = idle + iowait;
      }
  }

qDebug() << "Total: " << total << ", idleTotal: " << idleTotal;

return {total, idleTotal};
}
