#ifndef SUBTRACTIONCONFIGFRAME_H
#define SUBTRACTIONCONFIGFRAME_H

#include <QFrame>
#include "subtractionmodule.h"

namespace Ui {
    class SubtractionConfigFrame;
}

class SubtractionConfigFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SubtractionConfigFrame(QWidget *parent = 0);
    ~SubtractionConfigFrame();

    void setModule(SubtractionModule *mod);
    void setFirstMinimum(QString min);
    void setFirstMaximum(QString max);
    void setLastMinimum(QString min);
    void setLastMaximum(QString max);
    void setLargestNumberFirst(bool b);

    SubtractionModule *module;

private:
    Ui::SubtractionConfigFrame *ui;

private slots:
    void on_largestNumberFirstCheckBox_stateChanged(int );
    void on_secondMaxLineEdit_editingFinished();
    void on_secondMinLineEdit_editingFinished();
    void on_maxNumberLineEdit_editingFinished();
    void on_minNumberLineEdit_editingFinished();
};

#endif // SUBTRACTIONCONFIGFRAME_H
