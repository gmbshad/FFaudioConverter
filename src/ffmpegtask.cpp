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

#include "ffmpegtask.h"
#include "mainwindow.h"

FFmpegTask::FFmpegTask(int id, QString inpath)
{
    this->id = id;
    this->infile = inpath;
    this->outdir = Settings::OutputDirectory;
}

void FFmpegTask::run()
{
    qDebug() << "Starting job" << id << "|" << infile;

    // Skip or copy files not containing audio according to settings
    if(Util::mayBeAudioOrVideoFile(infile)) {
        prepareOutput();
    } else {
        if(Settings::CopyNonAudioFiles) {
            qDebug() << "Copy file" << id;
            prepareOutput();
            QFile::copy(infile, outdir + QDir::separator() + infileInfo.fileName());
            emit StatusChange(id, ConvertStatus::Done);
        } else {
            qDebug() << "Skipping job" << id;
            emit StatusChange(id, ConvertStatus::Skipped);
        }
        emit ConvertDone(id);
        return;
    }

    prepareFFmpeg();

    // Skip this convert job under some conditions in fast mode
    if(Settings::QuickConvertMode) {
        // Skip file if it has already been converted
        if(QFile::exists(outfile)) {
            qDebug() << "Skipping job" << id;
            emit StatusChange(id, ConvertStatus::Skipped);
            emit ConvertDone(id);
            return;
        }
        // Copy file if input and output format are the same
        if(infileInfo.suffix().toLower() == outfileExt) {
            qDebug() << "Copy file of job" << id;
            if(QFile::copy(infile, outfile)) {
                emit StatusChange(id, ConvertStatus::Done);
            } else {
                emit StatusChange(id, ConvertStatus::Failed);
            }
            emit ConvertDone(id);
            return;
        }
    }

    emit StatusChange(id, ConvertStatus::Converting);

    // Create FFmpeg process
    QProcess *ffmpeg = new QProcess();

    ffmpeg->setProgram(Settings::FFmpegBinary);
    ffmpeg->setArguments(ffmpegArgs);

    QString result;
    ffmpeg->setProcessChannelMode(QProcess::MergedChannels);
    QObject::connect(ffmpeg, &QProcess::readyReadStandardOutput, [ffmpeg,&result]()->void{result+=ffmpeg->readAllStandardOutput();});

    // Run FFmpeg process
    ffmpeg->start();
    ffmpeg->waitForFinished(-1);
    int exitCode = ffmpeg->exitCode();

    qDebug() << "Result of job" << id << "|" << result;
    qDebug() << "Finished job" << id << "with exit code" << exitCode;

    bool success = (exitCode == 0);
    if(success) {
        emit StatusChange(id, ConvertStatus::Done);
    } else {
        emit StatusChange(id, ConvertStatus::Failed);
    }
    emit ConvertDone(id);
}

void FFmpegTask::prepareOutput() {
    infileInfo = QFileInfo(infile);
    QString sourcedir = infileInfo.absoluteDir().path();
#ifdef Q_OS_WIN
    // Fix path on Windows
    if(!outdir.startsWith("{sourcedir}")) {
        sourcedir = sourcedir.replace(":", "");
    }
#endif
    outdir = outdir.replace("{sourcedir}", sourcedir);  // Allow to use source directory as variable in output directory
    QDir().mkpath(outdir); // Create output dir if it does not exist
}

