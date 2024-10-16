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

#ifndef FFMPEGTASK_H
#define FFMPEGTASK_H

#include <QDebug>
#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QFile>
#include <QDir>

class FFmpegTask : public QObject, public QRunnable
{
    Q_OBJECT

public:
    FFmpegTask(int id, QString inpath);

    enum ConvertStatus {  // registered in "main.cpp"
        Pending,
        Converting,
        Failed,
        Skipped,
        Done
    };

private:
    int id;
    QString infile, infileExt;
    QFileInfo infileInfo;
    QString outdir, outfile, outfileExt;

    void prepareOutput();
    void prepareFFmpeg();

    QStringList ffmpegArgs;
    QString audioFilters;

signals:
    void StatusChange(int id, FFmpegTask::ConvertStatus status);
    void ConvertDone(int id);

protected:
    void run();
};

#endif // FFMPEGTASK_H
