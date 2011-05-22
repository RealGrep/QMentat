#include "additionconfigframe.h"
#include "ui_additionconfigframe.h"
#include "additionmodule.h"
#include "qulonglongvalidator.h"

AdditionConfigFrame::AdditionConfigFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AdditionConfigFrame)
{
    ui->setupUi(this);
    this->module = 0;

    QULongLongValidator *validator = new QULongLongValidator(this);
    ui->minNumberLineEdit->setValidator(validator);
    ui->maxNumberLineEdit->setValidator(validator);
    ui->secondMinLineEdit->setValidator(validator);
    ui->secondMaxLineEdit->setValidator(validator);
}

AdditionConfigFrame::~AdditionConfigFrame()
{
    delete ui;
}

void AdditionConfigFrame::setModule(AdditionModule *mod)
{
   this->module = mod;
}

void AdditionConfigFrame::setFirstMinimum(quint64 min)
{
   ui->minNumberLineEdit->setText(QString("%1").arg(min));
}

void AdditionConfigFrame::setFirstMaximum(quint64 max)
{
   ui->maxNumberLineEdit->setText(QString("%1").arg(max));
}

void AdditionConfigFrame::setLastMinimum(quint64 max)
{
   ui->secondMinLineEdit->setText(QString("%1").arg(max));
}

void AdditionConfigFrame::setLastMaximum(quint64 max)
{
   ui->secondMaxLineEdit->setText(QString("%1").arg(max));
}

void AdditionConfigFrame::setLargestNumberFirst(bool b)
{
   ui->largestNumberFirstCheckBox->setChecked(b);
}

void AdditionConfigFrame::setDecimalPlaces(quint32 decimalPlaces)
{
   ui->decimalPlacesLineEdit->setText(QString("%1").arg(decimalPlaces));
}

void AdditionConfigFrame::on_minNumberLineEdit_editingFinished()
{
   quint64 newMin = ui->minNumberLineEdit->text().toULongLong();
   //! \todo Check for wraparound
   this->module->setFirstMinimum(newMin);

}

void AdditionConfigFrame::on_maxNumberLineEdit_editingFinished()
{
   quint64 newMax = ui->maxNumberLineEdit->text().toULongLong();
   //! \todo Check for wraparound
   this->module->setFirstMaximum(newMax);
}

void AdditionConfigFrame::on_secondMinLineEdit_editingFinished()
{
   quint64 newMin = ui->secondMinLineEdit->text().toULongLong();
   //! \todo Check for wraparound
   this->module->setLastMinimum(newMin);
}

void AdditionConfigFrame::on_secondMaxLineEdit_editingFinished()
{
   quint64 newMax = ui->secondMaxLineEdit->text().toULongLong();
   //! \todo Check for wraparound
   this->module->setLastMaximum(newMax);
}

void AdditionConfigFrame::on_largestNumberFirstCheckBox_stateChanged(int state)
{
   if (state == Qt::Checked)
   {
      this->module->setLargestNumberFirst(true);
   } else {
      this->module->setLargestNumberFirst(false);
   }
}

void AdditionConfigFrame::on_decimalPlacesLineEdit_editingFinished()
{
    quint32 newDecimals = ui->decimalPlacesLineEdit->text().toUInt();
    this->module->setDecimalPlaces(newDecimals);
}
