#ifndef QUESTIONDISPLAYFORM_H
#define QUESTIONDISPLAYFORM_H

#include <QWidget>
#include <QPaintEvent>
#include <QString>
#include "questiondisplay.h"

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
};

#endif // QUESTIONDISPLAYFORM_H
