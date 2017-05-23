QT = core dbus
CONFIG += c++11

TEMPLATE = lib
TARGET = XPackageKit
VERSION = 0.1.0

CONFIG += link_pkgconfig
SOURCES += XTransaction.cpp XSsuTransaction.cpp XPackageKitTransaction.cpp XChainTransaction.cpp
HEADERS += XTransaction.hpp XSsuTransaction.hpp XPackageKitTransaction.hpp XChainTransaction.hpp

SOURCES += XTransactionManager.cpp PackageKitBackend.cpp
HEADERS += XTransactionManager.hpp PackageKitBackend.hpp

linux-g++ { # mer
    PKGCONFIG += packagekit-qt5
    DEFINES += NEMO_PACKAGE_KIT
} else { # Upstream PackageKit Qt5
    PKGCONFIG += packagekitqt5
}

target.path = /usr/lib

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

INSTALLS += target
