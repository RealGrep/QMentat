#include "questiondisplay.h"

#include <QtGui>

QuestionDisplay::QuestionDisplay(QWidget *parent) :
        QWidget(parent)
{
    this->text = "N/A";
}

QuestionDisplay::~QuestionDisplay()
{
}
