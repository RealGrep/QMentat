#include "additionmodule.h"

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
   decimalPlaces = 0;
   firstMin = 2;
   firstMax = 100;
   lastMin = 2;
   lastMax = 100;
   largestNumberFirst = false;
   firstNumber = 0;
   lastNumber = 0;
   answer = 0;

   genFirst = 0;
   genLast = 0;

   // Keep a copy for callbacks
   this->mainWindow = mw;

   // Make config frame
   configFrame = new AdditionConfigFrame();
   configFrame->setModule(this);
   configFrame->setDecimalPlaces(decimalPlaces);
   configFrame->setFirstMinimum(firstMin);
   configFrame->setFirstMaximum(firstMax);
   configFrame->setLastMinimum(lastMin);
   configFrame->setLastMaximum(lastMax);
   configFrame->setLargestNumberFirst(largestNumberFirst);


   // Make display frame
   displayFrame = (QuestionDisplay*)(new QuestionDisplayForm());

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

   firstNumber = (*genFirst)();
   lastNumber = (*genLast)();
   if (largestNumberFirst && (lastNumber > firstNumber))
   {
      int tmp = firstNumber;
      firstNumber = lastNumber;
      lastNumber = tmp;
   }
   answer = firstNumber + lastNumber;

   QString q = QString("%1\n+ %2")
               .arg(decimalize(firstNumber, decimalPlaces))
               .arg(decimalize(lastNumber, decimalPlaces));

   return q;
}

bool AdditionModule::isCorrect(QString& answerGiven)
{
   // Figure out how many decimal places we're "missing" and get them back
   // so the comparison works right
   int decimalPos = answerGiven.indexOf(QLocale::system().decimalPoint());
   quint64 answerNum = 0;
   if (decimalPos >= 0)
   {
      int missingDecimals = decimalPlaces - answerGiven.size() - 1
                        - decimalPos;
      answerNum = answerGiven.remove(QLocale::system().decimalPoint()).toULongLong();
      if (missingDecimals > 0)
      {
         answerNum *= static_cast<quint64>(pow(10, missingDecimals));
      }
   } else {
      answerNum = answerGiven.toULongLong();
   }

   //qDebug() << "isCorrect: answerGiven = " << answerNum << "; answer = " << answer;
   if (answerNum == this->answer)
   {
      return true;
   } else {
      return false;
   }
}

QString AdditionModule::getAnswerString()
{
   return QString("%1 + %2 = %3")
         .arg(decimalize(firstNumber, decimalPlaces))
         .arg(decimalize(lastNumber, decimalPlaces))
         .arg(decimalize(answer, decimalPlaces));
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
   quint64 min = firstMin * static_cast<quint64>(pow(10, decimalPlaces));
   quint64 max = firstMax * static_cast<quint64>(pow(10, decimalPlaces));

   genFirst = new RandomInt<quint64>(min, max);
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
   quint64 min = lastMin * static_cast<quint64>(pow(10, decimalPlaces));
   quint64 max = lastMax * static_cast<quint64>(pow(10, decimalPlaces));

   genLast = new RandomInt<quint64>(min, max);
}

void AdditionModule::setFirstMaximum(quint64 newMax)
{
   if (firstMax != newMax) {
      if (std::numeric_limits<quint64>::max() - lastMax <= newMax)
      {
         qDebug() << "OVERFLOW POSSIBLE!" << endl;

      } else {
         firstMax = newMax;
         firstRangeUpdated();
         mainWindow->newQuestion();
      }
   }
}

void AdditionModule::setFirstMinimum(quint64 newMin)
{
   if (firstMin != newMin) {
      firstMin = newMin;
      firstRangeUpdated();
      mainWindow->newQuestion();
   }
}

void AdditionModule::setLastMaximum(quint64 newMax)
{
   if (lastMax != newMax) {
      if (std::numeric_limits<quint64>::max() - newMax <= firstMax)
      {
         qDebug() << "OVERFLOW POSSIBLE!" << endl;

      } else {
         lastMax = newMax;
         lastRangeUpdated();
         mainWindow->newQuestion();
      }
   }
}

void AdditionModule::setLastMinimum(quint64 newMin)
{
   if (lastMin != newMin) {
      lastMin = newMin;
      lastRangeUpdated();
      mainWindow->newQuestion();
   }
}

void AdditionModule::setLargestNumberFirst(bool b)
{
   if (largestNumberFirst != b)
   {
      largestNumberFirst = b;
      mainWindow->newQuestion();
   }
}

void AdditionModule::setDecimalPlaces(quint32 newDecimals)
{
   if (decimalPlaces != newDecimals) {
      decimalPlaces = newDecimals;

      firstRangeUpdated();
      lastRangeUpdated();

      mainWindow->newQuestion();
   }
}
