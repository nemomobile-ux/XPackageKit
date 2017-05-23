QT = core dbus
CONFIG += c++11

TEMPLATE = app
TARGET = xpmanager

INCLUDEPATH += $$PWD/../..
LIBS += -L$$OUT_PWD/../../XPackageKit
LIBS += -lXPackageKit

SOURCES = xpmanager.cpp

INSTALLS += target
target.path = /usr/bin

linux-g++ { # mer
    PKGCONFIG += packagekit-qt5
    DEFINES += NEMO_PACKAGE_KIT
} else {
    PKGCONFIG += packagekitqt5
}
