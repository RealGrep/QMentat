#include "divisionmodule.h"

#include "mainwindow.h"
#include <iostream>
#include <vector>
#include <QtGui>
#include <string>
#include <QString>
#include "divisionconfigframe.h"
#include "mainwindow.h"
#include "practicemodule.h"
#include "mathdisplayform.h"

/*! \class DivisionModule
 *  \brief Handles logic of questions and answers for division practice.
 *  \author Mike Dusseault <mike.dusseault@gmail.com>
 */
DivisionModule::DivisionModule(MainWindow *mw)
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

   // Keep a copy for callbacks
   this->mainWindow = mw;

   // Make config frame
   configFrame = new DivisionConfigFrame();
   configFrame->setModule(this);
   configFrame->setFirstMinimum(this->firstMin);
   configFrame->setFirstMaximum(this->firstMax);
   configFrame->setLastMinimum(this->lastMin);
   configFrame->setLastMaximum(this->lastMax);
   configFrame->setLargestNumberFirst(this->largestNumberFirst);

   // Make display frame
   displayFrame = (QuestionDisplay*)(new MathDisplayForm());

   // Seed random numbers
   QTime midnight(0, 0, 0);
   qsrand(midnight.secsTo(QTime::currentTime()));
}

DivisionModule::~DivisionModule()
{
   this->mainWindow->layout()->removeWidget(configFrame);
   configFrame->close();
   delete configFrame;
}

/*! Return config frame.
 */
QFrame* DivisionModule::getConfigFrame()
{
   return configFrame;
}

/*! Return display frame.
 */
QuestionDisplay* DivisionModule::getDisplayFrame()
{
   return displayFrame;
}

/*! Tests to see if any divisors divide any of the possible dividends.
 *  \param firstMin Minimum of the dividend.
 *  \param firstMax Maximum of the dividend.
 *  \param lastMin Minimum of the divisor.
 *  \param lastMax Maximum of the divisor.
 *  \returns true if at least one of the possible divisors divides at
 *    least one of the possible dividends, false otherwise.
 */
bool DivisionModule::isRangeOk(int firstMin, int firstMax,
                               int lastMin, int lastMax)
{
   bool isOk = false;

   // We ignore firstMax because we don't care if it's only divisible by itself
   for (int i = firstMax-1; i >= firstMin; --i)
   {
      for (int j = lastMin; j < lastMax; ++j)
      {
         //std::cout << "[" << i << "/" << j << "]" << std::endl;
         if (((i % j) == 0) && (i != j))
         {
            isOk = true;
            break;
         }
      }
      if (isOk)
         break;
   }

   return isOk;
}

/*! Finds divisors in a certain range for a number.
 * \param num Number for which we need divisors.
 * \param min Minimum divisor to consider.
 * \param max Maximum divisor to consider.
 * \returns Vector of divisors between min and max of num.
 */
std::vector<unsigned long> *DivisionModule::getDivisors(unsigned long num,
                                                        unsigned long min,
                                                        unsigned long max)
{
   std::vector<unsigned long> *divisors = new std::vector<unsigned long>();
   //std::cout << num << " is divisible by ";
   // No use going past num/2
   unsigned long last = std::min(num/2, max) + 1;
   //std::cout << "[" << min << "->" << last << "] ";
   for (unsigned long i = min; i < last; ++i)
   {
      if ((num % i) == 0)
      {
         //isOk = true;
         //std::cout << i << " ";
         divisors->push_back(i);
      }
   }
   //std::cout << std::endl;

   return divisors;
}

/*! \returns QString containing new question in MathML format.
 */
