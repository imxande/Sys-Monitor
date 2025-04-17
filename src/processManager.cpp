#include "processManager.h"
#include <QDir>
#include <QRegularExpression>
#include <QTimer>
#include <fstream>
#include <pwd.h>
#include <sstream>
#include <sys/types.h>

// constructor
ProcessManager::ProcessManager(QObject *parent) : QObject(parent) {
  // delay signal till slot ready and connection is established - populate
  // processes list
  // QTimer::singleShot(0, this, [this]() { updateProcessList(); });
  QTimer *processTimer = new QTimer(this);

  // connect timer
  connect(processTimer, &QTimer::timeout, this, &ProcessManager::updateProcessList);
  processTimer->start(1000);

  // initial fetch
  updateProcessList();
}

// destructor
ProcessManager::~ProcessManager() {}

// Fetch processes
QList<ProcessInfo> ProcessManager::getProcesses() const { return processList; }

// Populate list with dummy data for now
void ProcessManager::updateProcessList() {
  // update process list
  processList = scanProcStatus();

  // sort by % CPU 
  sortByCpuUsage(processList);

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
    QString username;
    double memMB = 0.0;
    double cpuPercent = 0.0;
    std::string line;

    // calculate cpu usage of each process
    cpuPercent = calculateCpuUsage(pid);

    while (std::getline(statusFile, line)) {
      QString qline = QString::fromStdString(line);

      if (qline.startsWith("Name:")) {
        name = qline.section(":", 1).trimmed();
      } else if (qline.startsWith("Uid:")) {
        uidStr = qline.section(":", 1).simplified().section(" ", 0, 0);
        username = resolveUser(uidStr);
      } else if (qline.startsWith("VmRSS:")) {
        QString value = qline.section(":", 1).simplified().section(" ", 0, 0);
        bool ok = false;
        memMB = value.toDouble(&ok) / 1024.0;
        if (!ok)
          memMB = 0.0;
      }

      if (!name.isEmpty() && !uidStr.isEmpty() && memMB > 0.0)
        break;
    }

    // populate ProcessInfo struct
    ProcessInfo info;
    info.name = name;
    info.user = username;
    info.cpuPercent = cpuPercent;
    info.pid = pid;
    info.memory = memMB;

    processes.append(info);

  }

  return processes;
}

// Conver uid to user
QString ProcessManager::resolveUser(const QString &uidStr) const {
  // placeholders
  bool ok;
  uid_t uid = static_cast<uid_t>(uidStr.toUInt(&ok));
  if (!ok)
    return "unknown";

  struct passwd *pw = getpwuid(uid);
  return pw ? QString(pw->pw_name) : "unknown";
}

// Calculate CPU usage
double ProcessManager::calculateCpuUsage(qint64 pid) const {
  // placeholders
  QString statPath = "/proc/" + QString::number(pid) + "/stat";
  std::ifstream statFile(statPath.toStdString());

  if (!statFile.is_open())
    return 0.0;

  std::string content;
  std::getline(statFile, content);
  std::istringstream iss(content);
  std::vector<std::string> tokens;
  std::string token;

  while (iss >> token)
    tokens.push_back(token);
  if (tokens.size() < 22)
    return 0.0;

  long utime = std::stol(tokens[13]);
  long stime = std::stol(tokens[14]);
  long startime = std::stol(tokens[21]);

  // get system uptime
  std::ifstream uptimeFile("/proc/uptime");
  double uptime = 0.0;
  uptimeFile >> uptime;

  long totalTime = utime + stime;
  long hertz = sysconf(_SC_CLK_TCK);
  double seconds = uptime - (startime / static_cast<double>(hertz));
  if (seconds <= 0)
    return 0.0;

  return 100 * ((totalTime / static_cast<double>(hertz)) / seconds);
}

void ProcessManager::sortByCpuUsage(QList<ProcessInfo> &list) {
  std::sort(list.begin(), list.end(), [](const ProcessInfo &a, const ProcessInfo &b) {
    return a.cpuPercent > b.cpuPercent;
  });
}
