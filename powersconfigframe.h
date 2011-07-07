#ifndef POWERSCONFIGFRAME_H
#define POWERSCONFIGFRAME_H

#include <QFrame>
#include "powersmodule.h"

class QString;

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
    void setMinimum(QString min);
    void setMaximum(QString max);
    void setPowerMinimum(int min);
    void setPowerMaximum(int max);
    void setDecimalPlaces(int newDecimals);
    void setRoundingMode(int mode);

    PowersModule *module;

private:
    Ui::PowersConfigFrame *ui;

private slots:
    void on_roundingComboBox_currentIndexChanged(int index);
    void on_decimalPlacesLineEdit_editingFinished();
    void on_maxPowerLineEdit_editingFinished();
    void on_minPowerLineEdit_editingFinished();
    void on_maxNumberLineEdit_editingFinished();
    void on_minNumberLineEdit_editingFinished();
};

#endif // POWERSCONFIGFRAME_H
