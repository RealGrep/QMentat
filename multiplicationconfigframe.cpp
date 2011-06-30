#include "multiplicationconfigframe.h"
#include "ui_multiplicationconfigframe.h"
#include "multiplicationmodule.h"

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

void MultiplicationConfigFrame::setFirstMinimum(quint64 min)
{
    ui->minNumberLineEdit->setText(QString("%1").arg(min));
}

void MultiplicationConfigFrame::setFirstMaximum(quint64 max)
{
    ui->maxNumberLineEdit->setText(QString("%1").arg(max));
}

void MultiplicationConfigFrame::setLastMinimum(quint64 max)
{
    ui->secondMinLineEdit->setText(QString("%1").arg(max));
}

void MultiplicationConfigFrame::setLastMaximum(quint64 max)
{
    ui->secondMaxLineEdit->setText(QString("%1").arg(max));
}

void MultiplicationConfigFrame::setLargestNumberFirst(bool b)
{
    ui->largestNumberFirstCheckBox->setChecked(b);
}

void MultiplicationConfigFrame::setDecimalPlaces(quint32 decimalPlaces)
{
    ui->decimalPlacesLineEdit->setText(QString("%1").arg(decimalPlaces));
}

void MultiplicationConfigFrame::on_minNumberLineEdit_editingFinished()
{
    int newMin = ui->minNumberLineEdit->text().toInt();
    this->module->setFirstMinimum(newMin);
}

void MultiplicationConfigFrame::on_maxNumberLineEdit_editingFinished()
{
    int newMax = ui->maxNumberLineEdit->text().toInt();
    this->module->setFirstMaximum(newMax);
}

void MultiplicationConfigFrame::on_secondMinLineEdit_editingFinished()
{
    int newMin = ui->secondMinLineEdit->text().toInt();
    this->module->setLastMinimum(newMin);
}

void MultiplicationConfigFrame::on_secondMaxLineEdit_editingFinished()
{
    int newMax = ui->secondMaxLineEdit->text().toInt();
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
    quint32 newDecimals = ui->decimalPlacesLineEdit->text().toUInt();
    this->module->setDecimalPlaces(newDecimals);
}
