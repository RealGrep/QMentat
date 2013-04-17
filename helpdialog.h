#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QHelpEngineCore>

class QHelpEngine;

namespace Ui {
    class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = 0);
    ~HelpDialog();

private:
    Ui::HelpDialog *ui;
    QHelpEngine *helpEngine;
};

#endif // HELPDIALOG_H
