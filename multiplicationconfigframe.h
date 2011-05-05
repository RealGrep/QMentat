#ifndef MULTIPLICATIONCONFIGFRAME_H
#define MULTIPLICATIONCONFIGFRAME_H

#include <QFrame>
#include "multiplicationmodule.h"

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
    void setFirstMinimum(int min);
    void setFirstMaximum(int max);
    void setLastMinimum(int min);
    void setLastMaximum(int max);
    void setLargestNumberFirst(bool b);

    MultiplicationModule *module;

private:
    Ui::MultiplicationConfigFrame *ui;

private slots:
    void on_largestNumberFirstCheckBox_stateChanged(int );
    void on_secondMaxLineEdit_editingFinished();
    void on_secondMinLineEdit_editingFinished();
    void on_maxNumberLineEdit_editingFinished();
    void on_minNumberLineEdit_editingFinished();
};

#endif // MULTIPLICATIONCONFIGFRAME_H
