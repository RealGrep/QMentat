#include "licensedialog.h"
#include "ui_licensedialog.h"

#include <QtDebug>

LicenseDialog::LicenseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LicenseDialog)
{
    ui->setupUi(this);
}

LicenseDialog::~LicenseDialog()
{
    delete ui;
}


void LicenseDialog::on_yesButton_toggled(bool checked)
{
        ui->okButton->setEnabled(checked);
}

void LicenseDialog::on_cancelButton_clicked()
{
    this->reject();
}

void LicenseDialog::on_okButton_clicked()
{
    this->accept();
}