QString DivisionModule::question()
{
   //bool bIntegerResults = true;
   //bool trivial = false;
   //int attempts = 0;

   // Check range
   // TODO: Fix it so it can never happen
   bool rangeOk = isRangeOk(this->firstMin, this->firstMax,
                            this->lastMin, this->lastMax);
   /*if (rangeOk) {
      std::cout << "Range OK" << std::endl;
   } else {
      std::cout << "RANGE ERROR!" << std::endl;
   }*/
   if (!rangeOk) return "<math>ERR</math>";

   // Ensure we don't get a division by 0
   do {
      // Generate the numbers
      this->firstNumber = (qrand() % (this->firstMax - this->firstMin + 1))
                          + this->firstMin;

      std::vector<unsigned long> *divisors = getDivisors(this->firstNumber,
                                                   this->lastMin, this->lastMax);
      /*
      std::cout << "Divisors for " << this->firstNumber << ": ";
      for (auto it = divisors->begin(); it != divisors->end(); ++it)
      {
         std::cout << (*it) << " ";
      }
      std::cout << std::endl;
*/
      if (divisors->empty()) {
         // No divisors, try again
         //std::cout << "No divisors!" << std::endl;
         this->lastNumber = 0;
         delete divisors;
         continue;
      } else {
         int random = qrand() % divisors->size();
         this->lastNumber = (int)(*divisors)[random];
         delete divisors;
      }

      /*
      this->lastNumber = (qrand() % (this->lastMax - this->lastMin + 1)) + this->lastMin;
      if (this->largestNumberFirst && (this->lastNumber > this->firstNumber))
      {
         int tmp = this->firstNumber;
         this->firstNumber = this->lastNumber;
         this->lastNumber = tmp;
      }
      */
      this->answer = this->firstNumber / this->lastNumber;

      /*
      trivial = false;
      if (this->firstNumber == this->lastNumber) {
         trivial = true;
      }
      ++attempts;
      */
   } while (this->lastNumber == 0);

   QString q;
   //q = QString::number(this->firstNumber) + "\n÷ " + QString::number(this->lastNumber);
   q = "<math><mfrac><mi>" + QString::number(this->firstNumber) + "</mi><mn>" + QString::number(this->lastNumber) + "</mn></mfrac></math>\n";

   return q;
}

/*! Tells if the supplied answer is correct.
 * \returns true if correct, false otherwise.
 */
bool DivisionModule::isCorrect(QString& answerGiven)
{
   unsigned long answerNum = answerGiven.toULong();
   if (answerNum == this->answer)
   {
      return true;
   } else {
      return false;
   }
}

/*! Produces answer string for current question.
 * \returns QString containing full answer for display.
 */
QString DivisionModule::getAnswerString()
{
   QString ans;
   ans = QString::number(this->firstNumber) + " / "
         + QString::number(this->lastNumber) + " = "
         + QString::number(this->answer);
   return ans;
}

/*! Set maximum for the dividend.
 * \param newMax New maximum for the dividend.
 */
void DivisionModule::setFirstMaximum(int newMax)
{
   if (this->firstMax != newMax) {
      this->firstMax = newMax;
      mainWindow->newQuestion();
   }
}

/*! Set minimum for the dividend.
 * \param newMin New minimum for the dividend.
 */
void DivisionModule::setFirstMinimum(int newMin)
{
   if (this->firstMin != newMin) {
      this->firstMin = newMin;
      mainWindow->newQuestion();
   }
}

/*! Set maximum for the divisor.
 * \param newMax New maximum for the divisor.
 */
void DivisionModule::setLastMaximum(int newMax)
{
   if (this->lastMax != newMax) {
      this->lastMax = newMax;
      mainWindow->newQuestion();
   }
}

/*! Set minimum for the divisor.
 * \param newMin New minimum for the divisor.
 */
void DivisionModule::setLastMinimum(int newMin)
{
   if (this->lastMin != newMin) {
      this->lastMin = newMin;
      mainWindow->newQuestion();
   }
}

void DivisionModule::setLargestNumberFirst(bool b)
{
   if (this->largestNumberFirst != b)
   {
      this->largestNumberFirst = b;
      mainWindow->newQuestion();
   }
}
