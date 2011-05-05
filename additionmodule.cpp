#include "additionmodule.h"
#include "mainwindow.h"

#include <QtGui>
#include <QString>

#include <cassert>

#include "additionconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
#include "questiondisplayform.h"
#include "random.h"

AdditionModule::AdditionModule(MainWindow *mw)
{
   // Init sane defaults
   this->firstMin = 2;
   this->firstMax = 100;
   this->lastMin = 2;
   this->lastMax = 100;
   this->largestNumberFirst = false;
   this->firstNumber = 0;
   this->lastNumber = 0;
   this->answer = 0;

   genFirst = 0;
   genLast = 0;

   // Keep a copy for callbacks
   this->mainWindow = mw;

   // Make config frame
   configFrame = new AdditionConfigFrame();
   configFrame->setModule(this);
   configFrame->setFirstMinimum(this->firstMin);
   configFrame->setFirstMaximum(this->firstMax);
   configFrame->setLastMinimum(this->lastMin);
   configFrame->setLastMaximum(this->lastMax);
   configFrame->setLargestNumberFirst(this->largestNumberFirst);

   // Make display frame
   displayFrame = (QuestionDisplay*)(new QuestionDisplayForm());

   // Seed random numbers
   //QTime midnight(0, 0, 0);
   //qsrand(midnight.secsTo(QTime::currentTime()));

   firstRangeUpdated();
   lastRangeUpdated();
}

AdditionModule::~AdditionModule()
{
   assert(configFrame != 0);
   assert(displayFrame != 0);

   if (genFirst != 0)
   {
      delete genFirst;
      genFirst = 0;
   }
   if (genLast != 0)
   {
      delete genLast;
      genLast = 0;
   }

   this->mainWindow->layout()->removeWidget(configFrame);
   configFrame->close();
   delete configFrame;
   configFrame = 0;

   delete displayFrame;
   displayFrame = 0;
}

QFrame* AdditionModule::getConfigFrame()
{
   assert(configFrame != 0);
   return configFrame;
}

QuestionDisplay* AdditionModule::getDisplayFrame()
{
   assert(displayFrame != 0);
   return displayFrame;
}

QString AdditionModule::question()
{
   assert(genFirst != 0);
   assert(genLast != 0);

   this->firstNumber = (*genFirst)();
   this->lastNumber = (*genLast)();
   if (this->largestNumberFirst && (this->lastNumber > this->firstNumber))
   {
      int tmp = this->firstNumber;
      this->firstNumber = this->lastNumber;
      this->lastNumber = tmp;
   }
   this->answer = this->firstNumber + this->lastNumber;

   QString q;
   q = QString("%L1\n + %L2").arg(this->firstNumber).arg(this->lastNumber);

   return q;
}

bool AdditionModule::isCorrect(QString& answerGiven)
{
   unsigned long answerNum = answerGiven.toULong();
   if (answerNum == this->answer)
   {
      return true;
   } else {
      return false;
   }
}

QString AdditionModule::getAnswerString()
{
   return QString("%L1 + %L2 = %L3")
         .arg(this->firstNumber)
         .arg(this->lastNumber)
         .arg(this->answer);
}

void AdditionModule::firstRangeUpdated()
{
   // Get rid of previous generators
   if (genFirst != 0)
   {
      delete genFirst;
      genFirst = 0;
   }

   // Make new generator
   genFirst = new RandomInt<quint64>(this->firstMin, this->firstMax);
}

void AdditionModule::lastRangeUpdated()
{
   // Get rid of previous generator
   if (genLast != 0)
   {
      delete genLast;
      genLast = 0;
   }

   // Make new generator
   genLast = new RandomInt<quint64>(this->lastMin, this->lastMax);
}

void AdditionModule::setFirstMaximum(quint64 newMax)
{
   if (this->firstMax != newMax) {
      if (std::numeric_limits<quint64>::max() - this->lastMax <= newMax)
      {
         qDebug() << "OVERFLOW POSSIBLE!" << endl;

      } else {
         this->firstMax = newMax;
         firstRangeUpdated();
         mainWindow->newQuestion();
      }
   }
}

void AdditionModule::setFirstMinimum(quint64 newMin)
{
   if (this->firstMin != newMin) {
      this->firstMin = newMin;
      firstRangeUpdated();
      mainWindow->newQuestion();
   }
}

void AdditionModule::setLastMaximum(quint64 newMax)
{
   if (this->lastMax != newMax) {
      if (std::numeric_limits<quint64>::max() - newMax <= this->firstMax)
      {
         qDebug() << "OVERFLOW POSSIBLE!" << endl;

      } else {
         this->lastMax = newMax;
         lastRangeUpdated();
         mainWindow->newQuestion();
      }
   }
}

void AdditionModule::setLastMinimum(quint64 newMin)
{
   if (this->lastMin != newMin) {
      this->lastMin = newMin;
      lastRangeUpdated();
      mainWindow->newQuestion();
   }
}

void AdditionModule::setLargestNumberFirst(bool b)
{
   if (this->largestNumberFirst != b)
   {
      this->largestNumberFirst = b;
      mainWindow->newQuestion();
   }
}
