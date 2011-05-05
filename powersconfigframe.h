#ifndef POWERSCONFIGFRAME_H
#define POWERSCONFIGFRAME_H

#include <QFrame>
#include "powersmodule.h"

namespace Ui {
    class PowersConfigFrame;
}

class PowersConfigFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PowersConfigFrame(QWidget *parent = 0);
    ~PowersConfigFrame();

    void setModule(PowersModule *mod);
    void setMinimum(int min);
    void setMaximum(int max);
    void setPowerMinimum(int min);
    void setPowerMaximum(int max);

    PowersModule *module;

private:
    Ui::PowersConfigFrame *ui;

private slots:
    void on_maxPowerLineEdit_editingFinished();
    void on_minPowerLineEdit_editingFinished();
    void on_maxNumberLineEdit_editingFinished();
    void on_minNumberLineEdit_editingFinished();
};

#endif // POWERSCONFIGFRAME_H
