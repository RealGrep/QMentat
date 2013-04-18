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
#ifndef ROOTSCONFIGFRAME_H
#define ROOTSCONFIGFRAME_H

#include <QFrame>

class QString;
class RootsModule;

namespace Ui {
    class RootsConfigFrame;
}

class RootsConfigFrame : public QFrame
{
    Q_OBJECT

public:
    explicit RootsConfigFrame(QWidget *parent = 0);
    ~RootsConfigFrame();

    void setModule(RootsModule *mod);
    void setMinimum(QString min);
    void setMaximum(QString max);
    void setRootMinimum(int min);
    void setRootMaximum(int max);
    void setDecimalPlaces(int newDecimals);
    void setRoundingMode(int mode);
    void setIntegersOnly(bool intsOnly);

    bool applyConfig();

private:
    Ui::RootsConfigFrame *ui;

    RootsModule *module;

private slots:
    void on_integerResultCheckBox_stateChanged(int );

};

#endif // ROOTSCONFIGFRAME_H
