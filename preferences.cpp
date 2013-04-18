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
#include "preferences.h"

#include <QtGui>
#include <vector>

#include "preferenceslistener.h"

Preferences::Preferences()
{
    questionFont = QFont("Arial", 30);
    answerFont = QFont("Arial", 30);
}

void Preferences::save()
{
    QSettings settings;
    settings.setValue("main/questionfont", questionFont.toString());
    settings.setValue("main/answerfont", answerFont.toString());
}

void Preferences::restore()
{
    QSettings settings;
    questionFont.fromString(settings.value("main/questionfont", QFont("Arial", 30).toString()).toString());
    answerFont.fromString(settings.value("main/answerfont", QFont("Arial", 30).toString()).toString());
    notifyListeners();
}

void Preferences::addListener(PreferencesListener *listener)
{
    listeners.push_back(listener);
}

void Preferences::removeListener(PreferencesListener *listener)
{
    for (size_t i = 0; i < listeners.size(); i++)
    {
        if (listeners[i] == listener)
        {
            listeners.erase(listeners.begin()+i);
        }
    }
}

void Preferences::notifyListeners()
{
    //qDebug() << "Notify all listeners.";
    //qDebug() << "There are " << listeners.size() << " listeners.";
    for (size_t i = 0; i < listeners.size(); i++)
    {
        listeners[i]->preferencesChanged();
    }
}

void Preferences::setQuestionFont(const QFont &font)
{
    questionFont = font;
    notifyListeners();
}

QFont Preferences::getQuestionFont() const
{
    return questionFont;
}

void Preferences::setAnswerFont(const QFont &font)
{
    answerFont = font;
    notifyListeners();
}

QFont Preferences::getAnswerFont() const
{
    return answerFont;
}
