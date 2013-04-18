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
#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include <QtGui>

#include "preferences.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    QPushButton* applyButton = ui->preferencesButtonBox->button(QDialogButtonBox::Apply);
    connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));

    ui->questionFontLineEdit->setFont(Preferences::getInstance().getQuestionFont());
    ui->answerFontLineEdit->setFont(Preferences::getInstance().getAnswerFont());
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::apply()
{
    if (ui->questionFontLineEdit->font() != Preferences::getInstance().getQuestionFont())
    {
        Preferences::getInstance().setQuestionFont(ui->questionFontLineEdit->font());
    }

    if (ui->answerFontLineEdit->font() != Preferences::getInstance().getAnswerFont())
    {
        Preferences::getInstance().setAnswerFont(ui->answerFontLineEdit->font());
    }
}

void PreferencesDialog::on_questionFontButton_clicked()
{
    QFontDialog fontDialog;
    bool ok = false;
    QFont font = fontDialog.getFont(&ok, ui->questionFontLineEdit->font(), this);
    if (ok)
    {
        //qDebug() << "QUESTION Font: " << font.toString();
        ui->questionFontLineEdit->setFont(font);
    } else {
        //qDebug() << "QUESTION: No font.";
    }
}

void PreferencesDialog::on_answerFontButton_clicked()
{
    QFontDialog fontDialog;
    bool ok = false;
    QFont font = fontDialog.getFont(&ok, ui->answerFontLineEdit->font(), this);
    if (ok)
    {
        //qDebug() << "ANSWER Font: " << font.toString();
        ui->answerFontLineEdit->setFont(font);
    } else {
        //qDebug() << "ANSWER: No font.";
    }
}

void PreferencesDialog::on_preferencesButtonBox_accepted()
{
    apply();
    accept();
}
