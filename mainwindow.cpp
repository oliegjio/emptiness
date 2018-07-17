#include "mainwindow.h"

MainWindow::MainWindow(RunGuard* guard, QWidget *parent)
    : QMainWindow(parent)
    , guard(guard)
{
    editor = new PlainTextEdit();
    layout = new QVBoxLayout();
    centralWidget = new QWidget();
    title = new LineEdit();

    init();
    openInitialFile();
}

void MainWindow::init()
{
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);

    layout->setSpacing(0);
    layout->setMargin(0);

    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(":/fonts/ubuntu-mono-regular.ttf");

    QFont font;
    font.setFamily("Ubuntu Mono");
    font.setPointSize(12);
    editor->setFont(font);
    title->setFont(font);

    QPalette editorPalette = editor->palette();
    editorPalette.setColor(QPalette::Base, QColor(0, 0, 0));
    editorPalette.setColor(QPalette::Text, QColor(255, 255, 255));
    editor->setPalette(editorPalette);

    QPalette titlePalette = title->palette();
    titlePalette.setColor(QPalette::Base, QColor(50, 50, 50));
    titlePalette.setColor(QPalette::Text, QColor(255, 255, 255));
    title->setPalette(titlePalette);

    title->setFrame(false);
    editor->setFrameStyle(0);

    layout->addWidget(title);
    layout->addWidget(editor);

    editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    editor->setFocus();

    connect(guard, SIGNAL(sharedMemoryChanged(QString)), this, SLOT(setFilePath(QString)));
}

void MainWindow::setFilePath(const QString& path)
{
    filePath = path;
    title->setText(getAbsoluteFilePath(filePath));
    loadFile(editor, filePath);
}

void MainWindow::openInitialFile()
{
    if (QCoreApplication::arguments().length() == 1) return;

    filePath = QCoreApplication::arguments().at(1);
    title->setText(getAbsoluteFilePath(filePath));
    loadFile(editor, filePath);
}

void MainWindow::loadFile(QPlainTextEdit* editor, QString path)
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

void MainWindow::createPath(QString path)
{
    QFileInfo info(path);
    QString absoluteDirPath = info.absoluteDir().absolutePath();

    QDir directory(absoluteDirPath);
    directory.mkpath(absoluteDirPath);
}

void MainWindow::saveFile(QPlainTextEdit* editor, QString path)
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

bool MainWindow::updateFilePath()
{
    filePath = title->text();

    if (filePath == "")
    {
        title->setFocus();
        return false;
    }

    return true;
}

QString MainWindow::getAbsoluteFilePath(QString path)
{
    QFileInfo info(path);
    return info.absoluteFilePath();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();

    if (key == Qt::Key_S)
    {
        if (updateFilePath()) saveFile(editor, filePath);
    }
    else if (key == Qt::Key_Q)
    {
        exit(EXIT_SUCCESS);
    }
}

MainWindow::~MainWindow() {}
