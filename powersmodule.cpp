#include "powersmodule.h"

#include "mainwindow.h"
#include <iostream>
#include <QtGui>
#include <string>
#include <QString>
#include "powersconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
#include "mathdisplayform.h"

PowersModule::PowersModule(MainWindow *mw)
{
   // Init sane defaults
   this->min = 2;
   this->max = 20;
   this->powerMin = 2;
   this->powerMax = 2;
   this->firstNumber = 0;
   this->power = 0;
   this->answer = 0;

   // Keep a copy for callbacks
   this->mainWindow = mw;

   // Make config frame
   configFrame = new PowersConfigFrame();
   configFrame->setModule(this);
   configFrame->setMinimum(this->min);
   configFrame->setMaximum(this->max);
   configFrame->setPowerMinimum(this->powerMin);
   configFrame->setPowerMaximum(this->powerMax);

   // Make display frame
   displayFrame = (QuestionDisplay*)(new MathDisplayForm());

   // Seed random numbers
   QTime midnight(0, 0, 0);
   qsrand(midnight.secsTo(QTime::currentTime()));
}

PowersModule::~PowersModule()
{
   this->mainWindow->layout()->removeWidget(configFrame);
   configFrame->close();
   delete configFrame;
}

QFrame* PowersModule::getConfigFrame()
{
   return configFrame;
}

QuestionDisplay* PowersModule::getDisplayFrame()
{
   return displayFrame;
}

QString PowersModule::question()
{
   if (this->max == this->min)
   {
      this->firstNumber = this->max;
   } else {
      this->firstNumber = (qrand() % (this->max - this->min + 1)) + this->min;
   }

   if (this->powerMax == this->powerMin) {
      this->power = this->powerMax;
   } else {
      this->power = (qrand() % (this->powerMax - this->powerMin + 1)) + this->powerMin;
   }
   this->answer = pow(this->firstNumber, this->power);

   QString q;
   q = "<math><msup><mi>" + QString::number(this->firstNumber) + "</mi><mn>" + QString::number(this->power) + "</mn></msup></math>\n";

   return q;
}

bool PowersModule::isCorrect(QString& answerGiven)
{
   unsigned long answerNum = answerGiven.toULong();
   if (answerNum == this->answer)
   {
      return true;
   } else {
      return false;
   }
}

QString PowersModule::getAnswerString()
{
   QString ans;
   ans = QString::number(this->firstNumber) + "^"
         + QString::number(this->power) + " = "
         + QString::number(this->answer);
   return ans;
}

void PowersModule::setMaximum(int newMax)
{
   if (this->max != newMax) {
      this->max = newMax;
      mainWindow->newQuestion();
   }
}

void PowersModule::setMinimum(int newMin)
{
   if (this->min != newMin) {
      this->min = newMin;
      mainWindow->newQuestion();
   }
}

void PowersModule::setPowerMaximum(int newMax)
{
   if (this->powerMax != newMax) {
      this->powerMax = newMax;
      mainWindow->newQuestion();
   }
}

void PowersModule::setPowerMinimum(int newMin)
{
   if (this->powerMin != newMin) {
      this->powerMin = newMin;
      mainWindow->newQuestion();
   }
}