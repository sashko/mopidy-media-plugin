TARGET = media_mopidy
QT_FOR_CONFIG += ivimedia-private

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = ivimedia
PLUGIN_CLASS_NAME = MopidyMediaPlugin

QT += core ivicore ivimedia multimedia websockets

CONFIG += c++11

load(qt_plugin)

qtConfig(system-taglib) {
    QMAKE_USE += taglib
} else:qtConfig(taglib) {
    include($$PWD/../../../3rdparty/taglib/taglib.pri)
}

DISTFILES += media_mopidy.json

HEADERS += \
    mediaplugin.h \
    mediaplayerbackend.h \
    mediaindexerbackend.h \
    logging.h \
    mopidy/basecontroller.h \
    mopidy/basecontroller_p.h \
    mopidy/eventhandler.h \
    mopidy/eventhandler_p.h \
    mopidy/jsonrpchandler.h \
    mopidy/jsonrpchandler_p.h \
    mopidy/librarycontroller.h \
    mopidy/libraryhelper.h \
    mopidy/libraryhelper_p.h \
    mopidy/mixercontroller.h \
    mopidy/models.h \
    mopidy/modeltranslator.h \
    mopidy/mopidyclient.h \
    mopidy/mopidyclient_p.h \
    mopidy/playbackcontroller.h \
    mopidy/playlistscontroller.h \
    mopidy/tracklistcontroller.h

SOURCES += \
    mediaplugin.cpp \
    mediaplayerbackend.cpp \
    mediaindexerbackend.cpp \
    logging.cpp \
    mopidy/basecontroller.cpp \
    mopidy/eventhandler.cpp \
    mopidy/jsonrpchandler.cpp \
    mopidy/librarycontroller.cpp \
    mopidy/libraryhelper.cpp \
    mopidy/mixercontroller.cpp \
    mopidy/modeltranslator.cpp \
    mopidy/mopidyclient.cpp \
    mopidy/playbackcontroller.cpp \
    mopidy/playlistscontroller.cpp \
    mopidy/tracklistcontroller.cpp
