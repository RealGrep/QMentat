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
