#ifndef ROOTSCONFIGFRAME_H
#define ROOTSCONFIGFRAME_H

#include <QFrame>

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
    void setRoundingMode(int mode);
    void setIntegersOnly(bool intsOnly);

    bool applyConfig();

private:
    Ui::RootsConfigFrame *ui;

    RootsModule *module;

private slots:
    void on_integerResultCheckBox_stateChanged(int );

};

#endif // ROOTSCONFIGFRAME_H
