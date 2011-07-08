#include "divisionconfigframe.h"
#include "ui_divisionconfigframe.h"
#include "divisionmodule.h"

#include <limits>
#include <QIntValidator>

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

void DivisionConfigFrame::setFirstMinimum(QString min)
{
    ui->minNumberLineEdit->setText(min);
}

void DivisionConfigFrame::setFirstMaximum(QString max)
{
    ui->maxNumberLineEdit->setText(max);
}

void DivisionConfigFrame::setLastMinimum(QString max)
{
    ui->secondMinLineEdit->setText(max);
}

void DivisionConfigFrame::setLastMaximum(QString max)
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

void DivisionConfigFrame::setRoundingMode(int mode)
{
    ui->roundingComboBox->setCurrentIndex(mode);
}

void DivisionConfigFrame::setIntegersOnly(bool intsOnly)
{
    ui->integerResultCheckBox->setChecked(intsOnly);
    ui->decimalPlacesLineEdit->setEnabled(!intsOnly);
    ui->roundingComboBox->setEnabled(!intsOnly);
    ui->largestNumberFirstCheckBox->setEnabled(!intsOnly);

    // Ensure orderly transition from different ints mode

    if (!this->module->isIntegersOnly() && intsOnly)
    {
        BigFixedPoint oldFirstMin(ui->minNumberLineEdit->text());
        this->module->setFirstMinimum(oldFirstMin.toLongLong());
        BigFixedPoint oldFirstMax(ui->maxNumberLineEdit->text());
        this->module->setFirstMaximum(oldFirstMax.toLongLong());
        BigFixedPoint oldLastMin(ui->secondMinLineEdit->text());
        this->module->setLastMinimum(oldLastMin.toLongLong());
        BigFixedPoint oldLastMax(ui->secondMaxLineEdit->text());
        this->module->setLastMaximum(oldLastMax.toLongLong());

        //qDebug() << QString("%1").arg(std::numeric_limits<qint64>::min())
        //        << QString("%1").arg(std::numeric_limits<qint64>::max());
        numValidator->setRange(
                BigFixedPoint(QString("%1")
                              .arg(std::numeric_limits<qint64>::min())),
                BigFixedPoint(QString("%1")
                              .arg(std::numeric_limits<qint64>::max())));

    } else if (this->module->isIntegersOnly() && !intsOnly) {
        QString str = ui->minNumberLineEdit->text();
        qint64 oldFirstMin = str.remove(QLocale::system().groupSeparator()).toLongLong();
        this->module->setFirstMinimum(BigFixedPoint(oldFirstMin));
        str = ui->maxNumberLineEdit->text();
        qint64 oldFirstMax = str.remove(QLocale::system().groupSeparator()).toLongLong();
        this->module->setFirstMaximum(BigFixedPoint(oldFirstMax));
        str = ui->secondMinLineEdit->text();
        qint64 oldLastMin = str.remove(QLocale::system().groupSeparator()).toLongLong();
        this->module->setLastMinimum(BigFixedPoint(oldLastMin));
        str = ui->secondMaxLineEdit->text();
        qint64 oldLastMax = str.remove(QLocale::system().groupSeparator()).toLongLong();
        this->module->setLastMaximum(BigFixedPoint(oldLastMax));

        numValidator->setRangeEnabled(false);
    }
}

void DivisionConfigFrame::on_minNumberLineEdit_editingFinished()
{
    if (this->module->isIntegersOnly())
    {
        qint64 newMin = ui->minNumberLineEdit->text().remove(QLocale::system().groupSeparator()).toLongLong();
        //qDebug() << "firstMin (configFrame) = " << ui->minNumberLineEdit->text() << "(" << newMin << ")";
        this->module->setFirstMinimum(newMin);
    } else {
        BigFixedPoint newMin(ui->minNumberLineEdit->text());
        this->module->setFirstMinimum(newMin);
    }
}

void DivisionConfigFrame::on_maxNumberLineEdit_editingFinished()
{
    //if (ui->integerResultCheckBox->isChecked())
    if (this->module->isIntegersOnly())
    {
        qint64 newMax = ui->maxNumberLineEdit->text().remove(QLocale::system().groupSeparator()).toLongLong();
        //qDebug() << "firstMax (configFrame) = " << ui->maxNumberLineEdit->text() << "(" << newMax << ")";
        this->module->setFirstMaximum(newMax);
    } else {
        BigFixedPoint newMax(ui->maxNumberLineEdit->text());
        this->module->setFirstMaximum(newMax);
    }
}

void DivisionConfigFrame::on_secondMinLineEdit_editingFinished()
{
    if (this->module->isIntegersOnly())
    //if (ui->integerResultCheckBox->isChecked())
    {
        qint64 newMin = ui->secondMinLineEdit->text().remove(QLocale::system().groupSeparator()).toLongLong();
        //qDebug() << "secondMin (configFrame) = " << ui->secondMinLineEdit->text() << "(" << newMin << ")";
        this->module->setLastMinimum(newMin);
    } else {
        BigFixedPoint newMin(ui->secondMinLineEdit->text());
        this->module->setLastMinimum(newMin);
    }
}

