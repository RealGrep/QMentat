/* Copyright (c) 2013 Mike Dusseault
 *
 * This file is part of QMentat.
 *
 * QMentat is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QMentat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QMentat.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "questiondisplayform.h"
#include "ui_questiondisplayform.h"
#include <QtGui>
#include <QRect>
#include <QFontMetrics>
#include <QMessageBox>
#include "questiondisplay.h"
#include "preferences.h"

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

    displayFont = Preferences::getInstance().getQuestionFont();
    Preferences::getInstance().addListener(this);

    preferencesChanged();

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
                || text[nlPos] == QChar('/')
                || text[nlPos] == QChar('^')
                || text[nlPos] == QChar('|'))
            {
                operation = text[nlPos];
                break;
            } else if (text[nlPos] == QChar('x')) {
                operation = QLatin1Char('\xD7');
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

    repaint();
}

/*! Handles paint even by rendering formula to widget
 */
void QuestionDisplayForm::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(palette().color(QPalette::Text));
    painter.setBackground(palette().color(QPalette::Base));
    painter.setFont(displayFont);

    // Figure out how wide the text will be when drawn
    QFontMetrics metrics = painter.fontMetrics();

    if (operation == QChar('^'))
    {
        // 1st number - base
        int firstWidth = metrics.horizontalAdvance(first);
        QFont expFont = displayFont;
        expFont.setPointSize(displayFont.pointSize()*0.65);
        painter.setFont(expFont);
        int expWidth = metrics.horizontalAdvance(last);

        int numsWidth = firstWidth + expWidth;

        int y_pos = (height()/3) * 2;
        int x_pos = (width() - numsWidth) / 2;

        painter.setFont(displayFont);
        painter.drawText(x_pos, y_pos, first);

        x_pos += firstWidth;
        y_pos -= metrics.height() / 2;

        painter.setFont(expFont);
        painter.drawText(x_pos, y_pos, last);

    } else if (operation == QChar('|')) {
        int firstWidth = metrics.horizontalAdvance(first);
        int firstHeight = metrics.ascent();

        //int lineThickness = std::max(1, (int)((displayFont.pointSize()/8)+0.5));
        int lineThickness = std::max(1, static_cast<int>(std::round(displayFont.pointSize() / 8.0)));

        QFont rootFont = displayFont;
        rootFont.setPointSize(displayFont.pointSize()*0.65);
        painter.setFont(rootFont);
        QFontMetrics rootMetrics = painter.fontMetrics();
        int rootWidth = rootMetrics.horizontalAdvance(last);
        int rootHeight = rootMetrics.ascent();
        int gap = rootMetrics.averageCharWidth()/2 + 1;
        int numsWidth = firstWidth + rootWidth + gap;

        // Draw base
        painter.setFont(displayFont);
        int hMargins = (width() - numsWidth) / 2;
        int y_pos = (height()/3) * 2;
        int x_pos = width() - hMargins - firstWidth;
        painter.drawText(x_pos, y_pos, first);

        // Draw root
        painter.setFont(rootFont);
        x_pos = hMargins;
        y_pos -= metrics.height() / 2;
        painter.drawText(x_pos, y_pos, last);

        // Radical sign
        // 1st, line above base
        y_pos = (height()/3) * 2 - metrics.ascent();
        x_pos = width() - hMargins - firstWidth;

        QPen pen(palette().color(QPalette::Text), lineThickness,
                 Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
        painter.setPen(pen);

        painter.drawLine(x_pos, y_pos, x_pos + firstWidth, y_pos);
        painter.drawLine(x_pos, y_pos,
                         x_pos - metrics.averageCharWidth()/2,
                         y_pos+metrics.ascent());
        x_pos -= metrics.averageCharWidth()/2;
        y_pos += metrics.ascent();
        painter.drawLine(x_pos, y_pos,
                         x_pos - metrics.averageCharWidth()/2,
                         y_pos - firstHeight + rootHeight + lineThickness);
    } else if (operation == QChar('/')) {
        //int lineThickness = std::max(1, (int)((displayFont.pointSize()/8)+0.5));
        int lineThickness = std::max(1, static_cast<int>(std::round(displayFont.pointSize() / 8.0)));

        int firstWidth = metrics.horizontalAdvance(first);
        int lastWidth = metrics.horizontalAdvance(last);
        int numsWidth = std::max(firstWidth, lastWidth);

        int y_pos = (height() / 2);
        int x_pos = (width() / 2) - (firstWidth / 2);

        painter.drawText(x_pos, y_pos, first);

        y_pos = (height() / 2) + metrics.ascent();
        x_pos = (width() / 2) - (lastWidth / 2);
        painter.drawText(x_pos, y_pos, last);

        y_pos = height() / 2 + lineThickness;
        x_pos = (width() / 2) - (numsWidth / 2);

        QPen pen(palette().color(QPalette::Text), lineThickness, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
        painter.setPen(pen);
        painter.drawLine(x_pos, y_pos,
                         x_pos + numsWidth,
                         y_pos);
    } else {
        //int lineThickness = std::max(1, (int)((displayFont.pointSize()/8)+0.5));
        int lineThickness = std::max(1, static_cast<int>(std::round(displayFont.pointSize() / 8.0)));
        int hGap = lineThickness*2;

        int firstNumSep = first.count(QLocale::system().groupSeparator());
        int lastNumSep = last.count(QLocale::system().groupSeparator());
        int sepWidth = metrics.horizontalAdvance(QLocale::system().groupSeparator());
        int sepAdj = widestChar - sepWidth;

        int firstWidth = first.size()*widestChar - (firstNumSep*sepAdj);
        int lastWidth = last.size()*widestChar - (lastNumSep*sepAdj);
        int numsWidth = std::max(firstWidth, lastWidth);
        int barHeight = lineThickness * 3;

        int cols = std::max(first.size(), last.size()) + 1;

        // Center the block (operation column + number columns + right gap)
        int totalWidth = cols * widestChar + hGap;
        int rightEdge = width()/2 + totalWidth/2;

        // Line 1 - first
        int y_pos = height() - (metrics.ascent() + barHeight);
        int x_pos = rightEdge - firstWidth;
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
        y_pos += metrics.ascent();
        x_pos = rightEdge - hGap - (cols * widestChar - (lastNumSep*sepAdj));
        painter.drawText(x_pos, y_pos, QString(operation));

        x_pos = rightEdge - (last.size()*widestChar - (lastNumSep*sepAdj));
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
        QPen pen(palette().color(QPalette::Text), lineThickness, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
        painter.setPen(pen);
        painter.drawLine(rightEdge - numsWidth, height() - barHeight/2,
                         rightEdge, height() - barHeight/2);
    }
}

void QuestionDisplayForm::preferencesChanged()
{
    displayFont = Preferences::getInstance().getQuestionFont();
    widestChar = 0;

    // Find out the width of the widest digit we use
    QString ourDigits = "-0123456789.,";
    QFontMetrics metrics(displayFont);
    for (int i = 0; i < ourDigits.size(); ++i)
    {
        int currentDigit = metrics.horizontalAdvance(ourDigits[i]);
        widestChar = std::max(widestChar, currentDigit);
    }

    int minHeight = metrics.height()*2 + 10;
    this->setMinimumHeight(minHeight);
    this->setMaximumHeight(minHeight);

    repaint();
}
