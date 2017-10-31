TEMPLATE = subdirs
SUBDIRS = XPackageKit tools
tools.depends = XPackageKit

scripts.files = \
    scripts/xpackagekit-verify-cache
scripts.path = /usr/bin

INSTALLS += scripts

OTHER_FILES += rpm/libXPackageKit.spec
