#ifndef ADDITIONCONFIGFRAME_H
#define ADDITIONCONFIGFRAME_H

#include <QFrame>

class QWidget;
class QString;
class AdditionModule;
class QBigFixedValidator;

namespace Ui {
    class AdditionConfigFrame;
}

class AdditionConfigFrame : public QFrame
{
    Q_OBJECT

public:
    AdditionConfigFrame(QWidget *parent = 0);
    ~AdditionConfigFrame();

    void setModule(AdditionModule *mod);
    void setFirstMinimum(QString min);
    void setFirstMaximum(QString max);
    void setLastMinimum(QString min);
    void setLastMaximum(QString max);
    void setLargestNumberFirst(bool b);

    bool applyConfig();

private:
    Ui::AdditionConfigFrame *ui;

    AdditionModule *module;

private slots:

};

#endif // ADDITIONCONFIGFRAME_H

