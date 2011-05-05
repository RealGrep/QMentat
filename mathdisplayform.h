#ifndef MATHDISPLAYFORM_H
#define MATHDISPLAYFORM_H

#include <QWidget>
#include "questiondisplay.h"
#include <QtMmlWidget>

namespace Ui {
    class MathDisplayForm;
}

class MathDisplayForm : public QuestionDisplay
{
    Q_OBJECT

public:
    explicit MathDisplayForm(QWidget *parent = 0);
    ~MathDisplayForm();

    void setText(QString text);

private:
    Ui::MathDisplayForm *ui;
    QtMmlWidget *mml_widget;
};

#endif // MATHDISPLAYFORM_H
