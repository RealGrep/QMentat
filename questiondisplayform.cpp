#include "questiondisplayform.h"
#include "ui_questiondisplayform.h"
#include <QtGui>
#include <QRect>
#include <iostream>
#include "questiondisplay.h"
#include "preferences.h"
#include "preferenceslistener.h"

/*! \class QuestionDisplayForm
 *  \author Mike Dusseault <mike.dusseault@gmail.com>
 *
 * A very simple widget for displaying a question in ASCII form,
 * with no special formatting. It draws a bar under the text.
 */
QuestionDisplayForm::QuestionDisplayForm(QWidget *parent) :
        QuestionDisplay(parent),
        ui(new Ui::QuestionDisplayForm)
{
    ui->setupUi(this);

    //displayFont = QFont("Arial", 30);
    //Preferences prefs = Preferences::getInstance();
    displayFont = Preferences::getInstance().getQuestionFont();
    Preferences::getInstance().addListener(this);
    widestChar = 0;

    // Find out the width of the widest digit we use
    QString ourDigits = "+-x/0123456789.,";
    QFontMetrics metrics(displayFont);
    for (int i = 0; i < ourDigits.size(); ++i)
    {
        int currentDigit = metrics.charWidth(ourDigits, i);
        widestChar = std::max(widestChar, currentDigit);
    }
    //qDebug() << "Widest char = " << widestChar;

    int minHeight = metrics.height()*2 + 10;
    this->setMinimumHeight(minHeight);
    this->setMaximumHeight(minHeight);

    this->text = tr("N/A");
}

QuestionDisplayForm::~QuestionDisplayForm()
{
    Preferences::getInstance().removeListener(this);
    delete ui;
}

/*! Changes the text being displayed by the widget, and causes a repaint.
 * \param text The new text to be displayed, in straight ASCII.
 */
void QuestionDisplayForm::setText(QString text)
{
    this->text = text;
    // Isolate the first number, the operation symbol, and the second number
    // from the string. A bit brittle, but I have complete control of the
    // input, so should be fine.
    int nlPos = text.indexOf(QChar('\n'));
    if (nlPos > 0)
    {
        // First number
        first = text.left(nlPos);

        // Operation
        nlPos++;
        while (nlPos < text.size())
        {
            if (text[nlPos] == QChar('+')
                || text[nlPos] == QChar('-')
                || text[nlPos] == QChar('x')
                || text[nlPos] == QChar('/'))
            {
                operation = text[nlPos];
                break;
            }
            nlPos++;
        }

        // Last
        nlPos++;
        while (nlPos < text.size())
        {
            if (!text[nlPos].isSpace())
            {
                break;
            }
            nlPos++;
        }

        //qDebug() << "Last starts at " << (nlPos+1);
        last = text.mid(nlPos);
    }
/*
    qDebug() << "first = " << first;
    qDebug() << "operation = " << operation;
    qDebug() << "last = " << last;
*/
    repaint();
}

/*! Handles paint even by rendering formula to widget
 */
void QuestionDisplayForm::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.setFont(displayFont);

    // Figure out how wide the text will be when drawn
    QFontMetrics metrics = painter.fontMetrics();

    int firstNumSep = first.count(QLocale::system().groupSeparator());
    int lastNumSep = last.count(QLocale::system().groupSeparator());
    int sepWidth = metrics.width(QLocale::system().groupSeparator());
    int sepAdj = widestChar - sepWidth;

    int firstWidth = first.size()*widestChar - (firstNumSep*sepAdj);
    int lastWidth = last.size()*widestChar - (lastNumSep*sepAdj);
    int numsWidth = std::max(firstWidth, lastWidth);

    //int totalWidth = numsWidth + widestChar;

    int cols = std::max(first.size(), last.size()) + 1;
    /*
    qDebug() << "cols = " << cols;
    qDebug() << "widestChar = " << widestChar;
    qDebug() << "firstWidth = " << firstWidth << "; lastWidth = " << lastWidth;
    qDebug() << "totalWidth = " << totalWidth;
    */

    // Line 1 - first
    int y_pos = (height() / 3);
    int x_pos = width() - firstWidth;
    for (int i = 0; i < first.size(); ++i)
    {
        painter.drawText(x_pos, y_pos, QString(first[i]));
        if (first[i] == QLocale::system().groupSeparator())
        {
            x_pos += sepWidth;
        } else {
            x_pos += widestChar;
        }
    }

    // Line 2 - operation and last
    y_pos += metrics.lineSpacing();
    x_pos = width() - (cols * widestChar - (lastNumSep*sepAdj));
    painter.drawText(x_pos, y_pos, QString(operation));

    x_pos = width() - (last.size()*widestChar - (lastNumSep*sepAdj));
    for (int i = 0; i < last.size(); ++i)
    {
        painter.drawText(x_pos, y_pos, QString(last[i]));
        if (last[i] == QLocale::system().groupSeparator())
        {
            x_pos += sepWidth;
        } else {
            x_pos += widestChar;
        }
    }

    // Draw a line under all the text, of the right length
    painter.setPen(Qt::black);
    QPen pen(Qt::black, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    painter.setPen(pen);
    //qDebug() << "Line from (" << (width()-textWidth) << ", " << (height()-10)
    //        << ") to (" << width() << ", " << (height()-10) << ")";
    painter.drawLine(width() - numsWidth, height() - 10, width(), height() - 10);
}

void QuestionDisplayForm::preferencesChanged()
{
    //qDebug() << "QuestionDisplayForm: Prefs changed!";
    displayFont = Preferences::getInstance().getQuestionFont();
    //qDebug() << "QDF: New font = " << displayFont.toString();
    widestChar = 0;

    // Find out the width of the widest digit we use
    QString ourDigits = "+-x/0123456789.,";
    QFontMetrics metrics(displayFont);
    for (int i = 0; i < ourDigits.size(); ++i)
    {
        int currentDigit = metrics.charWidth(ourDigits, i);
        widestChar = std::max(widestChar, currentDigit);
    }

    int minHeight = metrics.height()*2 + 10;
    this->setMinimumHeight(minHeight);
    this->setMaximumHeight(minHeight);

    repaint();
}
