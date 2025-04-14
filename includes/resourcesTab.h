#ifndef RESOURCES_TAB_H
#define RESOURCES_TAB_H

#include "resourceMonitor.h"
#include <QGroupBox>
#include <QQuickWidget>
#include <QWidget>

/* @brief ResourceTab provides a real-time visual monitoring of overall system metrics.
 * Displays CPU Usage, Memory and Swap, Network Activity and Disk I/O live graphs.
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

  /* @brief Helper method to create collapsible section.
   * Toggles dopdow to display and hide graphs.
   *
   * @param title - A QString for section title.
   * @param content - A pointer to a QWidget representing the content of the section. 
   *
   * @return Qt widget pointer to collapsible section.
   */
  QWidget* createSection(const QString &title);

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
