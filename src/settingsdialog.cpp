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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>

#include "util.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // Disable resize
    setFixedSize(width(), height());

    // Init settings elements
    ui->comboBox_OutputFormat->addItem("MP3 (.mp3)", "mp3");
    ui->comboBox_OutputFormat->addItem("AAC (.m4a)", "m4a");
    ui->comboBox_OutputFormat->addItem("Ogg Vorbis (.ogg)", "ogg");
    ui->comboBox_OutputFormat->addItem("Opus (.opus)", "opus");
    ui->comboBox_OutputFormat->addItem("FLAC (.flac)", "flac");
    ui->comboBox_OutputFormat->addItem("WAV (.wav)", "wav");

    ui->comboBox_OutputSamplerate->addItem(tr("Keep"), "");
    ui->comboBox_OutputSamplerate->addItem("44100 Hz", "44100");
    ui->comboBox_OutputSamplerate->addItem("48000 Hz", "48000");
    ui->comboBox_OutputSamplerate->addItem("96000 Hz", "96000");

    ui->comboBox_OutputChannelCount->addItem(tr("Keep"), 0);
    ui->comboBox_OutputChannelCount->addItem(tr("1 (Mono)"), 1);
    ui->comboBox_OutputChannelCount->addItem(tr("2 (Stereo)"), 2);
    ui->comboBox_OutputChannelCount->addItem(tr("5.1 (DolbyProLogic"), 6);
    ui->comboBox_OutputChannelCount->addItem(tr("7.1 (DolbySurround"), 8);
    ui->comboBox_OutputChannelCount->addItem(tr("5.1.4 (DolbyAtmos)"), 10);
    ui->comboBox_OutputChannelCount->addItem(tr("7.1.4 (DolbySpatial3D"), 12);
    
    ui->comboBox_AudioFiltersPresets->addItem(tr("Disabled"), "");
    ui->comboBox_AudioFiltersPresets->addItem(tr("Increase volume"), "volume=6dB");
    ui->comboBox_AudioFiltersPresets->addItem(tr("Reduce volume"), "volume=-6dB");
    ui->comboBox_AudioFiltersPresets->addItem(tr("Fade-in and fade-out"), "afade=duration=5\nareverse\nafade=duration=5\nareverse");
    ui->comboBox_AudioFiltersPresets->addItem(tr("Spatial/Surround ADM BWF Panner + REDDeffects"), "pan=7.1+TFL+TFR+TBL+TBR|FL=c0|FR=c1|FC=c2|LFE=c3|SL=c6|SR=c7|BL=c4|BR=c5|TFL=c8|TFR=c9|TBL=c10|TBR=c11\ndynaudnorm\nsurround\nsuperequalizer\ndeesser\naresample=matrix_encoding=dolby\naemphasis=type=cd\nacompressor\ncrystalizer");
    ui->comboBox_AudioFiltersPresets->addItem(tr("Apply REDDeffects"), "extrastereo\nsuperequalizer\naresample=matrix_encoding=dplii\naemphasis=type=cd\nacompressor\ncrystalizer");
    ui->comboBox_AudioFiltersPresets->addItem(tr("Bass boost"), "aemphasis=type=riaa\nsuperequalizer\ndynaudnorm\nloudnorm");
    ui->comboBox_AudioFiltersPresets->addItem(tr("Speed up"), "atempo=.6");
    ui->comboBox_AudioFiltersPresets->setCurrentIndex(-1);

    // Set settings in GUI
    LoadSettings();

    // Fix cursor position in output directory textfield
    ui->lineEdit_OutputDirectory->setCursorPosition(0);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    ApplySettings();
}

void SettingsDialog::ApplySettings()
{
    Settings::FFmpegBinary = ui->lineEdit_FFmpegBinary->text().trimmed();
    Settings::OutputFormat = ui->comboBox_OutputFormat->currentData().toString();
    Settings::OutputQualityArguments = ui->comboBox_Quality->currentData().toString();
    Settings::OutputDirectory = ui->lineEdit_OutputDirectory->text().trimmed();
    Settings::OutputSamplerate = ui->comboBox_OutputSamplerate->currentData().toString();
    Settings::OutputChannelCount = ui->comboBox_OutputChannelCount->currentData().toInt();
    Settings::AudioFilters = ui->plainTextEdit_AudioFilters->toPlainText().trimmed();
    Settings::QuickConvertMode = ui->checkBox_QuickConvertMode->isChecked();
    Settings::CopyNonAudioFiles = ui->checkBox_CopyNonAudioFiles->isChecked();
    Settings::UseSoXresampler = ui->checkBox_UseSoXresampler->isChecked();
    Settings::Threads = ui->spinBox_Threads->value();

    Settings::save();
}

