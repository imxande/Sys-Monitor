#ifndef FILESYSTEM_INFO_H
#define FILESYSTEM_INFO_H

#include <QString>

struct FileSystemInfo {
  QString available;
  QString device;
  QString directory;
  QString type;
  QString used;
  QString total;
};



#endif // FILESYSTEM_INFO_H
