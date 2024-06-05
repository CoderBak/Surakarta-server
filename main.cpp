//In this file, we have main function
#include <QApplication>
#include "server.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.setServer(new Server());
    w.show();
    return QApplication::exec();
}