void SettingsDialog::LoadSettings()
{
    ui->lineEdit_FFmpegBinary->setText(Settings::FFmpegBinary);
    ui->comboBox_OutputFormat->setCurrentIndex(ui->comboBox_OutputFormat->findData(Settings::OutputFormat));
    updateQualityComboBox(); // Fill quality options depending on output format
    ui->comboBox_Quality->setCurrentIndex(ui->comboBox_Quality->findData(Settings::OutputQualityArguments));
    ui->lineEdit_OutputDirectory->setText(Settings::OutputDirectory);
    ui->comboBox_OutputSamplerate->setCurrentIndex(ui->comboBox_OutputSamplerate->findData(Settings::OutputSamplerate));
    ui->comboBox_OutputChannelCount->setCurrentIndex(ui->comboBox_OutputChannelCount->findData(Settings::OutputChannelCount));
    ui->plainTextEdit_AudioFilters->setPlainText(Settings::AudioFilters);
    ui->checkBox_QuickConvertMode->setChecked(Settings::QuickConvertMode);
    ui->checkBox_CopyNonAudioFiles->setChecked(Settings::CopyNonAudioFiles);
    ui->checkBox_UseSoXresampler->setChecked(Settings::UseSoXresampler);
    ui->spinBox_Threads->setValue(Settings::Threads);
}

void SettingsDialog::on_toolButton_SelectOutputDirectory_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), Settings::OutputDirectory, QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    if(!Util::isNullOrEmpty(dir)) {
        ui->lineEdit_OutputDirectory->setText(dir + QDir::separator() + "{sourcedir}");
    }
}

void SettingsDialog::on_comboBox_AudioFiltersPresets_activated(int index)
{
    ui->plainTextEdit_AudioFilters->setPlainText(ui->comboBox_AudioFiltersPresets->itemData(index).toString());
}

void SettingsDialog::on_comboBox_OutputFormat_activated(int /* unused */)
{
    updateQualityComboBox();
}

