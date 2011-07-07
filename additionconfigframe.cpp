#include "additionconfigframe.h"
#include "ui_additionconfigframe.h"
#include "additionmodule.h"
#include "qbigfixedvalidator.h"

AdditionConfigFrame::AdditionConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::AdditionConfigFrame)
{
    ui->setupUi(this);
    this->module = 0;

    QBigFixedValidator *bfv = new QBigFixedValidator(
            BigFixedPoint(QString("-9999999999999999999999999999")),
            BigFixedPoint(QString("9999999999999999999999999999")),
            this);

    ui->minNumberLineEdit->setValidator(bfv);
    ui->maxNumberLineEdit->setValidator(bfv);
    ui->secondMinLineEdit->setValidator(bfv);
    ui->secondMaxLineEdit->setValidator(bfv);

    /*
    QULongLongValidator *validator = new QULongLongValidator(this);
    ui->minNumberLineEdit->setValidator(validator);
    ui->maxNumberLineEdit->setValidator(validator);
    ui->secondMinLineEdit->setValidator(validator);
    ui->secondMaxLineEdit->setValidator(validator);
    */
}

AdditionConfigFrame::~AdditionConfigFrame()
{
    delete ui;
}

void AdditionConfigFrame::setModule(AdditionModule *mod)
{
    this->module = mod;
}

void AdditionConfigFrame::setFirstMinimum(QString min)
{
    ui->minNumberLineEdit->setText(min);
}

void AdditionConfigFrame::setFirstMaximum(QString max)
{
    ui->maxNumberLineEdit->setText(max);
}

void AdditionConfigFrame::setLastMinimum(QString min)
{
    ui->secondMinLineEdit->setText(min);
}

void AdditionConfigFrame::setLastMaximum(QString max)
{
    ui->secondMaxLineEdit->setText(max);
}

void AdditionConfigFrame::setLargestNumberFirst(bool b)
{
    ui->largestNumberFirstCheckBox->setChecked(b);
}

void AdditionConfigFrame::on_minNumberLineEdit_editingFinished()
{
    BigFixedPoint newMin(ui->minNumberLineEdit->text());
    this->module->setFirstMinimum(newMin);
}

void AdditionConfigFrame::on_maxNumberLineEdit_editingFinished()
{
    BigFixedPoint newMax(ui->maxNumberLineEdit->text());
    this->module->setFirstMaximum(newMax);
}

void AdditionConfigFrame::on_secondMinLineEdit_editingFinished()
{
    BigFixedPoint newMin(ui->secondMinLineEdit->text());
    this->module->setLastMinimum(newMin);
}

void AdditionConfigFrame::on_secondMaxLineEdit_editingFinished()
{
    BigFixedPoint newMax(ui->secondMaxLineEdit->text());
    this->module->setLastMaximum(newMax);
}

void AdditionConfigFrame::on_largestNumberFirstCheckBox_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        this->module->setLargestNumberFirst(true);
    } else {
        this->module->setLargestNumberFirst(false);
    }
}

