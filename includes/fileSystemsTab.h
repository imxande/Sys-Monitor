#ifndef FILESYSTEMS_TAB_H
#define FILESYSTEMS_TAB_H

#include "fileSystemInfo.h"
#include <QStringList>
#include <QTableWidget>
#include <QWidget>

/*
 * @brief File System Tab displays mounted file systems 
*/
class FileSystemsTab : public QWidget {
  // for signal and slots
  Q_OBJECT

public:
  // constructor
  explicit FileSystemsTab(QWidget *parent = nullptr);

  // destructor
  ~FileSystemsTab();

  /*
   * @brief Loads file system tab layout
   *
   * @param labels List of labels of the header
   */
  void setLayout(const QStringList &labels);

  /*
   * @brief Gets header labels
   *
   * @return labels List of labels
   */
  const QStringList getHeaderLabels() const;

  /* @brief Populates file system info struct
   *
   * @return info for each item in a list of all mounted file systems
   */
  QList<FileSystemInfo> getMountedFileSystems();

  /*
   * @brief Populate file system table 
   */
  void populateTable(const QList<FileSystemInfo> &list);

private:
  QTableWidget *sysTable;
  const QStringList labels = {"Available", "Device", "Directory",
                              "Type",      "Total",  "Used"};
};

#endif // FILESYSTEMS_TAB_H

