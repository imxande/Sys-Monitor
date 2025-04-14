#ifndef RESOURCES_TAB_H
#define RESOURCES_TAB_H

#include "resourceMonitor.h"
#include <QGroupBox>
#include <QQuickWidget>
#include <QWidget>

/* @brief ResourceTab provides a real-time visual monitoring of overall system
 * metrics. Displays CPU Usage, Memory and Swap, Network Activity and Disk I/O
 * live graphs.
 */
class ResourcesTab : public QWidget {
  // for signals and slots
  Q_OBJECT

public:
  explicit ResourcesTab(QWidget *parent = nullptr);
  ~ResourcesTab();

  /* @brief Load resources tab layout
   */
  void setResourcesLayout();

private:
  // UI elements
  QGroupBox *cpuBox;
  QGroupBox *memSwapBox;
  QGroupBox *networkBox;
  QGroupBox *diskBox;
  QQuickWidget *cpuGraph;
  ResourceMonitor *monitor;
};

#endif // RESOURCES_TAB_H
