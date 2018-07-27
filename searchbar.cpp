#include "searchbar.h"

SearchBar::SearchBar(QWidget *parent) : QLineEdit(parent) {}

SearchBar::~SearchBar() {}

void SearchBar::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    int modifier = event->modifiers();

    if (modifier == Qt::ShiftModifier && key == Qt::Key_Return)
    {
        emit shiftEnterPressed(text());
        return;
    }
    else if (modifier == Qt::ShiftModifier || modifier == Qt::ControlModifier || modifier == Qt::AltModifier)
    {
        return;
    }
    else
    {
        QLineEdit::keyPressEvent(event);
        emit keyPress(text());
    }
}

void SearchBar::focusOutEvent(QFocusEvent *event)
{
    emit focusOut();
    QLineEdit::focusOutEvent(event);
}
