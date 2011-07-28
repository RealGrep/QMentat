#include "mathdisplayform.h"
#include "ui_mathdisplayform.h"
#include <iostream>
#include <QtMmlWidget>
#include <QVBoxLayout>

#include "preferences.h"

/*! \class MathDisplayForm
 *  \author Mike Dusseault <mike.dusseault@gmail.com>
 *  Displays more complicated MathML formulas. Useful for those more
 *  fancy questions, like n'th roots and powers.
 */
MathDisplayForm::MathDisplayForm(QWidget *parent) :
        QuestionDisplay(parent),
        //QtMmlWidget(parent),
        ui(new Ui::MathDisplayForm)
{
    ui->setupUi(this);
    //this->setMaximumHeight(100);
    this->setMinimumWidth(100);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    this->setLayout(layout);

    mml_widget = new QtMmlWidget(parent);
    mml_widget->setUpdatesEnabled(true);
    QFont font = Preferences::getInstance().getQuestionFont();
    mml_widget->setBaseFontPointSize(font.pointSize());
    mml_widget->setMinimumHeight(font.pointSize()*3);
    this->setMinimumHeight(font.pointSize()*3);
    //mml_widget->setMaximumHeight(100);
    mml_widget->setMinimumWidth(100);
    layout->addWidget(mml_widget);
    mml_widget->show();

    Preferences::getInstance().addListener(this);
}

MathDisplayForm::~MathDisplayForm()
{
    Preferences::getInstance().removeListener(this);
    delete ui;
}

/*! Updates the currently displayed text of the widget. Expects it's string
 * in MathML format.
 * \param text Text in MathML format to be shown in the widget.
 */
void MathDisplayForm::setText(QString text)
{
    this->text = text;

    QString error_msg;

    mml_widget->clear();
    /*
   //QString fontName = mml_widget->fontName(QtMmlWidget::SansSerifFont);
   QString fontName = mml_widget->fontName(QtMmlWidget::FrakturFont);
   std::cout << fontName.toStdString() << std::endl;
   mml_widget->setFontName(QtMmlWidget::FrakturFont, fontName);
   */
    //mml_widget->setBaseFontPointSize(30);

    int error_line, error_column;
    bool result = mml_widget->setContent(text, &error_msg, &error_line, &error_column);

    if (!result) {
        std::cout << "Parse error: line " << error_line
                << ", col " << error_column
                << ": " << error_msg.toStdString();
        return;
    }

    //this->setMinimumWidth(mml_widget->minimumWidth());
    mml_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mml_widget->adjustSize();
    mml_widget->show();
    this->layout()->update();
}

void MathDisplayForm::preferencesChanged()
{
    QFont font = Preferences::getInstance().getQuestionFont();
    //! \todo Fix this crappy font problem by replacing the entire QMMlWidget.
    mml_widget->setBaseFontPointSize(font.pointSize());
    mml_widget->setMinimumHeight(font.pointSize()*3);
    this->setMinimumHeight(font.pointSize()*3);
    mml_widget->adjustSize();
    mml_widget->update();
    this->layout()->update();
}
