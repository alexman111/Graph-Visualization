#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QRandomGenerator>

#include "randomgraphgenerator.h"
#include "makeedgewidget.h"
#include "iostream"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;
   w.show();

   return a.exec();
}
