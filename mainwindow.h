#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QTextStream>
#include <QCoreApplication>
#include <QLineEdit>
#include <QFileInfo>

#include "plaintextedit.h"
#include "lineedit.h"
#include "runguard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(RunGuard* guard, QWidget* parent = 0);
    ~MainWindow();

private:
    QWidget* centralWidget;
    QVBoxLayout* layout;
    PlainTextEdit* editor;
    LineEdit* title;
    QString filePath = "";
    RunGuard* guard;

    void init();
    void openInitialFile();
    bool updateFilePath();
    void loadFile(QPlainTextEdit* editor, QString path);
    void saveFile(QPlainTextEdit* editor, QString path);
    QString getAbsoluteFilePath(QString path);

    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void setFilePath(const QString& path);
};

#endif // MAINWINDOW_H
