/* ----------------------------------------------------------------------
FFaudioConverter
Copyright (C) 2018-2024  REDDev
https://github.com/gmbshad/FFaudioConverter
gmbgetmoney@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
---------------------------------------------------------------------- */

#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include "translator.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("REDDev");
    QCoreApplication::setApplicationName("FFaudioConverter");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QGuiApplication::setDesktopFileName("com.github.REDDev.FFaudioConverter");

    QCommandLineParser commandLine;
    commandLine.addHelpOption();
    commandLine.addVersionOption();
    commandLine.addPositionalArgument("path", "Open with files or directories added");
    QCommandLineOption noTranslationsOption("no-translations", "Do not load translations");
    commandLine.addOption(noTranslationsOption);
    commandLine.process(a);

    qDebug().noquote() << "Starting" << QCoreApplication::applicationName() + " v" + QCoreApplication::applicationVersion();

      a.setWindowIcon(QIcon(":/com.github.REDDev.FFaudioConverter.ico"));

#ifdef Q_OS_WIN
    a.setStyle(QStyleFactory::create("Fusion"));  // Use Fusion style on Windows because it looks better
#endif

    // Register ENUMs
    qRegisterMetaType<FFmpegTask::ConvertStatus>("FFmpegTask::ConvertStatus");

    // Load settings
    Settings::init();

    // Load translations
    if (!commandLine.isSet(noTranslationsOption)) Translator::init();

    // Show GUI
    MainWindow w;
    w.show();

    if(commandLine.positionalArguments().length() > 0) {
        w.processCommandLine(commandLine.positionalArguments());
    }

    return a.exec();
}
