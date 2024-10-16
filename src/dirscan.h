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

#ifndef DIRSCAN_H
#define DIRSCAN_H

#include <QtDebug>
#include <QDir>

class DirScan
{
public:
    static QList<QString> ListFiles(QString path);
private:
    static QList<QString> files;
    static void scanDir(QDir dir);
};

#endif // DIRSCAN_H
