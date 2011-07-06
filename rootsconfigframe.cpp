#include "rootsconfigframe.h"
#include "ui_rootsconfigframe.h"

#include "bigfixedpoint.h"

RootsConfigFrame::RootsConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::RootsConfigFrame)
{
    ui->setupUi(this);
    this->module = 0;
}

RootsConfigFrame::~RootsConfigFrame()
{
    delete ui;
}

void RootsConfigFrame::setModule(RootsModule *mod)
{
    this->module = mod;
}

void RootsConfigFrame::setMinimum(QString min)
{
    ui->minNumberLineEdit->setText(min);
}

void RootsConfigFrame::setMaximum(QString max)
{
    ui->maxNumberLineEdit->setText(max);
}

void RootsConfigFrame::setRootMinimum(int min)
{
    ui->minRootLineEdit->setText(QString("%1").arg(min));
}

void RootsConfigFrame::setRootMaximum(int max)
{
    ui->maxRootLineEdit->setText(QString("%1").arg(max));
}

void RootsConfigFrame::setDecimalPlaces(int decimals)
{
    ui->decimalPlacesLineEdit->setText(QString("%1").arg(decimals));
}

void RootsConfigFrame::setIntegersOnly(bool intsOnly)
{
    ui->integerResultCheckBox->setChecked(intsOnly);
    ui->decimalPlacesLineEdit->setEnabled(!intsOnly);
}

void RootsConfigFrame::on_minNumberLineEdit_editingFinished()
{
    BigFixedPoint newMin(ui->minNumberLineEdit->text());
    this->module->setMinimum(newMin);
}

void RootsConfigFrame::on_maxNumberLineEdit_editingFinished()
{
    BigFixedPoint newMax(ui->maxNumberLineEdit->text());
    this->module->setMaximum(newMax);
}

void RootsConfigFrame::on_minRootLineEdit_editingFinished()
{
    int newMin = ui->minRootLineEdit->text().toInt();
    this->module->setRootMinimum(newMin);
}

void RootsConfigFrame::on_maxRootLineEdit_editingFinished()
{
    int newMax = ui->maxRootLineEdit->text().toInt();
    this->module->setRootMaximum(newMax);
}

void RootsConfigFrame::on_decimalPlacesLineEdit_editingFinished()
{
    int newDecimals = ui->decimalPlacesLineEdit->text().toInt();
    this->module->setDecimalPlaces(newDecimals);
}

void RootsConfigFrame::on_integerResultCheckBox_stateChanged(int state)
{
    bool intsOnly = (state == Qt::Checked);
    ui->decimalPlacesLineEdit->setEnabled(!intsOnly);
    this->module->setIntegersOnly(intsOnly);
}
