#include "textedit.h"

#include <QTextBlock>
#include <QVector>

TextEdit::TextEdit() {}

#include <QDebug>

void TextEdit::handleTabKeyPress()
{
    QTextCursor cursor = textCursor();
    cursor.insertText(spaceTab);
    setTextCursor(cursor);
}

void TextEdit::handleControlAltTabKeyPress()
{
    QTextCursor cursor = textCursor();
    cursor.insertText(tabTab);
    setTextCursor(cursor);
}

void TextEdit::handleControlTabKeyPress()
{
    QTextCursor cursor = textCursor();
    QString text = toPlainText();
    int position = cursor.position();
    int newLinePosition = 0;
    bool onNewLine = text[position] == '\n' ? true : false;
    for (int i = position; i > 0; i--)
    {
        if (text[i] == '\n')
        {
            if (onNewLine)
            {
                onNewLine = !onNewLine;
                continue;
            }
            else
            {
                newLinePosition = i + 1;
                break;
            }
        }
    }
    cursor.setPosition(newLinePosition);
    cursor.insertText(spaceTab);
    cursor.setPosition(position + spaceTab.length());
    setTextCursor(cursor);
}

void TextEdit::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    Qt::KeyboardModifiers modifier = event->modifiers();


    switch (key)
    {
        case Qt::Key_Tab: {
            if (modifier.testFlag(Qt::ControlModifier) &&
                modifier.testFlag(Qt::AltModifier))
            {
                handleControlAltTabKeyPress();
                return;
            }
            else if (modifier.testFlag(Qt::ControlModifier))
            {
                handleControlTabKeyPress();
                return;
            }
            else
            {
                handleTabKeyPress();
                return;
            }
        }
    }

    return QPlainTextEdit::keyPressEvent(event);
}
