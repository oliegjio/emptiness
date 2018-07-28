#include "searchbar.h"

SearchBar::SearchBar(QWidget *parent) : QLineEdit(parent) {}

SearchBar::~SearchBar() {}

void SearchBar::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    int modifier = event->modifiers();

    if (modifier == Qt::ShiftModifier && key == Qt::Key_Return)
    {
        emit shiftReturnPressed(text());
        return;
    }
    else if (modifier == Qt::NoModifier && key == Qt::Key_Return)
    {
        emit returnPressed(text());
        return;
    }

    QLineEdit::keyPressEvent(event);
}

void SearchBar::focusOutEvent(QFocusEvent *event)
{
    emit focusOut();
    QLineEdit::focusOutEvent(event);
}
