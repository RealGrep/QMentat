#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

private slots:
    void apply();
    void on_questionFontButton_clicked();
    void on_answerFontButton_clicked();

    void on_preferencesButtonBox_accepted();

private:
    Ui::PreferencesDialog *ui;
};

#endif // PREFERENCESDIALOG_H
