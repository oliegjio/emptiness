QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = emptiness
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    singleapplication.cpp \
    textedit.cpp \
    randomstring.cpp \
    searchbar.cpp \
    titlebar.cpp

HEADERS += \
    mainwindow.h \
    singleapplication.h \
    textedit.h \
    randomstring.h \
    searchbar.h \
    titlebar.h

RESOURCES += \
    resources.qrc
