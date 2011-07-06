#ifndef MULTIPLICATIONCONFIGFRAME_H
#define MULTIPLICATIONCONFIGFRAME_H

#include "multiplicationmodule.h"

class QFrame;
class QString;

namespace Ui {
    class MultiplicationConfigFrame;
}

class MultiplicationConfigFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MultiplicationConfigFrame(QWidget *parent = 0);
    ~MultiplicationConfigFrame();

    void setModule(MultiplicationModule *mod);
    void setFirstMinimum(QString min);
    void setFirstMaximum(QString max);
    void setLastMinimum(QString min);
    void setLastMaximum(QString max);
    void setLargestNumberFirst(bool b);
    void setDecimalPlaces(int decimalPlaces);

    MultiplicationModule *module;

private:
    Ui::MultiplicationConfigFrame *ui;

private slots:
    void on_decimalPlacesLineEdit_editingFinished();
    void on_largestNumberFirstCheckBox_stateChanged(int );
    void on_secondMaxLineEdit_editingFinished();
    void on_secondMinLineEdit_editingFinished();
    void on_maxNumberLineEdit_editingFinished();
    void on_minNumberLineEdit_editingFinished();
};

#endif // MULTIPLICATIONCONFIGFRAME_H
