#ifndef LICENSEDIALOG_H
#define LICENSEDIALOG_H

#include <QDialog>

namespace Ui {
    class LicenseDialog;
}

class LicenseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LicenseDialog(QWidget *parent = 0);
    ~LicenseDialog();

private slots:

    void on_yesButton_toggled(bool checked);

    void on_cancelButton_clicked();

    void on_okButton_clicked();

private:
    Ui::LicenseDialog *ui;
};

#endif // LICENSEDIALOG_H
