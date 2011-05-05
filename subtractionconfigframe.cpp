#include "subtractionconfigframe.h"
#include "ui_subtractionconfigframe.h"
#include "subtractionmodule.h"

SubtractionConfigFrame::SubtractionConfigFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SubtractionConfigFrame)
{
    ui->setupUi(this);
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

void SubtractionConfigFrame::setFirstMinimum(int min)
{
   ui->minNumberLineEdit->setText(QString("%1").arg(min));
}

void SubtractionConfigFrame::setFirstMaximum(int max)
{
   ui->maxNumberLineEdit->setText(QString("%1").arg(max));
}

void SubtractionConfigFrame::setLastMinimum(int max)
{
   ui->secondMinLineEdit->setText(QString("%1").arg(max));
}

void SubtractionConfigFrame::setLastMaximum(int max)
{
   ui->secondMaxLineEdit->setText(QString("%1").arg(max));
}

void SubtractionConfigFrame::setLargestNumberFirst(bool b)
{
   ui->largestNumberFirstCheckBox->setChecked(b);
}

void SubtractionConfigFrame::on_minNumberLineEdit_editingFinished()
{
   int newMin = ui->minNumberLineEdit->text().toInt();
   this->module->setFirstMinimum(newMin);
}

void SubtractionConfigFrame::on_maxNumberLineEdit_editingFinished()
{
   int newMax = ui->maxNumberLineEdit->text().toInt();
   this->module->setFirstMaximum(newMax);
}

void SubtractionConfigFrame::on_secondMinLineEdit_editingFinished()
{
   int newMin = ui->secondMinLineEdit->text().toInt();
   this->module->setLastMinimum(newMin);
}

void SubtractionConfigFrame::on_secondMaxLineEdit_editingFinished()
{
   int newMax = ui->secondMaxLineEdit->text().toInt();
   this->module->setLastMaximum(newMax);
}

void SubtractionConfigFrame::on_largestNumberFirstCheckBox_stateChanged(int state)
{
   if (state == Qt::Checked)
   {
      this->module->setLargestNumberFirst(true);
   } else {
      this->module->setLargestNumberFirst(false);
   }
}