void DivisionConfigFrame::on_secondMaxLineEdit_editingFinished()
{
    if (this->module->isIntegersOnly())
    //if (ui->integerResultCheckBox->isChecked())
    {
        qint64 newMax = ui->secondMaxLineEdit->text().remove(QLocale::system().groupSeparator()).toLongLong();
        //qDebug() << "secondMax (configFrame) = " << ui->secondMaxLineEdit->text() << "(" << newMax << ")";
        this->module->setLastMaximum(newMax);
    } else {
        BigFixedPoint newMax(ui->secondMaxLineEdit->text());
        this->module->setLastMaximum(newMax);
    }
}

void DivisionConfigFrame::on_largestNumberFirstCheckBox_stateChanged(int state)
{
    this->module->setLargestNumberFirst(state == Qt::Checked);
}

void DivisionConfigFrame::on_decimalPlacesLineEdit_editingFinished()
{
    quint32 newDecimals = ui->decimalPlacesLineEdit->text().toUInt();
    this->module->setDecimalPlaces(newDecimals);
}

void DivisionConfigFrame::on_integerResultCheckBox_stateChanged(int state)
{
    using namespace std;
    bool intsOnly = (state == Qt::Checked);

    ui->decimalPlacesLineEdit->setEnabled(!intsOnly);
    ui->roundingComboBox->setEnabled(!intsOnly);
    ui->largestNumberFirstCheckBox->setEnabled(!intsOnly);

    // Ensure orderly transition from different ints mode
    //bool modIntsOnly = module->integersOnly;
    if (intsOnly)
    {
        QString str = ui->minNumberLineEdit->text();
        qDebug() << "oldFirstMin text = " << str;
        BigFixedPoint oldFirstMin(str.remove(QLocale::system().groupSeparator()));
        qint64 num = oldFirstMin.toLongLong();
        ui->minNumberLineEdit->setText(QString("%1").arg(num));
        this->module->setFirstMinimum(num);

        str = ui->maxNumberLineEdit->text();
        qDebug() << "oldFirstMax text = " << str;
        BigFixedPoint oldFirstMax(str.remove(QLocale::system().groupSeparator()));
        qDebug() << "oldFirstMax = " << str;
        num = oldFirstMax.toLongLong();
        ui->maxNumberLineEdit->setText(QString("%1").arg(num));
        this->module->setFirstMaximum(num);

        str = ui->secondMinLineEdit->text();
        qDebug() << "oldLastMin text = " << str;
        BigFixedPoint oldLastMin(str.remove(QLocale::system().groupSeparator()));
        num = oldLastMin.toLongLong();
        ui->secondMinLineEdit->setText(QString("%1").arg(num));
        this->module->setLastMinimum(num);

        str = ui->secondMaxLineEdit->text();
        qDebug() << "oldLastMax text = " << str;
        BigFixedPoint oldLastMax(str.remove(QLocale::system().groupSeparator()));
        num = oldLastMax.toLongLong();
        ui->secondMaxLineEdit->setText(QString("%1").arg(num));
        this->module->setLastMaximum(oldLastMax.toLongLong());

        //qDebug() << QString("%1").arg(std::numeric_limits<qint64>::min())
        //        << QString("%1").arg(std::numeric_limits<qint64>::max());
        numValidator->setRange(
                BigFixedPoint(QString("%1")
                              .arg(std::numeric_limits<qint64>::min())),
                BigFixedPoint(QString("%1")
                              .arg(std::numeric_limits<qint64>::max())));
    } else {
        /*! \todo Fix - Switching from intsOnly seems to cause a massive lag
         * as if it's factoring or something. Find out what it's doing and
         * see if it's not factoring new numbers before the official switch
         * out of integer mode.
         */
        QString str = ui->minNumberLineEdit->text();
        qint64 oldFirstMin = str.remove(QLocale::system().groupSeparator()).toLongLong();
        this->module->setFirstMinimum(BigFixedPoint(oldFirstMin));

        str = ui->maxNumberLineEdit->text();
        //qDebug() << "firstMax Line edit: " << str;
        qint64 oldFirstMax = str.remove(QLocale::system().groupSeparator()).toLongLong();
        //qDebug() << "oldFirstMax = " << oldFirstMax;
        this->module->setFirstMaximum(BigFixedPoint(oldFirstMax));

        str = ui->secondMinLineEdit->text();
        //qDebug() << "lastMin Line edit: " << str;
        qint64 oldLastMin = str.remove(QLocale::system().groupSeparator()).toLongLong();
        this->module->setLastMinimum(BigFixedPoint(oldLastMin));

        //qDebug() << "lastMin Line edit: " << str;
        str = ui->secondMaxLineEdit->text();
        qint64 oldLastMax = str.remove(QLocale::system().groupSeparator()).toLongLong();
        this->module->setLastMaximum(BigFixedPoint(oldLastMax));

        numValidator->setRangeEnabled(false);
    }

    this->module->setIntegersOnly(intsOnly);
}

void DivisionConfigFrame::on_roundingComboBox_currentIndexChanged(int index)
{
    this->module->setRoundingMode(index == 1);
}
