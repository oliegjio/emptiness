#include "searchbar.h"

SearchBar::SearchBar(QWidget *parent) : QLineEdit(parent) {}

SearchBar::~SearchBar() {}

void SearchBar::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    int modifier = event->modifiers();

    if (modifier == Qt::ShiftModifier && key == Qt::Key_Return)
    {
        emit shiftEnterPressed(text(), true);
        return;
    }
    else if (modifier == Qt::NoModifier && key == Qt::Key_Return)
    {
        emit keyPress(text(), true);
        return;
    }
    else if (modifier == Qt::ShiftModifier || modifier == Qt::ControlModifier || modifier == Qt::AltModifier)
    {
        QLineEdit::keyPressEvent(event);
        return;
    }
    else
    {
        QLineEdit::keyPressEvent(event);
        emit keyPress(text(), false);
    }
}

void SearchBar::focusOutEvent(QFocusEvent *event)
{
    emit focusOut();
    QLineEdit::focusOutEvent(event);
}
