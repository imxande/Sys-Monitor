#ifndef PROCESS_TAB_H
#define PROCESS_TAB_H

#include "processManager.h"
#include "processInfo.h"
#include <QStringList>
#include <QTableWidget>
#include <QWidget>

class ProcessTab : public QWidget {
  // for signals and slots
  Q_OBJECT

public:
  /*
   * @brief Constructs the ProcessTab
   */
  explicit ProcessTab(QWidget *parent = nullptr);

  // Destructor
  ~ProcessTab();

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

#endif // PROCESS_TAB_H
