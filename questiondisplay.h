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
#ifndef QUESTIONDISPLAY_H
#define QUESTIONDISPLAY_H

#include <QWidget>

class QString;

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
