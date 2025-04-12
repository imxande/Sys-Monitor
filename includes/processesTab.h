#ifndef PROCESSES_TAB_H
#define PROCESSES_TAB_H

#include "processManager.h"
#include "processInfo.h"
#include <QStringList>
#include <QTableWidget>
#include <QWidget>

class ProcessesTab : public QWidget {
  // for signals and slots
  Q_OBJECT

public:
  /*
   * @brief Constructs the ProcessTab
   */
  explicit ProcessesTab(QWidget *parent = nullptr);

  // Destructor
  ~ProcessesTab();

  /*
   * @brief Set process layout configuration
   *
   * @param labels - list of labels for the header section
   */
  void setProcessLayout(const QStringList &labels);

  /*
   * @brief Gets the header labels
   *
   * @return a list of labels for the header section
   */
  const QStringList getHeaderLabels() const;

private slots:
  void updateProcessTable(const QList<ProcessInfo> &processList);


private:
  const QStringList headerLabels = {"Process", "User", "%CPU", "ID", "Memory"};
  ProcessManager *processManager; // ProcessManager instance to handle processes
  QTableWidget *processTable;


};

#endif // PROCESSES_TAB_H
