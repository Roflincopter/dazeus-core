#-------------------------------------------------
#
# Project created by QtCreator 2010-03-04T14:16:14
#
#-------------------------------------------------

QT       += network sql
QT       -= gui

TARGET    = data/davinci
CONFIG   += console libircclient-qt
CONFIG   -= app_bundle
VERSION   = 1.9.1

TEMPLATE = app
MOC_DIR = build
OBJECTS_DIR = build

SOURCES += main.cpp
SOURCES += davinci.cpp network.cpp config.cpp user.cpp server.cpp database.cpp
HEADERS += davinci.h network.h config.h user.h server.h database.h
# plugins
SOURCES += plugins/pluginmanager.cpp plugins/testplugin.cpp plugins/plugin.cpp
HEADERS += plugins/pluginmanager.h plugins/testplugin.h plugins/plugin.h
SOURCES += plugins/statistics.cpp plugins/karmaplugin.cpp
HEADERS += plugins/statistics.h plugins/karmaplugin.h

# perl plugin
SOURCES += plugins/perlplugin.cpp
HEADERS += plugins/perlplugin.h
# perl implementation
QMAKE_PRE_LINK = "cd plugins/perl; sh make.sh;"
QMAKE_CLEAN += plugins/perl/DaZeus2.c plugins/perl/DaZeus2.o \
      plugins/perl/embedperl.o plugins/perl/xsinit.c plugins/perl/xsinit.o \
      plugins/perl/embedperl.a
LIBS += plugins/perl/embedperl.a -Lplugins/perl -lperl -lircclient-qt
unix {
  !macx {
    LIBS += -Wl,--export-dynamic -lcrypt -ldl
  }
}
