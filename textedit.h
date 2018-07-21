#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QKeyEvent>

class TextEdit : public QPlainTextEdit
{
public:
    TextEdit();
    ~TextEdit();

private:
    void handleTabKeyPress();
    void handleControlAltTabKeyPress();

    void keyPressEvent(QKeyEvent* event) override;
};

#endif // PLAINTEXTEDIT_H
