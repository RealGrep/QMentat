#ifndef QUESTIONDISPLAY_H
#define QUESTIONDISPLAY_H

#include <QWidget>

class QuestionDisplay : public QWidget
{
    Q_OBJECT
public:
    QuestionDisplay(QWidget *parent = 0);
    virtual ~QuestionDisplay() = 0;
    virtual void setText(QString text) = 0;

protected:
    QString text;


signals:

public slots:

};

#endif // QUESTIONDISPLAY_H
