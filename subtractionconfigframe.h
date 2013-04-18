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
#ifndef SUBTRACTIONCONFIGFRAME_H
#define SUBTRACTIONCONFIGFRAME_H

#include <QFrame>

class SubtractionModule;
class QBigFixedValidator;

namespace Ui {
    class SubtractionConfigFrame;
}

class SubtractionConfigFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SubtractionConfigFrame(QWidget *parent = 0);
    ~SubtractionConfigFrame();

    void setModule(SubtractionModule *mod);
    void setFirstMinimum(QString min);
    void setFirstMaximum(QString max);
    void setLastMinimum(QString min);
    void setLastMaximum(QString max);
    void setLargestNumberFirst(bool b);

    bool applyConfig();

private:
    Ui::SubtractionConfigFrame *ui;

    SubtractionModule *module;

private slots:

};

#endif // SUBTRACTIONCONFIGFRAME_H
