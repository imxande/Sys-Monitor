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
}

// destructor
ResourceMonitor::~ResourceMonitor() {}

// Get CPU usage
float ResourceMonitor::getCpuUsage() { return cpuUsage; }

// Update CPU usage
void ResourceMonitor::updateCpuUsage() {
  // placeholders
  qulonglong deltaTotal;
  qulonglong deltaIdle;

  // read CPU usage stats
  auto [totalNow, idleNow] = readCpuUsage();

  // compute deltas
  if (prevTotal != 0 && prevIdle != 0) {
    deltaTotal = totalNow - prevTotal;
    deltaIdle = idleNow - prevIdle;

    if (deltaTotal > 0) {
      float usage = 1.0f - (static_cast<float>(deltaIdle) / deltaTotal);

      // calculate actual CPU percentage
      cpuUsage = usage * 100.0f;

      // emit signal
      emit cpuUsageChanged();
      qDebug() << "CPU usage (%)" << cpuUsage;
    }
  }

  // capture previous usage
  prevTotal = totalNow;
  prevIdle = idleNow;
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

  // capture all usage into tokens
  if (!line.isEmpty()) {
    tokens = line.split(" ", Qt::SkipEmptyParts);

    // usage stats
    if (tokens.size() >= 11) {
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

  return {total, idleTotal};
}
