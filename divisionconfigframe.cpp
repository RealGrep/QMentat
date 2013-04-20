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
#include "divisionconfigframe.h"
#include "ui_divisionconfigframe.h"
#include "divisionmodule.h"

#include <limits>
#include <QtGui>
#include <QIntValidator>

#include "bigfixedpoint.h"
#include "qbigfixedvalidator.h"

DivisionConfigFrame::DivisionConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::DivisionConfigFrame)
{
    ui->setupUi(this);

    numValidator = new QBigFixedValidator(this);

    ui->minNumberLineEdit->setValidator(numValidator);
    ui->maxNumberLineEdit->setValidator(numValidator);
    ui->secondMinLineEdit->setValidator(numValidator);
    ui->secondMaxLineEdit->setValidator(numValidator);

    QIntValidator *intValidator = new QIntValidator(0, 100, this);
    ui->decimalPlacesLineEdit->setValidator(intValidator);

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

void DivisionConfigFrame::setFirstMinimum(const QString& min)
{
    ui->minNumberLineEdit->setText(min);
}

void DivisionConfigFrame::setFirstMaximum(const QString& max)
{
    ui->maxNumberLineEdit->setText(max);
}

void DivisionConfigFrame::setLastMinimum(const QString& max)
{
    ui->secondMinLineEdit->setText(max);
}

void DivisionConfigFrame::setLastMaximum(const QString& max)
{
    ui->secondMaxLineEdit->setText(max);
}

void DivisionConfigFrame::setLargestNumberFirst(bool b)
{
    ui->largestNumberFirstCheckBox->setChecked(b);
}

void DivisionConfigFrame::setDecimalPlaces(int decimalPlaces)
{
    ui->decimalPlacesLineEdit->setText(QString("%1").arg(decimalPlaces));
}

void DivisionConfigFrame::setRoundingMode(RoundingMode mode)
{
    ui->roundingComboBox->setCurrentIndex(mode);
}

void DivisionConfigFrame::setIntegersOnly(bool intsOnly)
{
    using namespace std;

    bool intsOnlyState = ui->integerResultCheckBox->isChecked();
    if (intsOnlyState != intsOnly)
    {
        ui->integerResultCheckBox->setChecked(intsOnly);
    }
    ui->decimalPlacesLineEdit->setEnabled(!intsOnly);
    ui->roundingComboBox->setEnabled(!intsOnly);
    ui->largestNumberFirstCheckBox->setEnabled(!intsOnly);

    // Ensure orderly transition from different ints mode
    if (intsOnly)
    {
        QString str = ui->minNumberLineEdit->text();
        BigFixedPoint oldFirstMin(str.remove(QLocale::system().groupSeparator()));
        qint64 num;
        if (!oldFirstMin.fitsInLongLong())
        {
            num = 1000000000000LL;
        } else {
            num = oldFirstMin.toLongLong();
        }
        ui->minNumberLineEdit->setText(QString("%1").arg(num));

        str = ui->maxNumberLineEdit->text();
        BigFixedPoint oldFirstMax(str.remove(QLocale::system().groupSeparator()));
        if (!oldFirstMax.fitsInLongLong())
        {
            num = 1000000000000LL;
        } else {
            num = oldFirstMax.toLongLong();
        }
        ui->maxNumberLineEdit->setText(QString("%1").arg(num));

        str = ui->secondMinLineEdit->text();
        BigFixedPoint oldLastMin(str.remove(QLocale::system().groupSeparator()));
        if (!oldLastMin.fitsInLongLong())
        {
            num = 1000000000000LL;
        } else {
            num = oldLastMin.toLongLong();
        }
        ui->secondMinLineEdit->setText(QString("%1").arg(num));

        str = ui->secondMaxLineEdit->text();
        BigFixedPoint oldLastMax(str.remove(QLocale::system().groupSeparator()));
        if (!oldLastMax.fitsInLongLong())
        {
            num = 1000000000000LL;
        } else {
            num = oldLastMax.toLongLong();
        }
        ui->secondMaxLineEdit->setText(QString("%1").arg(num));

        numValidator->setRange(
                BigFixedPoint(QString("%1")
                              .arg(std::numeric_limits<qint64>::min())),
                BigFixedPoint(QString("%1")
                              .arg(std::numeric_limits<qint64>::max())));
    } else {
        numValidator->setRangeEnabled(false);
    }
}

void DivisionConfigFrame::on_integerResultCheckBox_stateChanged(int state)
{
    using namespace std;
    bool intsOnly = (state == Qt::Checked);
    this->setIntegersOnly(intsOnly);
}

bool DivisionConfigFrame::applyConfig()
{
    int integersOnly = ui->integerResultCheckBox->isChecked();
    int rounding = ui->roundingComboBox->currentIndex() == 1;
    int decimalPlaces = ui->decimalPlacesLineEdit->text().toInt();
    int largestNumberFirst = ui->largestNumberFirstCheckBox->isChecked();

    if (integersOnly)
    {
        QString str = ui->minNumberLineEdit->text();
        BigFixedPoint firstMin(str.remove(QLocale::system().groupSeparator()));
        qint64 firstMinIR = firstMin.toLongLong();

        str = ui->maxNumberLineEdit->text();
        BigFixedPoint firstMax(str.remove(QLocale::system().groupSeparator()));
        qint64 firstMaxIR = firstMax.toLongLong();

        str = ui->secondMinLineEdit->text();
        BigFixedPoint lastMin(str.remove(QLocale::system().groupSeparator()));
        qint64 lastMinIR = lastMin.toLongLong();

        str = ui->secondMaxLineEdit->text();
        BigFixedPoint lastMax(str.remove(QLocale::system().groupSeparator()));
        qint64 lastMaxIR = lastMax.toLongLong();

        bool rangeOk = module->isRangeOk(firstMinIR, firstMaxIR,
                                         lastMinIR, lastMaxIR);

        if (firstMaxIR < firstMinIR)
        {
            QMessageBox::warning(this, tr("Range Validation Error"),
                                 tr("Maximum of first number is smaller than "
                                    "the minimum."),
                                 QMessageBox::Ok);
            return false;
        } else if (lastMaxIR < lastMinIR) {
            QMessageBox::warning(this, tr("Range Validation Error"),
                                 tr("Maximum of last number is smaller than "
                                    "the minimum."),
                                 QMessageBox::Ok);
            return false;
        } else if (!rangeOk) {
            QMessageBox::warning(this, tr("Range Validation Error"),
                                 tr("Range of first number has no divisors "
                                    "within range of last number."),
                                 QMessageBox::Ok);
            return false;
        } else {
            module->setSettings(firstMinIR, firstMaxIR, lastMinIR, lastMaxIR,
                                largestNumberFirst, integersOnly,
                                decimalPlaces, rounding);
        }
    } else {
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
                                largestNumberFirst, integersOnly,
                                decimalPlaces, rounding);
        }
    }

    return true;
}
