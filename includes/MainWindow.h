#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow {
  // for signals and slots
  Q_OBJECT

public:
  /*
   * @brief Constructs the Main window
   *
   * @param parent The parent QWidget
   */
  explicit MainWindow(QWidget *parent = nullptr);
  
  // Destructor
  ~MainWindow();

private:
  /* 
   * @brief Main window UI configuration
  */
  void configUI();

  /*
   * @brief Layout setup
  */
  void setLayout();


};

#endif // MAIN_WINDOW_H
