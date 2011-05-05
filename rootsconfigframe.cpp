#include "rootsconfigframe.h"
#include "ui_rootsconfigframe.h"

RootsConfigFrame::RootsConfigFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::RootsConfigFrame)
{
    ui->setupUi(this);
    //ui->minNumberLineEdit->setText(tr("2"));
    //ui->maxNumberLineEdit->setText(tr("100"));
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

void RootsConfigFrame::setMinimum(int min)
{
   ui->minNumberLineEdit->setText(QString("%1").arg(min));
}

void RootsConfigFrame::setMaximum(int max)
{
   ui->maxNumberLineEdit->setText(QString("%1").arg(max));
}

void RootsConfigFrame::setRootMinimum(int min)
{
   ui->minRootLineEdit->setText(QString("%1").arg(min));
}

void RootsConfigFrame::setRootMaximum(int max)
{
   ui->maxRootLineEdit->setText(QString("%1").arg(max));
}

void RootsConfigFrame::on_minNumberLineEdit_editingFinished()
{
   int newMin = ui->minNumberLineEdit->text().toInt();
   this->module->setMinimum(newMin);
}

void RootsConfigFrame::on_maxNumberLineEdit_editingFinished()
{
   int newMax = ui->maxNumberLineEdit->text().toInt();
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
