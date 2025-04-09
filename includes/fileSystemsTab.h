#ifndef FILESYSTEMS_TAB_H
#define FILESYSTEMS_TAB_H

#include <QWidget>

/*
 * @brief File System Tab displays mounted file systems */
class FileSystemsTab : public QWidget {
  // for signal and slots
  Q_OBJECT

public:
  // constructor 
  explicit FileSystemsTab(QWidget *parent = nullptr);

  // destructor
  ~FileSystemsTab();

};

#endif // FILESYSTEMS_TAB_H


