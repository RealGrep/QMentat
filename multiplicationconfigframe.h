#ifndef MULTIPLICATIONCONFIGFRAME_H
#define MULTIPLICATIONCONFIGFRAME_H

#include <QFrame>

class QWidget;
class QString;
class QBigFixedValidator;
class MultiplicationModule;

namespace Ui {
    class MultiplicationConfigFrame;
}

class MultiplicationConfigFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MultiplicationConfigFrame(QWidget *parent = 0);
    ~MultiplicationConfigFrame();

    void setModule(MultiplicationModule *mod);
    void setFirstMinimum(QString min);
    void setFirstMaximum(QString max);
    void setLastMinimum(QString min);
    void setLastMaximum(QString max);
    void setLargestNumberFirst(bool b);
    void setDecimalPlaces(int decimalPlaces);
    void setRoundingMode(int mode);

    bool applyConfig();

private:
    Ui::MultiplicationConfigFrame *ui;

    MultiplicationModule *module;

private slots:

};

#endif // MULTIPLICATIONCONFIGFRAME_H
