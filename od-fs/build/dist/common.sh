# shellcheck shell=sh
# Settings shared by the dist scripts in this directory (sourced, POSIX sh). Expects PROJECT_DIR to
# be set, and sets DIST_BUILD_DIR (creating the directory) and the functions
# check_configured_triplet, build_environment, macos_signing_identity, macos_signing_check and
# macos_codesign.
#
# Environment variables:
#
#   FSUAE_DIST_BUILD   Base directory for the dist scripts' output (default: .build). A relative
#                      path is relative to the project directory. The scripts write the build
#                      directory to <base>/build/<triplet>, the dist to <base>/dist/<triplet> and
#                      the archive to <base>/release/<version>. Use another base directory (for
#                      example a path outside .build) to make dists without touching the default
#                      .build/build, .build/dist and .build/release.
#
#   FSUAE_DIST_ORIGIN  Who makes the dist, as the program shows it (FSUAE_BUILD_ORIGIN in
#                      configure.ac): "Unofficial build" by default; the dist workflow sets the
#                      official origin in FrodeSolheim/fs-uae (.github/workflows/dist.yml).
#
#   FSUAE_DIST_ENVIRONMENT  What the dist is made in, as the program's build header shows it
#                           (FSUAE_BUILD_ENVIRONMENT in configure.ac); by default, from
#                           build_environment.
#
#   FSUAE_DOCKER_IMAGE  The Docker image the dist is made in, set by the Docker wrappers
#                       (od-fs/build/linux/docker-run, od-fs/build/windows/crossbuild/docker-run),
#                       for build_environment.
#
#   FSUAE_MACOS_SIGNING  Whether macOS dists are signed and notarized: auto (default: when the
#                        credentials are there), required or off. See macos_signing_check.
#
#   FSUAE_MACOS_KEYCHAIN  The keychain with the signing identity (default: the keychain search
#                         list). Set by macos/signing-keychain, which makes one in CI.

if [ -z "$PROJECT_DIR" ]; then
    echo "Error: PROJECT_DIR must be set before sourcing common.sh" >&2
    exit 1
fi

FSUAE_DIST_BUILD="${FSUAE_DIST_BUILD:-.build}"
case "$FSUAE_DIST_BUILD" in
    /*) DIST_BUILD_DIR="$FSUAE_DIST_BUILD" ;;
    *) DIST_BUILD_DIR="$PROJECT_DIR/$FSUAE_DIST_BUILD" ;;
esac
# Make it absolute without "..", so that the Docker scripts can tell whether it is in the project.
mkdir -p "$DIST_BUILD_DIR"
DIST_BUILD_DIR="$(cd "$DIST_BUILD_DIR" && pwd)"
# Export it absolute, so scripts run from here (in other directories) get the same base directory.
FSUAE_DIST_BUILD="$DIST_BUILD_DIR"
export FSUAE_DIST_BUILD

# Exits with an error unless the build directory $1 is configured for $HOST_TRIPLET (HOST_TRIPLET in
# its Makefile, which configure.ac normalises to the dist names, such as x86_64-linux-gnu). The dist
# is named for $HOST_TRIPLET, and FS-UAE finds its Python home in Libs/<the configured triplet>.
check_configured_triplet() {
    _configured_triplet="$(sed -n 's/^HOST_TRIPLET *= *//p' "$1/Makefile" 2> /dev/null)"
    if [ "$_configured_triplet" != "$HOST_TRIPLET" ]; then
        echo "Error: the build in $1 is configured for the triplet" \
            "${_configured_triplet:-(none)}, not $HOST_TRIPLET" \
            "(see HOST_TRIPLET in configure.ac)" >&2
        exit 1
    fi
}

# Prints what the dist is made in, for the build header (FSUAE_BUILD_ENVIRONMENT in configure.ac):
# the operating system (PRETTY_NAME in /etc/os-release, macOS's product name and version, or
# uname), then the Docker image (FSUAE_DOCKER_IMAGE) and the GitHub Actions runner image (ImageOS
# and ImageVersion, which GitHub's runners set) when there are any, separated by "; ". Characters
# that configure does not let through are replaced with "_".
build_environment() {
    if [ -f /etc/os-release ]; then
        _environment="$(. /etc/os-release && printf "%s\n" "${PRETTY_NAME:-$NAME}")"
    elif command -v sw_vers > /dev/null 2>&1; then
        _environment="$(sw_vers -productName) $(sw_vers -productVersion)"
    else
        _environment="$(uname -sr)"
    fi
    if [ -n "${FSUAE_DOCKER_IMAGE:-}" ]; then
        _environment="$_environment; Docker image $FSUAE_DOCKER_IMAGE"
    fi
    if [ "${GITHUB_ACTIONS:-}" = true ]; then
        _environment="$_environment; GitHub Actions runner image"
        _environment="$_environment ${ImageOS:-unknown}${ImageVersion:+ $ImageVersion}"
    fi
    printf '%s\n' "$_environment" | sed 's|[^A-Za-z0-9 .,:;()/+_#@=~-]|_|g' | cut -c 1-200
}

