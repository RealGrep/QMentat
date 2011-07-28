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
