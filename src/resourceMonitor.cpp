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
    qDebug() << "Timer triggered: updating resources"; // debug
    updateCpuUsage();
    updateCpuCoreUsages();
    updateMemoryUsage();
    updateNetworkRates();
    updateDiskStats();
  });

  // start timer for 1s intervals
  updateTimer->start(1000);
  QTimer::singleShot(
      3000, this, &ResourceMonitor::updateCpuCoreUsages); // one-time debug call
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
  qulonglong total = 0;
  qulonglong idleTotal = 0;

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
      memTotal = line.split(QRegularExpression("\\s+")).value(1).toULongLong();

    else if (line.startsWith("MemAvailable:"))
      memAvailable =
          line.split(QRegularExpression("\\s+")).value(1).toULongLong();

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
  if (!netFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Error reading" << netFile.fileName();
    return;
  }

  // read file
  QTextStream in(&netFile);
  QString line;

  while (!in.atEnd()) {
    line = in.readLine().trimmed();

    if (!line.contains(":"))
      continue;

    QStringList parts =
        line.split(QRegularExpression("[:\\s]+"), Qt::SkipEmptyParts);

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

// Get readRate
qulonglong ResourceMonitor::getReadRate() { return readRate; }

// Get writeRate
qulonglong ResourceMonitor::getWriteRate() { return writeRate; }

// Disk rate update method
void ResourceMonitor::updateDiskStats() {
  // open file
  QFile file("/proc/diskstats");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Error reading " << file.fileName();
  }

  // read file
  QTextStream in(&file);
  qulonglong totalReadSectors = 0;
  qulonglong totalWriteSectors = 0;

  while (!in.atEnd()) {
    QString line = in.readLine().trimmed();
    QStringList parts =
        line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    // field: major, minor, device, reads, write, etc, observed from cat
    // /proc/diskstats
    if (parts.size() > 14) {
      QString deviceName = parts[2];

      // skip loop, ram, or partitions(sda1, nvme0n1p1)
      if (deviceName.startsWith("loop") || deviceName.startsWith("ram") ||
          deviceName.contains("p"))
        continue;

      totalReadSectors += parts[5].toULongLong();
      totalWriteSectors += parts[9].toULongLong();
    }
  }

  // convert sectors to bytes (512 bytes per sector)
  const qulonglong sectorSize = 512;
  qulonglong totalReadBytes = totalReadSectors * sectorSize;
  qulonglong totalWriteBytes = totalWriteSectors * sectorSize;

  // compute data
  if (prevReadBytes > 0 && prevWriteBytes > 0) {
    readRate = totalReadBytes - prevReadBytes;
    writeRate = totalWriteBytes - prevWriteBytes;

    // emit signals
    emit readRateChanged();
    emit writeRateChanged();
  }

  // store next delta calculation
  prevReadBytes = totalReadBytes;
  prevWriteBytes = totalWriteBytes;
}

// Get CPU Core Usages
QVariantList ResourceMonitor::getCpuCoreUsages() const {
  QVariantList list;
  for (float usage : cpuCoreUsages) {
    list.append(usage);
  }
  return list;
}

// Read per core usage
QList<QPair<qulonglong, qulonglong>> ResourceMonitor::readPerCoreUsage() {
  QList<QPair<qulonglong, qulonglong>> perCoreData;
  QFile statFile("/proc/stat");

  if (!statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "ERROR: Could not open" << statFile.fileName() << "-"
             << statFile.errorString();
    return {};
  }

  QTextStream in(&statFile);
  int cpuIndex = -1;

  while (!in.atEnd()) {
    QString line = in.readLine().trimmed();
    qDebug() << "Line:" << line;

    if (line.startsWith("cpu")) {
      cpuIndex++;
      if (cpuIndex == 0)
        continue; // skip total "cpu" line

      QStringList tokens = line.split(" ", Qt::SkipEmptyParts);
      if (tokens.size() < 8)
        continue;

      qulonglong user = tokens[1].toULongLong();
      qulonglong nice = tokens[2].toULongLong();
      qulonglong system = tokens[3].toULongLong();
      qulonglong idle = tokens[4].toULongLong();
      qulonglong iowait = tokens[5].toULongLong();
      qulonglong irq = tokens[6].toULongLong();
      qulonglong softirq = tokens[7].toULongLong();
      qulonglong steal = tokens.value(8, "0").toULongLong();

      qulonglong total =
          user + nice + system + idle + iowait + irq + softirq + steal;
      qulonglong idleTotal = idle + iowait;

      perCoreData.append({total, idleTotal});
    }
  }

  qDebug() << "Parsed per-core data. Total cores:" << perCoreData.size();
  return perCoreData;
}

// update cpu core usages
void ResourceMonitor::updateCpuCoreUsages() {
  qDebug() << "updateCpuCoreUsages() called";
  QList<QPair<qulonglong, qulonglong>> coreData = readPerCoreUsage();

  if (coreData.isEmpty()) {
    qDebug() << "No core data found.";
    return;
  }

  if (prevTotalPerCore.size() != coreData.size()) {
    qDebug() << "Detected" << coreData.size() << "CPU cores";
    prevTotalPerCore = QVector<qulonglong>(coreData.size());
    prevIdlePerCore = QVector<qulonglong>(coreData.size());
    cpuCoreUsages = QList<float>(coreData.size(), 0.0f);
  }

  for (int i = 0; i < coreData.size(); ++i) {
    qulonglong totalNow = coreData[i].first;
    qulonglong idleNow = coreData[i].second;

    qulonglong deltaTotal = totalNow - prevTotalPerCore[i];
    qulonglong deltaIdle = idleNow - prevIdlePerCore[i];

    float usage = 0.0f;
    if (deltaTotal > 0)
      usage = (1.0f - static_cast<float>(deltaIdle) / deltaTotal) * 100.0f;

    cpuCoreUsages[i] = usage;

    prevTotalPerCore[i] = totalNow;
    prevIdlePerCore[i] = idleNow;
  }

  qDebug() << "Updated core usages:" << cpuCoreUsages;
  emit cpuCoreUsagesChanged();
}
