#include "multiplicationconfigframe.h"
#include "ui_multiplicationconfigframe.h"
#include "multiplicationmodule.h"
#include "bigfixedpoint.h"
#include "qbigfixedvalidator.h"

MultiplicationConfigFrame::MultiplicationConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::MultiplicationConfigFrame)
{
    ui->setupUi(this);

    QBigFixedValidator *bfv = new QBigFixedValidator(
            BigFixedPoint(QString("-9999999999999999999999999999")),
            BigFixedPoint(QString("9999999999999999999999999999")),
            this);

    ui->minNumberLineEdit->setValidator(bfv);
    ui->maxNumberLineEdit->setValidator(bfv);
    ui->secondMinLineEdit->setValidator(bfv);
    ui->secondMaxLineEdit->setValidator(bfv);


    QIntValidator *intValidator = new QIntValidator(0, 100, this);
    ui->decimalPlacesLineEdit->setValidator(intValidator);

    this->module = 0;
}

MultiplicationConfigFrame::~MultiplicationConfigFrame()
{
    delete ui;
}

void MultiplicationConfigFrame::setModule(MultiplicationModule *mod)
{
    this->module = mod;
}

void MultiplicationConfigFrame::setFirstMinimum(QString min)
{
    ui->minNumberLineEdit->setText(min);
}

void MultiplicationConfigFrame::setFirstMaximum(QString max)
{
    ui->maxNumberLineEdit->setText(max);
}

void MultiplicationConfigFrame::setLastMinimum(QString min)
{
    ui->secondMinLineEdit->setText(min);
}

void MultiplicationConfigFrame::setLastMaximum(QString max)
{
    ui->secondMaxLineEdit->setText(max);
}

void MultiplicationConfigFrame::setLargestNumberFirst(bool b)
{
    ui->largestNumberFirstCheckBox->setChecked(b);
}

void MultiplicationConfigFrame::setDecimalPlaces(int decimalPlaces)
{
    ui->decimalPlacesLineEdit->setText(QString("%1").arg(decimalPlaces));
}

void MultiplicationConfigFrame::setRoundingMode(int mode)
{
    ui->roundingComboBox->setCurrentIndex(mode);
}

void MultiplicationConfigFrame::on_minNumberLineEdit_editingFinished()
{
    BigFixedPoint newMin(ui->minNumberLineEdit->text());
    this->module->setFirstMinimum(newMin);
}

void MultiplicationConfigFrame::on_maxNumberLineEdit_editingFinished()
{
    BigFixedPoint newMax(ui->maxNumberLineEdit->text());
    this->module->setFirstMaximum(newMax);
}

void MultiplicationConfigFrame::on_secondMinLineEdit_editingFinished()
{
    BigFixedPoint newMin(ui->secondMinLineEdit->text());
    this->module->setLastMinimum(newMin);
}

void MultiplicationConfigFrame::on_secondMaxLineEdit_editingFinished()
{
    BigFixedPoint newMax(ui->secondMaxLineEdit->text());
    this->module->setLastMaximum(newMax);
}

void MultiplicationConfigFrame::on_largestNumberFirstCheckBox_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        this->module->setLargestNumberFirst(true);
    } else {
        this->module->setLargestNumberFirst(false);
    }
}

void MultiplicationConfigFrame::on_decimalPlacesLineEdit_editingFinished()
{
    int newDecimals = ui->decimalPlacesLineEdit->text().toInt();
    this->module->setDecimalPlaces(newDecimals);
}

void MultiplicationConfigFrame::on_roundingComboBox_currentIndexChanged(int index)
{
    this->module->setRoundingMode(index == 1);
}
