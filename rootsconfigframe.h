#ifndef ROOTSCONFIGFRAME_H
#define ROOTSCONFIGFRAME_H

#include <QFrame>
#include "rootsmodule.h"

class QString;

class RootsModule;

namespace Ui {
    class RootsConfigFrame;
}

class RootsConfigFrame : public QFrame
{
    Q_OBJECT

public:
    explicit RootsConfigFrame(QWidget *parent = 0);
    ~RootsConfigFrame();

    void setModule(RootsModule *mod);
    void setMinimum(QString min);
    void setMaximum(QString max);
    void setRootMinimum(int min);
    void setRootMaximum(int max);
    void setDecimalPlaces(int newDecimals);
    void setIntegersOnly(bool intsOnly);

    RootsModule *module;

private:
    Ui::RootsConfigFrame *ui;

private slots:
    void on_integerResultCheckBox_stateChanged(int );
    void on_decimalPlacesLineEdit_editingFinished();
    void on_maxRootLineEdit_editingFinished();
    void on_minRootLineEdit_editingFinished();
    void on_maxNumberLineEdit_editingFinished();
    void on_minNumberLineEdit_editingFinished();
};

#endif // ROOTSCONFIGFRAME_H
