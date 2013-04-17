#include "helpbrowser.h"

#include <QtGui>
#include <QHelpEngine>

//#define XSTR(x) #x
//#define STR(x) XSTR(x)

//#if !defined(SHARE_DIR)
//#   define SHARE_DIR "/home/michel/code/QMentat-install/"
//#endif

HelpBrowser::HelpBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
    helpEngine = 0;
    qDebug() << "HelpEngine: ctor";
}

void HelpBrowser::setHelpEngine(QHelpEngine *engine)
{
    helpEngine = engine;
}

void HelpBrowser::setSource(const QUrl &url)
{
    if (helpEngine != 0)
    {
        //qDebug() << "setSource = " << url.toString();

        QByteArray helpData = helpEngine->fileData(url);
        // show the documentation to the user
        if (!helpData.isEmpty())
        {
            setHtml(QLatin1String(helpData));
        }

        int anchorPos = url.toString().indexOf(QChar('#'));
        if (anchorPos >= 0)
        {
            QString anchor = url.toString().mid(anchorPos+1);
            //qDebug() << "Anchor = " << anchor;
            scrollToAnchor(anchor);
        }
    }
}
