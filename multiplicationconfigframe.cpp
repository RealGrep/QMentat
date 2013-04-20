/* Copyright (c) 2013 Mike Dusseault
 *
 * This file is part of QMentat.
 *
 * QMentat is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QMentat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QMentat.  If not, see <http://www.gnu.org/licenses/>.
 */
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

void MultiplicationConfigFrame::setFirstMinimum(const QString& min)
{
    ui->minNumberLineEdit->setText(min);
}

void MultiplicationConfigFrame::setFirstMaximum(const QString& max)
{
    ui->maxNumberLineEdit->setText(max);
}

void MultiplicationConfigFrame::setLastMinimum(const QString& min)
{
    ui->secondMinLineEdit->setText(min);
}

void MultiplicationConfigFrame::setLastMaximum(const QString& max)
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