# The Developer ID certificate's identity that macOS dists are signed with.
macos_signing_identity() {
    echo "Developer ID Application: Frode Solheim ($NOTARYTOOL_TEAM_ID)"
}

# In a GitHub Actions job, prints the message as a warning annotation (a workflow command), so that
# it shows on the run's summary page too, not only in the log. Does nothing elsewhere. (The message
# must not have "%" or a newline in it, which would have to be escaped.)
github_warning() {
    if [ "${GITHUB_ACTIONS:-}" = true ]; then
        echo "::warning::$*"
    fi
}

# Decides whether a macOS dist is signed and notarized, and sets MACOS_SIGNING to 1 or 0. It is
# when the notarytool credentials (NOTARYTOOL_APPLE_ID, NOTARYTOOL_PASSWORD, NOTARYTOOL_TEAM_ID)
# are set and the keychain has the signing identity (macos_signing_identity). Otherwise the dist is
# made unsigned (the app signed ad hoc, the .dmg not signed or notarized), with a warning saying
# what is missing. The identity is looked for in FSUAE_MACOS_KEYCHAIN when it is set, otherwise in
# the keychain search list. FSUAE_MACOS_SIGNING changes this: "auto" (the default) as above,
# "required" fails instead of making an unsigned dist (for release builds), "off" never signs.
macos_signing_check() {
    MACOS_SIGNING=0
    case "${FSUAE_MACOS_SIGNING:-auto}" in
        auto | required) ;;
        off)
            echo "Warning: the macOS dist is not signed or notarized (FSUAE_MACOS_SIGNING=off)" >&2
            return
            ;;
        *)
            echo "Error: FSUAE_MACOS_SIGNING must be auto, required or off," \
                "not $FSUAE_MACOS_SIGNING" >&2
            exit 1
            ;;
    esac
    _missing=""
    [ -n "$NOTARYTOOL_APPLE_ID" ] || _missing="$_missing NOTARYTOOL_APPLE_ID"
    [ -n "$NOTARYTOOL_PASSWORD" ] || _missing="$_missing NOTARYTOOL_PASSWORD"
    [ -n "$NOTARYTOOL_TEAM_ID" ] || _missing="$_missing NOTARYTOOL_TEAM_ID"
    if [ -z "$_missing" ] && ! security find-identity -v -p codesigning \
        ${FSUAE_MACOS_KEYCHAIN:+"$FSUAE_MACOS_KEYCHAIN"} 2> /dev/null |
        grep -qF "\"$(macos_signing_identity)\""; then
        _missing=" the signing identity \"$(macos_signing_identity)\" in the keychain"
        _missing="$_missing${FSUAE_MACOS_KEYCHAIN:+ $FSUAE_MACOS_KEYCHAIN}"
    fi
    if [ -z "$_missing" ]; then
        # shellcheck disable=SC2034 # Read by the scripts that source this file.
        MACOS_SIGNING=1
        return
    fi
    if [ "$FSUAE_MACOS_SIGNING" = "required" ]; then
        echo "Error: cannot sign the macOS dist (FSUAE_MACOS_SIGNING=required);" \
            "missing:$_missing" >&2
        exit 1
    fi
    echo "Warning: the macOS dist is not signed or notarized; missing:$_missing" >&2
    github_warning "The macOS dist is not signed or notarized; missing:$_missing"
    echo "Warning: the app is only signed ad hoc and the .dmg is not signed, so macOS does not" \
        "open it from a download without an override (fine for testing, not for a release)." >&2
}

# Runs codesign with the given arguments, and when signing with the Developer ID certificate
# (MACOS_SIGNING=1, see macos_signing_check) with the identity from FSUAE_MACOS_KEYCHAIN only, when
# it is set: codesign refuses an identity that is in more than one keychain in the search list, as
# on a Mac with the certificate in the login keychain that runs the dist workflow (with act).
macos_codesign() {
    if [ "$MACOS_SIGNING" = 1 ] && [ -n "$FSUAE_MACOS_KEYCHAIN" ]; then
        codesign --keychain "$FSUAE_MACOS_KEYCHAIN" "$@"
    else
        codesign "$@"
    fi
}
