#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QKeyEvent>

class TextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    TextEdit();

private:
    void handleTabKeyPress();
    void handleControlAltTabKeyPress();

    void keyPressEvent(QKeyEvent* event) override;
};

#endif // PLAINTEXTEDIT_H
