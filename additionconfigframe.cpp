#include "additionconfigframe.h"
#include "ui_additionconfigframe.h"
#include "additionmodule.h"
#include "qbigfixedvalidator.h"
#include "bigfixedpoint.h"

AdditionConfigFrame::AdditionConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::AdditionConfigFrame)
{
    ui->setupUi(this);
    this->module = 0;

    QBigFixedValidator *numValidator = new QBigFixedValidator(this);

    ui->minNumberLineEdit->setValidator(numValidator);
    ui->maxNumberLineEdit->setValidator(numValidator);
    ui->secondMinLineEdit->setValidator(numValidator);
    ui->secondMaxLineEdit->setValidator(numValidator);
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

bool AdditionConfigFrame::applyConfig()
{
    int largestNumberFirst = ui->largestNumberFirstCheckBox->isChecked();

    QString str = ui->minNumberLineEdit->text();
    BigFixedPoint firstMin(str.remove(QLocale::system().groupSeparator()));

    str = ui->maxNumberLineEdit->text();
    BigFixedPoint firstMax(str.remove(QLocale::system().groupSeparator()));

    str = ui->secondMinLineEdit->text();
    BigFixedPoint lastMin(str.remove(QLocale::system().groupSeparator()));

    str = ui->secondMaxLineEdit->text();
    BigFixedPoint lastMax(str.remove(QLocale::system().groupSeparator()));

    if (firstMax < firstMin)
    {
        QMessageBox::warning(this, tr("Range Validation Error"), tr("Maximum of first number is smaller than the minimum."), QMessageBox::Ok);
        return false;
    } else if (lastMax < lastMin) {
        QMessageBox::warning(this, tr("Range Validation Error"), tr("Maximum of last number is smaller than the minimum."), QMessageBox::Ok);
        return false;
    } else {
        module->setSettings(firstMin, firstMax, lastMin, lastMax,
                            largestNumberFirst);
    }

    return true;
}
