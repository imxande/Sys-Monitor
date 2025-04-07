#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <QObject>
#include <QString>
#include <QList>

class ProcessManager : public QObject {
  // for signals and slots
  Q_OBJECT

public:
  explicit ProcessManager(QObject *parent = nullptr);
  ~ProcessManager();

  // process info struct
  struct ProcessInfo {
    QString name;
    QString user;
    float cpuPercent;
    qint64 pid;
    double memory;
  };

  // fetch process
  QList<ProcessInfo> getProcesses() const;

signals:
  // notify ProcessTab UI
  void processListUpdated(const QList<struct ProcessInfo> &processList);
  void testSignal(const QStringList &data);


private:
  void updateProcessList();
  QList<ProcessInfo> processList;
};

#endif // PROCESS_MANAGER_H
