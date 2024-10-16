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

#include "settings.h"

QString Settings::FFmpegBinary;
QString Settings::OutputFormat;
QString Settings::OutputQualityArguments;
QString Settings::OutputDirectory;
QString Settings::OutputSamplerate;
int Settings::OutputChannelCount;
QString Settings::AudioFilters;
bool Settings::QuickConvertMode;
bool Settings::CopyNonAudioFiles;
bool Settings::UseSoXresampler;
int Settings::Threads;

void Settings::init() {
    QSettings settings;

    Settings::FFmpegBinary = settings.value("FFmpegBinary", "/usr/bin/ffmpeg").toString();
    Settings::OutputFormat = settings.value("OutputFormat", "wav").toString();
    Settings::OutputQualityArguments = settings.value("OutputQualityArguments", "-q:a 0 -map_metadata 0").toString();
    Settings::OutputDirectory = settings.value("OutputDirectory", "QStandardPaths::writableLocation(QStandardPaths::HomeLocation").toString();
    Settings::OutputSamplerate = settings.value("OutputSamplerate", "").toString();
    Settings::OutputChannelCount = settings.value("OutputChannelCount", 0).toInt();
    Settings::AudioFilters = settings.value("AudioFilters", "#REMEMBER to Apply REDDeffects for ADM/BWF WAV files if converstion fails!").toString();
    Settings::QuickConvertMode = settings.value("QuickConvertMode", false).toBool();
    Settings::CopyNonAudioFiles = settings.value("CopyNonAudioFiles", true).toBool();
    Settings::UseSoXresampler = settings.value("UseSoXresampler", false).toBool();
    Settings::Threads = settings.value("Threads", 0).toInt();

    qDebug() << "Settings loaded";
}

void Settings::save() {
    QSettings settings;

    settings.setValue("FFmpegBinary", Settings::FFmpegBinary);
    settings.setValue("OutputFormat", Settings::OutputFormat);
    settings.setValue("OutputQualityArguments", Settings::OutputQualityArguments);
    settings.setValue("OutputDirectory", Settings::OutputDirectory);
    settings.setValue("OutputSamplerate", Settings::OutputSamplerate);
    settings.setValue("OutputChannelCount", Settings::OutputChannelCount);
    settings.setValue("AudioFilters", Settings::AudioFilters);
    settings.setValue("QuickConvertMode", Settings::QuickConvertMode);
    settings.setValue("CopyNonAudioFiles", Settings::CopyNonAudioFiles);
    settings.setValue("UseSoXresampler", Settings::UseSoXresampler);
    settings.setValue("Threads", Settings::Threads);

    qDebug() << "Settings saved";
}
