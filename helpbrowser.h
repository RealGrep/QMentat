#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QTextBrowser>

#include <QHelpEngine>

class HelpBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit HelpBrowser(QWidget *parent = 0);

    void setHelpEngine(QHelpEngine *engine);

private:
    QHelpEngine *helpEngine;

signals:

public slots:
    void setSource(const QUrl &url);


};

#endif // HELPBROWSER_H
