QT = core dbus
CONFIG += c++11

TEMPLATE = lib
TARGET = XPackageKit
VERSION = 0.1.0

CONFIG += link_pkgconfig
SOURCES += \
    XSsuTransaction.cpp \
    XPackageKitTransaction.cpp \
    XChainTransaction.cpp \
    XTransaction.cpp
HEADERS += \
    XTransaction.hpp \
    XSsuTransaction.hpp \
    XPackageKitTransaction.hpp \
    XChainTransaction.hpp

SOURCES += XTransactionManager.cpp PackageKitBackend.cpp
HEADERS += XTransactionManager.hpp PackageKitBackend.hpp

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII

QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Werror=return-type

linux-g++ { # mer
    PKGCONFIG += packagekit-qt5
    DEFINES += NEMO_PACKAGE_KIT
} else { # Upstream PackageKit Qt5
    PKGCONFIG += packagekitqt5
}

target.path = /usr/lib

PUBLIC_HEADERS = \
    XTransaction \
    XTransaction.hpp \
    XTransactionManager \
    XTransactionManager.hpp \
    XChainTransaction \
    XChainTransaction.hpp

public_headers.files = $$PUBLIC_HEADERS
public_headers.path = /usr/include/$${TARGET}

CONFIG += create_pc create_prl no_install_prl
QMAKE_PKGCONFIG_DESCRIPTION = PackageKitQt wrapper library
QMAKE_PKGCONFIG_LIBDIR = $$target.path
#QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig
QMAKE_SUBSTITUTES += $${pkgconfig.files}
pkgconfig.CONFIG = no_check_exist
pkgconfig.path  = $${INSTALL_LIBDIR}/pkgconfig
QMAKE_EXTRA_TARGETS += pkgconfig
QMAKE_CLEAN += $${pkgconfig.files}

INSTALLS += target public_headers
