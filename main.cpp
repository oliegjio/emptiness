#include "mainwindow.h"

#include <QApplication>

#include "runguard.h"

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    RunGuard* guard = new RunGuard("Emptiness");
    if (!guard->tryToRun()) return EXIT_FAILURE;

    application.setWindowIcon(QIcon("icon.png"));
    QCoreApplication::setApplicationName("Emptiness");

    MainWindow window(guard);
    window.show();
    window.setFixedSize(800, 450);

    return application.exec();
}
