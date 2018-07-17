#include "plaintextedit.h"

PlainTextEdit::PlainTextEdit() {}

void PlainTextEdit::handleTabKey()
{
    QTextCursor cursor = textCursor();
    cursor.insertText("    ");
    setTextCursor(cursor);
}

void PlainTextEdit::keyPressEvent (QKeyEvent* event)
{
    int key = event->key();

    if (key == Qt::Key_Tab)
    {
        handleTabKey();
        return;
    }

    return QPlainTextEdit::keyPressEvent(event);
}
