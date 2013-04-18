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
#ifndef QUESTIONDISPLAYFORM_H
#define QUESTIONDISPLAYFORM_H

#include <QWidget>
#include <QPaintEvent>
#include <QString>
#include "questiondisplay.h"

#include "preferenceslistener.h"

class QFont;

namespace Ui {
    class QuestionDisplayForm;
}

class QuestionDisplayForm : public QuestionDisplay, public PreferencesListener
{
    Q_OBJECT

public:
    explicit QuestionDisplayForm(QWidget *parent = 0);
    ~QuestionDisplayForm();
    void paintEvent(QPaintEvent *);

    void setText(QString text);

    void preferencesChanged();

private:
    Ui::QuestionDisplayForm *ui;

    QFont displayFont;
    int widestChar;

    QString first;
    QString last;
    QChar operation;
};

#endif // QUESTIONDISPLAYFORM_H
