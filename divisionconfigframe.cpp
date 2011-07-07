#include "divisionconfigframe.h"
#include "ui_divisionconfigframe.h"
#include "divisionmodule.h"

#include "qbigfixedvalidator.h"

DivisionConfigFrame::DivisionConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::DivisionConfigFrame)
{
    ui->setupUi(this);

    QBigFixedValidator *bfv = new QBigFixedValidator(
            BigFixedPoint(QString("-9999999999999999999999999999")),
            BigFixedPoint(QString("9999999999999999999999999999")),
            this);

    ui->minNumberLineEdit->setValidator(bfv);
    ui->maxNumberLineEdit->setValidator(bfv);
    ui->secondMinLineEdit->setValidator(bfv);
    ui->secondMaxLineEdit->setValidator(bfv);

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
}

void DivisionConfigFrame::on_minNumberLineEdit_editingFinished()
{
    BigFixedPoint newMin(ui->minNumberLineEdit->text());
    this->module->setFirstMinimum(newMin);
}

void DivisionConfigFrame::on_maxNumberLineEdit_editingFinished()
{
    BigFixedPoint newMax(ui->maxNumberLineEdit->text());
    this->module->setFirstMaximum(newMax);
}

void DivisionConfigFrame::on_secondMinLineEdit_editingFinished()
{
    BigFixedPoint newMin(ui->secondMinLineEdit->text());
    this->module->setLastMinimum(newMin);
}

void DivisionConfigFrame::on_secondMaxLineEdit_editingFinished()
{
    BigFixedPoint newMax(ui->secondMaxLineEdit->text());
    this->module->setLastMaximum(newMax);
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
    bool intsOnly = (state == Qt::Checked);
    this->module->setIntegersOnly(intsOnly);
    ui->decimalPlacesLineEdit->setEnabled(!intsOnly);
    ui->roundingComboBox->setEnabled(!intsOnly);
    ui->largestNumberFirstCheckBox->setEnabled(!intsOnly);
}

void DivisionConfigFrame::on_roundingComboBox_currentIndexChanged(int index)
{
    this->module->setRoundingMode(index == 1);
}
