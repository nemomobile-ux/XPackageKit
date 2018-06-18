Name:       libXPackageKit
Summary:    PackageKitQt wrapper library
Version:    1.0.0
Release:    1
Group:      System/Libraries
License:    WTFPL
URL:        https://git.merproject.org/mer-core/libXPackageKit
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(packagekit-qt5)
BuildRequires:  qt5-tools
BuildRequires:  mer-qdoc-template

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

%package doc
Summary:    XPackageKit library documentation
Group:      System/Libraries

%description doc
%{summary}.

%package verify
Summary:    XPackageKit verification scripts
Group:      System/Libraries

%description verify
%{summary}.

%prep
%setup -q -n %{name}-%{version}

%build
export QT_INSTALL_DOCS=%{_docdir}/qt5
%qtc_qmake5
make %{?_smp_mflags}

%install
export QT_INSTALL_DOCS=%{_docdir}/qt5
rm -rf %{buildroot}
%qmake5_install

%files
%defattr(-,root,root,-)
%{_libdir}/libXPackageKit.so*

%files devel
%defattr(-,root,root,-)
%{_libdir}/pkgconfig/XPackageKit.pc
%{_includedir}/XPackageKit/*

%files tools
%defattr(-,root,root,-)
%{_bindir}/xpmanager
%{_bindir}/xptest

%files doc
%defattr(-,root,root,-)
%dir %{_datadir}/doc/xpackagekit
%{_datadir}/doc/xpackagekit/XPackageKit.qch

%files verify
%defattr(-,root,root,-)
%attr(0755, root, root) %{_bindir}/xpackagekit-verify-cache

%post
/sbin/ldconfig

%postun
/sbin/ldconfig
