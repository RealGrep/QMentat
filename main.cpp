/* Copyright (c) 2026 Mike Dusseault
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
#include <QApplication>
#include <QTransform>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QString locale = QLocale::system().name();

    // This looks for "QMentat_fr.qm" inside the embedded resources
    QTranslator translator;
    //if (translator.load(QLocale(), "QMentat", "_", ":/translations")) {
    if (translator.load(QLocale(), "qmentat", "_", ":/translations")) {
        QCoreApplication::installTranslator(&translator);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
