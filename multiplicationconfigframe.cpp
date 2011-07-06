#include "multiplicationconfigframe.h"
#include "ui_multiplicationconfigframe.h"
#include "multiplicationmodule.h"
#include "bigfixedpoint.h"

MultiplicationConfigFrame::MultiplicationConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::MultiplicationConfigFrame)
{
    ui->setupUi(this);
    //ui->minNumberLineEdit->setText(tr("2"));
    //ui->maxNumberLineEdit->setText(tr("100"));
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
