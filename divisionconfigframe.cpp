#include "divisionconfigframe.h"
#include "ui_divisionconfigframe.h"
#include "divisionmodule.h"

DivisionConfigFrame::DivisionConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::DivisionConfigFrame)
{
    ui->setupUi(this);
    //ui->minNumberLineEdit->setText(QString("2"));
    //ui->maxNumberLineEdit->setText(QString("100"));
    this->module = 0;
}

DivisionConfigFrame::~DivisionConfigFrame()
{
    delete ui;
}

void DivisionConfigFrame::setModule(DivisionModule *mod)
{
    this->module = mod;
}

void DivisionConfigFrame::setFirstMinimum(quint64 min)
{
    ui->minNumberLineEdit->setText(QString("%1").arg(min));
}

void DivisionConfigFrame::setFirstMaximum(quint64 max)
{
    ui->maxNumberLineEdit->setText(QString("%1").arg(max));
}

void DivisionConfigFrame::setLastMinimum(quint64 max)
{
    ui->secondMinLineEdit->setText(QString("%1").arg(max));
}

void DivisionConfigFrame::setLastMaximum(quint64 max)
{
    ui->secondMaxLineEdit->setText(QString("%1").arg(max));
}

void DivisionConfigFrame::setLargestNumberFirst(bool b)
{
    ui->largestNumberFirstCheckBox->setChecked(b);
}

void DivisionConfigFrame::setDecimalPlaces(quint32 decimalPlaces)
{
    ui->decimalPlacesLineEdit->setText(QString("%1").arg(decimalPlaces));
}

void DivisionConfigFrame::on_minNumberLineEdit_editingFinished()
{
    int newMin = ui->minNumberLineEdit->text().toInt();
    this->module->setFirstMinimum(newMin);
}

void DivisionConfigFrame::on_maxNumberLineEdit_editingFinished()
{
    int newMax = ui->maxNumberLineEdit->text().toInt();
    this->module->setFirstMaximum(newMax);
}

void DivisionConfigFrame::on_secondMinLineEdit_editingFinished()
{
    int newMin = ui->secondMinLineEdit->text().toInt();
    this->module->setLastMinimum(newMin);
}

void DivisionConfigFrame::on_secondMaxLineEdit_editingFinished()
{
    int newMax = ui->secondMaxLineEdit->text().toInt();
    this->module->setLastMaximum(newMax);
}

void DivisionConfigFrame::on_largestNumberFirstCheckBox_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        this->module->setLargestNumberFirst(true);
    } else {
        this->module->setLargestNumberFirst(false);
    }
}

void DivisionConfigFrame::on_decimalPlacesLineEdit_editingFinished()
{
    quint32 newDecimals = ui->decimalPlacesLineEdit->text().toUInt();
    this->module->setDecimalPlaces(newDecimals);
}
