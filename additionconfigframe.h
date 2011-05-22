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
    void setFirstMinimum(quint64 min);
    void setFirstMaximum(quint64 max);
    void setLastMinimum(quint64 min);
    void setLastMaximum(quint64 max);
    void setLargestNumberFirst(bool b);
    void setDecimalPlaces(quint32 decimalPlaces);

    AdditionModule *module;

private:
    Ui::AdditionConfigFrame *ui;

private slots:
    void on_decimalPlacesLineEdit_editingFinished();
    void on_largestNumberFirstCheckBox_stateChanged(int );
    void on_secondMaxLineEdit_editingFinished();
    void on_secondMinLineEdit_editingFinished();
    void on_maxNumberLineEdit_editingFinished();
    void on_minNumberLineEdit_editingFinished();
};

#endif // ADDITIONCONFIGFRAME_H

