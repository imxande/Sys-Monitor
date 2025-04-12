#ifndef RESOURCE_TAB_H
#define RESOURCE_TAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QQuickWidget>

/* @brief ResourceTab provides a real-time visual monitoring of overall system metrics.
 * Displays CPU Usage, Memory and Swap, Network Activity and Disk I/O live graphs.
 */
class ResourceTab : public QWidget {
  // for signals and slots 
  Q_OBJECT

public:
  explicit ResourceTab(QWidget *parent = nullptr); 
  ~ResourceTab();
 
};

#endif // RESOURCE_TAB_H
