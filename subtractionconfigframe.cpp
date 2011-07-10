#include "subtractionconfigframe.h"
#include "ui_subtractionconfigframe.h"
#include "subtractionmodule.h"
#include "bigfixedpoint.h"
#include "qbigfixedvalidator.h"

SubtractionConfigFrame::SubtractionConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::SubtractionConfigFrame)
{
    ui->setupUi(this);

    QBigFixedValidator *numValidator = new QBigFixedValidator(this);

    ui->minNumberLineEdit->setValidator(numValidator);
    ui->maxNumberLineEdit->setValidator(numValidator);
    ui->secondMinLineEdit->setValidator(numValidator);
    ui->secondMaxLineEdit->setValidator(numValidator);

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

bool SubtractionConfigFrame::applyConfig()
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

