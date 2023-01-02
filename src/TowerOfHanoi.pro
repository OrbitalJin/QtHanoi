#-------------------------------------------------
#
# Project created by QtCreator 2022-20-12T07:52:32
#
#-------------------------------------------------

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += "-fno-sized-deallocation"
QMAKE_CXXFLAGS += "-std=c++11"

QT      += core gui multimedia widgets
TARGET   = TowerOfHanoi
TEMPLATE = app

SOURCES += main.cpp  \
           hanoi.cpp \
           pole.cpp  \
           disk.cpp  \
           move.cpp

HEADERS  += hanoi.h  \
            pole.h   \
            disk.h   \
            move.h

FORMS    += hanoi.ui

RESOURCES+= res.qrc
