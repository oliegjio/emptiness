#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class TitleBar : public QLineEdit
{
    Q_OBJECT

public:
    TitleBar();
    ~TitleBar();

private:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // LINEEDIT_H
