#include "fileSystemsTab.h"
#include <QDebug>
#include <QVBoxLayout>

// Init file system tab
FileSystemsTab::FileSystemsTab(QWidget *parent) : QWidget(parent) {
  // get labels
  const QStringList labels = getHeaderLabels();

  // set layout
  setLayout(labels);
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

  // add table to layout
  layout->addWidget(sysTable);
}

const QStringList FileSystemsTab::getHeaderLabels() const { return labels; }
