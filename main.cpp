#include <QtGui/QApplication>
#include <QTransform>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   QString locale = QLocale::system().name();

   QTranslator translator;
   translator.load(QString("qmentat_") + locale);
   a.installTranslator(&translator);

   MainWindow w;
   w.show();

   return a.exec();
}
