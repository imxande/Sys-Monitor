#include "resourceMonitor.h"
#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QString>
#include <QTextStream>

// Init resource monitor
ResourceMonitor::ResourceMonitor(QObject *parent) : QObject(parent) {
  // create timer
  updateTimer = new QTimer(this);

  // connect timeout signal to approptiate slot
  connect(updateTimer, &QTimer::timeout, this, [this]() {
    updateCpuUsage();
    updateMemoryUsage();
    updateNetworkRates();
  });

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

// Get Memory usage
float ResourceMonitor::getMemoryUsage() { return memoryUsage; }

void ResourceMonitor::updateMemoryUsage() {
  // placeholder
  QFile memFile("/proc/meminfo");

  // open file
  if (!memFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Error reading " << memFile.fileName();
    return;
  }

  // read file
  QTextStream in(&memFile);
  while (!in.atEnd()) {
    QString line = in.readLine();

    if (line.startsWith("MemTotal:"))
      memTotal = line.split(QRegularExpression("\\s+"))[1].toULongLong();

    else if (line.startsWith("MemAvailable:"))
      memAvailable = line.split(QRegularExpression("\\s+"))[1].toULongLong();

    if (memTotal && memAvailable)
      break;
  }

  if (memTotal > 0) {
    memoryUsage = 100.0f * (1.0 - (float(memAvailable) / memTotal));
    emit memoryUsageChanged();
  }
}

// Get rxRate
float ResourceMonitor::getRxRate() { return rxRate; }

// Get txRate
float ResourceMonitor::getTxRate() { return txRate; }

// Update network usage
void ResourceMonitor::updateNetworkRates() {
  // filename
  QFile netFile("/proc/net/dev");

  // open file
  if(!netFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Error reading" << netFile.fileName();
    return;
  }

  // read file
  QTextStream in(&netFile);
  QString line;

  while (!in.atEnd()) {
    line = in.readLine().trimmed();

    if (!line.contains(":")) continue;

    QStringList parts = line.split(QRegularExpression("[:\\s]+"), Qt::SkipEmptyParts);

    if (parts.size() >= 10) {
      totalRx += parts[1].toULongLong(); // bytes received
      totalTx += parts[1].toULongLong(); // bytes sent
    }
  }

  if (prevRx != 0 && prevTx != 0) {
    rxRate = (totalRx - prevRx); // bytes/sec
    txRate = (totalTx - prevTx);

    // emit signals
    emit rxRateChanged();
    emit txRateChanged();
  }

  prevRx = totalRx;
  prevTx = totalTx;


}
