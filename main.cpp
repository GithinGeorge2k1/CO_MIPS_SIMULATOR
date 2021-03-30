#include "mainwindow.h"
#include "Maps.h"

#include <QApplication>
#include <QFile>
static MainWindow* w=NULL;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleSheet(":/stylesheet/StyleSheet/style.qss");
    styleSheet.open(QFile::ReadOnly);

    QString style=styleSheet.readAll();
    a.setStyleSheet(style);
    if(w==NULL)
        w=new MainWindow();
    w->show();
    return a.exec();
}
