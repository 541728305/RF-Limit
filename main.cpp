#include "mainwindow.h"

#include <QApplication>
#include <vld.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *widget = new MainWindow();
    QMainWindow w;
    w.setWindowTitle("RF Limit 32bit-By Chan");
    w.setCentralWidget(widget);
    w.resize(960,540);
    w.show();
    return a.exec();
}

