QT += core dbus sql
QT -= gui

CONFIG += c++11

TARGET = dbus
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    grx-dbus.h
