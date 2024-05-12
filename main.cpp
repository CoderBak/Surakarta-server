#include <QApplication>
#include "server.h"
#include "Mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    Server server;
    return QApplication::exec();
}
