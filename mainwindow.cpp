#include "mainwindow.h"

MainWindow::MainWindow(SingleApplication* singleApplication, QWidget *parent)
    : QMainWindow(parent)
    , singleApplication(singleApplication)
{
    editor = new TextEdit();
    layout = new QVBoxLayout();
    centralWidget = new QWidget();
    titleBar = new LineEdit();

    init();
    newFile(getAbsolutePathFromArguments());
}

void MainWindow::init()
{
    resize(QSize(800, 500));

    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);

    layout->setSpacing(0);
    layout->setMargin(0);

    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(":/fonts/ubuntu-mono-regular.ttf");

    QFont font;
    font.setFamily("Ubuntu Mono");
    font.setPointSize(14);
    editor->setFont(font);
    titleBar->setFont(font);

    QPalette editorPalette = editor->palette();
    editorPalette.setColor(QPalette::Base, QColor(0, 0, 0));
    editorPalette.setColor(QPalette::Text, QColor(255, 255, 255));
    editor->setPalette(editorPalette);

    QPalette titlePalette = titleBar->palette();
    titlePalette.setColor(QPalette::Base, QColor(50, 50, 50));
    titlePalette.setColor(QPalette::Text, QColor(255, 255, 255));
    titleBar->setPalette(titlePalette);

    titleBar->setFrame(false);
    editor->setFrameStyle(0);

    layout->addWidget(titleBar);
    layout->addWidget(editor);

    editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    editor->setFocus();

    editor->setFocusPolicy(Qt::ClickFocus);
    titleBar->setFocusPolicy(Qt::ClickFocus);

    connect(singleApplication,
            &SingleApplication::sharedMemoryForWorkingDirectoryChanged,
            this,
            &MainWindow::sharedMemoryForWorkingDirectoryChanged);
    connect(singleApplication,
            &SingleApplication::sharedMemoryForPathChanged,
            this,
            &MainWindow::sharedMemoryForPathChanged);
    connect(titleBar, &LineEdit::returnPressed, this, &MainWindow::returnPressed);
}

void MainWindow::returnPressed()
{
    openFile(titleBar->text());
}

void MainWindow::sharedMemoryForPathChanged(const QString& path)
{
    newFile(toAbsolutePath(path));
}

void MainWindow::sharedMemoryForWorkingDirectoryChanged(const QString& path)
{
    QDir::setCurrent(path);
}

QString MainWindow::getAbsolutePathFromArguments()
{
    QStringList arguments = QCoreApplication::arguments();
    if (arguments.length() == 1) return "";
    else return toAbsolutePath(arguments.at(1));
}

void MainWindow::newFile(const QString& path)
{
    currentFilePath = path;
    titleBar->setText(path);
    openFile(path);
}

void MainWindow::openFile(const QString& path)
{
    QFile file(path);

    if (!file.exists(path))
    {
        editor->clear();
        return;
    }

    QString text;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            text = stream.readAll();
            editor->document()->setPlainText(text);
        }
    }
}

void MainWindow::createPath(const QString& path)
{
    if (QDir(path).exists()) return;

    QFileInfo info(path);
    QString absoluteDirPath = info.absoluteDir().absolutePath();

    QDir directory(absoluteDirPath);
    directory.mkpath(absoluteDirPath);
}

void MainWindow::saveFile(const QString& path)
{
    QFile file(path);

    createPath(path);

    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        QString text = editor->document()->toPlainText();
        stream << text;
    }
}

QString MainWindow::toAbsolutePath(const QString& path)
{
    QFileInfo info(path);
    return info.absoluteFilePath();
}

void MainWindow::toggleFocus()
{
    if (focusWidget() == editor) titleBar->setFocus();
    else if (focusWidget() == titleBar) editor->setFocus();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    int modifier = event->modifiers();

    if (modifier == Qt::ControlModifier && key == Qt::Key_S)
        saveFile(currentFilePath);

    if (modifier == Qt::ControlModifier && key == Qt::Key_Q)
        exit(EXIT_SUCCESS);

    if (modifier == Qt::NoModifier && key == Qt::Key_Escape)
        toggleFocus();
}

MainWindow::~MainWindow() {}
