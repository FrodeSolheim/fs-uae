Name: fs-uae
Version: 2.5.33dev
Release: 1%{?dist}
Summary: Amiga emulator with on-screen GUI and online play support
License: GPL-2.0+
Group: System/Emulators/Other
Url: http://fs-uae.net/
Source0: http://fs-uae.net/devel/%{version}/fs-uae-%{version}.tar.gz
%if 0%{?suse_version}
BuildRequires: fdupes
%endif
BuildRequires: gcc-c++
BuildRequires: hicolor-icon-theme
BuildRequires: libpng-devel
BuildRequires: libXi-devel
BuildRequires: pkgconfig(gl)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(glu)
BuildRequires: pkgconfig(gthread-2.0)
BuildRequires: pkgconfig(openal)
BuildRequires: pkgconfig(sdl2)
BuildRequires: pkgconfig(x11)
BuildRequires: pkgconfig(zlib)
BuildRequires: pkgconfig(freetype2)
BuildRequires: zip

%description
FS-UAE is an Amiga emulator for Windows, Linux and Mac OS X
based on UAE/WinUAE, with a focus on emulating games.

Features include emulation of Amiga 500, 1200, 4000, CD32
and CDTV, perfectly smooth scrolling on 50Hz displays, support
for floppy images in ADF and IPF formats, CD-ROM images in ISO
or BIN/CUE format, mounting folders on your computer as Amiga
hard drives, support for Picasso 96 drivers for high-color and
high-resolution Workbench displays, and more.

A unique feature is support for cross-platform online play. You
can now play Amiga games against (or with) friends over the
Internet.

The emulator uses the latest Amiga emulation code from the
WinUAE project and requires a moderately fast computer with
accelerated graphics (OpenGL) to work. A game pad or joystick is
recommended, but not required (FS-UAE can emulate a joystick
using the cursor keys and right ctrl/alt keys).

%prep
%setup -q

%build
%configure
# CFLAGS='%{optflags}'
# CXXFLAGS='%{optflags}'
# %if 0%{?mandriva_version}
# CFLAGS="${CFLAGS} -Wno-error=format-security"
# CXXFLAGS="${CXXFLAGS} -Wno-error=format-security"
# %endif
# export CFLAGS; export CXXFLAGS
make %{?_smp_mflags}

%install
%make_install docdir='%{_defaultdocdir}/fs-uae/'
%find_lang %{name}
%if 0%{?suse_version}
%fdupes %{buildroot}%{_datadir}/fs-uae/
%endif

%if 0%{?suse_version}
%post
%icon_theme_cache_post

%postun
%icon_theme_cache_postun
%endif

%files -f %{name}.lang
%defattr(0644,root,root,0755)
%attr(0755,root,root) %{_bindir}/fs-uae
%attr(0755,root,root) %{_bindir}/fs-uae-device-helper
%{_datadir}/fs-uae/
%{_defaultdocdir}/fs-uae/
%{_datadir}/applications/fs-uae.desktop
%{_datadir}/icons/*/*/*/*
%{_datadir}/mime/packages/fs-uae.xml

%changelog
