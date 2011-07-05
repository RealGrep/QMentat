#ifndef ADDITIONCONFIGFRAME_H
#define ADDITIONCONFIGFRAME_H

#include <QFrame>
#include "additionmodule.h"

class AdditionModule;

namespace Ui {
    class AdditionConfigFrame;
}

class AdditionConfigFrame : public QFrame
{
    Q_OBJECT

public:
    AdditionConfigFrame(QWidget *parent = 0);
    ~AdditionConfigFrame();

    void setModule(AdditionModule *mod);
    void setFirstMinimum(QString min);
    void setFirstMaximum(QString max);
    void setLastMinimum(QString min);
    void setLastMaximum(QString max);
    void setLargestNumberFirst(bool b);

    AdditionModule *module;

private:
    Ui::AdditionConfigFrame *ui;

private slots:
    void on_largestNumberFirstCheckBox_stateChanged(int );
    void on_secondMaxLineEdit_editingFinished();
    void on_secondMinLineEdit_editingFinished();
    void on_maxNumberLineEdit_editingFinished();
    void on_minNumberLineEdit_editingFinished();
};

#endif // ADDITIONCONFIGFRAME_H

