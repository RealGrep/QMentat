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
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QFont>
#include <vector>

class PreferencesListener;

class Preferences
{
public:
    static Preferences& getInstance()
    {
        static Preferences singleton;
        return singleton;
    }

    void addListener(PreferencesListener *listener);
    void removeListener(PreferencesListener *listener);

    void setQuestionFont(const QFont& font);
    QFont getQuestionFont() const;
    void setAnswerFont(const QFont& font);
    QFont getAnswerFont() const;

    void save();
    void restore();

private:
    Preferences();
    QFont questionFont;
    QFont answerFont;

    std::vector<PreferencesListener*> listeners;

    void notifyListeners();
};

#endif // PREFERENCES_H