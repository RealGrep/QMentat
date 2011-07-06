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
    void setFirstMinimum(QString min);
    void setFirstMaximum(QString max);
    void setLastMinimum(QString min);
    void setLastMaximum(QString max);
    void setLargestNumberFirst(bool b);
    void setDecimalPlaces(int decimalPlaces);
    void setIntegersOnly(bool intsOnly);

    DivisionModule *module;

private:
    Ui::DivisionConfigFrame *ui;

private slots:
    void on_integerResultCheckBox_stateChanged(int );
    void on_decimalPlacesLineEdit_editingFinished();
    void on_largestNumberFirstCheckBox_stateChanged(int );
    void on_secondMaxLineEdit_editingFinished();
    void on_secondMinLineEdit_editingFinished();
    void on_maxNumberLineEdit_editingFinished();
    void on_minNumberLineEdit_editingFinished();
};

#endif // DIVISIONCONFIGFRAME_H
