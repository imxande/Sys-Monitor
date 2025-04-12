#ifndef RESOURCE_MONITOR_H
#define RESOURCE_MONITOR_H

#include <QObject>
#include <QTimer>
#include <QPair>

/* @brief Resource Monitor class collects and emits system resource metrics such as CPU usage
 * memory swap, network and disk activity at regular intervals.
 *
 * Data is exposed to the QML frontend to drive real-time graphs and statistics
 */
class ResourceMonitor : public QObject {
  // for signals and slots
  Q_OBJECT

  // expose cpu usage to QML
  Q_PROPERTY(float cpuUsage READ getCpuUsage NOTIFY cpuUsageChanged)

public:
  explicit ResourceMonitor(QObject *parent = nullptr);
  ~ResourceMonitor();

  /*
   * @brief Getter method for cpu usage property
   *
   * @return cpuUsage The actual cpu usage property
   */
  float getCpuUsage();

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
  QPair<qulonglong, qulonglong>readCpuUsage();

signals:
  /*
   * @brief Notify QML that cpu usage value has change
   */
  void cpuUsageChanged();

private slots:
  /* @bried updateCpuUsage runs on a 1s timer
   * This slot reacts to a system event, timeout
   * Notifies QML via signal
   */
  void updateCpuUsage();

private:
  QTimer *updateTimer;
  float cpuUsage;
  qulonglong prevTotal = 0;
  qulonglong prevIdle = 0;
};

#endif // RESOURCE_MONITOR_H
