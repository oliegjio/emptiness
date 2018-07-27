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
    void keyPress(QString string, bool ignore);
    void shiftEnterPressed(QString string, bool ignore);
    void focusOut();
};

#endif // SEARCHBAR_H
