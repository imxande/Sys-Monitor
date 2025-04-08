#include "processManager.h"
#include <QDir>
#include <QRegularExpression>
#include <QTimer>
#include <fstream>
#include <sstream>

// constructor
ProcessManager::ProcessManager(QObject *parent) : QObject(parent) {
  // delay signal till slot ready and connection is established - populate
  // processes list
  QTimer::singleShot(0, this, [this]() { updateProcessList(); });
}

// destructor
ProcessManager::~ProcessManager() {}

// Fetch processes
QList<ProcessInfo> ProcessManager::getProcesses() const { return processList; }

// Populate list with dummy data for now
void ProcessManager::updateProcessList() {
  // update process list
  processList = scanProcStatus();

  // notify ProcessTab
  emit processListUpdated(processList);
}

// Scan active processes
QList<ProcessInfo> ProcessManager::scanProcStatus() {
  // placeholder
  QList<ProcessInfo> processes;
  QDir proc("/proc");
  // open /proc directory
  if (!proc.exists()) {
    qDebug() << "File " + proc.absolutePath() + " does not exist!";
  }

  // get all entries
  QList entryList = proc.entryList();

  // filter to get some useful process status
  for (const QString &entry : entryList) {
    // check for numeric values
    bool isNum;
    qint64 pid = entry.toInt(&isNum);

    if (!isNum)
      continue;

    // build path to status
    QString statusPath = "/proc/" + entry + "/status";
    std::ifstream statusFile(statusPath.toStdString());
    if (!statusFile.is_open()) {
      qDebug() << "Failed to open:" << statusPath;
      continue;
    }

    // get process info
    QString name;
    QString uidStr;

    std::string line;
    while (std::getline(statusFile, line)) {
      QString qline = QString::fromStdString(line);

      if (qline.startsWith("Name:")) {
        name = qline.section(":", 1).trimmed();
      } else if (qline.startsWith("Uid:")) {
        uidStr = qline.section(":", 1).simplified().section(" ", 0, 0);
      }

      if (!name.isEmpty() && !uidStr.isEmpty())
        break;
    }

    qDebug() << "Process Name:" << name;
    qDebug() << "UID:" << uidStr;
  }

  return processes;
}

