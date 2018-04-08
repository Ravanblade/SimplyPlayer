#ifndef SIMPLYDEFINITIONS_H
#define SIMPLYDEFINITIONS_H

#include <QDir>

#define PLTIME 10
#define LLTIME 25
#define RTTIME 10
#define NAME_LENGTH 290
#define STATE_POS 120
#define CONFIG_PATH QDir::fromNativeSeparators(QApplication::applicationDirPath() + "//data//config.ini")
#define SIGNALS_PATH QDir::fromNativeSeparators(QApplication::applicationDirPath() + "//data//signal.sig")
#define PLAYLISTS_PATH QDir::fromNativeSeparators(QApplication::applicationDirPath() + "//data//playlists//playlists.ini")
#define PLAYLISTD_PATH QDir::fromNativeSeparators(QApplication::applicationDirPath() + "//data//playlists//")
#define STYLES_PATH QDir::fromNativeSeparators(QApplication::applicationDirPath() + "//data//temp//stylesheets//")
#define SKINCONFIG_PATH QDir::fromNativeSeparators( QApplication::applicationDirPath() + "//data//temp//config.ini")
#define SKINIMAGE_PATH QDir::fromNativeSeparators(QApplication::applicationDirPath() + "//data//temp//skin//")
#define SIGNALS_PATH QDir::fromNativeSeparators(QApplication::applicationDirPath() + "//data//signal.sig")
#define SPECTRUMCONFIG_PATH QDir::fromNativeSeparators( QApplication::applicationDirPath() + "//data//temp//spectrum.ini")
#define APPLICATION_PATH QDir::fromNativeSeparators( QApplication::applicationDirPath() + "//")
#define MODULES_PATH QDir::fromNativeSeparators( QApplication::applicationDirPath() + "//data//modules//")

enum SIMPLY_LABEL
{
    STATE_NAME_1 = 0,
    STATE_NAME_2 = 1,
    STATE_VOLUME_1 = 2,
    STATE_BALANCE_1 = 3,
    STATE_VOLUME_2 = 4,
    STATE_BALANCE_2 = 5,
    STATE_CONNECTING =6
};

enum SIMPLY_PLS
{
    NO_ENTRIES = 0,
    INVALID_FORMAT = 1,
    INVALID_DIR = 2
};

enum SIMPLY_PLAYLIST
{
    INVALID_PATH = 0,
    FORMAT_NOT_SUPPORTED = 1
};

enum LOWER_TYPE
{
    PAUSE = 0,
    STOP = 1,
    EXIT2 = 2
};

enum STATE
{
    APPEARING = 0,
    DISAPPEARING = 1,
    EXIT = 2
};

enum SIGNAL_TYPE
{
    INTEGER = 0,
    CHARACTER = 1
};




//typedef PlayerTime PTime;
#endif // SIMPLYDEFINITIONS_H
