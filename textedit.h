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
    QString spaceTab = "    ";
    QString tabTab = QString(9);

    void handleTabKeyPress();
    void handleControlAltTabKeyPress();
    void handleControlTabKeyPress();

    void keyPressEvent(QKeyEvent* event) override;
};

#endif // PLAINTEXTEDIT_H
