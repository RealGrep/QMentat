#include "subtractionconfigframe.h"
#include "ui_subtractionconfigframe.h"
#include "subtractionmodule.h"
#include "bigfixedpoint.h"

SubtractionConfigFrame::SubtractionConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::SubtractionConfigFrame)
{
    ui->setupUi(this);
    this->module = 0;
}

SubtractionConfigFrame::~SubtractionConfigFrame()
{
    delete ui;
}

void SubtractionConfigFrame::setModule(SubtractionModule *mod)
{
    this->module = mod;
}

void SubtractionConfigFrame::setFirstMinimum(QString min)
{
    ui->minNumberLineEdit->setText(min);
}

void SubtractionConfigFrame::setFirstMaximum(QString max)
{
    ui->maxNumberLineEdit->setText(max);
}

void SubtractionConfigFrame::setLastMinimum(QString min)
{
    ui->secondMinLineEdit->setText(min);
}

void SubtractionConfigFrame::setLastMaximum(QString max)
{
    ui->secondMaxLineEdit->setText(max);
}

void SubtractionConfigFrame::setLargestNumberFirst(bool b)
{
    ui->largestNumberFirstCheckBox->setChecked(b);
}

void SubtractionConfigFrame::on_minNumberLineEdit_editingFinished()
{
    BigFixedPoint newMin(ui->minNumberLineEdit->text());
    this->module->setFirstMinimum(newMin);
}

void SubtractionConfigFrame::on_maxNumberLineEdit_editingFinished()
{
    BigFixedPoint newMax(ui->maxNumberLineEdit->text());
    this->module->setFirstMaximum(newMax);
}

void SubtractionConfigFrame::on_secondMinLineEdit_editingFinished()
{
    BigFixedPoint newMin(ui->secondMinLineEdit->text());
    this->module->setLastMinimum(newMin);
}

void SubtractionConfigFrame::on_secondMaxLineEdit_editingFinished()
{
    BigFixedPoint newMax(ui->secondMaxLineEdit->text());
    this->module->setLastMaximum(newMax);
}

void SubtractionConfigFrame::on_largestNumberFirstCheckBox_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        this->module->setLargestNumberFirst(true);
    } else {
        this->module->setLargestNumberFirst(false);
    }
}
