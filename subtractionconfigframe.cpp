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

void SubtractionConfigFrame::setFirstMinimum(const QString& min)
{
    ui->minNumberLineEdit->setText(min);
}

void SubtractionConfigFrame::setFirstMaximum(const QString& max)
{
    ui->maxNumberLineEdit->setText(max);
}

void SubtractionConfigFrame::setLastMinimum(const QString& min)
{
    ui->secondMinLineEdit->setText(min);
}

void SubtractionConfigFrame::setLastMaximum(const QString& max)
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

