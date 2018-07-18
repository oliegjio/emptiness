#include "mainwindow.h"

#include <QApplication>

#include "singleapplication.h"

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    SingleApplication* singleApplication = new SingleApplication("Emptiness");
    if (!singleApplication->tryToRun()) return EXIT_SUCCESS;

    application.setWindowIcon(QIcon("icon.png"));
    QCoreApplication::setApplicationName("Emptiness");

    MainWindow window(singleApplication);
    window.show();

    return application.exec();
}
