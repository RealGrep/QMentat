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
#ifndef ADDITIONCONFIGFRAME_H
#define ADDITIONCONFIGFRAME_H

#include <QFrame>

class QWidget;
class QString;
class AdditionModule;
class QBigFixedValidator;

namespace Ui {
    class AdditionConfigFrame;
}

class AdditionConfigFrame : public QFrame
{
    Q_OBJECT

public:
    AdditionConfigFrame(QWidget *parent = 0);
    ~AdditionConfigFrame();

    void setModule(AdditionModule *mod);
    void setFirstMinimum(const QString& min);
    void setFirstMaximum(const QString& max);
    void setLastMinimum(const QString& min);
    void setLastMaximum(const QString& max);
    void setLargestNumberFirst(bool b);

    bool applyConfig();

private:
    Ui::AdditionConfigFrame *ui;

    AdditionModule *module;

private slots:

};

#endif // ADDITIONCONFIGFRAME_H

