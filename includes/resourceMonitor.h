#ifndef RESOURCE_MONITOR_H
#define RESOURCE_MONITOR_H

#include <QObject>
#include <QPair>
#include <QTimer>

/* @brief Resource Monitor class collects and emits system resource metrics such
 * as CPU usage memory swap, network and disk activity at regular intervals.
 *
 * Data is exposed to the QML frontend to drive real-time graphs and statistics
 */
class ResourceMonitor : public QObject {
  // for signals and slots
  Q_OBJECT

  // expose cpu usage to QML
  Q_PROPERTY(float cpuUsage READ getCpuUsage NOTIFY cpuUsageChanged)
  Q_PROPERTY(float memoryUsage READ getMemoryUsage NOTIFY memoryUsageChanged)

public:
  explicit ResourceMonitor(QObject *parent = nullptr);
  ~ResourceMonitor();

  /*
   * @brief Getter method for cpu usage property
   *
   * @return cpuUsage The actual cpu usage property
   */
  float getCpuUsage();

  /*
   * @brief Getter method for memory usage property
   *
   * @return memoryUsage The actual memory usage property
   */
  float getMemoryUsage();

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

signals:
  /*
   * @brief Notify QML that cpu usage value has change
   */
  void cpuUsageChanged();

  /*
   * @brief Notify QML tha memory usage value has change
   */
  void memoryUsageChanged();

private slots:
  /* @brief updateCpuUsage runs on a 1s timer
   * This slot reacts to a system event, timeout
   * Notifies QML via signal
   */
  void updateCpuUsage();

  /*
   * @brief Periodically updates memory usage percentage.
   *
   * Reads from `/proc/meminfo` to retrieve total and available memory in kB.
   * Calculates memory usage and emits `memoryUsageChanged()` to notify QML.
   *
   * This method is intended to be called once per second by a timer.
   */
  void updateMemoryUsage();

private:
  QTimer *updateTimer;
  float cpuUsage;
  float memoryUsage = 0.0f;
  qulonglong prevTotal = 0;
  qulonglong prevIdle = 0;
  qulonglong memTotal = 0;
  qulonglong memAvailable = 0;
};

#endif // RESOURCE_MONITOR_H
