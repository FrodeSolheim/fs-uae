#!/bin/sh
#
# Prints the origin of an OBS package build (FSUAE_BUILD_ORIGIN, see configure.ac), or with
# --build-environment its build environment (FSUAE_BUILD_ENVIRONMENT), for the RPM spec and
# debian/rules, which pass them to configure. Copied next to them by obs.sh and deb-src.sh.
#
# Usage: obs-origin.sh [--build-environment] [<disturl>]
#
# The RPM spec passes the DISTURL (the %disturl macro). Without one, the script reads it from the
# file obs-build writes at the top of the build root, /.buildenv (BUILD_DISTURL='...'), which Debian
# builds have too (FSUAE_OBS_BUILDENV overrides the path, for the tests).
#
# The build is official only when it is made in the maintainer's OBS project on build.opensuse.org
# (or a subproject of it, which only the maintainer can make): "Official build (openSUSE Build
# Service, <distribution>)". OBS names the project in the DISTURL
# (obs://<OBS instance>/<project>/<repository>/<source md5>-<package>), which the OBS worker passes
# to every build. Builds in other projects (a branch of the package, or a distribution reusing these
# files) say "Unofficial build (...)" with where they were made. Without a DISTURL (not built on
# OBS), it prints nothing, and configure's default ("Unofficial build") applies. Like every origin,
# this is self-declared: someone can make a package that claims anything.
#
# The build environment is the build root's distribution (PRETTY_NAME in /etc/os-release, or
# FSUAE_OBS_OS_RELEASE for the tests), and the DISTURL, which names the exact OBS build.

OFFICIAL_INSTANCE="build.opensuse.org"
OFFICIAL_PROJECT="home:FrodeSolheim"

PRINT_BUILD_ENVIRONMENT=0
if [ "$1" = --build-environment ]; then
    PRINT_BUILD_ENVIRONMENT=1
    shift
fi

DISTURL="$1"
BUILDENV="${FSUAE_OBS_BUILDENV:-/.buildenv}"
if [ -z "$DISTURL" ] && [ -f "$BUILDENV" ]; then
    # (Read, not sourced: only this line is needed.)
    DISTURL="$(sed -n "s/^BUILD_DISTURL='\(.*\)'\$/\1/p" "$BUILDENV" | head -n 1)"
fi
case "$DISTURL" in
    obs://*/*/*/*) ;;
    *) DISTURL="" ;;
esac

# Prints its arguments with only the characters configure lets through (FSUAE_CHECK_IDENTITY in
# configure.ac), and at most 200 of them.
print_identity() {
    printf '%s\n' "$*" | sed 's|[^A-Za-z0-9 .,:;()/+_#@=~-]|_|g' | cut -c 1-200
}

if [ "$PRINT_BUILD_ENVIRONMENT" = 1 ]; then
    os_release="${FSUAE_OBS_OS_RELEASE:-/etc/os-release}"
    distribution="$(sed -n 's/^PRETTY_NAME="\{0,1\}\([^"]*\)"\{0,1\}$/\1/p' "$os_release" \
        2> /dev/null | head -n 1)"
    print_identity "${distribution:-unknown}${DISTURL:+; $DISTURL}"
    exit 0
fi

if [ -z "$DISTURL" ]; then
    exit 0
fi
rest="${DISTURL#obs://}"
instance="${rest%%/*}"
rest="${rest#*/}"
project="${rest%%/*}"
rest="${rest#*/}"
repository="${rest%%/*}"
# The OBS repository is named for the distribution, such as Debian_13, xUbuntu_24.04, Fedora_42 or
# openSUSE_Tumbleweed.
distribution="$(printf '%s\n' "$repository" | sed 's/^xUbuntu_/Ubuntu_/' | tr _ ' ')"

case "$instance/$project" in
    "$OFFICIAL_INSTANCE/$OFFICIAL_PROJECT" | "$OFFICIAL_INSTANCE/$OFFICIAL_PROJECT":*)
        print_identity "Official build (openSUSE Build Service, $distribution)"
        ;;
    "$OFFICIAL_INSTANCE"/*)
        print_identity "Unofficial build (openSUSE Build Service, project $project, $distribution)"
        ;;
    *)
        print_identity "Unofficial build (OBS at $instance, project $project, $distribution)"
        ;;
esac
