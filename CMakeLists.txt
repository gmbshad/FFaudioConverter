cmake_minimum_required(VERSION 3.16)
project(ffAudioConverter LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(ffAudioConverter
    src/aboutdialog.cpp
    src/aboutdialog.h
    src/dirscan.cpp
    src/dirscan.h
    src/ffmpegtask.cpp
    src/ffmpegtask.h
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/settings.cpp
    src/settings.h
    src/settingsdialog.cpp
    src/settingsdialog.h
    src/translator.cpp
    src/translator.h
    src/util.cpp
    src/util.h
)

target_sources(ffAudioConverter
    PRIVATE
        src/aboutdialog.ui
        src/mainwindow.ui
        src/settingsdialog.ui
)

qt_add_resources(ffAudioConverter_resources ffaudioconverter.qrc)
target_sources(ffAudioConverter
    PRIVATE
        ${ffAudioConverter_resources}
)

target_link_libraries(ffAudioConverter
    PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
