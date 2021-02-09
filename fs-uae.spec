# Copyright © 2011–2019 Frode Solheim <frode@fs-uae.net>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

%define name fs-uae
%define version 3.9.100dev
%define unmangled_version 3.9.100dev
%define release 1%{?dist}

%undefine _hardened_build

Summary: Amiga emulator with on-screen GUI and online play support
Name: %{name}
Version: %{version}
Release: %{release}
URL: https://fs-uae.net/
Source0: %{name}-%{unmangled_version}.tar.gz
License: GPL-2.0+
Group: System/Emulators/Other
%if 0%{?suse_version}
%else
# For joystick support in Fedora
Requires: kernel-modules-extra
%endif
%if 0%{?suse_version}
BuildRequires: fdupes
%endif
BuildRequires: gcc-c++
BuildRequires: hicolor-icon-theme
BuildRequires: libpng-devel
BuildRequires: libXi-devel
BuildRequires: libXtst-devel
BuildRequires: pkgconfig(gl)
BuildRequires: pkgconfig(glew)
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
%setup -n %{name}-%{unmangled_version}

%build
%configure --with-libmpeg2=builtin
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
