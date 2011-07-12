#ifndef QUESTIONDISPLAYFORM_H
#define QUESTIONDISPLAYFORM_H

#include <QWidget>
#include <QPaintEvent>
#include <QString>
#include "questiondisplay.h"

class QFont;

namespace Ui {
    class QuestionDisplayForm;
}

class QuestionDisplayForm : public QuestionDisplay
{
    Q_OBJECT

public:
    explicit QuestionDisplayForm(QWidget *parent = 0);
    ~QuestionDisplayForm();
    void paintEvent(QPaintEvent *);

    void setText(QString text);

private:
    Ui::QuestionDisplayForm *ui;

    QFont displayFont;
    int widestChar;

    QString first;
    QString last;
    QChar operation;
};

#endif // QUESTIONDISPLAYFORM_H
