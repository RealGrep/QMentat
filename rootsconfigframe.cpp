#include "rootsconfigframe.h"
#include "ui_rootsconfigframe.h"

#include "bigfixedpoint.h"
#include "qbigfixedvalidator.h"
#include "rootsmodule.h"

RootsConfigFrame::RootsConfigFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::RootsConfigFrame)
{
    ui->setupUi(this);

    QBigFixedValidator *numValidator = new QBigFixedValidator(
                BigFixedPoint(QString("0")),
                BigFixedPoint(QString("99999999999999999999999999999999999999999999")),
                this);
    ui->minNumberLineEdit->setValidator(numValidator);
    ui->maxNumberLineEdit->setValidator(numValidator);

    QIntValidator *decimalsValidator = new QIntValidator(0, 100, this);
    ui->decimalPlacesLineEdit->setValidator(decimalsValidator);

    QIntValidator *rootValidator = new QIntValidator(1, 1000, this);
    ui->minRootLineEdit->setValidator(rootValidator);
    ui->maxRootLineEdit->setValidator(rootValidator);

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

void RootsConfigFrame::setMinimum(QString min)
{
    ui->minNumberLineEdit->setText(min);
}

void RootsConfigFrame::setMaximum(QString max)
{
    ui->maxNumberLineEdit->setText(max);
}

void RootsConfigFrame::setRootMinimum(int min)
{
    ui->minRootLineEdit->setText(QString("%1").arg(min));
}

void RootsConfigFrame::setRootMaximum(int max)
{
    ui->maxRootLineEdit->setText(QString("%1").arg(max));
}

void RootsConfigFrame::setDecimalPlaces(int decimals)
{
    ui->decimalPlacesLineEdit->setText(QString("%1").arg(decimals));
}

void RootsConfigFrame::setRoundingMode(int mode)
{
    ui->roundingComboBox->setCurrentIndex(mode);
}

void RootsConfigFrame::setIntegersOnly(bool intsOnly)
{
    ui->integerResultCheckBox->setChecked(intsOnly);
    ui->decimalPlacesLineEdit->setEnabled(!intsOnly);
    ui->roundingComboBox->setEnabled(!intsOnly);
}

void RootsConfigFrame::on_integerResultCheckBox_stateChanged(int state)
{
    bool intsOnly = (state == Qt::Checked);
    ui->decimalPlacesLineEdit->setEnabled(!intsOnly);
    ui->roundingComboBox->setEnabled(!intsOnly);
}

bool RootsConfigFrame::applyConfig()
{
    int integersOnly = ui->integerResultCheckBox->isChecked();
    int rounding = ui->roundingComboBox->currentIndex() == 1;
    int decimalPlaces = ui->decimalPlacesLineEdit->text().toInt();

    QString str = ui->minNumberLineEdit->text();
    BigFixedPoint firstMin(str.remove(QLocale::system().groupSeparator()));

    str = ui->maxNumberLineEdit->text();
    BigFixedPoint firstMax(str.remove(QLocale::system().groupSeparator()));

    int rootMin = ui->minRootLineEdit->text().toInt();
    int rootMax = ui->maxRootLineEdit->text().toInt();

    if (!module->isRangeOk(firstMin, firstMax, rootMin, rootMax)) {
        QMessageBox::warning(this, tr("Range Validation Error"), tr("There are no possible answers for the ranges you have entered."), QMessageBox::Ok);
        return false;
    } else if (rootMin <= 0) {
        QMessageBox::warning(this, tr("Range Validation Error"), tr("Minimum of root must be greater than zero."), QMessageBox::Ok);
        return false;
    } else if (rootMax <= 0) {
        QMessageBox::warning(this, tr("Range Validation Error"), tr("Maximum of root must be greater than zero."), QMessageBox::Ok);
        return false;
    } else if (firstMax < firstMin) {
        QMessageBox::warning(this, tr("Range Validation Error"), tr("Maximum of base number is smaller than the minimum."), QMessageBox::Ok);
        return false;
    } else if (rootMax < rootMin) {
        QMessageBox::warning(this, tr("Range Validation Error"), tr("Maximum root is smaller than the minimum."), QMessageBox::Ok);
        return false;
    } else {
        module->setSettings(firstMin, firstMax, rootMin, rootMax,
                            integersOnly, decimalPlaces, rounding);
    }

    return true;
}
