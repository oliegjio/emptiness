#include "lineedit.h"

LineEdit::LineEdit() : QLineEdit() {}
LineEdit::~LineEdit() {}

void LineEdit::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    int modifier = event->modifiers();

    if (modifier == Qt::NoModifier && key == Qt::Key_Tab)
        return;

    return QLineEdit::keyPressEvent(event);
}
