# Copyright © 2011–2025 Frode Solheim <frode@fs-uae.net>
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

%define name fs-uae-5
# The package version, set by obs.sh (deb-src.sh makes it from the version of the source tarball).
%define version 5.0.7
%define release 1%{?dist}
%undefine _hardened_build

Summary: Amiga emulator with on-screen GUI and online play support
Name: %{name}
Version: %{version}
Release: %{release}
URL: https://fs-uae.net/
Source0: %{name}_%{version}.orig.tar.xz
# The origin and build environment for configure (see obs-origin.sh), copied here by obs.sh.
Source1: obs-origin.sh
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
BuildRequires: libmpeg2-devel
BuildRequires: libpng-devel
BuildRequires: mpg123-devel
BuildRequires: pkgconfig(flac)
BuildRequires: pkgconfig(gl)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(glu)
BuildRequires: pkgconfig(gthread-2.0)
BuildRequires: pkgconfig(libzstd)
BuildRequires: pkgconfig(sdl3-image)
BuildRequires: pkgconfig(slirp)
# BuildRequires: pkgconfig(sdl3-ttf)
BuildRequires: SDL3_ttf-devel
BuildRequires: pkgconfig(sdl3)
BuildRequires: pkgconfig(zlib)
BuildRequires: portmidi-devel
BuildRequires: python3-devel
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
%setup -n %{name}-%{version}

%build
# The origin is official only in the maintainer's OBS project, which OBS names in the DISTURL.
%configure --with-share-dir-name=fs-uae-5 --with-libslirp=system \
    FSUAE_BUILD_ORIGIN="$(sh %{SOURCE1} '%{?disturl}')" \
    FSUAE_BUILD_ENVIRONMENT="$(sh %{SOURCE1} --build-environment '%{?disturl}')"
make %{?_smp_mflags}

%install
%make_install docdir='%{_defaultdocdir}/fs-uae-5/'
mv %{buildroot}%{_bindir}/fs-uae %{buildroot}%{_bindir}/fs-uae-5

# Temporary disabled (no translations right now)
# %find_lang %{name}
%if 0%{?suse_version}
%fdupes %{buildroot}%{_datadir}/fs-uae-5/
%endif

%if 0%{?suse_version}
%post
%icon_theme_cache_post

%postun
%icon_theme_cache_postun
%endif

# Don't remember what -f %{name}.lang did...
# %files -f %{name}.lang

%files
%defattr(0644,root,root,0755)
%attr(0755,root,root) %{_bindir}/fs-uae-5
%{_datadir}/fs-uae-5/
# %{_defaultdocdir}/fs-uae-5/

# Temporarily disabled, no desktop files right now
#%{_datadir}/applications/fs-uae.desktop
#%{_datadir}/icons/*/*/*/*
#%{_datadir}/mime/packages/fs-uae.xml

%changelog
