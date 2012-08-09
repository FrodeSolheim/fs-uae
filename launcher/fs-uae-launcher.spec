%define is_mandrake %(test -e /etc/mandrake-release && echo 1 || echo 0)
%define is_suse %(test -e /etc/SuSE-release && echo 1 || echo 0)
%define is_fedora %(test -e /etc/fedora-release && echo 1 || echo 0)

Name:           fs-uae-launcher
Version:        1.3.23u6
Release:        1%{?dist}
Summary:        Graphical configuration frontend and launcher for FS-UAE

Requires:       pygame wxPython fs-uae

Group:          Applications/Emulators
License:        GPLv2+
URL:            http://fengestad.no/fs-uae/
Source0:        http://fengestad.no/fs-uae/files/%{version}/%{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildArch:      noarch
BuildRequires:  python-devel python-setuptools

%if 0%{?suse_version}

%else
%if 0%{?mandriva_version}

%else

%endif
%endif

%description
FS-UAE Launcher is a graphical configuration program and
launcher for FS-UAE.


%prep
%setup -q


%build
CFLAGS="$RPM_OPT_FLAGS" %{__python} setup.py build
make -f Makefile.mk


%install
%{__python} setup.py install --skip-build --root $RPM_BUILD_ROOT --prefix=/usr
make -f Makefile.mk install DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%_bindir/*
%{python_sitelib}/*
%_datadir/%{name}/
%_datadir/doc/%{name}/
%_datadir/applications/*.desktop
%_datadir/icons/*/*/*/*.png
%_datadir/locale/*/*/*.mo

%dir %_datadir/icons/hicolor
%dir %_datadir/icons/hicolor/128x128
%dir %_datadir/icons/hicolor/128x128/apps
%dir %_datadir/icons/hicolor/16x16
%dir %_datadir/icons/hicolor/16x16/apps
%dir %_datadir/icons/hicolor/256x256
%dir %_datadir/icons/hicolor/256x256/apps
%dir %_datadir/icons/hicolor/32x32
%dir %_datadir/icons/hicolor/32x32/apps
%dir %_datadir/icons/hicolor/48x48
%dir %_datadir/icons/hicolor/48x48/apps
%dir %_datadir/icons/hicolor/64x64
%dir %_datadir/icons/hicolor/64x64/apps


%doc


%changelog
