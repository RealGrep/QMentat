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
#include "licensedialog.h"
#include "ui_licensedialog.h"

#include <QtDebug>

LicenseDialog::LicenseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LicenseDialog)
{
    ui->setupUi(this);
    ui->noButton->setChecked(true);
    ui->okButton->setEnabled(false);
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
