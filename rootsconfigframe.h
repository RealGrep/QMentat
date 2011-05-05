#ifndef ROOTSCONFIGFRAME_H
#define ROOTSCONFIGFRAME_H

#include <QFrame>
#include "rootsmodule.h"

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
    void setMinimum(int min);
    void setMaximum(int max);
    void setRootMinimum(int min);
    void setRootMaximum(int max);

    RootsModule *module;

private:
    Ui::RootsConfigFrame *ui;

private slots:
    void on_maxRootLineEdit_editingFinished();
    void on_minRootLineEdit_editingFinished();
    void on_maxNumberLineEdit_editingFinished();
    void on_minNumberLineEdit_editingFinished();
};

#endif // ROOTSCONFIGFRAME_H
