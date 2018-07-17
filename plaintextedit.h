#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QPlainTextEdit>

class PlainTextEdit : public QPlainTextEdit
{
public:
    PlainTextEdit();

private:
    void handleTabKey();

    void keyPressEvent(QKeyEvent* event) override;
};

#endif // PLAINTEXTEDIT_H
