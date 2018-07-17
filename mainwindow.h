#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTextStream>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>

#include "textedit.h"
#include "lineedit.h"
#include "singleapplication.h"

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
    LineEdit* titleBar;
    QString currentFileAbsolutePath = "";
    SingleApplication* singleApplication;

    void init();

    void openFile(const QString& path);
    void saveFile(const QString& path);
    void createPath(const QString& path);
    void toggleFocus();

    QString getAbsoluteFilePath(const QString& path);
    QString getAbsolutePathFromArguments();

    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void updateWithNewPath(const QString& path);
    void returnPressed();
};

#endif // MAINWINDOW_H
