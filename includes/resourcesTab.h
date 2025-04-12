#ifndef RESOURCES_TAB_H
#define RESOURCES_TAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QQuickWidget>

/* @brief ResourceTab provides a real-time visual monitoring of overall system metrics.
 * Displays CPU Usage, Memory and Swap, Network Activity and Disk I/O live graphs.
 */
class ResourcesTab : public QWidget {
  // for signals and slots 
  Q_OBJECT

public:
  explicit ResourcesTab(QWidget *parent = nullptr); 
  ~ResourcesTab();
 
};

#endif // RESOURCES_TAB_H
