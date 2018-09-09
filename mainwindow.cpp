#include "mainwindow.h"

#include <QApplication>

MainWindow::MainWindow(SingleApplication* singleApplication, QWidget *parent)
    : QMainWindow(parent)
    , singleApplication(singleApplication)
{
    editor = new TextEdit();
    layout = new QVBoxLayout();
    centralWidget = new QWidget();
    titleBar = new TitleBar();
    searchBar = new SearchBar();

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

    QPalette editorPalette = editor->palette();
    editorPalette.setColor(QPalette::Base, QColor(0, 0, 0));
    editorPalette.setColor(QPalette::Text, QColor(255, 255, 255));
    editor->setPalette(editorPalette);

    QPalette titlePalette = titleBar->palette();
    titlePalette.setColor(QPalette::Base, QColor(50, 50, 50));
    titlePalette.setColor(QPalette::Text, QColor(255, 255, 255));
    titleBar->setPalette(titlePalette);

    QPalette searchPalette = searchBar->palette();
    searchPalette.setColor(QPalette::Base, QColor(50, 50, 100));
    searchPalette.setColor(QPalette::Text, QColor(255, 255, 255));
    searchBar->setPalette(searchPalette);

    titleBar->setFrame(false);
    searchBar->setFrame(false);
    editor->setFrameStyle(0);

    layout->addWidget(titleBar);
    layout->addWidget(editor);
    layout->addWidget(searchBar);

    editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    editor->setFocus();
    searchBar->hide();

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
    connect(titleBar, &TitleBar::returnPressed, this, &MainWindow::returnPressed);
    connect(searchBar, &SearchBar::returnPressed, this, &MainWindow::searchForward);
    connect(searchBar, &SearchBar::shiftReturnPressed, this, &MainWindow::searchBackward);
    connect(searchBar, &SearchBar::focusOut, this, &MainWindow::toggleSearch);
}

void MainWindow::increaseFontSize()
{
    QFont font = QApplication::font();
    font.setPointSize(font.pointSize() + 1);
    QApplication::setFont(font);
}

void MainWindow::decreaseFontSize()
{
    QFont font = QApplication::font();
    font.setPointSize(font.pointSize() - 1);
    QApplication::setFont(font);
}

void MainWindow::searchForward(QString string)
{
    QTextDocument *document = editor->document();
    QTextCursor cursor = editor->textCursor();

    QTextCursor findCursor = document->find(string, cursor);

    if (!findCursor.isNull()) editor->setTextCursor(findCursor);

    oldSearchQuery = string;
}

void MainWindow::searchBackward(QString string)
{
    QTextDocument *document = editor->document();
    QTextCursor cursor = editor->textCursor();

    QTextCursor findCursor = document->find(string, cursor, QTextDocument::FindBackward);

    if (!findCursor.isNull()) editor->setTextCursor(findCursor);

    oldSearchQuery = string;
}

void MainWindow::returnPressed()
{
    editor->setFocus();
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

    editor->clear();
    if (!file.exists(path)) return;

    QString text;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            text = stream.readAll();
            editor->document()->setPlainText(text);

            QTextCursor cursor = editor->textCursor();
            cursor.setPosition(0);
            editor->setTextCursor(cursor);
        }
    }
}

void MainWindow::createPath(const QString& path)
{
    QFileInfo info(path);
    QString absoluteDirPath = info.absoluteDir().absolutePath();

    QDir directory(absoluteDirPath);
    directory.mkpath(absoluteDirPath);
}

void MainWindow::saveFile(const QString& path)
{
    QFile file(path);

    createPath(path);

    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
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
    if (focusWidget() == searchBar) editor->setFocus();
}

void MainWindow::insertSourceHash()
{
    char buffer[10];
    randomString(buffer, sizeof(buffer));
    QString r(buffer);

    QTextCursor cursor = editor->textCursor();
    cursor.insertText("[%" + r + "%]");
    editor->setTextCursor(cursor);

    clipboard->setText("[#" + r + "#]");
}

void MainWindow::toggleSearch()
{
    if (searchBarActive)
    {
        searchBar->hide();
        editor->setFocus();
        searchBarActive = false;
    }
    else
    {
        searchBar->show();
        searchBar->setFocus();
        searchBarActive = true;
    }
    oldSearchQuery = "";
    searchBar->setText(editor->textCursor().selection().toPlainText());
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    int modifier = event->modifiers();

    if (modifier == Qt::ControlModifier && key == Qt::Key_S)
    {
        currentFilePath = titleBar->text();
        saveFile(currentFilePath);
    }

    if (modifier == Qt::ControlModifier && key == Qt::Key_Q)
        exit(EXIT_SUCCESS);

    if (modifier == Qt::NoModifier && key == Qt::Key_Escape)
        toggleFocus();

    if (modifier == Qt::ControlModifier && key == Qt::Key_L)
        insertSourceHash();

    if (modifier == Qt::ControlModifier && key == Qt::Key_F)
        toggleSearch();

    if (modifier == Qt::ControlModifier && key == Qt::Key_Minus)
        decreaseFontSize();

    if (modifier == Qt::ControlModifier && key == Qt::Key_Equal)
        increaseFontSize();

    QMainWindow::keyPressEvent(event);
}

MainWindow::~MainWindow() {}
