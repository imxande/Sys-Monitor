#ifndef RESOURCE_MONITOR_H
#define RESOURCE_MONITOR_H

#include <QObject>
#include <QPair>
#include <QTimer>
#include <QVector>
#include <QVariantList>

/* @brief Resource Monitor class collects and emits system resource metrics such
 * as CPU usage memory swap, network and disk activity at regular intervals.
 *
 * Data is exposed to the QML frontend to drive real-time graphs and statistics
 */
class ResourceMonitor : public QObject {
  // for signals and slots
  Q_OBJECT

  // expose properties to QML
  Q_PROPERTY(float cpuUsage READ getCpuUsage NOTIFY cpuUsageChanged)
  Q_PROPERTY(QVariantList cpuCoreUsages READ getCpuCoreUsages NOTIFY
                 cpuCoreUsagesChanged)
  Q_PROPERTY(float memoryUsage READ getMemoryUsage NOTIFY memoryUsageChanged)
  Q_PROPERTY(float rxRate READ getRxRate NOTIFY rxRateChanged)
  Q_PROPERTY(float txRate READ getTxRate NOTIFY txRateChanged)
  Q_PROPERTY(qulonglong readRate READ getReadRate NOTIFY readRateChanged)
  Q_PROPERTY(qulonglong writeRate READ getWriteRate NOTIFY writeRateChanged)

public:
  explicit ResourceMonitor(QObject *parent = nullptr);
  ~ResourceMonitor();

  /*
   * @brief Getter method for cpu usage property
   *
   * @return cpuUsage The actual cpu usage property
   */
  float getCpuUsage();

  /* @brief Getter method for cpu core usages property*
   *
   * @return cpuCoreUsages The actual usage per cpu core
   */
  QVariantList getCpuCoreUsages() const;

  /*
   * @brief Getter method for memory usage property
   *
   * @return memoryUsage The actual memory usage property
   */
  float getMemoryUsage();

  /*
   * @brief Getter method for rxRate property
   *
   * @return The most recently computed receive rate in bytes/sec
   */
  float getRxRate();

  /*
   * @brief Getter method for txRate property
   *
   * @return The most recently computed transmit rate in bytes/sec
   */
  float getTxRate();

  /*
   * @brief Getter method for readRate
   *
   * @return The current disk read throughput in bytes per second.
   */
  qulonglong getReadRate();

  /*
   * @brief Getter method for writeRate
   *
   * @return The current disk write throughput in bytes per second.
   */
  qulonglong getWriteRate();

  /* @brief Method reads CPU time stats from system
   *
   * It parses the first line of /proc/stat to retrieve the cummulatice
   * CPU time values since boot (e.g user, system, idle).
   * These values are required to compute CPU usage percentage over time.
   *
   * @return QPair containing:
   * A pair of values representing total and idle CPU time in jiffies
   * - first: total CPU time (qlonglong)
   * - second: idle CPU time (qlonglong)
   */
  QPair<qulonglong, qulonglong> readCpuUsage();

  QList<QPair<qulonglong, qulonglong>> readPerCoreUsage();

  /*
   * @brief Reads /proc/net/dev toextract total number of bytes received
   * and sent over all network interfaces.
   *
   * @return QPair containing:
   * - first: total received bytes
   * - second: total transmitted bytes
   */
  QPair<qulonglong, qulonglong> readNetworkBytes();

signals:
  /*
   * @brief Notify QML that cpu usage value has change
   */
  void cpuUsageChanged();

  /*@brief Notify QML that cpu core usages value has change*/
  void cpuCoreUsagesChanged();

  /*
   * @brief Notify QML that memory usage value has change
   */
  void memoryUsageChanged();

  /*
   * brief Notify QML when rxRate has new value.
   */
  void rxRateChanged();

  /*
   * @brief Notifies QML when txRate has new value.
   */
  void txRateChanged();

  /*
   * @brief Notify QML when readRate has new value.
   */
  void readRateChanged();

  /*
   * @brief Notify QML when writeRate has new value.
   */
  void writeRateChanged();

private slots:
  /* @brief updateCpuUsage runs on a 1s timer
   * This slot reacts to a system event, timeout
   * Notifies QML via signal
   */
  void updateCpuUsage();

  /* @brief updateCpuCoreUsages runs on a 1s timer
   * This slot reacts to a system event, timeout
   * Notifies QML via signal
   */
  void updateCpuCoreUsages();

  /*
   * @brief Periodically updates memory usage percentage.
   *
   * Reads from `/proc/meminfo` to retrieve total and available memory in kB.
   * Calculates memory usage and emits `memoryUsageChanged()` to notify QML.
   *
   * This method is intended to be called once per second by a timer.
   */
  void updateMemoryUsage();

  /*
   * @brief Method is called periodically by timer every 1 sec.
   * Reads current total bytes via readNetworkBytes().
   * Compute deltas with prev values.
   * Calculates rxRate and txRate in bytes/sec.
   * Emits rxRateChanged() and txRateChanged() signals for QML to update graph.
   * */
  void updateNetworkRates();

  /*
   * @brief Periodically reads disk activity statistics from /proc/diskstats
   * Computes disk read and write throughput in bytes per second.
   * Emits readReateChanged() and writeRateChanged() to notify QML of updated
   * values.
   */
  void updateDiskStats();

private:
  QTimer *updateTimer;
  float cpuUsage;
  QList<float> cpuCoreUsages;
  float memoryUsage = 0.0f;
  float rxRate = 0.0f;
  float txRate = 0.0f;
  qulonglong prevTotal = 0;
  qulonglong prevIdle = 0;
  qulonglong memTotal = 0;
  qulonglong memAvailable = 0;
  qulonglong totalRx = 0;
  qulonglong totalTx = 0;
  qulonglong prevRx = 0;
  qulonglong prevTx = 0;
  qulonglong readRate = 0;
  qulonglong writeRate = 0;
  qulonglong prevReadBytes = 0;
  qulonglong prevWriteBytes = 0;
  QVector<qulonglong> prevTotalPerCore;
  QVector<qulonglong> prevIdlePerCore;
};

#endif // RESOURCE_MONITOR_H
