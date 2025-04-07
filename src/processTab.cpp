#include "processManager.h"
#include "processTab.h"
#include <QAbstractItemView>
#include <QDebug>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QVBoxLayout>

// Init ProcessTab
ProcessTab::ProcessTab(QWidget *parent) : QWidget(parent) {
  // init process manager
  processManager = new ProcessManager(this);

 // connect signal from ProcessManager to update ProcessTab
  connect(processManager, &ProcessManager::processListUpdated, this,
          &ProcessTab::updateProcessTable);

  connect(processManager, &ProcessManager::testSignal,
        this, &ProcessTab::testReceiver);


  // set table layout
  const QStringList labels = getHeaderLabels();
  setProcessLayout(labels);
}

// destructor
ProcessTab::~ProcessTab() {}

// Process layout setup
void ProcessTab::setProcessLayout(const QStringList &labels) {
  // table layout
  QVBoxLayout *processLayout = new QVBoxLayout(this);

  // create table
  processTable = new QTableWidget(this);
  processTable->setColumnCount(labels.size());
  processTable->setHorizontalHeaderLabels(labels);

  // config and positioning
  processTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  processTable->verticalHeader()->setVisible(false);
  processTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

  // add table to layout
  processLayout->addWidget(processTable);
}

// Header Lables getter
const QStringList ProcessTab::getHeaderLabels() const { return headerLabels; }

// Update process table
void ProcessTab::updateProcessTable(
    const QList<ProcessManager::ProcessInfo> &processList) {
  // sanity check
  qDebug() << "[DEBUG] updateProcessTable called. Process count:"
           << processList.size();
  qDebug() << "[DEBUG] processTable exists:" << (processTable != nullptr);

  // clear rows
  processTable->setRowCount(0);

  // populate process table
  int row = 0;
  for (const auto &proc : processList) {
    processTable->insertRow(row);
    processTable->setItem(row, 0, new QTableWidgetItem(proc.name));
    processTable->setItem(row, 1, new QTableWidgetItem(proc.user));
    processTable->setItem(
        row, 2, new QTableWidgetItem(QString::number(proc.cpuPercent, 'f', 2)));
    processTable->setItem(row, 3,
                          new QTableWidgetItem(QString::number(proc.pid)));
    processTable->setItem(
        row, 4,
        new QTableWidgetItem(QString::number(proc.memory, 'f', 2) + "MB"));
    ++row;
  }
}

void ProcessTab::testReceiver(const QStringList &data) {
    qDebug() << "[DEBUG] testReceiver called:" << data;
}

