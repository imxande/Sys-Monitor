#include "processTab.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
	#include <QAbstractItemView>

// Init ProcessTab
ProcessTab::ProcessTab(QWidget *parent) : QWidget(parent) {
  const QStringList labels = getHeaderLabels();
  setProcessLayout(labels);
}

// destructor
ProcessTab::~ProcessTab(){}

// Process layout setup
void ProcessTab::setProcessLayout(const QStringList &labels) {
  // setup
  QVBoxLayout *processLayout = new QVBoxLayout(this);  
  QTableWidget *processTable = new QTableWidget(this);
  processTable->setColumnCount(5);
  processTable->setHorizontalHeaderLabels(labels);

  // dummy rows for now
  processTable->setRowCount(3);
  
  // process gnome-shell
  processTable->setItem(0, 0, new QTableWidgetItem("gnome-shell")); // name
  processTable->setItem(0, 1, new QTableWidgetItem("xande"));       // user
  processTable->setItem(0, 2, new QTableWidgetItem("15.3"));        // %cpu
  processTable->setItem(0, 3, new QTableWidgetItem("1234"));        // ID
  processTable->setItem(0, 4, new QTableWidgetItem("200 MB"));      // MEM

  // process xorg
  processTable->setItem(1, 0, new QTableWidgetItem("xorg")); // name
  processTable->setItem(1, 1, new QTableWidgetItem("root"));       // user
  processTable->setItem(1, 2, new QTableWidgetItem("8.1"));        // %cpu
  processTable->setItem(1, 3, new QTableWidgetItem("456"));        // ID
  processTable->setItem(1, 4, new QTableWidgetItem("150 MB"));      

  // process google-chrome
  processTable->setItem(2, 0, new QTableWidgetItem("google-chrome")); // name
  processTable->setItem(2, 1, new QTableWidgetItem("xande"));       // user
  processTable->setItem(2, 2, new QTableWidgetItem("33.7"));        // %cpu
  processTable->setItem(2, 3, new QTableWidgetItem("7890"));        // ID
  processTable->setItem(2, 4, new QTableWidgetItem("500 MB"));      

  // config and positioning
  processTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  processTable->verticalHeader()->setVisible(false);
  processTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);


  // add table to layout
  processLayout->addWidget(processTable);
}

// Header Lables getter
const QStringList ProcessTab::getHeaderLabels() const {
  return headerLabels;
}
