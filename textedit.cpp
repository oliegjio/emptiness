#include "textedit.h"

TextEdit::TextEdit() : QPlainTextEdit() {}
TextEdit::~TextEdit() {}

void TextEdit::handleTabKeyPress()
{
    QTextCursor cursor = textCursor();
    cursor.insertText("    ");
    setTextCursor(cursor);
}

void TextEdit::handleControlAltTabKeyPress()
{
    QTextCursor cursor = textCursor();
    cursor.insertText(QString(9));
    setTextCursor(cursor);
}

void TextEdit::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    int modifier = event->modifiers();

    if (modifier == Qt::NoModifier && key == Qt::Key_Tab)
    {
        handleTabKeyPress();
        return;
    }
    else if (modifier == (Qt::ControlModifier | Qt::AltModifier) && key == Qt::Key_Tab)
    {
        handleControlAltTabKeyPress();
        return;
    }

    return QPlainTextEdit::keyPressEvent(event);
}
