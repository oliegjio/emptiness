#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QLineEdit>
#include <QKeyEvent>

class SearchBar : public QLineEdit
{
    Q_OBJECT

public:
    SearchBar(QWidget *parent = nullptr);
    ~SearchBar();

    void keyPressEvent(QKeyEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

signals:
    void returnPressed(QString string);
    void shiftReturnPressed(QString string);
    void focusOut();
};

#endif // SEARCHBAR_H
