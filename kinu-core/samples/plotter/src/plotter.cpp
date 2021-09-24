#include <QtWidgets/QApplication>

#include "ui_mainwindow.h"

int main(int argc, char** argv)
{
  QApplication app(argc,argv);

  QMainWindow* mw = new QMainWindow();
  Ui_MainWindow ui;
  ui.setupUi(mw);
  mw->show();

  return app.exec();
}
