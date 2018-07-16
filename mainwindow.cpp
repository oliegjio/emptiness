#include "mainwindow.h"

#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    editor = new QPlainTextEdit();
    layout = new QVBoxLayout();
    centralWidget = new QWidget();
    title = new QLineEdit();

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

    editor->setFocus();
}

void MainWindow::openInitialFile()
{
    if (QCoreApplication::arguments().length() == 1) return;

    filePath = QCoreApplication::arguments().at(1);
    title->setText(filePath);
    loadFile(editor, filePath);
}

void MainWindow::loadFile(QPlainTextEdit* editor, QString path)
{
    QFile file(path);

    if (!file.exists(path)) return;

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

void MainWindow::saveFile(QPlainTextEdit* editor, QString path)
{
    QFile file(path);

    if (!file.exists(path)) return;

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

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();

    if (key == Qt::Key_S)
    {
        if (updateFilePath()) saveFile(editor, filePath);
    }
}

MainWindow::~MainWindow() {}
