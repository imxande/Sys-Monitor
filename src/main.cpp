#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
  // create app
  QApplication app(argc, argv);

  qDebug() << "Application entry point!";


  return app.exec();
}
