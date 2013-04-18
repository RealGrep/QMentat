#include "helpbrowser.h"

#include <QtGui>
#include <QHelpEngine>

//#define XSTR(x) #x
//#define STR(x) XSTR(x)

//#if !defined(SHARE_DIR)
//#   define SHARE_DIR "/home/michel/code/QMentat-install/"
//#endif

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
HelpBrowser::HelpBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
    helpEngine = 0;
    //qDebug() << "HelpEngine: ctor";
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
