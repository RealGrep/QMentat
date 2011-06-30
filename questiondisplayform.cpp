#include "questiondisplayform.h"
#include "ui_questiondisplayform.h"
#include <QtGui>
#include <QRect>
#include <iostream>
#include "questiondisplay.h"

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
    //std::cerr << "QuestionDisplayForm" << std::endl;
    this->setMinimumHeight(100);
    this->setMaximumHeight(100);
    this->text = tr("N/A");
}

QuestionDisplayForm::~QuestionDisplayForm()
{
    delete ui;
}

/*! Changes the text being displayed by the widget, and causes a repaint.
 * \param text The new text to be displayed, in straight ASCII.
 */
void QuestionDisplayForm::setText(QString text)
{
    this->text = text;
    //this->setText(text);
    this->repaint();
}

/*! Handles paint even by rendering formula to widget
 */
void QuestionDisplayForm::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 30));

    // Figure out how wide the text will be when drawn
    QRect textRect = painter.fontMetrics().boundingRect(rect(), Qt::AlignRight, this->text);
    int textWidth = textRect.width();

    // Draw the text
    painter.drawText(rect(), Qt::AlignRight, this->text);

    // Draw a line under all the text, of the right length
    painter.setPen(Qt::black);
    QPen pen(Qt::black, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine(this->width() - textWidth, this->height() - 10, this->width(), this->height() - 10);
}
