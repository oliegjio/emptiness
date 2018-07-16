#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QTextStream>
#include <QCoreApplication>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    QWidget* centralWidget;
    QVBoxLayout* layout;
    QPlainTextEdit* editor;
    QLineEdit* title;
    QString filePath = "";

    void init();
    void openInitialFile();
    bool updateFilePath();
    void loadFile(QPlainTextEdit* editor, QString path);
    void saveFile(QPlainTextEdit* editor, QString path);

    void keyPressEvent(QKeyEvent* event);
};

#endif // MAINWINDOW_H
