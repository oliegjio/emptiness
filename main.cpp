#include "mainwindow.h"

#include <QApplication>

#include "singleapplication.h"

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    SingleApplication* guard = new SingleApplication("Emptiness");
    if (!guard->tryToRun()) return EXIT_FAILURE;

    application.setWindowIcon(QIcon("icon.png"));
    QCoreApplication::setApplicationName("Emptiness");

    MainWindow window(guard);
    window.show();
    window.setFixedSize(800, 450);

    return application.exec();
}
