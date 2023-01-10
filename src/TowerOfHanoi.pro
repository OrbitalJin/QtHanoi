QT      += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG  += c++11

SOURCES += \
    main.cpp \
    hanoi.cpp \
    disk.cpp \
    pole.cpp \
    move.cpp \


HEADERS += \
    hanoi.h \
    disk.h \
    pole.h \
    move.h \

FORMS += \
    hanoi.ui

RESOURCES += \
    res.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS   += assets/icon.ico

