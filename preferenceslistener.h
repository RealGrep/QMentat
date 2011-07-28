#ifndef PREFERENCESLISTENER_H
#define PREFERENCESLISTENER_H

class PreferencesListener {
    //virtual ~PreferencesListener();
public:
    virtual void preferencesChanged() = 0;
};

#endif // PREFERENCESLISTENER_H
