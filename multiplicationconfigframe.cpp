#include "multiplicationconfigframe.h"
#include "ui_multiplicationconfigframe.h"

#include <QtGui>

#include "multiplicationmodule.h"
#include "bigfixedpoint.h"
#include "qbigfixedvalidator.h"

MultiplicationConfigFrame::MultiplicationConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::MultiplicationConfigFrame)
{
    ui->setupUi(this);

    QBigFixedValidator *numValidator = new QBigFixedValidator(this);

    ui->minNumberLineEdit->setValidator(numValidator);
    ui->maxNumberLineEdit->setValidator(numValidator);
    ui->secondMinLineEdit->setValidator(numValidator);
    ui->secondMaxLineEdit->setValidator(numValidator);

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

bool MultiplicationConfigFrame::applyConfig()
{
    int rounding = ui->roundingComboBox->currentIndex() == 1;
    int decimalPlaces = ui->decimalPlacesLineEdit->text().toInt();
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
                            largestNumberFirst, decimalPlaces, rounding);
    }

    return true;
}
