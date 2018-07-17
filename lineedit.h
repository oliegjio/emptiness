#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    LineEdit();
    ~LineEdit();

private:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // LINEEDIT_H
