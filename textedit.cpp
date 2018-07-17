#include "textedit.h"

TextEdit::TextEdit() : QPlainTextEdit() {}
TextEdit::~TextEdit() {}

void TextEdit::handleTabKey()
{
    QTextCursor cursor = textCursor();
    cursor.insertText("    ");
    setTextCursor(cursor);
}

void TextEdit::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    int modifier = event->modifiers();

    if (modifier == Qt::NoModifier && key == Qt::Key_Tab)
    {
        handleTabKey();
        return;
    }

    return QPlainTextEdit::keyPressEvent(event);
}