void SettingsDialog::updateQualityComboBox() // Fill quality options (depending on output format)
{
    QString format = ui->comboBox_OutputFormat->currentData().toString();

    ui->comboBox_Quality->clear();

    if (format == "mp3") {
        ui->comboBox_Quality->addItem("VBR, 0, ~245 kbps", "-q:a 0");
        ui->comboBox_Quality->addItem("VBR, 1, ~225 kbps", "-q:a 1");
        ui->comboBox_Quality->addItem("VBR, 2, ~190 kbps", "-q:a 2");
        ui->comboBox_Quality->addItem("VBR, 3, ~175 kbps", "-q:a 3");
        ui->comboBox_Quality->addItem("VBR, 4, ~165 kbps", "-q:a 4");
        ui->comboBox_Quality->addItem("VBR, 5, ~130 kbps", "-q:a 5");
        ui->comboBox_Quality->addItem("VBR, 6, ~115 kbps", "-q:a 6");
        ui->comboBox_Quality->addItem("VBR, 7, ~100 kbps", "-q:a 7");
        ui->comboBox_Quality->addItem("CBR, 320 kbps", "-b:a 320k");
        ui->comboBox_Quality->addItem("CBR, 256 kbps", "-b:a 256k");
        ui->comboBox_Quality->addItem("CBR, 224 kbps", "-b:a 224k");
        ui->comboBox_Quality->addItem("CBR, 192 kbps", "-b:a 192k");
        ui->comboBox_Quality->addItem("CBR, 160 kbps", "-b:a 160k");
        ui->comboBox_Quality->addItem("CBR, 128 kbps", "-b:a 128k");
        ui->comboBox_Quality->addItem("CBR, 96 kbps", "-b:a 96k");
        ui->comboBox_Quality->addItem("CBR, 64 kbps", "-b:a 64k");
        ui->comboBox_Quality->addItem("CBR, 32 kbps", "-b:a 32k");

        ui->comboBox_Quality->setCurrentIndex(2);

    } else if (format == "m4a") {
        ui->comboBox_Quality->addItem("CBR, 320 kbps", "-b:a 320k");
        ui->comboBox_Quality->addItem("CBR, 256 kbps", "-b:a 256k");
        ui->comboBox_Quality->addItem("CBR, 224 kbps", "-b:a 224k");
        ui->comboBox_Quality->addItem("CBR, 192 kbps", "-b:a 192k");
        ui->comboBox_Quality->addItem("CBR, 160 kbps", "-b:a 160k");
        ui->comboBox_Quality->addItem("CBR, 128 kbps", "-b:a 128k");
        ui->comboBox_Quality->addItem("CBR, 96 kbps", "-b:a 96k");
        ui->comboBox_Quality->addItem("CBR, 64 kbps", "-b:a 64k");
        ui->comboBox_Quality->addItem("CBR, 32 kbps", "-b:a 32k");

        ui->comboBox_Quality->setCurrentIndex(4);

    } else if (format == "ogg") {
        ui->comboBox_Quality->addItem("VBR, 9, ~320 kbps", "-q:a 9");
        ui->comboBox_Quality->addItem("VBR, 8, ~256 kbps", "-q:a 8");
        ui->comboBox_Quality->addItem("VBR, 7, ~224 kbps", "-q:a 7");
        ui->comboBox_Quality->addItem("VBR, 6, ~192 kbps", "-q:a 6");
        ui->comboBox_Quality->addItem("VBR, 5, ~160 kbps", "-q:a 5");
        ui->comboBox_Quality->addItem("VBR, 4, ~128 kbps", "-q:a 4");
        ui->comboBox_Quality->addItem("VBR, 3, ~112 kbps", "-q:a 3");
        ui->comboBox_Quality->addItem("VBR, 2, ~96 kbps", "-q:a 2");
        ui->comboBox_Quality->addItem("VBR, 1, ~80 kbps", "-q:a 1");
        ui->comboBox_Quality->addItem("VBR, 0, ~64 kbps", "-q:a 0");
        ui->comboBox_Quality->addItem("CBR, 320 kbps", "-b:a 320k");
        ui->comboBox_Quality->addItem("CBR, 256 kbps", "-b:a 256k");
        ui->comboBox_Quality->addItem("CBR, 224 kbps", "-b:a 224k");
        ui->comboBox_Quality->addItem("CBR, 192 kbps", "-b:a 192k");
        ui->comboBox_Quality->addItem("CBR, 160 kbps", "-b:a 160k");
        ui->comboBox_Quality->addItem("CBR, 128 kbps", "-b:a 128k");
        ui->comboBox_Quality->addItem("CBR, 96 kbps", "-b:a 96k");
        ui->comboBox_Quality->addItem("CBR, 64 kbps", "-b:a 64k");
        ui->comboBox_Quality->addItem("CBR, 48 kbps", "-b:a 48k");
        ui->comboBox_Quality->addItem("CBR, 32 kbps", "-b:a 32k");

        ui->comboBox_Quality->setCurrentIndex(4);

    } else if (format == "opus") {
        ui->comboBox_Quality->addItem("ABR, 256 kbps", "-b:a 256k");
        ui->comboBox_Quality->addItem("ABR, 224 kbps", "-b:a 224k");
        ui->comboBox_Quality->addItem("ABR, 192 kbps", "-b:a 192k");
        ui->comboBox_Quality->addItem("ABR, 160 kbps", "-b:a 160k");
        ui->comboBox_Quality->addItem("ABR, 128 kbps", "-b:a 128k");
        ui->comboBox_Quality->addItem("ABR, 112 kbps", "-b:a 112k");
        ui->comboBox_Quality->addItem("ABR, 96 kbps", "-b:a 96k");
        ui->comboBox_Quality->addItem("ABR, 80 kbps", "-b:a 80k");
        ui->comboBox_Quality->addItem("ABR, 64 kbps", "-b:a 64k");
        ui->comboBox_Quality->addItem("ABR, 48 kbps", "-b:a 48k");
        ui->comboBox_Quality->addItem("ABR, 32 kbps", "-b:a 32k");

        ui->comboBox_Quality->setCurrentIndex(4);

    } else if (format == "flac") {
        ui->comboBox_Quality->addItem("auto", "");
        ui->comboBox_Quality->addItem("16 bit", "-sample_fmt s16");

        ui->comboBox_Quality->setCurrentIndex(0);

    } else if (format == "wav") {
        ui->comboBox_Quality->addItem("32 bit", "-c:a pcm_s32le");
        ui->comboBox_Quality->addItem("24 bit", "-c:a pcm_s24le");
        ui->comboBox_Quality->addItem("16 bit", "-c:a pcm_s16le");

        ui->comboBox_Quality->setCurrentIndex(2);

    }
}
