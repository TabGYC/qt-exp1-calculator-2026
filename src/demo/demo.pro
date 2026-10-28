QT += widgets testlib
CONFIG += c++17
TARGET = demo
DEFINES += LAB1
INCLUDEPATH += ..
SOURCES += demo.cpp ../calculatorwindow.cpp ../calculatorengine.cpp
HEADERS += ../calculatorwindow.h ../calculatorengine.h
FORMS += ../calculatorwindow.ui
