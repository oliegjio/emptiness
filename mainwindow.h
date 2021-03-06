#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTextStream>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QClipboard>
#include <QTextDocumentFragment>

#include "textedit.h"
#include "titlebar.h"
#include "searchbar.h"
#include "singleapplication.h"
#include "randomstring.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(SingleApplication* singleApplication, QWidget* parent = nullptr);
    ~MainWindow();

private:
    QWidget* centralWidget;
    QVBoxLayout* layout;
    TextEdit* editor;
    TitleBar* titleBar;
    SearchBar* searchBar;
    QString currentFilePath = "";
    SingleApplication* singleApplication;
    QClipboard* clipboard;
    bool searchBarActive = false;
    QString oldSearchQuery = "s";

    void init();

    void openFile(const QString& path);
    void saveFile(const QString& path);
    void createPath(const QString& path);
    void newFile(const QString& path);
    void toggleFocus();
    QString toAbsolutePath(const QString& path);
    QString getAbsolutePathFromArguments();
    void insertSourceHash();

    void increaseFontSize();
    void decreaseFontSize();

    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void sharedMemoryForPathChanged(const QString& path);
    void sharedMemoryForWorkingDirectoryChanged(const QString& path);
    void returnPressed();
    void searchForward(QString string);
    void searchBackward(QString string);
    void toggleSearch();
};

#endif // MAINWINDOW_H
