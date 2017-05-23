TEMPLATE = subdirs
SUBDIRS = XPackageKit tools
tools.depends = XPackageKit

OTHER_FILES += rpm/libXPackageKit.spec
