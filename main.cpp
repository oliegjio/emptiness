#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    application.setWindowIcon(QIcon("icon.png"));
    QCoreApplication::setApplicationName("Emptiness");

    MainWindow window;
    window.show();
    window.setFixedSize(600, 400);

    return application.exec();
}
