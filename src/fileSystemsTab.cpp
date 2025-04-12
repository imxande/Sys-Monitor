#include "fileSystemsTab.h"
#include <QAbstractItemView>
#include <QDebug>
#include <QStorageInfo>
#include <QVBoxLayout>
#include <QTableWidgetItem>

// Init file system tab
FileSystemsTab::FileSystemsTab(QWidget *parent) : QWidget(parent) {
  // get labels
  const QStringList labels = getHeaderLabels();

  // set layout
  setLayout(labels);

  // gather mounted file systems info
  QList<FileSystemInfo> fsList = getMountedFileSystems();
  populateTable(fsList);
}

// destructor
FileSystemsTab::~FileSystemsTab() {}

// Sets file system tab layout
void FileSystemsTab::setLayout(const QStringList &labels) {
  // table layout
  QVBoxLayout *layout = new QVBoxLayout(this);

  // create table
  sysTable = new QTableWidget(this);
  sysTable->setColumnCount(labels.size());
  sysTable->setHorizontalHeaderLabels(labels);

  // config & positioning
  sysTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // add table to layout
  layout->addWidget(sysTable);
}

// Get header labels
const QStringList FileSystemsTab::getHeaderLabels() const { return labels; }

// Populate file system info
QList<FileSystemInfo> FileSystemsTab::getMountedFileSystems() {
  // placeholders
  QList<FileSystemInfo> infoList;
  const auto storages = QStorageInfo::mountedVolumes();

  for (const QStorageInfo &storage : storages) {
    if (!storage.isValid() || !storage.isReady())
      continue;

    // store info
    FileSystemInfo info;
    info.device = storage.device();
    info.directory = storage.rootPath();
    info.type = storage.fileSystemType();
    info.total = QString::number(storage.bytesTotal() / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
    info.used = QString::number((storage.bytesTotal() - storage.bytesAvailable()) / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
    info.available = QString::number(storage.bytesAvailable() / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";

    // add to list
    infoList.append(info);
  }

  return infoList;
}

// Populate file system table
void FileSystemsTab::populateTable(const QList<FileSystemInfo> &list) {
  // setup
  sysTable->setRowCount(list.size());

  // populate
  for (int i = 0; i < list.size(); ++i) {
    const auto &fs = list[i];
    sysTable->setItem(i, 0, new QTableWidgetItem(fs.available)); 
    sysTable->setItem(i, 1, new QTableWidgetItem(fs.device)); 
    sysTable->setItem(i, 2, new QTableWidgetItem(fs.directory)); 
    sysTable->setItem(i, 3, new QTableWidgetItem(fs.type)); 
    sysTable->setItem(i, 4, new QTableWidgetItem(fs.total)); 
    sysTable->setItem(i, 5, new QTableWidgetItem(fs.used)); 
  }
}

