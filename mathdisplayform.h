#ifndef MATHDISPLAYFORM_H
#define MATHDISPLAYFORM_H

#include <QWidget>
#include "questiondisplay.h"
#include <QtMmlWidget>

#include "preferenceslistener.h"

namespace Ui {
    class MathDisplayForm;
}

class MathDisplayForm : public QuestionDisplay, public PreferencesListener
{
    Q_OBJECT

public:
    explicit MathDisplayForm(QWidget *parent = 0);
    ~MathDisplayForm();

    void setText(QString text);

    void preferencesChanged();

private:
    Ui::MathDisplayForm *ui;
    QtMmlWidget *mml_widget;
};

#endif // MATHDISPLAYFORM_H
