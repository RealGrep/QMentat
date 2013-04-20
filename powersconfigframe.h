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
#ifndef POWERSCONFIGFRAME_H
#define POWERSCONFIGFRAME_H

#include <QFrame>

class PowersModule;
class QString;
class QBigFixedValidator;

namespace Ui {
    class PowersConfigFrame;
}

class PowersConfigFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PowersConfigFrame(QWidget *parent = 0);
    ~PowersConfigFrame();

    void setModule(PowersModule *mod);
    void setMinimum(const QString& min);
    void setMaximum(const QString& max);
    void setPowerMinimum(int min);
    void setPowerMaximum(int max);
    void setDecimalPlaces(int newDecimals);
    void setRoundingMode(int mode);

    bool applyConfig();

private:
    Ui::PowersConfigFrame *ui;

    PowersModule *module;

private slots:
    /*
    void on_roundingComboBox_currentIndexChanged(int index);
    void on_decimalPlacesLineEdit_editingFinished();
    void on_maxPowerLineEdit_editingFinished();
    void on_minPowerLineEdit_editingFinished();
    void on_maxNumberLineEdit_editingFinished();
    void on_minNumberLineEdit_editingFinished();
    */
};

#endif // POWERSCONFIGFRAME_H
