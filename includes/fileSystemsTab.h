#ifndef FILESYSTEMS_TAB_H
#define FILESYSTEMS_TAB_H

#include <QStringList>
#include <QWidget>
#include <QTableWidget>

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

  /*
   * @brief Loads file system tab layout
   *
   * @param labels List of labels of the header
   */
  void setLayout(const QStringList &labels);

  /* @brief Gets header labels
   *
   * @return labels List of labels
   */
  const QStringList getHeaderLabels() const;

private:
  QTableWidget *sysTable;
  const QStringList labels = {"Available", "Device", "Directory",
                        "Type",      "Total",  "Used"};
};

#endif // FILESYSTEMS_TAB_H
