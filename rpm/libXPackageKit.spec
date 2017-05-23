Name:       libXPackage
Summary:    PackageKitQt wrapper library
Version:    0.1.0
Release:    1
Group:      System/Libraries
License:    Proprietary
URL:        https://bitbucket.org/jolla/ui-sailfish-components-updater
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(packagekit-qt5)

%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}

%description
PackageKitQt wrapper library

%package devel
Summary:    Development package for XPackageKit
Group:      System/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
Development package for XPackageKit

%package tools
Summary:    XPackageKit cli tool
Group:      Utilities/System
Requires:   %{name} >= %{version}

%description tools
XPackageKit cli tool

%prep
%setup -q -n %{name}-%{version}

%build
%qtc_qmake5
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%qmake5_install

%files
%defattr(-,root,root,-)
%{_libdir}/libXPackageKit.so*

%files devel
%defattr(-,root,root,-)
%{_libdir}/pkgconfig/XPackageKit.pc
#%{_includedir}/libsailfishupdater/*

%files tools
%defattr(-,root,root,-)
%{_bindir}/xpmanager

%post
/sbin/ldconfig

%postun
/sbin/ldconfig
