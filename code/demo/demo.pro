#-------------------------------------------------
#
# Project created by QtCreator 2017-08-03T09:37:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo
TEMPLATE = app


SOURCES += main.cpp\
    mythread.cpp \
    startui.cpp \
    filemanager.cpp \
    ping.cpp \
    Net.cpp \
    led.cpp \
    Serial.cpp \
    GPIO.cpp

HEADERS  += \
    mythread.h \
    startui.h \
    filemanager.h \
    Net.h \
    led.h \
    serial.h \
    GPIO.h

FORMS    += \
    Net.ui \
    Serial.ui \
    StartUi.ui \
    GPIO.ui

RESOURCES += \
    img.qrc

DISTFILES += \
    Data.txt
