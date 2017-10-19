QT = core dbus
CONFIG += c++11

TEMPLATE = lib
TARGET = XPackageKit
VERSION = 0.1.0

CONFIG += link_pkgconfig
CONFIG += mer-qdoc-template
MER_QDOC.project = XPackageKit
MER_QDOC.config = doc/XPackageKit.qdocconf
MER_QDOC.style = offline
MER_QDOC.path = /usr/share/doc/xpackagekit

OTHER_FILES += \
    doc/src/XPackageKit-index.qdoc \
    XTransactionNamespace.qdoc

SOURCES += \
    XSsuTransaction.cpp \
    XChainTransaction.cpp \
    XTransaction.cpp \
    XRefreshTransaction.cpp \
    XInstallTransaction.cpp \
    XRemoveTransaction.cpp \
    XGetUpdatesTransaction.cpp \
    XResolveTransaction.cpp \
    XPackageKitTransaction.cpp \
    XGetDependsTransaction.cpp \
    XUpdateTransaction.cpp \
    XDownloadTransaction.cpp \
    XProcessTransaction.cpp \
    XTransactionManager.cpp \
    PackageKitBackend.cpp

PUBLIC_HEADERS = \
    XTransaction \
    XTransaction.hpp \
    XTransactionManager \
    XTransactionManager.hpp \
    XChainTransaction \
    XChainTransaction.hpp \
    XTransactionNamespace.hpp

HEADERS += \
    $${PUBLIC_HEADERS} \
    XSsuTransaction.hpp \
    XRefreshTransaction.hpp \
    XInstallTransaction.hpp \
    XRemoveTransaction.hpp \
    XGetUpdatesTransaction.hpp \
    XResolveTransaction.hpp \
    XPackageKitTransaction.hpp \
    XGetDependsTransaction.hpp \
    XUpdateTransaction.hpp \
    XDownloadTransaction.hpp \
    XProcessTransaction.hpp \
    PackageKitBackend.hpp

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII

QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Werror=return-type

linux-g++ { # mer
    PKGCONFIG += packagekit-qt5
    DEFINES += NEMO_PACKAGE_KIT
    INCLUDEPATH += /usr/include/PackageKit/packagekit-qt5
} else { # Upstream PackageKit Qt5
    PKGCONFIG += packagekitqt5
}

target.path = /usr/lib

public_headers.files = $$PUBLIC_HEADERS
public_headers.path = /usr/include/$${TARGET}

CONFIG += create_pc create_prl no_install_prl
QMAKE_PKGCONFIG_DESCRIPTION = PackageKitQt wrapper library
QMAKE_PKGCONFIG_LIBDIR = $${target.path}
QMAKE_PKGCONFIG_INCDIR = $${public_headers.path}
QMAKE_PKGCONFIG_DESTDIR = pkgconfig
QMAKE_SUBSTITUTES += $${pkgconfig.files}
pkgconfig.CONFIG = no_check_exist
pkgconfig.path  = $${INSTALL_LIBDIR}/pkgconfig
QMAKE_EXTRA_TARGETS += pkgconfig
QMAKE_CLEAN += $${pkgconfig.files}

INSTALLS += target public_headers
