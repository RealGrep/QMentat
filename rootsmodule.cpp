#include "rootsmodule.h"

#include "mainwindow.h"
#include <iostream>
#include <QtGui>
#include <string>
#include <QString>
#include "rootsconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
//#include "questiondisplayform.h"
#include "mathdisplayform.h"

RootsModule::RootsModule(MainWindow *mw)
{
    // Keep a copy for callbacks
    this->mainWindow = mw;

    // Init sane defaults
    this->firstNumber = 0;
    this->root = 0;
    this->answer = 0;

    // Read config
    QSettings settings;
    settings.beginGroup("rootsmodule");
    min = settings.value("min", 2).toULongLong();
    max = settings.value("max", 20).toULongLong();
    rootMin = settings.value("rootmin", 2).toULongLong();
    rootMax = settings.value("rootmax", 2).toULongLong();
    //decimalPlaces = settings.value("decimalplaces", 0).toUInt();
    settings.endGroup();

    // Make config frame
    configFrame = new RootsConfigFrame();
    configFrame->setModule(this);
    configFrame->setMinimum(this->min);
    configFrame->setMaximum(this->max);
    configFrame->setRootMinimum(this->rootMin);
    configFrame->setRootMaximum(this->rootMax);

    // Make display frame
    //displayFrame = (QuestionDisplay*)(new QuestionDisplayForm());
    displayFrame = (QuestionDisplay*)(new MathDisplayForm());

    // Seed random numbers
    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));
}

RootsModule::~RootsModule()
{
    this->mainWindow->layout()->removeWidget(configFrame);
    configFrame->close();
    delete configFrame;
}

QFrame* RootsModule::getConfigFrame()
{
    return configFrame;
}

QuestionDisplay* RootsModule::getDisplayFrame()
{
    return displayFrame;
}

QString RootsModule::question()
{
    bool bIntegerResults = true;
    //this->lastNumber = (qrand() % (this->max - this->min + 1)) + this->min;
    if (this->rootMax == this->rootMin) {
        this->root = this->rootMax;
    } else {
        this->root = (qrand() % (this->rootMax - this->rootMin + 1)) + this->rootMin;
    }

    if (this->max == this->min)
    {
        this->firstNumber = this->max;
    } else {
        this->firstNumber = (qrand() % (this->max - this->min + 1)) + this->min;
    }

    if (bIntegerResults) {
        int maxNum = pow(this->max, 1.0f/(float)this->root);
        int minNum = std::max(pow(this->min, 1.0f/(float)this->root), (double)this->min);
        int desiredResult = (qrand() % (maxNum - minNum + 1)) + minNum;
        this->firstNumber = pow(desiredResult, this->root);
    }

    this->answer = pow(this->firstNumber, 1.0f/(float)this->root) + 0.0000001;

    QString q;
    q = "<math><mroot><mi>" + QString::number(this->firstNumber) + "</mi><mn>" + QString::number(this->root) + "</mn></mroot></math>\n"; //^(1/" + QString::number(this->root) + ")";

    return q;
}

bool RootsModule::isCorrect(QString& answerGiven)
{
    unsigned long answerNum = answerGiven.toULong();
    if (answerNum == this->answer)
    {
        return true;
    } else {
        return false;
    }
}

QString RootsModule::getAnswerString()
{
    QString ans;
    ans = QString::number(this->firstNumber) + "^(1/"
          + QString::number(this->root) + ") = "
          + QString::number(this->answer);
    return ans;
}

void RootsModule::setMaximum(int newMax)
{
    if (this->max != newMax) {
        this->max = newMax;
        QSettings settings;
        settings.setValue("rootsmodule/max", max);

        mainWindow->newQuestion();
    }
}

void RootsModule::setMinimum(int newMin)
{
    if (this->min != newMin) {
        this->min = newMin;
        QSettings settings;
        settings.setValue("rootsmodule/min", min);
        mainWindow->newQuestion();
    }
}

void RootsModule::setRootMaximum(int newMax)
{
    if (this->rootMax != newMax) {
        this->rootMax = newMax;
        QSettings settings;
        settings.setValue("rootsmodule/rootmax", rootMax);

        mainWindow->newQuestion();
    }
}

void RootsModule::setRootMinimum(int newMin)
{
    if (this->rootMin != newMin) {
        this->rootMin = newMin;
        QSettings settings;
        settings.setValue("rootsmodule/rootmin", rootMin);

        mainWindow->newQuestion();
    }
}
