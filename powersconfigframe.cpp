#include "powersconfigframe.h"
#include "ui_powersconfigframe.h"

PowersConfigFrame::PowersConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::PowersConfigFrame)
{
    ui->setupUi(this);
    //ui->minNumberLineEdit->setText(tr("2"));
    //ui->maxNumberLineEdit->setText(tr("100"));
    this->module = 0;
}

PowersConfigFrame::~PowersConfigFrame()
{
    delete ui;
}

void PowersConfigFrame::setModule(PowersModule *mod)
{
    this->module = mod;
}

void PowersConfigFrame::setMinimum(int min)
{
    ui->minNumberLineEdit->setText(QString("%1").arg(min));
}

void PowersConfigFrame::setMaximum(int max)
{
    ui->maxNumberLineEdit->setText(QString("%1").arg(max));
}

void PowersConfigFrame::setPowerMinimum(int min)
{
    ui->minPowerLineEdit->setText(QString("%1").arg(min));
}

void PowersConfigFrame::setPowerMaximum(int max)
{
    ui->maxPowerLineEdit->setText(QString("%1").arg(max));
}

void PowersConfigFrame::on_minNumberLineEdit_editingFinished()
{
    int newMin = ui->minNumberLineEdit->text().toInt();
    this->module->setMinimum(newMin);
}

void PowersConfigFrame::on_maxNumberLineEdit_editingFinished()
{
    int newMax = ui->maxNumberLineEdit->text().toInt();
    this->module->setMaximum(newMax);
}

void PowersConfigFrame::on_minPowerLineEdit_editingFinished()
{
    int newMin = ui->minPowerLineEdit->text().toInt();
    this->module->setPowerMinimum(newMin);
}

void PowersConfigFrame::on_maxPowerLineEdit_editingFinished()
{
    int newMax = ui->maxPowerLineEdit->text().toInt();
    this->module->setPowerMaximum(newMax);
}
