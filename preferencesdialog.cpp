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
