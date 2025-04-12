#ifndef RESOURCE_MONITOR_H
#define RESOURCE_MONITOR_H

#include <QObject>
#include <QTimer>

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
  ResourceMonitor(QObject *parent = nullptr);
  ~ResourceMonitor();

  /*
   * @brief Getter method for cpu usage property
   *
   * @return cpuUsage The actual cpu usage property
   */
  float getCpuUsage();

signals:
  /*
   * @brief Notify QML that cpu usage value has change
   */
  void cpuUsageChanged();

private:
  QTimer *updateTimer;
  float cpuUsage;
};

#endif // RESOURCE_MONITOR_H