void FFmpegTask::prepareFFmpeg() {
    ffmpegArgs << "-hide_banner";
    if(Settings::QuickConvertMode) {
        ffmpegArgs << "-n";  // Not overwrite (keep existing file)
    } else {
        ffmpegArgs << "-y";  // Overwrite (reconvert if file exists)
    }
    ffmpegArgs << "-vn"; // Ignore video of input file, sadly this also removes the cover art
    ffmpegArgs << "-i" << infile;

    if(Settings::OutputFormat == "mp3") {
        outfileExt = "mp3";
        ffmpegArgs << "-c:a" << "libmp3lame";
        foreach (const QString &arg, Settings::OutputQualityArguments.split(" ")) {
            if (arg.length() > 0) ffmpegArgs << arg;
        }
        if (!Util::isNullOrEmpty(Settings::OutputSamplerate)) ffmpegArgs << "-ar" << Settings::OutputSamplerate;
        if (Settings::OutputChannelCount > 0) ffmpegArgs << "-ac" << QString::number(Settings::OutputChannelCount);
        ffmpegArgs << "-map_metadata" << "0";
        ffmpegArgs << "-map_metadata" << "0:s:0";
        ffmpegArgs << "-id3v2_version" << "3";

    } else if(Settings::OutputFormat == "m4a") {
        outfileExt = "m4a";
        ffmpegArgs << "-c:a" << "aac";
        foreach (const QString &arg, Settings::OutputQualityArguments.split(" ")) {
            if (arg.length() > 0) ffmpegArgs << arg;
        }
        if (!Util::isNullOrEmpty(Settings::OutputSamplerate)) ffmpegArgs << "-ar" << Settings::OutputSamplerate;
        if (Settings::OutputChannelCount > 0) ffmpegArgs << "-ac" << QString::number(Settings::OutputChannelCount);
        ffmpegArgs << "-map_metadata" << "0";
        ffmpegArgs << "-map_metadata" << "0:s:0";

    } else if(Settings::OutputFormat == "ogg") {
        outfileExt = "ogg";
        ffmpegArgs << "-c:a" << "libvorbis";
        foreach (const QString &arg, Settings::OutputQualityArguments.split(" ")) {
            if (arg.length() > 0) ffmpegArgs << arg;
        }
        if (!Util::isNullOrEmpty(Settings::OutputSamplerate)) ffmpegArgs << "-ar" << Settings::OutputSamplerate;
        if (Settings::OutputChannelCount > 0) ffmpegArgs << "-ac" << QString::number(Settings::OutputChannelCount);
        ffmpegArgs << "-map_metadata" << "0";
        ffmpegArgs << "-map_metadata" << "0:s:0";

    } else if(Settings::OutputFormat == "opus") {
        outfileExt = "opus";
        ffmpegArgs << "-c:a" << "libopus";  //TODO: opus currently loses album art (at least with ffmpeg 4.1)
        foreach (const QString &arg, Settings::OutputQualityArguments.split(" ")) {
            if (arg.length() > 0) ffmpegArgs << arg;
        }
        // ChangeSamplerate not possible for opus (always uses 48 kHz)
        if (Settings::OutputChannelCount > 0) ffmpegArgs << "-ac" << QString::number(Settings::OutputChannelCount);
        ffmpegArgs << "-map_metadata" << "0";
        ffmpegArgs << "-map_metadata" << "0:s:0";

    } else if(Settings::OutputFormat == "flac") {
        outfileExt = "flac";
        ffmpegArgs << "-c:a" << "flac";
        foreach (const QString &arg, Settings::OutputQualityArguments.split(" ")) {
            if (arg.length() > 0) ffmpegArgs << arg;
        }
        if (!Util::isNullOrEmpty(Settings::OutputSamplerate)) ffmpegArgs << "-ar" << Settings::OutputSamplerate;
        if (Settings::OutputChannelCount > 0) ffmpegArgs << "-ac" << QString::number(Settings::OutputChannelCount);
        ffmpegArgs << "-map_metadata" << "0";
        ffmpegArgs << "-map_metadata" << "0:s:0";

    } else if(Settings::OutputFormat == "wav") {
        outfileExt = "wav";
        foreach (const QString &arg, Settings::OutputQualityArguments.split(" ")) {
            if (arg.length() > 0) ffmpegArgs << arg;
        }
        if (!Util::isNullOrEmpty(Settings::OutputSamplerate)) ffmpegArgs << "-ar" << Settings::OutputSamplerate;
        if (Settings::OutputChannelCount > 0) ffmpegArgs << "-ac" << QString::number(Settings::OutputChannelCount);

    } else {
        // unknown format options (this should never happen)
        outfileExt = Settings::OutputFormat;
    }

    // Add SoX
    if(Settings::UseSoXresampler) {
        audioFilters.append("aresample=resampler=soxr ");  // Space at the end is important to allow further audio filters
    }

    // Add user audio filters
    audioFilters.append(Settings::AudioFilters);

    // Apply audio filters
    QString audioFiltersArg = audioFilters.simplified().replace(" ", ",");  // Convert newlines to spaces and spaces to commas
    if(!Util::isNullOrEmpty(audioFiltersArg)) {
        ffmpegArgs << "-af" << audioFiltersArg;
    }

    // Set output file
    outfile = outdir + QDir::separator() + infileInfo.completeBaseName() + "." + outfileExt;
    ffmpegArgs << outfile;
}
