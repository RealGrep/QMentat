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
#ifndef MULTIPLICATIONCONFIGFRAME_H
#define MULTIPLICATIONCONFIGFRAME_H

#include <QFrame>

class QWidget;
class QString;
class QBigFixedValidator;
class MultiplicationModule;

namespace Ui {
    class MultiplicationConfigFrame;
}

class MultiplicationConfigFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MultiplicationConfigFrame(QWidget *parent = 0);
    ~MultiplicationConfigFrame();

    void setModule(MultiplicationModule *mod);
    void setFirstMinimum(const QString& min);
    void setFirstMaximum(const QString& max);
    void setLastMinimum(const QString& min);
    void setLastMaximum(const QString& max);
    void setLargestNumberFirst(bool b);
    void setDecimalPlaces(int decimalPlaces);
    void setRoundingMode(int mode);

    bool applyConfig();

private:
    Ui::MultiplicationConfigFrame *ui;

    MultiplicationModule *module;

private slots:

};

#endif // MULTIPLICATIONCONFIGFRAME_H
