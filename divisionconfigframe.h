#ifndef DIVISIONCONFIGFRAME_H
#define DIVISIONCONFIGFRAME_H

#include <QFrame>

class QWidget;
class QString;
class QBigFixedValidator;
class DivisionModule;

namespace Ui {
    class DivisionConfigFrame;
}

class DivisionModule;

class DivisionConfigFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DivisionConfigFrame(QWidget *parent = 0);
    ~DivisionConfigFrame();

    enum RoundingMode {ROUNDING_TRUNCATE = 0, ROUNDING_ROUND = 1};

    void setModule(DivisionModule *mod);
    void setFirstMinimum(QString min);
    void setFirstMaximum(QString max);
    void setLastMinimum(QString min);
    void setLastMaximum(QString max);
    void setLargestNumberFirst(bool b);
    void setDecimalPlaces(int decimalPlaces);
    void setRoundingMode(RoundingMode mode);
    void setIntegersOnly(bool intsOnly);

    bool applyConfig();

private:
    Ui::DivisionConfigFrame *ui;

    DivisionModule *module;
    QBigFixedValidator *numValidator;

private slots:
    void on_integerResultCheckBox_stateChanged(int );
};

#endif // DIVISIONCONFIGFRAME_H
