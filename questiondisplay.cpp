#include "questiondisplay.h"

//#include <QtGui>
//#include <iostream>

QuestionDisplay::QuestionDisplay(QWidget *parent) :
    QWidget(parent)
{
    //this->setMinimumHeight(100);
    //this->setMaximumHeight(100);
    this->text = "N/A";
}

QuestionDisplay::~QuestionDisplay()
{
    //delete ui;
}

/*
void QuestionDisplayForm::setText(QString text)
{
   this->text = text;
   //this->repaint();
}
*/
/*
void QuestionDisplayForm::paintEvent(QPaintEvent *)
 {
     QPainter painter(this);
     painter.setPen(Qt::black);
     painter.setFont(QFont("Arial", 30));
     painter.drawText(rect(), Qt::AlignRight, this->text);
     painter.setPen(Qt::black);
     int height = this->height();
     int width = this->width();
     QPen pen(Qt::black, 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
     painter.setPen(pen);
     painter.drawLine(width - 100, height - 10, width, height - 10);
 }
*/
