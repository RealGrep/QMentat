#include "powersconfigframe.h"
#include "ui_powersconfigframe.h"

#include "bigfixedpoint.h"
#include "qbigfixedvalidator.h"

PowersConfigFrame::PowersConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::PowersConfigFrame)
{
    ui->setupUi(this);

    QBigFixedValidator *bfv = new QBigFixedValidator(
            BigFixedPoint(QString("-9999999999999999999999999999")),
            BigFixedPoint(QString("9999999999999999999999999999")),
            this);

    ui->minNumberLineEdit->setValidator(bfv);
    ui->maxNumberLineEdit->setValidator(bfv);

    QIntValidator *powerValidator = new QIntValidator(0, 1000, this);
    ui->maxPowerLineEdit->setValidator(powerValidator);
    ui->minPowerLineEdit->setValidator(powerValidator);

    QIntValidator *decimalsValidator = new QIntValidator(0, 100, this);
    ui->decimalPlacesLineEdit->setValidator(decimalsValidator);

    this->module = 0;
}

PowersConfigFrame::~PowersConfigFrame()
{
    delete ui;
}

void PowersConfigFrame::setModule(PowersModule *mod)
{
    this->module = mod;
}

void PowersConfigFrame::setMinimum(QString min)
{
    ui->minNumberLineEdit->setText(min);
}

void PowersConfigFrame::setMaximum(QString max)
{
    ui->maxNumberLineEdit->setText(max);
}

void PowersConfigFrame::setPowerMinimum(int min)
{
    ui->minPowerLineEdit->setText(QString("%1").arg(min));
}

void PowersConfigFrame::setPowerMaximum(int max)
{
    ui->maxPowerLineEdit->setText(QString("%1").arg(max));
}

void PowersConfigFrame::setDecimalPlaces(int decimals)
{
    ui->decimalPlacesLineEdit->setText(QString("%1").arg(decimals));
}

void PowersConfigFrame::setRoundingMode(int mode)
{
    ui->roundingComboBox->setCurrentIndex(mode);
}

void PowersConfigFrame::on_minNumberLineEdit_editingFinished()
{
    BigFixedPoint newMin(ui->minNumberLineEdit->text());
    this->module->setMinimum(newMin);
}

void PowersConfigFrame::on_maxNumberLineEdit_editingFinished()
{
    BigFixedPoint newMax(ui->maxNumberLineEdit->text());
    this->module->setMaximum(newMax);
}

void PowersConfigFrame::on_minPowerLineEdit_editingFinished()
{
    int newMin = ui->minPowerLineEdit->text().toInt();
    this->module->setPowerMinimum(newMin);
}

void PowersConfigFrame::on_maxPowerLineEdit_editingFinished()
{
    int newMax = ui->maxPowerLineEdit->text().toInt();
    this->module->setPowerMaximum(newMax);
}

void PowersConfigFrame::on_decimalPlacesLineEdit_editingFinished()
{
    int newDecimals = ui->decimalPlacesLineEdit->text().toInt();
    this->module->setDecimalPlaces(newDecimals);
}

void PowersConfigFrame::on_roundingComboBox_currentIndexChanged(int index)
{
    this->module->setRoundingMode(index == 1);
}
