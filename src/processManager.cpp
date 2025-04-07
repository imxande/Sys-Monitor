#include "processManager.h"

ProcessManager::ProcessManager(QObject *parent) : QObject(parent) {
  // populate list of processes
  updateProcessList();
}

// destructor
ProcessManager::~ProcessManager() {}

// Fetch processes
QList<ProcessManager::ProcessInfo> ProcessManager::getProcesses() const {
  return processList;
}

// Populate list with dummy data for now
void ProcessManager::updateProcessList() {
  QList<ProcessInfo> data;


  // processes
  ProcessInfo p1 = {"firefox", "xande", 12.5f, 4567, 350.2};
  ProcessInfo p2 = {"gnome-shell", "xande", 9.3f, 1234, 210.1};
  ProcessInfo p3 = {"Xorg", "root", 7.8f, 2222, 175.4};

  // add dummy processes to data list
  data << p1 << p2 << p3;

  // update process list
  processList = data;

  QStringList dummy = { "Test", "Signal", "Working" };

  // notify ProcessTab
  emit processListUpdated(processList);
  emit testSignal(dummy);
}
