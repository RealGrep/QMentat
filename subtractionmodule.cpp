#include "subtractionmodule.h"

#include "mainwindow.h"
#include <iostream>
#include <QtGui>
#include <string>
#include <QString>
#include "subtractionconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
#include "questiondisplayform.h"

SubtractionModule::SubtractionModule(MainWindow *mw)
{
   // Init sane defaults
   this->firstMin = 2;
   this->firstMax = 100;
   this->lastMin = 2;
   this->lastMax = 100;
   this->firstNumber = 0;
   this->lastNumber = 0;
   this->answer = 0;
   this->largestNumberFirst = true;

   // Keep a copy for callbacks
   this->mainWindow = mw;

   // Make config frame
   configFrame = new SubtractionConfigFrame();
   configFrame->setModule(this);
   configFrame->setFirstMinimum(this->firstMin);
   configFrame->setFirstMaximum(this->firstMax);
   configFrame->setLastMinimum(this->lastMin);
   configFrame->setLastMaximum(this->lastMax);
   configFrame->setLargestNumberFirst(this->largestNumberFirst);

   // Make display frame
   displayFrame = (QuestionDisplay*)(new QuestionDisplayForm());

   // Seed random numbers
   QTime midnight(0, 0, 0);
   qsrand(midnight.secsTo(QTime::currentTime()));
}

SubtractionModule::~SubtractionModule()
{
   this->mainWindow->layout()->removeWidget(configFrame);
   configFrame->close();
   delete configFrame;
}

QFrame* SubtractionModule::getConfigFrame()
{
   return configFrame;
}

QuestionDisplay* SubtractionModule::getDisplayFrame()
{
   return displayFrame;
}

QString SubtractionModule::question()
{
   this->firstNumber = (qrand() % (this->firstMax - this->firstMin + 1)) + this->firstMin;
   this->lastNumber = (qrand() % (this->lastMax - this->lastMin + 1)) + this->lastMin;
   if (this->largestNumberFirst && (this->lastNumber > this->firstNumber))
   {
      int tmp = this->firstNumber;
      this->firstNumber = this->lastNumber;
      this->lastNumber = tmp;
   }
   this->answer = this->firstNumber - this->lastNumber;

   QString q;
   q = QString::number(this->firstNumber) + "\n- " + QString::number(this->lastNumber);

   return q;
}

bool SubtractionModule::isCorrect(QString& answerGiven)
{
   unsigned long answerNum = answerGiven.toULong();
   if (answerNum == this->answer)
   {
      return true;
   } else {
      return false;
   }
}

QString SubtractionModule::getAnswerString()
{
   QString ans;
   ans = QString::number(this->firstNumber) + " - "
         + QString::number(this->lastNumber) + " = "
         + QString::number(this->answer);
   return ans;
}

void SubtractionModule::setFirstMaximum(int newMax)
{
   if (this->firstMax != newMax) {
      this->firstMax = newMax;
      mainWindow->newQuestion();
   }
}

void SubtractionModule::setFirstMinimum(int newMin)
{
   if (this->firstMin != newMin) {
      this->firstMin = newMin;
      mainWindow->newQuestion();
   }
}

void SubtractionModule::setLastMaximum(int newMax)
{
   if (this->lastMax != newMax) {
      this->lastMax = newMax;
      mainWindow->newQuestion();
   }
}

void SubtractionModule::setLastMinimum(int newMin)
{
   if (this->lastMin != newMin) {
      this->lastMin = newMin;
      mainWindow->newQuestion();
   }
}

void SubtractionModule::setLargestNumberFirst(bool b)
{
   if (this->largestNumberFirst != b)
   {
      this->largestNumberFirst = b;
      mainWindow->newQuestion();
   }
}
