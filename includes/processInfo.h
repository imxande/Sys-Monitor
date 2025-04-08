#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <QString>
#include <QMetaType>

struct ProcessInfo {
  QString name;
  QString user;
  float cpuPercent;
  qint64 pid;
  double memory;
};

/*
 * Needed in order to pass the struct in a Qt signal to display the processes
 * Qt's mov system wont process the nested type in signals/slots
 * Thus the macro makes the type Type known to QMetaType,
 * the idea is to allow moc to generate the right meta-object code for it
*/
Q_DECLARE_METATYPE(ProcessInfo)

#endif // PROCESS_INFO_H
