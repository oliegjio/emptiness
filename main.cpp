#include "mainwindow.h"

#include <QApplication>

#include "singleapplication.h"

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    SingleApplication* singleApplication = new SingleApplication("Emptiness");
    if (!singleApplication->tryToRun()) return EXIT_SUCCESS;

    srand(static_cast<unsigned>(time(nullptr)));

    application.setWindowIcon(QIcon("icon.png"));
    QCoreApplication::setApplicationName("Emptiness");

    MainWindow window(singleApplication);
    window.show();

    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(":/fonts/ubuntu-mono-regular.ttf");

    QFont font;
    font.setFamily("Ubuntu Mono");
    font.setPointSize(14);
    application.setFont(font);

    return application.exec();
}
