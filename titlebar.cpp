#include "titlebar.h"

TitleBar::TitleBar() {}

void TitleBar::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    unsigned int modifier = event->modifiers();

    if (modifier == Qt::NoModifier && key == Qt::Key_Tab)
        return;

    return QLineEdit::keyPressEvent(event);
}
