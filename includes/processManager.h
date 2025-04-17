#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "processInfo.h"
#include <QObject>
#include <QString>
#include <QList>

class ProcessManager : public QObject {
  // for signals and slots
  Q_OBJECT

public:
  explicit ProcessManager(QObject *parent = nullptr);
  ~ProcessManager();

  // fetch process
  QList<ProcessInfo> getProcesses() const;

signals:
  // notify ProcessTab UI
  void processListUpdated(const QList<struct ProcessInfo> &processList);


private:
  QList<ProcessInfo> processList;
  void updateProcessList();
  QList<ProcessInfo> scanProcStatus();
  QString resolveUser(const QString &uidStr) const;
  double calculateCpuUsage(qint64 pid) const;
  void sortByCpuUsage(QList<ProcessInfo> &list);
};

#endif // PROCESS_MANAGER_H
