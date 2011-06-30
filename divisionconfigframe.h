#ifndef DIVISIONCONFIGFRAME_H
#define DIVISIONCONFIGFRAME_H

#include <QFrame>
#include "divisionmodule.h"

namespace Ui {
    class DivisionConfigFrame;
}

class DivisionModule;

class DivisionConfigFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DivisionConfigFrame(QWidget *parent = 0);
    ~DivisionConfigFrame();

    void setModule(DivisionModule *mod);
    void setFirstMinimum(quint64 min);
    void setFirstMaximum(quint64 max);
    void setLastMinimum(quint64 min);
    void setLastMaximum(quint64 max);
    void setLargestNumberFirst(bool b);
    void setDecimalPlaces(quint32 decimalPlaces);

    DivisionModule *module;

private:
    Ui::DivisionConfigFrame *ui;

private slots:
    void on_decimalPlacesLineEdit_editingFinished();
    void on_largestNumberFirstCheckBox_stateChanged(int );
    void on_secondMaxLineEdit_editingFinished();
    void on_secondMinLineEdit_editingFinished();
    void on_maxNumberLineEdit_editingFinished();
    void on_minNumberLineEdit_editingFinished();
};

#endif // DIVISIONCONFIGFRAME_H
