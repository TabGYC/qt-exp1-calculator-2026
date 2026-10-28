QT += widgets testlib
CONFIG += c++17 testcase
TARGET = test_calculator
INCLUDEPATH += ..
SOURCES += test_calculator.cpp ../calculatorengine.cpp ../calculatorwindow.cpp
HEADERS += ../calculatorengine.h ../calculatorwindow.h
FORMS += ../calculatorwindow.ui
