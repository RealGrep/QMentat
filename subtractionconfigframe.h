#ifndef SUBTRACTIONCONFIGFRAME_H
#define SUBTRACTIONCONFIGFRAME_H

#include <QFrame>

class SubtractionModule;
class QBigFixedValidator;

namespace Ui {
    class SubtractionConfigFrame;
}

class SubtractionConfigFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SubtractionConfigFrame(QWidget *parent = 0);
    ~SubtractionConfigFrame();

    void setModule(SubtractionModule *mod);
    void setFirstMinimum(QString min);
    void setFirstMaximum(QString max);
    void setLastMinimum(QString min);
    void setLastMaximum(QString max);
    void setLargestNumberFirst(bool b);

    bool applyConfig();

private:
    Ui::SubtractionConfigFrame *ui;

    SubtractionModule *module;

private slots:

};

#endif // SUBTRACTIONCONFIGFRAME_H
