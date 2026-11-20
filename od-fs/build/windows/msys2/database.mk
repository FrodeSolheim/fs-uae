override VERSION_OF_mingw-w64-x86_64-7zip := 26.06-1
.PHONY: PKG@@mingw-w64-x86_64-7zip
PKG@@mingw-w64-x86_64-7zip: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-7zip'

override VERSION_OF_mingw-w64-x86_64-SDL := 1.2.15+r419+gef3a6c05-2
.PHONY: PKG@@mingw-w64-x86_64-SDL
PKG@@mingw-w64-x86_64-SDL: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-SDL'

override VERSION_OF_mingw-w64-x86_64-SDL2 := 2.32.10-1
.PHONY: PKG@@mingw-w64-x86_64-SDL2
PKG@@mingw-w64-x86_64-SDL2: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-vulkan) ; @echo 'PKG@@mingw-w64-x86_64-SDL2'

override VERSION_OF_mingw-w64-x86_64-abseil-cpp := 20260817.0-1
.PHONY: PKG@@mingw-w64-x86_64-abseil-cpp
PKG@@mingw-w64-x86_64-abseil-cpp: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-abseil-cpp'

override VERSION_OF_mingw-w64-x86_64-adios2 := 2.12.1-4
.PHONY: PKG@@mingw-w64-x86_64-adios2
PKG@@mingw-w64-x86_64-adios2: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-sdk-cpp PKG@@mingw-w64-x86_64-blosc2 PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-hdf5 PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libsodium PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-pugixml PKG@@mingw-w64-x86_64-sqlite3 PKG@@mingw-w64-x86_64-yaml-cpp PKG@@mingw-w64-x86_64-zeromq PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-adios2'

override VERSION_OF_mingw-w64-x86_64-adobe-source-code-pro-fonts := 2.042u+1.062i+1.026vf-1
.PHONY: PKG@@mingw-w64-x86_64-adobe-source-code-pro-fonts
PKG@@mingw-w64-x86_64-adobe-source-code-pro-fonts: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-adobe-source-code-pro-fonts'

override VERSION_OF_mingw-w64-x86_64-adwaita-icon-theme := 51.0-1
.PHONY: PKG@@mingw-w64-x86_64-adwaita-icon-theme
PKG@@mingw-w64-x86_64-adwaita-icon-theme: $(if $(__deps),PKG@@mingw-w64-x86_64-adwaita-icon-theme-legacy PKG@@mingw-w64-x86_64-gtk-update-icon-cache PKG@@mingw-w64-x86_64-hicolor-icon-theme PKG@@mingw-w64-x86_64-librsvg) ; @echo 'PKG@@mingw-w64-x86_64-adwaita-icon-theme'

override VERSION_OF_mingw-w64-x86_64-adwaita-icon-theme-legacy := 46.2-1
.PHONY: PKG@@mingw-w64-x86_64-adwaita-icon-theme-legacy
PKG@@mingw-w64-x86_64-adwaita-icon-theme-legacy: $(if $(__deps),PKG@@mingw-w64-x86_64-gtk-update-icon-cache PKG@@mingw-w64-x86_64-hicolor-icon-theme) ; @echo 'PKG@@mingw-w64-x86_64-adwaita-icon-theme-legacy'

override VERSION_OF_mingw-w64-x86_64-alembic := 1.8.12-1
.PHONY: PKG@@mingw-w64-x86_64-alembic
PKG@@mingw-w64-x86_64-alembic: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-hdf5 PKG@@mingw-w64-x86_64-imath) ; @echo 'PKG@@mingw-w64-x86_64-alembic'

override VERSION_OF_mingw-w64-x86_64-amf-headers := 1.5.3-1
.PHONY: PKG@@mingw-w64-x86_64-amf-headers
PKG@@mingw-w64-x86_64-amf-headers: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-amf-headers'

override VERSION_OF_mingw-w64-x86_64-angleproject := 2.1.r25748.890b5d8f-6
.PHONY: PKG@@mingw-w64-x86_64-angleproject
PKG@@mingw-w64-x86_64-angleproject: $(if $(__deps),PKG@@mingw-w64-x86_64-egl-headers PKG@@mingw-w64-x86_64-gles-headers PKG@@mingw-w64-x86_64-jsoncpp PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-rapidjson PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-angleproject'

override VERSION_OF_mingw-w64-x86_64-aom := 3.15.3-1
.PHONY: PKG@@mingw-w64-x86_64-aom
PKG@@mingw-w64-x86_64-aom: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-aom'

override VERSION_OF_mingw-w64-x86_64-appstream-glib := 0.8.3-2
.PHONY: PKG@@mingw-w64-x86_64-appstream-glib
PKG@@mingw-w64-x86_64-appstream-glib: $(if $(__deps),PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-gdk-pixbuf2 PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gtk3 PKG@@mingw-w64-x86_64-json-glib PKG@@mingw-w64-x86_64-libarchive PKG@@mingw-w64-x86_64-libyaml) ; @echo 'PKG@@mingw-w64-x86_64-appstream-glib'

override VERSION_OF_mingw-w64-x86_64-argon2 := 20190702-2
.PHONY: PKG@@mingw-w64-x86_64-argon2
PKG@@mingw-w64-x86_64-argon2: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-argon2'

override VERSION_OF_mingw-w64-x86_64-armadillo := 15.6.2-1
.PHONY: PKG@@mingw-w64-x86_64-armadillo
PKG@@mingw-w64-x86_64-armadillo: $(if $(__deps),PKG@@mingw-w64-x86_64-arpack PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-openblas PKG@@mingw-w64-x86_64-superlu) ; @echo 'PKG@@mingw-w64-x86_64-armadillo'

override VERSION_OF_mingw-w64-x86_64-arpack := 3.9.1-9
.PHONY: PKG@@mingw-w64-x86_64-arpack PKG@@mingw-w64-x86_64-arpack-ng
PKG@@mingw-w64-x86_64-arpack PKG@@mingw-w64-x86_64-arpack-ng: $(if $(__deps),PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-openblas) ; @echo 'PKG@@mingw-w64-x86_64-arpack'

override VERSION_OF_mingw-w64-x86_64-arpack64 := 3.9.1-9
.PHONY: PKG@@mingw-w64-x86_64-arpack64
PKG@@mingw-w64-x86_64-arpack64: $(if $(__deps),PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-openblas64) ; @echo 'PKG@@mingw-w64-x86_64-arpack64'

override VERSION_OF_mingw-w64-x86_64-arrow := 26.0.1-1
.PHONY: PKG@@mingw-w64-x86_64-arrow
PKG@@mingw-w64-x86_64-arrow: $(if $(__deps),PKG@@mingw-w64-x86_64-abseil-cpp PKG@@mingw-w64-x86_64-aws-sdk-cpp PKG@@mingw-w64-x86_64-brotli PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-c-ares PKG@@mingw-w64-x86_64-double-conversion PKG@@mingw-w64-x86_64-flatbuffers PKG@@mingw-w64-x86_64-gflags PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-grpc PKG@@mingw-w64-x86_64-libutf8proc PKG@@mingw-w64-x86_64-llvm-libs PKG@@mingw-w64-x86_64-lz4 PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-protobuf PKG@@mingw-w64-x86_64-re2 PKG@@mingw-w64-x86_64-simdjson PKG@@mingw-w64-x86_64-snappy PKG@@mingw-w64-x86_64-thrift PKG@@mingw-w64-x86_64-uriparser PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-arrow'

override VERSION_OF_mingw-w64-x86_64-asciidoc := 10.2.1-5
.PHONY: PKG@@mingw-w64-x86_64-asciidoc
PKG@@mingw-w64-x86_64-asciidoc: $(if $(__deps),PKG@@mingw-w64-x86_64-docbook-xsl PKG@@mingw-w64-x86_64-libxslt PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-asciidoc'

override VERSION_OF_mingw-w64-x86_64-asciidoctor := 2.0.26-2
.PHONY: PKG@@mingw-w64-x86_64-asciidoctor
PKG@@mingw-w64-x86_64-asciidoctor: $(if $(__deps),PKG@@mingw-w64-x86_64-ruby) ; @echo 'PKG@@mingw-w64-x86_64-asciidoctor'

override VERSION_OF_mingw-w64-x86_64-asio := 1.38.2-1
.PHONY: PKG@@mingw-w64-x86_64-asio
PKG@@mingw-w64-x86_64-asio: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-asio'

override VERSION_OF_mingw-w64-x86_64-asiosdk := 2.3.4-1
.PHONY: PKG@@mingw-w64-x86_64-asiosdk
PKG@@mingw-w64-x86_64-asiosdk: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-asiosdk'

override VERSION_OF_mingw-w64-x86_64-assimp := 6.0.5-2
.PHONY: PKG@@mingw-w64-x86_64-assimp
PKG@@mingw-w64-x86_64-assimp: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-minizip PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-assimp'

override VERSION_OF_mingw-w64-x86_64-atk := 2.62.1-1
.PHONY: PKG@@mingw-w64-x86_64-atk
PKG@@mingw-w64-x86_64-atk: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2) ; @echo 'PKG@@mingw-w64-x86_64-atk'

override VERSION_OF_mingw-w64-x86_64-autotools := 2026.08.04-1
.PHONY: PKG@@mingw-w64-x86_64-autotools
PKG@@mingw-w64-x86_64-autotools: $(if $(__deps),PKG@@autoconf PKG@@automake PKG@@make PKG@@mingw-w64-x86_64-gettext-tools PKG@@mingw-w64-x86_64-libtool PKG@@mingw-w64-x86_64-pkgconf) ; @echo 'PKG@@mingw-w64-x86_64-autotools'

override VERSION_OF_mingw-w64-x86_64-aws-c-auth := 1.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-aws-c-auth
PKG@@mingw-w64-x86_64-aws-c-auth: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-c-common PKG@@mingw-w64-x86_64-aws-c-http PKG@@mingw-w64-x86_64-aws-c-sdkutils) ; @echo 'PKG@@mingw-w64-x86_64-aws-c-auth'

override VERSION_OF_mingw-w64-x86_64-aws-c-cal := 1.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-aws-c-cal
PKG@@mingw-w64-x86_64-aws-c-cal: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-c-common PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-aws-c-cal'

override VERSION_OF_mingw-w64-x86_64-aws-c-common := 1.0.2-1
.PHONY: PKG@@mingw-w64-x86_64-aws-c-common
PKG@@mingw-w64-x86_64-aws-c-common: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-aws-c-common'

override VERSION_OF_mingw-w64-x86_64-aws-c-compression := 1.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-aws-c-compression
PKG@@mingw-w64-x86_64-aws-c-compression: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-c-common) ; @echo 'PKG@@mingw-w64-x86_64-aws-c-compression'

override VERSION_OF_mingw-w64-x86_64-aws-c-event-stream := 1.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-aws-c-event-stream
PKG@@mingw-w64-x86_64-aws-c-event-stream: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-c-common PKG@@mingw-w64-x86_64-aws-c-io PKG@@mingw-w64-x86_64-aws-checksums) ; @echo 'PKG@@mingw-w64-x86_64-aws-c-event-stream'

override VERSION_OF_mingw-w64-x86_64-aws-c-http := 1.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-aws-c-http
PKG@@mingw-w64-x86_64-aws-c-http: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-c-common PKG@@mingw-w64-x86_64-aws-c-compression PKG@@mingw-w64-x86_64-aws-c-io) ; @echo 'PKG@@mingw-w64-x86_64-aws-c-http'

override VERSION_OF_mingw-w64-x86_64-aws-c-io := 1.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-aws-c-io
PKG@@mingw-w64-x86_64-aws-c-io: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-c-cal PKG@@mingw-w64-x86_64-aws-c-common PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-aws-c-io'

override VERSION_OF_mingw-w64-x86_64-aws-c-mqtt := 1.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-aws-c-mqtt
PKG@@mingw-w64-x86_64-aws-c-mqtt: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-c-common PKG@@mingw-w64-x86_64-aws-c-http PKG@@mingw-w64-x86_64-aws-c-io) ; @echo 'PKG@@mingw-w64-x86_64-aws-c-mqtt'

override VERSION_OF_mingw-w64-x86_64-aws-c-s3 := 1.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-aws-c-s3
PKG@@mingw-w64-x86_64-aws-c-s3: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-c-auth PKG@@mingw-w64-x86_64-aws-c-common PKG@@mingw-w64-x86_64-aws-checksums) ; @echo 'PKG@@mingw-w64-x86_64-aws-c-s3'

override VERSION_OF_mingw-w64-x86_64-aws-c-sdkutils := 1.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-aws-c-sdkutils
PKG@@mingw-w64-x86_64-aws-c-sdkutils: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-c-common) ; @echo 'PKG@@mingw-w64-x86_64-aws-c-sdkutils'

override VERSION_OF_mingw-w64-x86_64-aws-checksums := 1.0.1-1
.PHONY: PKG@@mingw-w64-x86_64-aws-checksums
PKG@@mingw-w64-x86_64-aws-checksums: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-c-common) ; @echo 'PKG@@mingw-w64-x86_64-aws-checksums'

override VERSION_OF_mingw-w64-x86_64-aws-crt-cpp := 0.43.9-1
.PHONY: PKG@@mingw-w64-x86_64-aws-crt-cpp
PKG@@mingw-w64-x86_64-aws-crt-cpp: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-c-auth PKG@@mingw-w64-x86_64-aws-c-cal PKG@@mingw-w64-x86_64-aws-c-common PKG@@mingw-w64-x86_64-aws-c-event-stream PKG@@mingw-w64-x86_64-aws-c-http PKG@@mingw-w64-x86_64-aws-c-io PKG@@mingw-w64-x86_64-aws-c-mqtt PKG@@mingw-w64-x86_64-aws-c-s3 PKG@@mingw-w64-x86_64-aws-c-sdkutils PKG@@mingw-w64-x86_64-aws-checksums PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-aws-crt-cpp'

override VERSION_OF_mingw-w64-x86_64-aws-sdk-cpp := 1.11.893-1
.PHONY: PKG@@mingw-w64-x86_64-aws-sdk-cpp
PKG@@mingw-w64-x86_64-aws-sdk-cpp: $(if $(__deps),PKG@@mingw-w64-x86_64-aws-c-common PKG@@mingw-w64-x86_64-aws-c-event-stream PKG@@mingw-w64-x86_64-aws-crt-cpp PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-aws-sdk-cpp'

override VERSION_OF_mingw-w64-x86_64-b3sum := 1.8.7-2
.PHONY: PKG@@mingw-w64-x86_64-b3sum
PKG@@mingw-w64-x86_64-b3sum: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-b3sum'

override VERSION_OF_mingw-w64-x86_64-bin2header := 0.3.1-3
.PHONY: PKG@@mingw-w64-x86_64-bin2header
PKG@@mingw-w64-x86_64-bin2header: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-bin2header'

override VERSION_OF_mingw-w64-x86_64-binaryen := 133-1
.PHONY: PKG@@mingw-w64-x86_64-binaryen
PKG@@mingw-w64-x86_64-binaryen: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-binaryen'

override VERSION_OF_mingw-w64-x86_64-binutils := 2.47-4
.PHONY: PKG@@mingw-w64-x86_64-binutils
PKG@@mingw-w64-x86_64-binutils: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-binutils'

override VERSION_OF_mingw-w64-x86_64-blas := 3.12.1-2
.PHONY: PKG@@mingw-w64-x86_64-blas
PKG@@mingw-w64-x86_64-blas: $(if $(__deps),PKG@@mingw-w64-x86_64-gcc-libgfortran) ; @echo 'PKG@@mingw-w64-x86_64-blas'

override VERSION_OF_mingw-w64-x86_64-blas64 := 3.12.1-2
.PHONY: PKG@@mingw-w64-x86_64-blas64
PKG@@mingw-w64-x86_64-blas64: $(if $(__deps),PKG@@mingw-w64-x86_64-gcc-libgfortran) ; @echo 'PKG@@mingw-w64-x86_64-blas64'

override VERSION_OF_mingw-w64-x86_64-blosc := 1.21.6-3
.PHONY: PKG@@mingw-w64-x86_64-blosc
PKG@@mingw-w64-x86_64-blosc: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-lz4 PKG@@mingw-w64-x86_64-snappy PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-blosc'

override VERSION_OF_mingw-w64-x86_64-blosc2 := 3.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-blosc2
PKG@@mingw-w64-x86_64-blosc2: $(if $(__deps),PKG@@mingw-w64-x86_64-lz4 PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-blosc2'

override VERSION_OF_mingw-w64-x86_64-boost := 1.92.0-4
.PHONY: PKG@@mingw-w64-x86_64-boost
PKG@@mingw-w64-x86_64-boost: $(if $(__deps),PKG@@mingw-w64-x86_64-boost-libs) ; @echo 'PKG@@mingw-w64-x86_64-boost'

override VERSION_OF_mingw-w64-x86_64-boost-libs := 1.92.0-4
.PHONY: PKG@@mingw-w64-x86_64-boost-libs
PKG@@mingw-w64-x86_64-boost-libs: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libquadmath PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-boost-libs'

override VERSION_OF_mingw-w64-x86_64-brotli := 1.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-brotli
PKG@@mingw-w64-x86_64-brotli: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-brotli'

override VERSION_OF_mingw-w64-x86_64-brotli-testdata := 1.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-brotli-testdata
PKG@@mingw-w64-x86_64-brotli-testdata: $(if $(__deps),PKG@@mingw-w64-x86_64-brotli) ; @echo 'PKG@@mingw-w64-x86_64-brotli-testdata'

override VERSION_OF_mingw-w64-x86_64-bullet := 3.25-11
.PHONY: PKG@@mingw-w64-x86_64-bullet
PKG@@mingw-w64-x86_64-bullet: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-freeglut PKG@@mingw-w64-x86_64-openvr) ; @echo 'PKG@@mingw-w64-x86_64-bullet'

override VERSION_OF_mingw-w64-x86_64-bullet-dp := 3.25-11
.PHONY: PKG@@mingw-w64-x86_64-bullet-dp
PKG@@mingw-w64-x86_64-bullet-dp: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-freeglut PKG@@mingw-w64-x86_64-openvr) ; @echo 'PKG@@mingw-w64-x86_64-bullet-dp'

override VERSION_OF_mingw-w64-x86_64-bzip2 := 1.0.8-4
.PHONY: PKG@@mingw-w64-x86_64-bzip2
PKG@@mingw-w64-x86_64-bzip2: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-bzip2'

override VERSION_OF_mingw-w64-x86_64-c-ares := 1.34.9-1
.PHONY: PKG@@mingw-w64-x86_64-c-ares
PKG@@mingw-w64-x86_64-c-ares: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-c-ares'

override VERSION_OF_mingw-w64-x86_64-ca-certificates := 20260816-3
.PHONY: PKG@@mingw-w64-x86_64-ca-certificates
PKG@@mingw-w64-x86_64-ca-certificates: $(if $(__deps),PKG@@mingw-w64-x86_64-p11-kit) ; @echo 'PKG@@mingw-w64-x86_64-ca-certificates'

override VERSION_OF_mingw-w64-x86_64-cairo := 1.18.6-2
.PHONY: PKG@@mingw-w64-x86_64-cairo
PKG@@mingw-w64-x86_64-cairo: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-fontconfig PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-lzo2 PKG@@mingw-w64-x86_64-pixman PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-cairo'

override VERSION_OF_mingw-w64-x86_64-cantarell-fonts := 0.311-1
.PHONY: PKG@@mingw-w64-x86_64-cantarell-fonts
PKG@@mingw-w64-x86_64-cantarell-fonts: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cantarell-fonts'

override VERSION_OF_mingw-w64-x86_64-capstone := 5.0.9-1
.PHONY: PKG@@mingw-w64-x86_64-capstone
PKG@@mingw-w64-x86_64-capstone: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-capstone'

override VERSION_OF_mingw-w64-x86_64-cargo-c := 0.10.26-2
.PHONY: PKG@@mingw-w64-x86_64-cargo-c
PKG@@mingw-w64-x86_64-cargo-c: $(if $(__deps),PKG@@mingw-w64-x86_64-libgit2 PKG@@mingw-w64-x86_64-libssh2 PKG@@mingw-w64-x86_64-rust PKG@@mingw-w64-x86_64-sqlite) ; @echo 'PKG@@mingw-w64-x86_64-cargo-c'

override VERSION_OF_mingw-w64-x86_64-cargo-edit := 0.13.14-2
.PHONY: PKG@@mingw-w64-x86_64-cargo-edit
PKG@@mingw-w64-x86_64-cargo-edit: $(if $(__deps),PKG@@mingw-w64-x86_64-rust) ; @echo 'PKG@@mingw-w64-x86_64-cargo-edit'

override VERSION_OF_mingw-w64-x86_64-cargo-generate := 0.25.1-1
.PHONY: PKG@@mingw-w64-x86_64-cargo-generate
PKG@@mingw-w64-x86_64-cargo-generate: $(if $(__deps),PKG@@mingw-w64-x86_64-libgit2 PKG@@mingw-w64-x86_64-libssh2 PKG@@mingw-w64-x86_64-rust) ; @echo 'PKG@@mingw-w64-x86_64-cargo-generate'

override VERSION_OF_mingw-w64-x86_64-cargo-local-registry := 0.2.9-1
.PHONY: PKG@@mingw-w64-x86_64-cargo-local-registry
PKG@@mingw-w64-x86_64-cargo-local-registry: $(if $(__deps),PKG@@mingw-w64-x86_64-libssh2 PKG@@mingw-w64-x86_64-rust PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-cargo-local-registry'

override VERSION_OF_mingw-w64-x86_64-cas-offinder := 2.4.1-6
.PHONY: PKG@@mingw-w64-x86_64-cas-offinder
PKG@@mingw-w64-x86_64-cas-offinder: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-opencl-icd) ; @echo 'PKG@@mingw-w64-x86_64-cas-offinder'

override VERSION_OF_mingw-w64-x86_64-castxml := 0.8.0-1
.PHONY: PKG@@mingw-w64-x86_64-castxml
PKG@@mingw-w64-x86_64-castxml: $(if $(__deps),PKG@@mingw-w64-x86_64-clang-libs PKG@@mingw-w64-x86_64-llvm-libs) ; @echo 'PKG@@mingw-w64-x86_64-castxml'

override VERSION_OF_mingw-w64-x86_64-catch := 3.16.1-1
.PHONY: PKG@@mingw-w64-x86_64-catch
PKG@@mingw-w64-x86_64-catch: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-catch'

override VERSION_OF_mingw-w64-x86_64-cblas := 3.12.1-2
.PHONY: PKG@@mingw-w64-x86_64-cblas
PKG@@mingw-w64-x86_64-cblas: $(if $(__deps),PKG@@mingw-w64-x86_64-blas) ; @echo 'PKG@@mingw-w64-x86_64-cblas'

override VERSION_OF_mingw-w64-x86_64-cblas64 := 3.12.1-2
.PHONY: PKG@@mingw-w64-x86_64-cblas64
PKG@@mingw-w64-x86_64-cblas64: $(if $(__deps),PKG@@mingw-w64-x86_64-blas64) ; @echo 'PKG@@mingw-w64-x86_64-cblas64'

override VERSION_OF_mingw-w64-x86_64-cc-libs := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-cc-libs
PKG@@mingw-w64-x86_64-cc-libs: $(if $(__deps),PKG@@mingw-w64-x86_64-libgcc PKG@@mingw-w64-x86_64-libstdc++) ; @echo 'PKG@@mingw-w64-x86_64-cc-libs'

override VERSION_OF_mingw-w64-x86_64-ccache := 4.14.2-1
.PHONY: PKG@@mingw-w64-x86_64-ccache
PKG@@mingw-w64-x86_64-ccache: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-fmt PKG@@mingw-w64-x86_64-hiredis PKG@@mingw-w64-x86_64-libblake3 PKG@@mingw-w64-x86_64-xxhash PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-ccache'

override VERSION_OF_mingw-w64-x86_64-cccl := 1.4-1
.PHONY: PKG@@mingw-w64-x86_64-cccl
PKG@@mingw-w64-x86_64-cccl: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cccl'

override VERSION_OF_mingw-w64-x86_64-ccmake := 4.4.5-1
.PHONY: PKG@@mingw-w64-x86_64-ccmake
PKG@@mingw-w64-x86_64-ccmake: $(if $(__deps),PKG@@mingw-w64-x86_64-cmake) ; @echo 'PKG@@mingw-w64-x86_64-ccmake'

override VERSION_OF_mingw-w64-x86_64-cddlib := 0.94n-3
.PHONY: PKG@@mingw-w64-x86_64-cddlib
PKG@@mingw-w64-x86_64-cddlib: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gmp) ; @echo 'PKG@@mingw-w64-x86_64-cddlib'

override VERSION_OF_mingw-w64-x86_64-cereal := 1.3.2-2
.PHONY: PKG@@mingw-w64-x86_64-cereal
PKG@@mingw-w64-x86_64-cereal: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cereal'

override VERSION_OF_mingw-w64-x86_64-ceres-solver := 2.2.0-11
.PHONY: PKG@@mingw-w64-x86_64-ceres-solver
PKG@@mingw-w64-x86_64-ceres-solver: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-glog PKG@@mingw-w64-x86_64-suitesparse) ; @echo 'PKG@@mingw-w64-x86_64-ceres-solver'

override VERSION_OF_mingw-w64-x86_64-cfitsio := 1~4.7.0-1
.PHONY: PKG@@mingw-w64-x86_64-cfitsio
PKG@@mingw-w64-x86_64-cfitsio: $(if $(__deps),PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-cfitsio'

override VERSION_OF_mingw-w64-x86_64-cgal := 6.2-1
.PHONY: PKG@@mingw-w64-x86_64-cgal
PKG@@mingw-w64-x86_64-cgal: $(if $(__deps),PKG@@mingw-w64-x86_64-boost PKG@@mingw-w64-x86_64-eigen3 PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-mpfr) ; @echo 'PKG@@mingw-w64-x86_64-cgal'

override VERSION_OF_mingw-w64-x86_64-cglm := 0.9.7-1
.PHONY: PKG@@mingw-w64-x86_64-cglm
PKG@@mingw-w64-x86_64-cglm: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cglm'

override VERSION_OF_mingw-w64-x86_64-cglm-docs := 0.9.7-1
.PHONY: PKG@@mingw-w64-x86_64-cglm-docs
PKG@@mingw-w64-x86_64-cglm-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cglm-docs'

override VERSION_OF_mingw-w64-x86_64-cgns := 4.5.2-2
.PHONY: PKG@@mingw-w64-x86_64-cgns
PKG@@mingw-w64-x86_64-cgns: $(if $(__deps),PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-hdf5) ; @echo 'PKG@@mingw-w64-x86_64-cgns'

override VERSION_OF_mingw-w64-x86_64-chafa := 1.18.2-1
.PHONY: PKG@@mingw-w64-x86_64-chafa
PKG@@mingw-w64-x86_64-chafa: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libavif PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libjxl PKG@@mingw-w64-x86_64-librsvg PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwebp) ; @echo 'PKG@@mingw-w64-x86_64-chafa'

override VERSION_OF_mingw-w64-x86_64-check := 0.15.2-4
.PHONY: PKG@@mingw-w64-x86_64-check
PKG@@mingw-w64-x86_64-check: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-check'

override VERSION_OF_mingw-w64-x86_64-chicken := 5.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-chicken
PKG@@mingw-w64-x86_64-chicken: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-chicken'

override VERSION_OF_mingw-w64-x86_64-cjose := 0.6.2.8-1
.PHONY: PKG@@mingw-w64-x86_64-cjose
PKG@@mingw-w64-x86_64-cjose: $(if $(__deps),PKG@@mingw-w64-x86_64-jansson PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-cjose'

override VERSION_OF_mingw-w64-x86_64-cjson := 1.7.19-1
.PHONY: PKG@@mingw-w64-x86_64-cjson
PKG@@mingw-w64-x86_64-cjson: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-cjson'

override VERSION_OF_mingw-w64-x86_64-clang := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-clang
PKG@@mingw-w64-x86_64-clang: $(if $(__deps),PKG@@mingw-w64-x86_64-clang-libs PKG@@mingw-w64-x86_64-gcc PKG@@mingw-w64-x86_64-llvm-libs) ; @echo 'PKG@@mingw-w64-x86_64-clang'

override VERSION_OF_mingw-w64-x86_64-clang-analyzer := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-clang-analyzer
PKG@@mingw-w64-x86_64-clang-analyzer: $(if $(__deps),PKG@@mingw-w64-x86_64-clang PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-clang-analyzer'

override VERSION_OF_mingw-w64-x86_64-clang-libs := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-clang-libs
PKG@@mingw-w64-x86_64-clang-libs: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-llvm-libs) ; @echo 'PKG@@mingw-w64-x86_64-clang-libs'

override VERSION_OF_mingw-w64-x86_64-clang-tools-extra := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-clang-tools-extra
PKG@@mingw-w64-x86_64-clang-tools-extra: $(if $(__deps),PKG@@mingw-w64-x86_64-clang) ; @echo 'PKG@@mingw-w64-x86_64-clang-tools-extra'

override VERSION_OF_mingw-w64-x86_64-clblast := 1.6.3-3
.PHONY: PKG@@mingw-w64-x86_64-clblast
PKG@@mingw-w64-x86_64-clblast: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-opencl-icd) ; @echo 'PKG@@mingw-w64-x86_64-clblast'

override VERSION_OF_mingw-w64-x86_64-cli11 := 2.7.2-1
.PHONY: PKG@@mingw-w64-x86_64-cli11
PKG@@mingw-w64-x86_64-cli11: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cli11'

override VERSION_OF_mingw-w64-x86_64-clucene := 2.3.3.4-7
.PHONY: PKG@@mingw-w64-x86_64-clucene
PKG@@mingw-w64-x86_64-clucene: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-clucene'

override VERSION_OF_mingw-w64-x86_64-cmake := 4.4.5-1
.PHONY: PKG@@mingw-w64-x86_64-cmake
PKG@@mingw-w64-x86_64-cmake: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-cppdap PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-jsoncpp PKG@@mingw-w64-x86_64-libarchive PKG@@mingw-w64-x86_64-libuv PKG@@mingw-w64-x86_64-ninja PKG@@mingw-w64-x86_64-pkgconf PKG@@mingw-w64-x86_64-rhash PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-cmake'

override VERSION_OF_mingw-w64-x86_64-cmake-bootstrap := 4.4.5-1
.PHONY: PKG@@mingw-w64-x86_64-cmake-bootstrap
PKG@@mingw-w64-x86_64-cmake-bootstrap: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-pkgconf) ; @echo 'PKG@@mingw-w64-x86_64-cmake-bootstrap'

override VERSION_OF_mingw-w64-x86_64-cmake-cmcldeps := 4.4.5-1
.PHONY: PKG@@mingw-w64-x86_64-cmake-cmcldeps
PKG@@mingw-w64-x86_64-cmake-cmcldeps: $(if $(__deps),PKG@@mingw-w64-x86_64-cmake) ; @echo 'PKG@@mingw-w64-x86_64-cmake-cmcldeps'

override VERSION_OF_mingw-w64-x86_64-cmake-docs := 4.4.5-1
.PHONY: PKG@@mingw-w64-x86_64-cmake-doc-qt PKG@@mingw-w64-x86_64-cmake-docs
PKG@@mingw-w64-x86_64-cmake-doc-qt PKG@@mingw-w64-x86_64-cmake-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cmake-docs'

override VERSION_OF_mingw-w64-x86_64-cmake-gui := 4.4.5-1
.PHONY: PKG@@mingw-w64-x86_64-cmake-gui
PKG@@mingw-w64-x86_64-cmake-gui: $(if $(__deps),PKG@@mingw-w64-x86_64-cmake PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-cmake-gui'

override VERSION_OF_mingw-w64-x86_64-cmakelang := 0.6.13-4
.PHONY: PKG@@mingw-w64-x86_64-cmakelang
PKG@@mingw-w64-x86_64-cmakelang: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-six) ; @echo 'PKG@@mingw-w64-x86_64-cmakelang'

override VERSION_OF_mingw-w64-x86_64-cmakerc := 2.0.1-3
.PHONY: PKG@@mingw-w64-x86_64-cmakerc
PKG@@mingw-w64-x86_64-cmakerc: $(if $(__deps),PKG@@mingw-w64-x86_64-cmake) ; @echo 'PKG@@mingw-w64-x86_64-cmakerc'

override VERSION_OF_mingw-w64-x86_64-cmocka := 2.0.2-1
.PHONY: PKG@@mingw-w64-x86_64-cmocka
PKG@@mingw-w64-x86_64-cmocka: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-cmocka'

override VERSION_OF_mingw-w64-x86_64-cninja := 3.7.9-2
.PHONY: PKG@@mingw-w64-x86_64-cninja
PKG@@mingw-w64-x86_64-cninja: $(if $(__deps),PKG@@mingw-w64-x86_64-clang PKG@@mingw-w64-x86_64-cmake PKG@@mingw-w64-x86_64-libc++ PKG@@mingw-w64-x86_64-lld PKG@@mingw-w64-x86_64-ninja) ; @echo 'PKG@@mingw-w64-x86_64-cninja'

override VERSION_OF_mingw-w64-x86_64-codec2 := 1.2.0-2
.PHONY: PKG@@mingw-w64-x86_64-codec2
PKG@@mingw-w64-x86_64-codec2: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-codec2'

override VERSION_OF_mingw-w64-x86_64-collada-dom := 2.5.0-8
.PHONY: PKG@@mingw-w64-x86_64-collada-dom
PKG@@mingw-w64-x86_64-collada-dom: $(if $(__deps),PKG@@mingw-w64-x86_64-boost-libs PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-minizip PKG@@mingw-w64-x86_64-uriparser PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-collada-dom'

override VERSION_OF_mingw-w64-x86_64-compiler-rt := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-compiler-rt
PKG@@mingw-w64-x86_64-compiler-rt: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-compiler-rt'

override VERSION_OF_mingw-w64-x86_64-connect := 1.105-5
.PHONY: PKG@@mingw-w64-x86_64-connect
PKG@@mingw-w64-x86_64-connect: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-connect'

override VERSION_OF_mingw-w64-x86_64-corrade := 2020.06-6
.PHONY: PKG@@mingw-w64-x86_64-corrade
PKG@@mingw-w64-x86_64-corrade: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-corrade'

override VERSION_OF_mingw-w64-x86_64-cppcheck := 2.21.0-1
.PHONY: PKG@@mingw-w64-x86_64-cppcheck
PKG@@mingw-w64-x86_64-cppcheck: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-pcre) ; @echo 'PKG@@mingw-w64-x86_64-cppcheck'

override VERSION_OF_mingw-w64-x86_64-cppcoro := r390.a87e97f-3
.PHONY: PKG@@mingw-w64-x86_64-cppcoro
PKG@@mingw-w64-x86_64-cppcoro: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cppcoro'

override VERSION_OF_mingw-w64-x86_64-cppdap := 1.65.r11.g6464cd7-1
.PHONY: PKG@@mingw-w64-x86_64-cppdap
PKG@@mingw-w64-x86_64-cppdap: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-cppdap'

override VERSION_OF_mingw-w64-x86_64-cppreference-qt := 20250209-1
.PHONY: PKG@@mingw-w64-x86_64-cppreference-qt
PKG@@mingw-w64-x86_64-cppreference-qt: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cppreference-qt'

override VERSION_OF_mingw-w64-x86_64-cpptest := 2.0.1-1
.PHONY: PKG@@mingw-w64-x86_64-cpptest
PKG@@mingw-w64-x86_64-cpptest: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-cpptest'

override VERSION_OF_mingw-w64-x86_64-cppunit := 1.15.1-3
.PHONY: PKG@@mingw-w64-x86_64-cppunit
PKG@@mingw-w64-x86_64-cppunit: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-cppunit'

override VERSION_OF_mingw-w64-x86_64-cpputest := 4.0-3
.PHONY: PKG@@mingw-w64-x86_64-cpputest
PKG@@mingw-w64-x86_64-cpputest: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cpputest'

override VERSION_OF_mingw-w64-x86_64-cppwinrt := 3.0.260818.1-3
.PHONY: PKG@@mingw-w64-x86_64-cppwinrt
PKG@@mingw-w64-x86_64-cppwinrt: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-cppwinrt'

override VERSION_OF_mingw-w64-x86_64-cppzmq := 4.11.0-2
.PHONY: PKG@@mingw-w64-x86_64-cppzmq
PKG@@mingw-w64-x86_64-cppzmq: $(if $(__deps),PKG@@mingw-w64-x86_64-zeromq) ; @echo 'PKG@@mingw-w64-x86_64-cppzmq'

override VERSION_OF_mingw-w64-x86_64-crt := 14.0.0.r598.ge3d18511f-1
.PHONY: PKG@@mingw-w64-x86_64-crt PKG@@mingw-w64-x86_64-crt-git
PKG@@mingw-w64-x86_64-crt PKG@@mingw-w64-x86_64-crt-git: $(if $(__deps),PKG@@mingw-w64-x86_64-headers) ; @echo 'PKG@@mingw-w64-x86_64-crt'

override VERSION_OF_mingw-w64-x86_64-crypto++ := 8.9.0-2
.PHONY: PKG@@mingw-w64-x86_64-crypto++
PKG@@mingw-w64-x86_64-crypto++: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-crypto++'

override VERSION_OF_mingw-w64-x86_64-ctags := 6.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-ctags PKG@@mingw-w64-x86_64-universal-ctags PKG@@mingw-w64-x86_64-universal-ctags-git
PKG@@mingw-w64-x86_64-ctags PKG@@mingw-w64-x86_64-universal-ctags PKG@@mingw-w64-x86_64-universal-ctags-git: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-jansson PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-libyaml PKG@@mingw-w64-x86_64-pcre2) ; @echo 'PKG@@mingw-w64-x86_64-ctags'

override VERSION_OF_mingw-w64-x86_64-ctpl := 0.3.5-1
.PHONY: PKG@@mingw-w64-x86_64-ctpl PKG@@mingw-w64-x86_64-ctpl-git
PKG@@mingw-w64-x86_64-ctpl PKG@@mingw-w64-x86_64-ctpl-git: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2) ; @echo 'PKG@@mingw-w64-x86_64-ctpl'

override VERSION_OF_mingw-w64-x86_64-cunit := 2.1.3-4
.PHONY: PKG@@mingw-w64-x86_64-cunit
PKG@@mingw-w64-x86_64-cunit: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cunit'

override VERSION_OF_mingw-w64-x86_64-curl := 8.23.0-1
.PHONY: PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-wcurl
PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-wcurl: $(if $(__deps),PKG@@mingw-w64-x86_64-brotli PKG@@mingw-w64-x86_64-c-ares PKG@@mingw-w64-x86_64-ca-certificates PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libidn2 PKG@@mingw-w64-x86_64-libpsl PKG@@mingw-w64-x86_64-libssh2 PKG@@mingw-w64-x86_64-nghttp2 PKG@@mingw-w64-x86_64-nghttp3 PKG@@mingw-w64-x86_64-ngtcp2 PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-curl'

override VERSION_OF_mingw-w64-x86_64-curl-gnutls := 8.23.0-1
.PHONY: PKG@@mingw-w64-x86_64-curl-gnutls
PKG@@mingw-w64-x86_64-curl-gnutls: $(if $(__deps),PKG@@mingw-w64-x86_64-brotli PKG@@mingw-w64-x86_64-c-ares PKG@@mingw-w64-x86_64-ca-certificates PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gnutls PKG@@mingw-w64-x86_64-libidn2 PKG@@mingw-w64-x86_64-libpsl PKG@@mingw-w64-x86_64-libssh2 PKG@@mingw-w64-x86_64-nghttp2 PKG@@mingw-w64-x86_64-nghttp3 PKG@@mingw-w64-x86_64-ngtcp2 PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-curl-gnutls'

override VERSION_OF_mingw-w64-x86_64-curl-winssl := 8.23.0-1
.PHONY: PKG@@mingw-w64-x86_64-curl-winssl
PKG@@mingw-w64-x86_64-curl-winssl: $(if $(__deps),PKG@@mingw-w64-x86_64-brotli PKG@@mingw-w64-x86_64-c-ares PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libidn2 PKG@@mingw-w64-x86_64-libpsl PKG@@mingw-w64-x86_64-libssh2-wincng PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-curl-winssl'

override VERSION_OF_mingw-w64-x86_64-cusp := 0.5.1-2
.PHONY: PKG@@mingw-w64-x86_64-cusp
PKG@@mingw-w64-x86_64-cusp: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cusp'

override VERSION_OF_mingw-w64-x86_64-cxxopts := 3.3.1-1
.PHONY: PKG@@mingw-w64-x86_64-cxxopts
PKG@@mingw-w64-x86_64-cxxopts: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-cxxopts'

override VERSION_OF_mingw-w64-x86_64-cyrus-sasl := 2.1.28-6
.PHONY: PKG@@mingw-w64-x86_64-cyrus-sasl
PKG@@mingw-w64-x86_64-cyrus-sasl: $(if $(__deps),PKG@@mingw-w64-x86_64-gdbm PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-cyrus-sasl'

override VERSION_OF_mingw-w64-x86_64-cython := 3.3.1-1
.PHONY: PKG@@mingw-w64-x86_64-cython
PKG@@mingw-w64-x86_64-cython: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-cython'

override VERSION_OF_mingw-w64-x86_64-d-feet := 0.3.16-10
.PHONY: PKG@@mingw-w64-x86_64-d-feet
PKG@@mingw-w64-x86_64-d-feet: $(if $(__deps),PKG@@mingw-w64-x86_64-gtk3 PKG@@mingw-w64-x86_64-hicolor-icon-theme PKG@@mingw-w64-x86_64-python-gobject) ; @echo 'PKG@@mingw-w64-x86_64-d-feet'

override VERSION_OF_mingw-w64-x86_64-dav1d := 1.5.4-1
.PHONY: PKG@@mingw-w64-x86_64-dav1d
PKG@@mingw-w64-x86_64-dav1d: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-dav1d'

override VERSION_OF_mingw-w64-x86_64-db := 6.2.32-1
.PHONY: PKG@@mingw-w64-x86_64-db
PKG@@mingw-w64-x86_64-db: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-db'

override VERSION_OF_mingw-w64-x86_64-db-docs := 6.2.32-1
.PHONY: PKG@@mingw-w64-x86_64-db-docs
PKG@@mingw-w64-x86_64-db-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-db-docs'

override VERSION_OF_mingw-w64-x86_64-dbus := 1.16.2-4
.PHONY: PKG@@mingw-w64-x86_64-dbus
PKG@@mingw-w64-x86_64-dbus: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-dbus'

override VERSION_OF_mingw-w64-x86_64-dbus-c++ := 0.9.0-5
.PHONY: PKG@@mingw-w64-x86_64-dbus-c++
PKG@@mingw-w64-x86_64-dbus-c++: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-dbus PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-dbus-c++'

override VERSION_OF_mingw-w64-x86_64-dbus-doc := 1.16.2-4
.PHONY: PKG@@mingw-w64-x86_64-dbus-doc
PKG@@mingw-w64-x86_64-dbus-doc: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-dbus-doc'

override VERSION_OF_mingw-w64-x86_64-dbus-glib := 0.114-4
.PHONY: PKG@@mingw-w64-x86_64-dbus-glib
PKG@@mingw-w64-x86_64-dbus-glib: $(if $(__deps),PKG@@mingw-w64-x86_64-dbus PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-glib2) ; @echo 'PKG@@mingw-w64-x86_64-dbus-glib'

override VERSION_OF_mingw-w64-x86_64-dcadec := 0.2.0-4
.PHONY: PKG@@mingw-w64-x86_64-dcadec
PKG@@mingw-w64-x86_64-dcadec: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-dcadec'

override VERSION_OF_mingw-w64-x86_64-devil := 1.8.0-12
.PHONY: PKG@@mingw-w64-x86_64-DevIL PKG@@mingw-w64-x86_64-devil
PKG@@mingw-w64-x86_64-DevIL PKG@@mingw-w64-x86_64-devil: $(if $(__deps),PKG@@mingw-w64-x86_64-freeglut PKG@@mingw-w64-x86_64-jasper PKG@@mingw-w64-x86_64-lcms2 PKG@@mingw-w64-x86_64-libmng PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libsquish PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-devil'

override VERSION_OF_mingw-w64-x86_64-directx-headers := 1~1.619.5-1
.PHONY: PKG@@mingw-w64-x86_64-directx-headers
PKG@@mingw-w64-x86_64-directx-headers: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-directx-headers'

override VERSION_OF_mingw-w64-x86_64-directxmath := 3.20.b-1
.PHONY: PKG@@mingw-w64-x86_64-directxmath
PKG@@mingw-w64-x86_64-directxmath: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-directxmath'

override VERSION_OF_mingw-w64-x86_64-directxmesh := 1.6.9-1
.PHONY: PKG@@mingw-w64-x86_64-directxmesh
PKG@@mingw-w64-x86_64-directxmesh: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-directx-headers PKG@@mingw-w64-x86_64-directxmath) ; @echo 'PKG@@mingw-w64-x86_64-directxmesh'

override VERSION_OF_mingw-w64-x86_64-directxtex := 2.0.9-2
.PHONY: PKG@@mingw-w64-x86_64-directxtex
PKG@@mingw-w64-x86_64-directxtex: $(if $(__deps),PKG@@mingw-w64-x86_64-directx-headers PKG@@mingw-w64-x86_64-directxmath PKG@@mingw-w64-x86_64-openexr) ; @echo 'PKG@@mingw-w64-x86_64-directxtex'

override VERSION_OF_mingw-w64-x86_64-djvulibre := 3.5.30-1
.PHONY: PKG@@mingw-w64-x86_64-djvulibre
PKG@@mingw-w64-x86_64-djvulibre: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-djvulibre'

override VERSION_OF_mingw-w64-x86_64-dlfcn := 1.4.2-1
.PHONY: PKG@@mingw-w64-x86_64-dlfcn
PKG@@mingw-w64-x86_64-dlfcn: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-dlfcn'

override VERSION_OF_mingw-w64-x86_64-dnssec-anchors := 20130320-2
.PHONY: PKG@@mingw-w64-x86_64-dnssec-anchors
PKG@@mingw-w64-x86_64-dnssec-anchors: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-dnssec-anchors'

override VERSION_OF_mingw-w64-x86_64-docbook-dsssl := 1.79-4
.PHONY: PKG@@mingw-w64-x86_64-docbook-dsssl
PKG@@mingw-w64-x86_64-docbook-dsssl: $(if $(__deps),PKG@@mingw-w64-x86_64-perl PKG@@mingw-w64-x86_64-sgml-common) ; @echo 'PKG@@mingw-w64-x86_64-docbook-dsssl'

override VERSION_OF_mingw-w64-x86_64-docbook-mathml := 1.1CR1-5
.PHONY: PKG@@mingw-w64-x86_64-docbook-mathml
PKG@@mingw-w64-x86_64-docbook-mathml: $(if $(__deps),PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-docbook-mathml'

override VERSION_OF_mingw-w64-x86_64-docbook-xml := 1~4.5-4
.PHONY: PKG@@mingw-w64-x86_64-docbook-xml
PKG@@mingw-w64-x86_64-docbook-xml: $(if $(__deps),PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-docbook-xml'

override VERSION_OF_mingw-w64-x86_64-docbook-xsl := 1.79.2-9
.PHONY: PKG@@mingw-w64-x86_64-docbook-xsl
PKG@@mingw-w64-x86_64-docbook-xsl: $(if $(__deps),PKG@@mingw-w64-x86_64-docbook-xml PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-libxslt) ; @echo 'PKG@@mingw-w64-x86_64-docbook-xsl'

override VERSION_OF_mingw-w64-x86_64-docbook5-xml := 5.1-3
.PHONY: PKG@@mingw-w64-x86_64-docbook5-xml
PKG@@mingw-w64-x86_64-docbook5-xml: $(if $(__deps),PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-docbook5-xml'

override VERSION_OF_mingw-w64-x86_64-doctest := 2.5.3-1
.PHONY: PKG@@mingw-w64-x86_64-doctest
PKG@@mingw-w64-x86_64-doctest: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-doctest'

override VERSION_OF_mingw-w64-x86_64-docx2txt-rs := 0.2.0-3
.PHONY: PKG@@mingw-w64-x86_64-docx2txt-rs
PKG@@mingw-w64-x86_64-docx2txt-rs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-docx2txt-rs'

override VERSION_OF_mingw-w64-x86_64-double-conversion := 3.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-double-conversion
PKG@@mingw-w64-x86_64-double-conversion: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-double-conversion'

override VERSION_OF_mingw-w64-x86_64-doxygen := 1.18.0-5
.PHONY: PKG@@mingw-w64-x86_64-doxygen
PKG@@mingw-w64-x86_64-doxygen: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-clang-libs PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-spdlog PKG@@mingw-w64-x86_64-sqlite3 PKG@@mingw-w64-x86_64-xapian-core) ; @echo 'PKG@@mingw-w64-x86_64-doxygen'

override VERSION_OF_mingw-w64-x86_64-draco := 1.5.9-1
.PHONY: PKG@@mingw-w64-x86_64-draco
PKG@@mingw-w64-x86_64-draco: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-draco'

override VERSION_OF_mingw-w64-x86_64-dsdp := 5.8-6
.PHONY: PKG@@mingw-w64-x86_64-dsdp
PKG@@mingw-w64-x86_64-dsdp: $(if $(__deps),PKG@@mingw-w64-x86_64-openblas) ; @echo 'PKG@@mingw-w64-x86_64-dsdp'

override VERSION_OF_mingw-w64-x86_64-duktape := 2.7.0-2
.PHONY: PKG@@mingw-w64-x86_64-duktape
PKG@@mingw-w64-x86_64-duktape: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-duktape'

override VERSION_OF_mingw-w64-x86_64-efxc2 := 0.0.14.279-2
.PHONY: PKG@@mingw-w64-x86_64-efxc2
PKG@@mingw-w64-x86_64-efxc2: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-efxc2'

override VERSION_OF_mingw-w64-x86_64-egl-headers := 1.5.r284.3ae2b7c-1
.PHONY: PKG@@mingw-w64-x86_64-egl-headers
PKG@@mingw-w64-x86_64-egl-headers: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-egl-headers'

override VERSION_OF_mingw-w64-x86_64-eigen3 := 5.0.1-1
.PHONY: PKG@@mingw-w64-x86_64-eigen3
PKG@@mingw-w64-x86_64-eigen3: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-eigen3'

override VERSION_OF_mingw-w64-x86_64-expat := 2.10.0-1
.PHONY: PKG@@mingw-w64-x86_64-expat
PKG@@mingw-w64-x86_64-expat: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-expat'

override VERSION_OF_mingw-w64-x86_64-exprtk := 0.0.3-1
.PHONY: PKG@@mingw-w64-x86_64-exprtk
PKG@@mingw-w64-x86_64-exprtk: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-exprtk'

override VERSION_OF_mingw-w64-x86_64-extra-cmake-modules := 6.31.0-1
.PHONY: PKG@@mingw-w64-x86_64-extra-cmake-modules
PKG@@mingw-w64-x86_64-extra-cmake-modules: $(if $(__deps),PKG@@mingw-w64-x86_64-cmake) ; @echo 'PKG@@mingw-w64-x86_64-extra-cmake-modules'

override VERSION_OF_mingw-w64-x86_64-fast_float := 8.3.3-1
.PHONY: PKG@@mingw-w64-x86_64-fast_float
PKG@@mingw-w64-x86_64-fast_float: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-fast_float'

override VERSION_OF_mingw-w64-x86_64-ffmpeg := 9.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-ffmpeg
PKG@@mingw-w64-x86_64-ffmpeg: $(if $(__deps),PKG@@mingw-w64-x86_64-aom PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-dav1d PKG@@mingw-w64-x86_64-fontconfig PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-frei0r-plugins PKG@@mingw-w64-x86_64-fribidi PKG@@mingw-w64-x86_64-ggml PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-gnutls PKG@@mingw-w64-x86_64-gsm PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-lame PKG@@mingw-w64-x86_64-libass PKG@@mingw-w64-x86_64-libbluray PKG@@mingw-w64-x86_64-libcaca PKG@@mingw-w64-x86_64-libexif PKG@@mingw-w64-x86_64-libgme PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libjxl PKG@@mingw-w64-x86_64-liblc3 PKG@@mingw-w64-x86_64-libmodplug PKG@@mingw-w64-x86_64-libplacebo PKG@@mingw-w64-x86_64-librsvg PKG@@mingw-w64-x86_64-libsoxr PKG@@mingw-w64-x86_64-libssh PKG@@mingw-w64-x86_64-libtheora PKG@@mingw-w64-x86_64-libva PKG@@mingw-w64-x86_64-libvorbis PKG@@mingw-w64-x86_64-libvpl PKG@@mingw-w64-x86_64-libvpx PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-libx264 PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-openal PKG@@mingw-w64-x86_64-opencore-amr PKG@@mingw-w64-x86_64-openjpeg2 PKG@@mingw-w64-x86_64-opus PKG@@mingw-w64-x86_64-rav1e PKG@@mingw-w64-x86_64-rtmpdump PKG@@mingw-w64-x86_64-sdl2 PKG@@mingw-w64-x86_64-speex PKG@@mingw-w64-x86_64-srt PKG@@mingw-w64-x86_64-svt-av1 PKG@@mingw-w64-x86_64-vid.stab PKG@@mingw-w64-x86_64-vulkan PKG@@mingw-w64-x86_64-whisper.cpp PKG@@mingw-w64-x86_64-x265 PKG@@mingw-w64-x86_64-xvidcore PKG@@mingw-w64-x86_64-zimg PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zvbi) ; @echo 'PKG@@mingw-w64-x86_64-ffmpeg'

override VERSION_OF_mingw-w64-x86_64-ffms2 := 5.0-5
.PHONY: PKG@@mingw-w64-x86_64-ffms2
PKG@@mingw-w64-x86_64-ffms2: $(if $(__deps),PKG@@mingw-w64-x86_64-ffmpeg) ; @echo 'PKG@@mingw-w64-x86_64-ffms2'

override VERSION_OF_mingw-w64-x86_64-ffnvcodec-headers := 13.0.19.0-1
.PHONY: PKG@@mingw-w64-x86_64-ffnvcodec-headers
PKG@@mingw-w64-x86_64-ffnvcodec-headers: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-ffnvcodec-headers'

override VERSION_OF_mingw-w64-x86_64-fftw := 3.3.11-2
.PHONY: PKG@@mingw-w64-x86_64-fftw
PKG@@mingw-w64-x86_64-fftw: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libquadmath PKG@@mingw-w64-x86_64-omp) ; @echo 'PKG@@mingw-w64-x86_64-fftw'

override VERSION_OF_mingw-w64-x86_64-file := 5.48-1
.PHONY: PKG@@mingw-w64-x86_64-file
PKG@@mingw-w64-x86_64-file: $(if $(__deps),PKG@@mingw-w64-x86_64-libsystre) ; @echo 'PKG@@mingw-w64-x86_64-file'

override VERSION_OF_mingw-w64-x86_64-firebird := 5.0.4.1812-4
.PHONY: PKG@@mingw-w64-x86_64-firebird
PKG@@mingw-w64-x86_64-firebird: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libtomcrypt PKG@@mingw-w64-x86_64-libtommath PKG@@mingw-w64-x86_64-re2) ; @echo 'PKG@@mingw-w64-x86_64-firebird'

override VERSION_OF_mingw-w64-x86_64-flac := 1.5.0-2
.PHONY: PKG@@mingw-w64-x86_64-flac
PKG@@mingw-w64-x86_64-flac: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libogg PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-flac'

override VERSION_OF_mingw-w64-x86_64-flatbuffers := 25.12.19-1
.PHONY: PKG@@mingw-w64-x86_64-flatbuffers
PKG@@mingw-w64-x86_64-flatbuffers: $(if $(__deps),PKG@@mingw-w64-x86_64-libsystre) ; @echo 'PKG@@mingw-w64-x86_64-flatbuffers'

override VERSION_OF_mingw-w64-x86_64-flif := 0.4-3
.PHONY: PKG@@mingw-w64-x86_64-flif
PKG@@mingw-w64-x86_64-flif: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-sdl2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-flif'

override VERSION_OF_mingw-w64-x86_64-fluidsynth := 2.6.2-2
.PHONY: PKG@@mingw-w64-x86_64-fluidsynth
PKG@@mingw-w64-x86_64-fluidsynth: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libsndfile PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-portaudio PKG@@mingw-w64-x86_64-readline PKG@@mingw-w64-x86_64-sdl3) ; @echo 'PKG@@mingw-w64-x86_64-fluidsynth'

override VERSION_OF_mingw-w64-x86_64-fmt := 12.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-fmt
PKG@@mingw-w64-x86_64-fmt: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-fmt'

override VERSION_OF_mingw-w64-x86_64-fontconfig := 2.18.3-1
.PHONY: PKG@@mingw-w64-x86_64-fontconfig
PKG@@mingw-w64-x86_64-fontconfig: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-freetype) ; @echo 'PKG@@mingw-w64-x86_64-fontconfig'

override VERSION_OF_mingw-w64-x86_64-freeglut := 3.8.0-1
.PHONY: PKG@@mingw-w64-x86_64-freeglut
PKG@@mingw-w64-x86_64-freeglut: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-freeglut'

override VERSION_OF_mingw-w64-x86_64-freeimage := 3.18.0-19
.PHONY: PKG@@mingw-w64-x86_64-freeimage
PKG@@mingw-w64-x86_64-freeimage: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-imath PKG@@mingw-w64-x86_64-jxrlib PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libraw PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-openexr PKG@@mingw-w64-x86_64-openjpeg2) ; @echo 'PKG@@mingw-w64-x86_64-freeimage'

override VERSION_OF_mingw-w64-x86_64-freetype := 2.15.0-1
.PHONY: PKG@@mingw-w64-x86_64-freetype
PKG@@mingw-w64-x86_64-freetype: $(if $(__deps),PKG@@mingw-w64-x86_64-brotli PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-freetype'

override VERSION_OF_mingw-w64-x86_64-frei0r-plugins := 3.7.0-1
.PHONY: PKG@@mingw-w64-x86_64-frei0r-plugins
PKG@@mingw-w64-x86_64-frei0r-plugins: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-frei0r-plugins'

override VERSION_OF_mingw-w64-x86_64-fribidi := 1.0.18-1
.PHONY: PKG@@mingw-w64-x86_64-fribidi
PKG@@mingw-w64-x86_64-fribidi: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-fribidi'

override VERSION_OF_mingw-w64-x86_64-fxc2 := r19.807d26f-1
.PHONY: PKG@@mingw-w64-x86_64-fxc2 PKG@@mingw-w64-x86_64-fxc2-git
PKG@@mingw-w64-x86_64-fxc2 PKG@@mingw-w64-x86_64-fxc2-git: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-fxc2'

override VERSION_OF_mingw-w64-x86_64-gavl := 1.4.0-3
.PHONY: PKG@@mingw-w64-x86_64-gavl
PKG@@mingw-w64-x86_64-gavl: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-gavl'

override VERSION_OF_mingw-w64-x86_64-gc := 8.2.12-1
.PHONY: PKG@@mingw-w64-x86_64-gc
PKG@@mingw-w64-x86_64-gc: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libatomic_ops PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-gc'

override VERSION_OF_mingw-w64-x86_64-gcab := 1.6-3
.PHONY: PKG@@mingw-w64-x86_64-gcab
PKG@@mingw-w64-x86_64-gcab: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-gcab'

override VERSION_OF_mingw-w64-x86_64-gcc := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-cc PKG@@mingw-w64-x86_64-gcc
PKG@@mingw-w64-x86_64-cc PKG@@mingw-w64-x86_64-gcc: $(if $(__deps),PKG@@mingw-w64-x86_64-binutils PKG@@mingw-w64-x86_64-crt PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-headers PKG@@mingw-w64-x86_64-isl PKG@@mingw-w64-x86_64-libgcc PKG@@mingw-w64-x86_64-libstdc++ PKG@@mingw-w64-x86_64-mpc PKG@@mingw-w64-x86_64-mpfr PKG@@mingw-w64-x86_64-windows-default-manifest PKG@@mingw-w64-x86_64-winpthreads PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-gcc'

override VERSION_OF_mingw-w64-x86_64-gcc-ada := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-gcc-ada
PKG@@mingw-w64-x86_64-gcc-ada: $(if $(__deps),PKG@@mingw-w64-x86_64-gcc PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-isl PKG@@mingw-w64-x86_64-libgcc PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-mpc PKG@@mingw-w64-x86_64-mpfr PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-gcc-ada'

override VERSION_OF_mingw-w64-x86_64-gcc-fortran := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-fc PKG@@mingw-w64-x86_64-gcc-fortran
PKG@@mingw-w64-x86_64-fc PKG@@mingw-w64-x86_64-gcc-fortran: $(if $(__deps),PKG@@mingw-w64-x86_64-gcc PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-isl PKG@@mingw-w64-x86_64-libgfortran PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-mpc PKG@@mingw-w64-x86_64-mpfr PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-gcc-fortran'

override VERSION_OF_mingw-w64-x86_64-gcc-libs := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-gcc-libs
PKG@@mingw-w64-x86_64-gcc-libs: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libatomic PKG@@mingw-w64-x86_64-libgomp PKG@@mingw-w64-x86_64-libquadmath) ; @echo 'PKG@@mingw-w64-x86_64-gcc-libs'

override VERSION_OF_mingw-w64-x86_64-gcc-objc := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-gcc-objc
PKG@@mingw-w64-x86_64-gcc-objc: $(if $(__deps),PKG@@mingw-w64-x86_64-gcc PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-isl PKG@@mingw-w64-x86_64-libobjc PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-mpc PKG@@mingw-w64-x86_64-mpfr PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-gcc-objc'

override VERSION_OF_mingw-w64-x86_64-gcc-plugin := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-gcc-plugin
PKG@@mingw-w64-x86_64-gcc-plugin: $(if $(__deps),PKG@@mingw-w64-x86_64-gcc) ; @echo 'PKG@@mingw-w64-x86_64-gcc-plugin'

override VERSION_OF_mingw-w64-x86_64-gdal := 3.13.3-5
.PHONY: PKG@@mingw-w64-x86_64-gdal
PKG@@mingw-w64-x86_64-gdal: $(if $(__deps),PKG@@mingw-w64-x86_64-armadillo PKG@@mingw-w64-x86_64-arrow PKG@@mingw-w64-x86_64-blosc PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-cfitsio PKG@@mingw-w64-x86_64-crypto++ PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-geos PKG@@mingw-w64-x86_64-giflib PKG@@mingw-w64-x86_64-hdf4 PKG@@mingw-w64-x86_64-hdf5 PKG@@mingw-w64-x86_64-imath PKG@@mingw-w64-x86_64-json-c PKG@@mingw-w64-x86_64-lerc PKG@@mingw-w64-x86_64-libaec PKG@@mingw-w64-x86_64-libarchive PKG@@mingw-w64-x86_64-libdeflate PKG@@mingw-w64-x86_64-libfreexl PKG@@mingw-w64-x86_64-libgeotiff PKG@@mingw-w64-x86_64-libheif PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libjxl PKG@@mingw-w64-x86_64-libkml PKG@@mingw-w64-x86_64-libmariadbclient PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libspatialite PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-lz4 PKG@@mingw-w64-x86_64-netcdf PKG@@mingw-w64-x86_64-opencl-icd PKG@@mingw-w64-x86_64-openexr PKG@@mingw-w64-x86_64-openjpeg2 PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-pcre2 PKG@@mingw-w64-x86_64-podofo PKG@@mingw-w64-x86_64-poppler PKG@@mingw-w64-x86_64-postgresql PKG@@mingw-w64-x86_64-proj PKG@@mingw-w64-x86_64-qhull PKG@@mingw-w64-x86_64-sfcgal PKG@@mingw-w64-x86_64-sqlite PKG@@mingw-w64-x86_64-xerces-c PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-gdal'

override VERSION_OF_mingw-w64-x86_64-gdb := 18.1-3
.PHONY: PKG@@mingw-w64-x86_64-gdb
PKG@@mingw-w64-x86_64-gdb: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-mpfr PKG@@mingw-w64-x86_64-ncurses PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-readline PKG@@mingw-w64-x86_64-xxhash PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-gdb'

override VERSION_OF_mingw-w64-x86_64-gdb-multiarch := 18.1-3
.PHONY: PKG@@mingw-w64-x86_64-gdb-multiarch
PKG@@mingw-w64-x86_64-gdb-multiarch: $(if $(__deps),PKG@@mingw-w64-x86_64-gdb) ; @echo 'PKG@@mingw-w64-x86_64-gdb-multiarch'

override VERSION_OF_mingw-w64-x86_64-gdbm := 1.26-1
.PHONY: PKG@@mingw-w64-x86_64-gdbm
PKG@@mingw-w64-x86_64-gdbm: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-readline) ; @echo 'PKG@@mingw-w64-x86_64-gdbm'

override VERSION_OF_mingw-w64-x86_64-gdk-pixbuf2 := 2.44.8-1
.PHONY: PKG@@mingw-w64-x86_64-gdk-pixbuf2
PKG@@mingw-w64-x86_64-gdk-pixbuf2: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff) ; @echo 'PKG@@mingw-w64-x86_64-gdk-pixbuf2'

override VERSION_OF_mingw-w64-x86_64-gdk-pixbuf2-docs := 2.44.8-1
.PHONY: PKG@@mingw-w64-x86_64-gdk-pixbuf2-docs
PKG@@mingw-w64-x86_64-gdk-pixbuf2-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-gdk-pixbuf2-docs'

override VERSION_OF_mingw-w64-x86_64-gdl := 3.40.0-6
.PHONY: PKG@@mingw-w64-x86_64-gdl
PKG@@mingw-w64-x86_64-gdl: $(if $(__deps),PKG@@mingw-w64-x86_64-gtk3 PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-gdl'

override VERSION_OF_mingw-w64-x86_64-geocode-glib := 3.26.4-3
.PHONY: PKG@@mingw-w64-x86_64-geocode-glib
PKG@@mingw-w64-x86_64-geocode-glib: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gtk-update-icon-cache PKG@@mingw-w64-x86_64-json-glib PKG@@mingw-w64-x86_64-libsoup3) ; @echo 'PKG@@mingw-w64-x86_64-geocode-glib'

override VERSION_OF_mingw-w64-x86_64-geographiclib := 2.7-1
.PHONY: PKG@@mingw-w64-x86_64-geographiclib
PKG@@mingw-w64-x86_64-geographiclib: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-geographiclib'

override VERSION_OF_mingw-w64-x86_64-geoip := 1.6.12-3
.PHONY: PKG@@mingw-w64-x86_64-geoip
PKG@@mingw-w64-x86_64-geoip: $(if $(__deps),PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-geoip'

override VERSION_OF_mingw-w64-x86_64-geos := 3.15.0-1
.PHONY: PKG@@mingw-w64-x86_64-geos
PKG@@mingw-w64-x86_64-geos: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-geos'

override VERSION_OF_mingw-w64-x86_64-gettext-libtextstyle := 1.0-1
.PHONY: PKG@@mingw-w64-x86_64-gettext-libtextstyle
PKG@@mingw-w64-x86_64-gettext-libtextstyle: $(if $(__deps),PKG@@mingw-w64-x86_64-libiconv) ; @echo 'PKG@@mingw-w64-x86_64-gettext-libtextstyle'

override VERSION_OF_mingw-w64-x86_64-gettext-runtime := 1.0-1
.PHONY: PKG@@mingw-w64-x86_64-gettext-runtime
PKG@@mingw-w64-x86_64-gettext-runtime: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libiconv) ; @echo 'PKG@@mingw-w64-x86_64-gettext-runtime'

override VERSION_OF_mingw-w64-x86_64-gettext-tools := 1.0-1
.PHONY: PKG@@mingw-w64-x86_64-gettext PKG@@mingw-w64-x86_64-gettext-tools
PKG@@mingw-w64-x86_64-gettext PKG@@mingw-w64-x86_64-gettext-tools: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gettext-libtextstyle PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-libiconv) ; @echo 'PKG@@mingw-w64-x86_64-gettext-tools'

override VERSION_OF_mingw-w64-x86_64-gf2x := 1.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-gf2x
PKG@@mingw-w64-x86_64-gf2x: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-gf2x'

override VERSION_OF_mingw-w64-x86_64-gflags := 2.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-gflags
PKG@@mingw-w64-x86_64-gflags: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-gflags'

override VERSION_OF_mingw-w64-x86_64-ggml := 0.29.0-1
.PHONY: PKG@@mingw-w64-x86_64-ggml
PKG@@mingw-w64-x86_64-ggml: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-openblas PKG@@mingw-w64-x86_64-opencl-icd PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-vulkan-loader) ; @echo 'PKG@@mingw-w64-x86_64-ggml'

override VERSION_OF_mingw-w64-x86_64-ghostscript := 10.08.0-2
.PHONY: PKG@@mingw-w64-x86_64-ghostscript
PKG@@mingw-w64-x86_64-ghostscript: $(if $(__deps),PKG@@mingw-w64-x86_64-dbus PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-fontconfig PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-gdk-pixbuf2 PKG@@mingw-w64-x86_64-jbig2dec PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libidn PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libpaper PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-openjpeg2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-ghostscript'

override VERSION_OF_mingw-w64-x86_64-gi-docgen := 2026.1-1
.PHONY: PKG@@mingw-w64-x86_64-gi-docgen
PKG@@mingw-w64-x86_64-gi-docgen: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-jinja PKG@@mingw-w64-x86_64-python-markdown PKG@@mingw-w64-x86_64-python-markupsafe PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-pygments PKG@@mingw-w64-x86_64-python-typogrify) ; @echo 'PKG@@mingw-w64-x86_64-gi-docgen'

override VERSION_OF_mingw-w64-x86_64-giflib := 6.1.3-1
.PHONY: PKG@@mingw-w64-x86_64-giflib
PKG@@mingw-w64-x86_64-giflib: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-giflib'

override VERSION_OF_mingw-w64-x86_64-git := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git
PKG@@mingw-w64-x86_64-git: $(if $(__deps),PKG@@mingw-w64-x86_64-ca-certificates PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-pcre2 PKG@@nano) ; @echo 'PKG@@mingw-w64-x86_64-git'

override VERSION_OF_mingw-w64-x86_64-git-archimport := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git-archimport
PKG@@mingw-w64-x86_64-git-archimport: $(if $(__deps),PKG@@mingw-w64-x86_64-git PKG@@mingw-w64-x86_64-git-perl) ; @echo 'PKG@@mingw-w64-x86_64-git-archimport'

override VERSION_OF_mingw-w64-x86_64-git-credential-wincred := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git-credential-wincred
PKG@@mingw-w64-x86_64-git-credential-wincred: $(if $(__deps),PKG@@mingw-w64-x86_64-git) ; @echo 'PKG@@mingw-w64-x86_64-git-credential-wincred'

override VERSION_OF_mingw-w64-x86_64-git-cvs := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git-cvs
PKG@@mingw-w64-x86_64-git-cvs: $(if $(__deps),PKG@@mingw-w64-x86_64-git PKG@@mingw-w64-x86_64-git-perl PKG@@perl-dbi) ; @echo 'PKG@@mingw-w64-x86_64-git-cvs'

override VERSION_OF_mingw-w64-x86_64-git-doc-html := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git-doc-html
PKG@@mingw-w64-x86_64-git-doc-html: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-git-doc-html'

override VERSION_OF_mingw-w64-x86_64-git-doc-man := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git-doc-man
PKG@@mingw-w64-x86_64-git-doc-man: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-git-doc-man'

override VERSION_OF_mingw-w64-x86_64-git-for-windows-addons := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git-for-windows-addons
PKG@@mingw-w64-x86_64-git-for-windows-addons: $(if $(__deps),PKG@@mingw-w64-x86_64-git PKG@@mingw-w64-x86_64-git-credential-wincred PKG@@mingw-w64-x86_64-git-gui PKG@@mingw-w64-x86_64-git-send-email PKG@@mingw-w64-x86_64-git-subtree PKG@@mingw-w64-x86_64-gitk) ; @echo 'PKG@@mingw-w64-x86_64-git-for-windows-addons'

override VERSION_OF_mingw-w64-x86_64-git-gui := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git-gui
PKG@@mingw-w64-x86_64-git-gui: $(if $(__deps),PKG@@mingw-w64-x86_64-git PKG@@mingw-w64-x86_64-tcl PKG@@mingw-w64-x86_64-tk) ; @echo 'PKG@@mingw-w64-x86_64-git-gui'

override VERSION_OF_mingw-w64-x86_64-git-lfs := 3.8.1-1
.PHONY: PKG@@mingw-w64-x86_64-git-lfs
PKG@@mingw-w64-x86_64-git-lfs: $(if $(__deps),PKG@@mingw-w64-x86_64-git) ; @echo 'PKG@@mingw-w64-x86_64-git-lfs'

override VERSION_OF_mingw-w64-x86_64-git-p4 := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git-p4
PKG@@mingw-w64-x86_64-git-p4: $(if $(__deps),PKG@@mingw-w64-x86_64-git PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-git-p4'

override VERSION_OF_mingw-w64-x86_64-git-perl := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git-perl
PKG@@mingw-w64-x86_64-git-perl: $(if $(__deps),PKG@@mingw-w64-x86_64-git PKG@@perl PKG@@perl-error PKG@@perl-libwww PKG@@perl-termreadkey) ; @echo 'PKG@@mingw-w64-x86_64-git-perl'

override VERSION_OF_mingw-w64-x86_64-git-send-email := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git-send-email
PKG@@mingw-w64-x86_64-git-send-email: $(if $(__deps),PKG@@mingw-w64-x86_64-git PKG@@mingw-w64-x86_64-git-perl PKG@@perl-authen-sasl PKG@@perl-mime-tools PKG@@perl-net-smtp-ssl) ; @echo 'PKG@@mingw-w64-x86_64-git-send-email'

override VERSION_OF_mingw-w64-x86_64-git-subtree := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git-subtree
PKG@@mingw-w64-x86_64-git-subtree: $(if $(__deps),PKG@@mingw-w64-x86_64-git) ; @echo 'PKG@@mingw-w64-x86_64-git-subtree'

override VERSION_OF_mingw-w64-x86_64-git-svn := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-git-svn
PKG@@mingw-w64-x86_64-git-svn: $(if $(__deps),PKG@@mingw-w64-x86_64-git PKG@@mingw-w64-x86_64-git-perl PKG@@subversion) ; @echo 'PKG@@mingw-w64-x86_64-git-svn'

override VERSION_OF_mingw-w64-x86_64-gitk := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-gitk
PKG@@mingw-w64-x86_64-gitk: $(if $(__deps),PKG@@mingw-w64-x86_64-git PKG@@mingw-w64-x86_64-tcl PKG@@mingw-w64-x86_64-tk) ; @echo 'PKG@@mingw-w64-x86_64-gitk'

override VERSION_OF_mingw-w64-x86_64-gitweb := 2.56.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-gitweb
PKG@@mingw-w64-x86_64-gitweb: $(if $(__deps),PKG@@mingw-w64-x86_64-git PKG@@perl) ; @echo 'PKG@@mingw-w64-x86_64-gitweb'

override VERSION_OF_mingw-w64-x86_64-gl2ps := 1.4.3-1
.PHONY: PKG@@mingw-w64-x86_64-gl2ps
PKG@@mingw-w64-x86_64-gl2ps: $(if $(__deps),PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-gl2ps'

override VERSION_OF_mingw-w64-x86_64-glbinding := 3.5.0-2
.PHONY: PKG@@mingw-w64-x86_64-glbinding
PKG@@mingw-w64-x86_64-glbinding: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-glbinding'

override VERSION_OF_mingw-w64-x86_64-gles-headers := 3.2.r1065.7fc154c-1
.PHONY: PKG@@mingw-w64-x86_64-gles-headers
PKG@@mingw-w64-x86_64-gles-headers: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-gles-headers'

override VERSION_OF_mingw-w64-x86_64-glew := 2.3.1-4
.PHONY: PKG@@mingw-w64-x86_64-glew
PKG@@mingw-w64-x86_64-glew: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-glew'

override VERSION_OF_mingw-w64-x86_64-glfw := 3.5.1-1
.PHONY: PKG@@mingw-w64-x86_64-glfw
PKG@@mingw-w64-x86_64-glfw: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-glfw'

override VERSION_OF_mingw-w64-x86_64-glfw-docs := 3.5.1-1
.PHONY: PKG@@mingw-w64-x86_64-glfw-docs
PKG@@mingw-w64-x86_64-glfw-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-glfw-docs'

override VERSION_OF_mingw-w64-x86_64-glib-networking := 2.90.1-1
.PHONY: PKG@@mingw-w64-x86_64-glib-networking
PKG@@mingw-w64-x86_64-glib-networking: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gnutls PKG@@mingw-w64-x86_64-gsettings-desktop-schemas PKG@@mingw-w64-x86_64-libproxy PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-glib-networking'

override VERSION_OF_mingw-w64-x86_64-glib2 := 2.90.1-1
.PHONY: PKG@@mingw-w64-x86_64-glib2
PKG@@mingw-w64-x86_64-glib2: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-libffi PKG@@mingw-w64-x86_64-pcre2 PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-glib2'

override VERSION_OF_mingw-w64-x86_64-glm := 1.0.3-1
.PHONY: PKG@@mingw-w64-x86_64-glm
PKG@@mingw-w64-x86_64-glm: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-glm'

override VERSION_OF_mingw-w64-x86_64-global := 6.7-1
.PHONY: PKG@@mingw-w64-x86_64-global
PKG@@mingw-w64-x86_64-global: $(if $(__deps),PKG@@mingw-w64-x86_64-libltdl PKG@@mingw-w64-x86_64-libsystre PKG@@mingw-w64-x86_64-ncurses) ; @echo 'PKG@@mingw-w64-x86_64-global'

override VERSION_OF_mingw-w64-x86_64-globjects := 1.1.0.r183.ged3f8eeb-2
.PHONY: PKG@@mingw-w64-x86_64-globjects
PKG@@mingw-w64-x86_64-globjects: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-glbinding PKG@@mingw-w64-x86_64-glm) ; @echo 'PKG@@mingw-w64-x86_64-globjects'

override VERSION_OF_mingw-w64-x86_64-glog := 0.7.1-10
.PHONY: PKG@@mingw-w64-x86_64-glog
PKG@@mingw-w64-x86_64-glog: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gflags PKG@@mingw-w64-x86_64-libunwind PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-glog'

override VERSION_OF_mingw-w64-x86_64-glpk := 5.0-8
.PHONY: PKG@@mingw-w64-x86_64-glpk
PKG@@mingw-w64-x86_64-glpk: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-suitesparse PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-glpk'

override VERSION_OF_mingw-w64-x86_64-glsl-optimizer := 1~r66913.d78c3d2f24-1
.PHONY: PKG@@mingw-w64-x86_64-glsl-optimizer PKG@@mingw-w64-x86_64-glsl-optimizer-git
PKG@@mingw-w64-x86_64-glsl-optimizer PKG@@mingw-w64-x86_64-glsl-optimizer-git: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-glsl-optimizer'

override VERSION_OF_mingw-w64-x86_64-glslang := 16.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-glslang
PKG@@mingw-w64-x86_64-glslang: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-spirv-tools) ; @echo 'PKG@@mingw-w64-x86_64-glslang'

override VERSION_OF_mingw-w64-x86_64-gmp := 6.3.0-2
.PHONY: PKG@@mingw-w64-x86_64-gmp
PKG@@mingw-w64-x86_64-gmp: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-gmp'

override VERSION_OF_mingw-w64-x86_64-gn := 0.2540.150a9d6b-1
.PHONY: PKG@@mingw-w64-x86_64-gn
PKG@@mingw-w64-x86_64-gn: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-gn'

override VERSION_OF_mingw-w64-x86_64-gnome-common := 3.18.0-2
.PHONY: PKG@@mingw-w64-x86_64-gnome-common
PKG@@mingw-w64-x86_64-gnome-common: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-gnome-common'

override VERSION_OF_mingw-w64-x86_64-gnupg := 2.4.9-1
.PHONY: PKG@@mingw-w64-x86_64-gnupg
PKG@@mingw-w64-x86_64-gnupg: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-gnutls PKG@@mingw-w64-x86_64-libassuan PKG@@mingw-w64-x86_64-libgcrypt PKG@@mingw-w64-x86_64-libksba PKG@@mingw-w64-x86_64-libsystre PKG@@mingw-w64-x86_64-npth PKG@@mingw-w64-x86_64-readline PKG@@mingw-w64-x86_64-sqlite3 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-gnupg'

override VERSION_OF_mingw-w64-x86_64-gnustep-back := 0.32.0-6
.PHONY: PKG@@mingw-w64-x86_64-gnustep-back
PKG@@mingw-w64-x86_64-gnustep-back: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-gnustep-gui) ; @echo 'PKG@@mingw-w64-x86_64-gnustep-back'

override VERSION_OF_mingw-w64-x86_64-gnustep-base := 1.31.1-10
.PHONY: PKG@@mingw-w64-x86_64-gnustep-base
PKG@@mingw-w64-x86_64-gnustep-base: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gnutls PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-libffi PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libobjc2 PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-libxslt PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-gnustep-base'

override VERSION_OF_mingw-w64-x86_64-gnustep-gui := 0.32.0-7
.PHONY: PKG@@mingw-w64-x86_64-gnustep-gui
PKG@@mingw-w64-x86_64-gnustep-gui: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-giflib PKG@@mingw-w64-x86_64-gnustep-base PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-libffi PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff) ; @echo 'PKG@@mingw-w64-x86_64-gnustep-gui'

override VERSION_OF_mingw-w64-x86_64-gnustep-make := 2.9.3-2
.PHONY: PKG@@mingw-w64-x86_64-gnustep-make
PKG@@mingw-w64-x86_64-gnustep-make: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-gnustep-make'

override VERSION_OF_mingw-w64-x86_64-gnutls := 3.8.13-3
.PHONY: PKG@@mingw-w64-x86_64-gnutls
PKG@@mingw-w64-x86_64-gnutls: $(if $(__deps),PKG@@mingw-w64-x86_64-brotli PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-libidn2 PKG@@mingw-w64-x86_64-libtasn1 PKG@@mingw-w64-x86_64-libunistring PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-nettle PKG@@mingw-w64-x86_64-p11-kit PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-gnutls'

override VERSION_OF_mingw-w64-x86_64-go := 1.27.3-1
.PHONY: PKG@@mingw-w64-x86_64-go
PKG@@mingw-w64-x86_64-go: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-go'

override VERSION_OF_mingw-w64-x86_64-gobject-introspection := 1.86.0-4
.PHONY: PKG@@mingw-w64-x86_64-gobject-introspection
PKG@@mingw-w64-x86_64-gobject-introspection: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gobject-introspection-runtime PKG@@mingw-w64-x86_64-libffi PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-mako PKG@@mingw-w64-x86_64-python-markdown PKG@@mingw-w64-x86_64-python-setuptools) ; @echo 'PKG@@mingw-w64-x86_64-gobject-introspection'

override VERSION_OF_mingw-w64-x86_64-gobject-introspection-runtime := 1.86.0-4
.PHONY: PKG@@mingw-w64-x86_64-gobject-introspection-runtime
PKG@@mingw-w64-x86_64-gobject-introspection-runtime: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libffi) ; @echo 'PKG@@mingw-w64-x86_64-gobject-introspection-runtime'

override VERSION_OF_mingw-w64-x86_64-gperf := 3.3-1
.PHONY: PKG@@mingw-w64-x86_64-gperf
PKG@@mingw-w64-x86_64-gperf: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-gperf'

override VERSION_OF_mingw-w64-x86_64-gputils := 1.5.2-2
.PHONY: PKG@@mingw-w64-x86_64-gputils
PKG@@mingw-w64-x86_64-gputils: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-gputils'

override VERSION_OF_mingw-w64-x86_64-graphene := 1.10.8-3
.PHONY: PKG@@mingw-w64-x86_64-graphene
PKG@@mingw-w64-x86_64-graphene: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2) ; @echo 'PKG@@mingw-w64-x86_64-graphene'

override VERSION_OF_mingw-w64-x86_64-graphite2 := 1.3.15-1
.PHONY: PKG@@mingw-w64-x86_64-graphite2
PKG@@mingw-w64-x86_64-graphite2: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-graphite2'

override VERSION_OF_mingw-w64-x86_64-graphviz := 16.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-graphviz
PKG@@mingw-w64-x86_64-graphviz: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-devil PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-freeglut PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-ghostscript PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gts PKG@@mingw-w64-x86_64-libgd PKG@@mingw-w64-x86_64-libltdl PKG@@mingw-w64-x86_64-librsvg PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-pango PKG@@mingw-w64-x86_64-poppler PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-graphviz'

override VERSION_OF_mingw-w64-x86_64-graphviz-docs := 16.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-graphviz-docs
PKG@@mingw-w64-x86_64-graphviz-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-graphviz-docs'

override VERSION_OF_mingw-w64-x86_64-grpc := 1.85.0-3
.PHONY: PKG@@mingw-w64-x86_64-grpc
PKG@@mingw-w64-x86_64-grpc: $(if $(__deps),PKG@@mingw-w64-x86_64-abseil-cpp PKG@@mingw-w64-x86_64-c-ares PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-protobuf PKG@@mingw-w64-x86_64-re2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-grpc'

override VERSION_OF_mingw-w64-x86_64-gsettings-desktop-schemas := 51.0-1
.PHONY: PKG@@mingw-w64-x86_64-gsettings-desktop-schemas
PKG@@mingw-w64-x86_64-gsettings-desktop-schemas: $(if $(__deps),PKG@@mingw-w64-x86_64-adobe-source-code-pro-fonts PKG@@mingw-w64-x86_64-cantarell-fonts PKG@@mingw-w64-x86_64-glib2) ; @echo 'PKG@@mingw-w64-x86_64-gsettings-desktop-schemas'

override VERSION_OF_mingw-w64-x86_64-gsfonts := 20200910-3
.PHONY: PKG@@mingw-w64-x86_64-gsfonts
PKG@@mingw-w64-x86_64-gsfonts: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-gsfonts'

override VERSION_OF_mingw-w64-x86_64-gsl := 2.8-1
.PHONY: PKG@@mingw-w64-x86_64-gsl
PKG@@mingw-w64-x86_64-gsl: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-gsl'

override VERSION_OF_mingw-w64-x86_64-gsm := 1.0.24-1
.PHONY: PKG@@mingw-w64-x86_64-gsm
PKG@@mingw-w64-x86_64-gsm: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-gsm'

override VERSION_OF_mingw-w64-x86_64-gss := 1.0.4-2
.PHONY: PKG@@mingw-w64-x86_64-gss
PKG@@mingw-w64-x86_64-gss: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-shishi) ; @echo 'PKG@@mingw-w64-x86_64-gss'

override VERSION_OF_mingw-w64-x86_64-gst-plugins-base := 1.28.9-1
.PHONY: PKG@@mingw-w64-x86_64-gst-plugins-base
PKG@@mingw-w64-x86_64-gst-plugins-base: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-graphene PKG@@mingw-w64-x86_64-gstreamer PKG@@mingw-w64-x86_64-iso-codes PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libogg PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtheora PKG@@mingw-w64-x86_64-libvorbis PKG@@mingw-w64-x86_64-opus PKG@@mingw-w64-x86_64-orc PKG@@mingw-w64-x86_64-pango PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-gst-plugins-base'

override VERSION_OF_mingw-w64-x86_64-gstreamer := 1.28.9-1
.PHONY: PKG@@mingw-w64-x86_64-gstreamer
PKG@@mingw-w64-x86_64-gstreamer: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-gstreamer'

override VERSION_OF_mingw-w64-x86_64-gtest := 1.18.0-1
.PHONY: PKG@@mingw-w64-x86_64-gtest
PKG@@mingw-w64-x86_64-gtest: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-gtest'

override VERSION_OF_mingw-w64-x86_64-gtk-doc := 1.37.0-1
.PHONY: PKG@@mingw-w64-x86_64-gtk-doc
PKG@@mingw-w64-x86_64-gtk-doc: $(if $(__deps),PKG@@mingw-w64-x86_64-docbook-xml PKG@@mingw-w64-x86_64-docbook-xsl PKG@@mingw-w64-x86_64-libxslt PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-pygments) ; @echo 'PKG@@mingw-w64-x86_64-gtk-doc'

override VERSION_OF_mingw-w64-x86_64-gtk-update-icon-cache := 3.24.52-1
.PHONY: PKG@@mingw-w64-x86_64-gtk-update-icon-cache
PKG@@mingw-w64-x86_64-gtk-update-icon-cache: $(if $(__deps),PKG@@mingw-w64-x86_64-gdk-pixbuf2 PKG@@mingw-w64-x86_64-hicolor-icon-theme PKG@@mingw-w64-x86_64-librsvg) ; @echo 'PKG@@mingw-w64-x86_64-gtk-update-icon-cache'

override VERSION_OF_mingw-w64-x86_64-gtk3 := 3.24.52-1
.PHONY: PKG@@mingw-w64-x86_64-gtk3
PKG@@mingw-w64-x86_64-gtk3: $(if $(__deps),PKG@@mingw-w64-x86_64-adwaita-icon-theme PKG@@mingw-w64-x86_64-atk PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gdk-pixbuf2 PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gtk-update-icon-cache PKG@@mingw-w64-x86_64-json-glib PKG@@mingw-w64-x86_64-libepoxy PKG@@mingw-w64-x86_64-pango PKG@@mingw-w64-x86_64-shared-mime-info) ; @echo 'PKG@@mingw-w64-x86_64-gtk3'

override VERSION_OF_mingw-w64-x86_64-gts := 0.7.6-3
.PHONY: PKG@@mingw-w64-x86_64-gts
PKG@@mingw-w64-x86_64-gts: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2) ; @echo 'PKG@@mingw-w64-x86_64-gts'

override VERSION_OF_mingw-w64-x86_64-gumbo-parser := 0.14.1-1
.PHONY: PKG@@mingw-w64-x86_64-gumbo-parser
PKG@@mingw-w64-x86_64-gumbo-parser: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-gumbo-parser'

override VERSION_OF_mingw-w64-x86_64-harfbuzz := 15.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-harfbuzz
PKG@@mingw-w64-x86_64-harfbuzz: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-graphite2 PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-harfbuzz'

override VERSION_OF_mingw-w64-x86_64-harfbuzz-cairo := 15.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-harfbuzz-cairo
PKG@@mingw-w64-x86_64-harfbuzz-cairo: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-harfbuzz) ; @echo 'PKG@@mingw-w64-x86_64-harfbuzz-cairo'

override VERSION_OF_mingw-w64-x86_64-harfbuzz-docs := 15.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-harfbuzz-docs
PKG@@mingw-w64-x86_64-harfbuzz-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-harfbuzz-docs'

override VERSION_OF_mingw-w64-x86_64-harfbuzz-icu := 15.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-harfbuzz-icu
PKG@@mingw-w64-x86_64-harfbuzz-icu: $(if $(__deps),PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-icu) ; @echo 'PKG@@mingw-w64-x86_64-harfbuzz-icu'

override VERSION_OF_mingw-w64-x86_64-harfbuzz-utils := 15.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-harfbuzz-utils
PKG@@mingw-w64-x86_64-harfbuzz-utils: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-chafa PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-harfbuzz-cairo) ; @echo 'PKG@@mingw-w64-x86_64-harfbuzz-utils'

override VERSION_OF_mingw-w64-x86_64-hdf4 := 4.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-hdf4
PKG@@mingw-w64-x86_64-hdf4: $(if $(__deps),PKG@@mingw-w64-x86_64-gcc-libgfortran PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-hdf4'

override VERSION_OF_mingw-w64-x86_64-hdf5 := 2.2.0-4
.PHONY: PKG@@mingw-w64-x86_64-hdf5
PKG@@mingw-w64-x86_64-hdf5: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-libaec PKG@@mingw-w64-x86_64-zlib-ng) ; @echo 'PKG@@mingw-w64-x86_64-hdf5'

override VERSION_OF_mingw-w64-x86_64-headers := 14.0.0.r598.ge3d18511f-1
.PHONY: PKG@@mingw-w64-x86_64-headers PKG@@mingw-w64-x86_64-headers-git
PKG@@mingw-w64-x86_64-headers PKG@@mingw-w64-x86_64-headers-git: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-headers'

override VERSION_OF_mingw-w64-x86_64-hererocks := 0.25.1-4
.PHONY: PKG@@mingw-w64-x86_64-hererocks PKG@@mingw-w64-x86_64-python-hererocks
PKG@@mingw-w64-x86_64-hererocks PKG@@mingw-w64-x86_64-python-hererocks: $(if $(__deps),PKG@@mingw-w64-x86_64-git PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-unzip) ; @echo 'PKG@@mingw-w64-x86_64-hererocks'

override VERSION_OF_mingw-w64-x86_64-hicolor-icon-theme := 0.18-1
.PHONY: PKG@@mingw-w64-x86_64-hicolor-icon-theme
PKG@@mingw-w64-x86_64-hicolor-icon-theme: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-hicolor-icon-theme'

override VERSION_OF_mingw-w64-x86_64-highway := 1.4.0-3
.PHONY: PKG@@mingw-w64-x86_64-highway
PKG@@mingw-w64-x86_64-highway: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-highway'

override VERSION_OF_mingw-w64-x86_64-hiredis := 1.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-hiredis
PKG@@mingw-w64-x86_64-hiredis: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-hiredis'

override VERSION_OF_mingw-w64-x86_64-hlsl2glsl := 1~r848.14dd189-1
.PHONY: PKG@@mingw-w64-x86_64-hlsl2glsl PKG@@mingw-w64-x86_64-hlsl2glsl-git
PKG@@mingw-w64-x86_64-hlsl2glsl PKG@@mingw-w64-x86_64-hlsl2glsl-git: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-hlsl2glsl'

override VERSION_OF_mingw-w64-x86_64-htslib := 1.25-1
.PHONY: PKG@@mingw-w64-x86_64-htslib
PKG@@mingw-w64-x86_64-htslib: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-libdeflate PKG@@mingw-w64-x86_64-libsystre PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-htslib'

override VERSION_OF_mingw-w64-x86_64-http-parser := 2.9.4-3
.PHONY: PKG@@mingw-w64-x86_64-http-parser
PKG@@mingw-w64-x86_64-http-parser: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-http-parser'

override VERSION_OF_mingw-w64-x86_64-hunspell := 1.7.5-2
.PHONY: PKG@@mingw-w64-x86_64-hunspell
PKG@@mingw-w64-x86_64-hunspell: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-readline) ; @echo 'PKG@@mingw-w64-x86_64-hunspell'

override VERSION_OF_mingw-w64-x86_64-hunspell-en := 2026.02.25-1
.PHONY: PKG@@mingw-w64-x86_64-hunspell-en
PKG@@mingw-w64-x86_64-hunspell-en: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-hunspell-en'

override VERSION_OF_mingw-w64-x86_64-hwloc := 2.15.0-1
.PHONY: PKG@@mingw-w64-x86_64-hwloc
PKG@@mingw-w64-x86_64-hwloc: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libltdl) ; @echo 'PKG@@mingw-w64-x86_64-hwloc'

override VERSION_OF_mingw-w64-x86_64-hypercorn := 0.18.0-3
.PHONY: PKG@@mingw-w64-x86_64-hypercorn
PKG@@mingw-w64-x86_64-hypercorn: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-h11 PKG@@mingw-w64-x86_64-python-h2 PKG@@mingw-w64-x86_64-python-priority PKG@@mingw-w64-x86_64-python-toml PKG@@mingw-w64-x86_64-python-typing_extensions PKG@@mingw-w64-x86_64-python-wsproto) ; @echo 'PKG@@mingw-w64-x86_64-hypercorn'

override VERSION_OF_mingw-w64-x86_64-i2pd := 3.0.0.1-1
.PHONY: PKG@@mingw-w64-x86_64-i2pd
PKG@@mingw-w64-x86_64-i2pd: $(if $(__deps),PKG@@mingw-w64-x86_64-boost-libs PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-miniupnpc PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-i2pd'

override VERSION_OF_mingw-w64-x86_64-iconv := 1.19.1-1
.PHONY: PKG@@mingw-w64-x86_64-iconv
PKG@@mingw-w64-x86_64-iconv: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-libiconv) ; @echo 'PKG@@mingw-w64-x86_64-iconv'

override VERSION_OF_mingw-w64-x86_64-icoutils := 0.32.3-3
.PHONY: PKG@@mingw-w64-x86_64-icoutils
PKG@@mingw-w64-x86_64-icoutils: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-libpng) ; @echo 'PKG@@mingw-w64-x86_64-icoutils'

override VERSION_OF_mingw-w64-x86_64-icu := 78.3-4
.PHONY: PKG@@mingw-w64-x86_64-icu
PKG@@mingw-w64-x86_64-icu: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-icu'

override VERSION_OF_mingw-w64-x86_64-iir := 1.10.0-1
.PHONY: PKG@@mingw-w64-x86_64-iir
PKG@@mingw-w64-x86_64-iir: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-iir'

override VERSION_OF_mingw-w64-x86_64-imagemagick := 7.1.2.35-1
.PHONY: PKG@@mingw-w64-x86_64-imagemagick
PKG@@mingw-w64-x86_64-imagemagick: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-djvulibre PKG@@mingw-w64-x86_64-fftw PKG@@mingw-w64-x86_64-flif PKG@@mingw-w64-x86_64-fontconfig PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gsfonts PKG@@mingw-w64-x86_64-jbigkit PKG@@mingw-w64-x86_64-lcms2 PKG@@mingw-w64-x86_64-libheif PKG@@mingw-w64-x86_64-liblqr PKG@@mingw-w64-x86_64-libltdl PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libraqm PKG@@mingw-w64-x86_64-libraw PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-libwmf PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-openjpeg2 PKG@@mingw-w64-x86_64-ttf-dejavu PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-imagemagick'

override VERSION_OF_mingw-w64-x86_64-imagemagick-docs := 7.1.2.35-1
.PHONY: PKG@@mingw-w64-x86_64-imagemagick-docs
PKG@@mingw-w64-x86_64-imagemagick-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-imagemagick-docs'

override VERSION_OF_mingw-w64-x86_64-imath := 3.2.3-3
.PHONY: PKG@@mingw-w64-x86_64-imath
PKG@@mingw-w64-x86_64-imath: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-imath'

override VERSION_OF_mingw-w64-x86_64-iml := 1.0.5-1
.PHONY: PKG@@mingw-w64-x86_64-iml
PKG@@mingw-w64-x86_64-iml: $(if $(__deps),PKG@@mingw-w64-x86_64-cblas PKG@@mingw-w64-x86_64-gmp) ; @echo 'PKG@@mingw-w64-x86_64-iml'

override VERSION_OF_mingw-w64-x86_64-include-what-you-use := 0.27-1
.PHONY: PKG@@mingw-w64-x86_64-include-what-you-use
PKG@@mingw-w64-x86_64-include-what-you-use: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-include-what-you-use'

override VERSION_OF_mingw-w64-x86_64-indent := 2.2.13-1
.PHONY: PKG@@mingw-w64-x86_64-indent
PKG@@mingw-w64-x86_64-indent: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime) ; @echo 'PKG@@mingw-w64-x86_64-indent'

override VERSION_OF_mingw-w64-x86_64-inja := 3.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-inja
PKG@@mingw-w64-x86_64-inja: $(if $(__deps),PKG@@mingw-w64-x86_64-nlohmann-json) ; @echo 'PKG@@mingw-w64-x86_64-inja'

override VERSION_OF_mingw-w64-x86_64-isa-l := 2.31.1-1
.PHONY: PKG@@mingw-w64-x86_64-isa-l
PKG@@mingw-w64-x86_64-isa-l: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-isa-l'

override VERSION_OF_mingw-w64-x86_64-isl := 0.28-1
.PHONY: PKG@@mingw-w64-x86_64-isl
PKG@@mingw-w64-x86_64-isl: $(if $(__deps),PKG@@mingw-w64-x86_64-gmp) ; @echo 'PKG@@mingw-w64-x86_64-isl'

override VERSION_OF_mingw-w64-x86_64-iso-codes := 4.20.1-1
.PHONY: PKG@@mingw-w64-x86_64-iso-codes
PKG@@mingw-w64-x86_64-iso-codes: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-iso-codes'

override VERSION_OF_mingw-w64-x86_64-itstool := 2.0.7-3
.PHONY: PKG@@mingw-w64-x86_64-itstool
PKG@@mingw-w64-x86_64-itstool: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-lxml) ; @echo 'PKG@@mingw-w64-x86_64-itstool'

override VERSION_OF_mingw-w64-x86_64-jack2 := 1.9.22-4
.PHONY: PKG@@mingw-w64-x86_64-jack2
PKG@@mingw-w64-x86_64-jack2: $(if $(__deps),PKG@@mingw-w64-x86_64-db PKG@@mingw-w64-x86_64-libsamplerate PKG@@mingw-w64-x86_64-libsndfile PKG@@mingw-w64-x86_64-libsystre PKG@@mingw-w64-x86_64-opus PKG@@mingw-w64-x86_64-portaudio PKG@@mingw-w64-x86_64-readline) ; @echo 'PKG@@mingw-w64-x86_64-jack2'

override VERSION_OF_mingw-w64-x86_64-jansson := 2.15.1-1
.PHONY: PKG@@mingw-w64-x86_64-jansson
PKG@@mingw-w64-x86_64-jansson: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-jansson'

override VERSION_OF_mingw-w64-x86_64-jasper := 4.2.9-3
.PHONY: PKG@@mingw-w64-x86_64-jasper
PKG@@mingw-w64-x86_64-jasper: $(if $(__deps),PKG@@mingw-w64-x86_64-freeglut PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-jasper'

override VERSION_OF_mingw-w64-x86_64-jasper-docs := 4.2.9-3
.PHONY: PKG@@mingw-w64-x86_64-jasper-docs
PKG@@mingw-w64-x86_64-jasper-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-jasper-docs'

override VERSION_OF_mingw-w64-x86_64-jbig2dec := 0.20-1
.PHONY: PKG@@mingw-w64-x86_64-jbig2dec
PKG@@mingw-w64-x86_64-jbig2dec: $(if $(__deps),PKG@@mingw-w64-x86_64-libpng) ; @echo 'PKG@@mingw-w64-x86_64-jbig2dec'

override VERSION_OF_mingw-w64-x86_64-jbigkit := 2.1-6
.PHONY: PKG@@mingw-w64-x86_64-jbigkit
PKG@@mingw-w64-x86_64-jbigkit: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-jbigkit'

override VERSION_OF_mingw-w64-x86_64-jemalloc := 5.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-jemalloc
PKG@@mingw-w64-x86_64-jemalloc: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-jemalloc'

override VERSION_OF_mingw-w64-x86_64-json-c := 0.19-2
.PHONY: PKG@@mingw-w64-x86_64-json-c
PKG@@mingw-w64-x86_64-json-c: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-json-c'

override VERSION_OF_mingw-w64-x86_64-json-glib := 1.10.8-2
.PHONY: PKG@@mingw-w64-x86_64-json-glib
PKG@@mingw-w64-x86_64-json-glib: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2) ; @echo 'PKG@@mingw-w64-x86_64-json-glib'

override VERSION_OF_mingw-w64-x86_64-jsoncpp := 1.9.8-1
.PHONY: PKG@@mingw-w64-x86_64-jsoncpp
PKG@@mingw-w64-x86_64-jsoncpp: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-jsoncpp'

override VERSION_OF_mingw-w64-x86_64-jsonrpc-glib := 3.44.2-2
.PHONY: PKG@@mingw-w64-x86_64-jsonrpc-glib
PKG@@mingw-w64-x86_64-jsonrpc-glib: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-json-glib) ; @echo 'PKG@@mingw-w64-x86_64-jsonrpc-glib'

override VERSION_OF_mingw-w64-x86_64-jsonrpc-glib-docs := 3.44.2-2
.PHONY: PKG@@mingw-w64-x86_64-jsonrpc-glib-docs
PKG@@mingw-w64-x86_64-jsonrpc-glib-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-jsonrpc-glib-docs'

override VERSION_OF_mingw-w64-x86_64-junitparser := 3.2.0-4
.PHONY: PKG@@mingw-w64-x86_64-junitparser
PKG@@mingw-w64-x86_64-junitparser: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-junitparser'

override VERSION_OF_mingw-w64-x86_64-jxrlib := 1.1-5
.PHONY: PKG@@mingw-w64-x86_64-jxrlib
PKG@@mingw-w64-x86_64-jxrlib: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-jxrlib'

override VERSION_OF_mingw-w64-x86_64-keystone := 0.9.2-9
.PHONY: PKG@@mingw-w64-x86_64-keystone
PKG@@mingw-w64-x86_64-keystone: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-keystone'

override VERSION_OF_mingw-w64-x86_64-kiss_fft := 131.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-kiss_fft
PKG@@mingw-w64-x86_64-kiss_fft: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-kiss_fft'

override VERSION_OF_mingw-w64-x86_64-krpc-cpp := 0.5.4-7
.PHONY: PKG@@mingw-w64-x86_64-krpc-cpp
PKG@@mingw-w64-x86_64-krpc-cpp: $(if $(__deps),PKG@@mingw-w64-x86_64-asio PKG@@mingw-w64-x86_64-protobuf) ; @echo 'PKG@@mingw-w64-x86_64-krpc-cpp'

override VERSION_OF_mingw-w64-x86_64-kseexpr := 4.0.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-kseexpr
PKG@@mingw-w64-x86_64-kseexpr: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-kseexpr'

override VERSION_OF_mingw-w64-x86_64-ktx-software := 4.4.2-1
.PHONY: PKG@@mingw-w64-x86_64-ktx-software
PKG@@mingw-w64-x86_64-ktx-software: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-opencl-icd) ; @echo 'PKG@@mingw-w64-x86_64-ktx-software'

override VERSION_OF_mingw-w64-x86_64-kvazaar := 2.3.2-1
.PHONY: PKG@@mingw-w64-x86_64-kvazaar
PKG@@mingw-w64-x86_64-kvazaar: $(if $(__deps),PKG@@mingw-w64-x86_64-crypto++ PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-kvazaar'

override VERSION_OF_mingw-w64-x86_64-l-smash := 2.14.5-4
.PHONY: PKG@@mingw-w64-x86_64-l-smash
PKG@@mingw-w64-x86_64-l-smash: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-l-smash'

override VERSION_OF_mingw-w64-x86_64-ladspa-sdk := 1.17-2
.PHONY: PKG@@mingw-w64-x86_64-ladspa-sdk
PKG@@mingw-w64-x86_64-ladspa-sdk: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-ladspa-sdk'

override VERSION_OF_mingw-w64-x86_64-lager := 0.1.3-1
.PHONY: PKG@@mingw-w64-x86_64-lager
PKG@@mingw-w64-x86_64-lager: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-lager'

override VERSION_OF_mingw-w64-x86_64-lame := 4.0-1
.PHONY: PKG@@mingw-w64-x86_64-lame
PKG@@mingw-w64-x86_64-lame: $(if $(__deps),PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-mpg123) ; @echo 'PKG@@mingw-w64-x86_64-lame'

override VERSION_OF_mingw-w64-x86_64-lapack := 3.12.1-2
.PHONY: PKG@@mingw-w64-x86_64-lapack
PKG@@mingw-w64-x86_64-lapack: $(if $(__deps),PKG@@mingw-w64-x86_64-blas PKG@@mingw-w64-x86_64-gcc-libgfortran) ; @echo 'PKG@@mingw-w64-x86_64-lapack'

override VERSION_OF_mingw-w64-x86_64-lapack64 := 3.12.1-2
.PHONY: PKG@@mingw-w64-x86_64-lapack64
PKG@@mingw-w64-x86_64-lapack64: $(if $(__deps),PKG@@mingw-w64-x86_64-blas64 PKG@@mingw-w64-x86_64-gcc-libgfortran) ; @echo 'PKG@@mingw-w64-x86_64-lapack64'

override VERSION_OF_mingw-w64-x86_64-lapacke := 3.12.1-2
.PHONY: PKG@@mingw-w64-x86_64-lapacke
PKG@@mingw-w64-x86_64-lapacke: $(if $(__deps),PKG@@mingw-w64-x86_64-lapack) ; @echo 'PKG@@mingw-w64-x86_64-lapacke'

override VERSION_OF_mingw-w64-x86_64-lapacke64 := 3.12.1-2
.PHONY: PKG@@mingw-w64-x86_64-lapacke64
PKG@@mingw-w64-x86_64-lapacke64: $(if $(__deps),PKG@@mingw-w64-x86_64-lapack64) ; @echo 'PKG@@mingw-w64-x86_64-lapacke64'

override VERSION_OF_mingw-w64-x86_64-lcms2 := 2.20-1
.PHONY: PKG@@mingw-w64-x86_64-lcms2
PKG@@mingw-w64-x86_64-lcms2: $(if $(__deps),PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libtiff) ; @echo 'PKG@@mingw-w64-x86_64-lcms2'

override VERSION_OF_mingw-w64-x86_64-lcov := 1.16-1
.PHONY: PKG@@mingw-w64-x86_64-lcov
PKG@@mingw-w64-x86_64-lcov: $(if $(__deps),PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-lcov'

override VERSION_OF_mingw-w64-x86_64-ldns := 1.9.2-1
.PHONY: PKG@@mingw-w64-x86_64-ldns
PKG@@mingw-w64-x86_64-ldns: $(if $(__deps),PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-ldns'

override VERSION_OF_mingw-w64-x86_64-ledger := 3.4.1-1
.PHONY: PKG@@mingw-w64-x86_64-ledger
PKG@@mingw-w64-x86_64-ledger: $(if $(__deps),PKG@@mingw-w64-x86_64-boost-libs PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-mpfr PKG@@mingw-w64-x86_64-readline) ; @echo 'PKG@@mingw-w64-x86_64-ledger'

override VERSION_OF_mingw-w64-x86_64-lensfun := 0.3.4-8
.PHONY: PKG@@mingw-w64-x86_64-lensfun
PKG@@mingw-w64-x86_64-lensfun: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libsystre) ; @echo 'PKG@@mingw-w64-x86_64-lensfun'

override VERSION_OF_mingw-w64-x86_64-leptonica := 1.87.0-2
.PHONY: PKG@@mingw-w64-x86_64-leptonica
PKG@@mingw-w64-x86_64-leptonica: $(if $(__deps),PKG@@mingw-w64-x86_64-giflib PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-openjpeg2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-leptonica'

override VERSION_OF_mingw-w64-x86_64-lerc := 4.1.1-1
.PHONY: PKG@@mingw-w64-x86_64-lerc
PKG@@mingw-w64-x86_64-lerc: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-lerc'

override VERSION_OF_mingw-w64-x86_64-level-zero-headers := 1.26.2-3
.PHONY: PKG@@mingw-w64-x86_64-level-zero-headers
PKG@@mingw-w64-x86_64-level-zero-headers: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-level-zero-headers'

override VERSION_OF_mingw-w64-x86_64-level-zero-loader := 1.26.2-3
.PHONY: PKG@@mingw-w64-x86_64-level-zero-loader
PKG@@mingw-w64-x86_64-level-zero-loader: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-level-zero-loader'

override VERSION_OF_mingw-w64-x86_64-lib2geom := 1.4-2
.PHONY: PKG@@mingw-w64-x86_64-lib2geom
PKG@@mingw-w64-x86_64-lib2geom: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-double-conversion PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gsl) ; @echo 'PKG@@mingw-w64-x86_64-lib2geom'

override VERSION_OF_mingw-w64-x86_64-lib3mf := 2.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-lib3mf
PKG@@mingw-w64-x86_64-lib3mf: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libzip PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-lib3mf'

override VERSION_OF_mingw-w64-x86_64-libaacs := 0.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-libaacs
PKG@@mingw-w64-x86_64-libaacs: $(if $(__deps),PKG@@mingw-w64-x86_64-libgcrypt) ; @echo 'PKG@@mingw-w64-x86_64-libaacs'

override VERSION_OF_mingw-w64-x86_64-libaec := 1.1.7-2
.PHONY: PKG@@mingw-w64-x86_64-libaec
PKG@@mingw-w64-x86_64-libaec: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libaec'

override VERSION_OF_mingw-w64-x86_64-libarchive := 3.8.11-1
.PHONY: PKG@@mingw-w64-x86_64-libarchive PKG@@mingw-w64-x86_64-unzip
PKG@@mingw-w64-x86_64-libarchive PKG@@mingw-w64-x86_64-unzip: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-libb2 PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-lz4 PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-pcre2 PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-libarchive'

override VERSION_OF_mingw-w64-x86_64-libaribcaption := 1.1.2-1
.PHONY: PKG@@mingw-w64-x86_64-libaribcaption
PKG@@mingw-w64-x86_64-libaribcaption: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libaribcaption'

override VERSION_OF_mingw-w64-x86_64-libart_lgpl := 2.3.21-4
.PHONY: PKG@@mingw-w64-x86_64-libart_lgpl
PKG@@mingw-w64-x86_64-libart_lgpl: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libart_lgpl'

override VERSION_OF_mingw-w64-x86_64-libass := 0.17.5-1
.PHONY: PKG@@mingw-w64-x86_64-libass
PKG@@mingw-w64-x86_64-libass: $(if $(__deps),PKG@@mingw-w64-x86_64-fontconfig PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-fribidi PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libunibreak) ; @echo 'PKG@@mingw-w64-x86_64-libass'

override VERSION_OF_mingw-w64-x86_64-libassuan := 2.5.7-2
.PHONY: PKG@@mingw-w64-x86_64-libassuan
PKG@@mingw-w64-x86_64-libassuan: $(if $(__deps),PKG@@mingw-w64-x86_64-libgpg-error) ; @echo 'PKG@@mingw-w64-x86_64-libassuan'

override VERSION_OF_mingw-w64-x86_64-libatomic := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-libatomic
PKG@@mingw-w64-x86_64-libatomic: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-libatomic'

override VERSION_OF_mingw-w64-x86_64-libatomic_ops := 7.10.0-1
.PHONY: PKG@@mingw-w64-x86_64-libatomic_ops
PKG@@mingw-w64-x86_64-libatomic_ops: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libatomic_ops'

override VERSION_OF_mingw-w64-x86_64-libavif := 1.4.2-1
.PHONY: PKG@@mingw-w64-x86_64-libavif
PKG@@mingw-w64-x86_64-libavif: $(if $(__deps),PKG@@mingw-w64-x86_64-aom PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-dav1d PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libsharpyuv PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-libyuv PKG@@mingw-w64-x86_64-rav1e PKG@@mingw-w64-x86_64-svt-av1 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libavif'

override VERSION_OF_mingw-w64-x86_64-libavro := 1.12.0-4
.PHONY: PKG@@mingw-w64-x86_64-libavro
PKG@@mingw-w64-x86_64-libavro: $(if $(__deps),PKG@@mingw-w64-x86_64-boost-libs PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-fmt PKG@@mingw-w64-x86_64-jansson PKG@@mingw-w64-x86_64-snappy PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libavro'

override VERSION_OF_mingw-w64-x86_64-libb2 := 0.98.1-3
.PHONY: PKG@@mingw-w64-x86_64-libb2
PKG@@mingw-w64-x86_64-libb2: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libb2'

override VERSION_OF_mingw-w64-x86_64-libb64 := 1.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-libb64
PKG@@mingw-w64-x86_64-libb64: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libb64'

override VERSION_OF_mingw-w64-x86_64-libbdplus := 0.2.0-2
.PHONY: PKG@@mingw-w64-x86_64-libbdplus
PKG@@mingw-w64-x86_64-libbdplus: $(if $(__deps),PKG@@mingw-w64-x86_64-libaacs PKG@@mingw-w64-x86_64-libgpg-error) ; @echo 'PKG@@mingw-w64-x86_64-libbdplus'

override VERSION_OF_mingw-w64-x86_64-libblake3 := 1.8.7-2
.PHONY: PKG@@mingw-w64-x86_64-libblake3
PKG@@mingw-w64-x86_64-libblake3: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-tbb) ; @echo 'PKG@@mingw-w64-x86_64-libblake3'

override VERSION_OF_mingw-w64-x86_64-libblocksruntime-swift := 6.1.1-4
.PHONY: PKG@@mingw-w64-x86_64-libblocksruntime PKG@@mingw-w64-x86_64-libblocksruntime-swift
PKG@@mingw-w64-x86_64-libblocksruntime PKG@@mingw-w64-x86_64-libblocksruntime-swift: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libblocksruntime-swift'

override VERSION_OF_mingw-w64-x86_64-libbluray := 1.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-libbluray
PKG@@mingw-w64-x86_64-libbluray: $(if $(__deps),PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-libbluray'

override VERSION_OF_mingw-w64-x86_64-libbotan := 3.13.0-1
.PHONY: PKG@@mingw-w64-x86_64-libbotan
PKG@@mingw-w64-x86_64-libbotan: $(if $(__deps),PKG@@mingw-w64-x86_64-boost PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-sqlite3 PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libbotan'

override VERSION_OF_mingw-w64-x86_64-libbs2b := 3.1.0-2
.PHONY: PKG@@mingw-w64-x86_64-libbs2b
PKG@@mingw-w64-x86_64-libbs2b: $(if $(__deps),PKG@@mingw-w64-x86_64-libsndfile) ; @echo 'PKG@@mingw-w64-x86_64-libbs2b'

override VERSION_OF_mingw-w64-x86_64-libbsdf := 0.11.0-3
.PHONY: PKG@@mingw-w64-x86_64-libbsdf
PKG@@mingw-w64-x86_64-libbsdf: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libbsdf'

override VERSION_OF_mingw-w64-x86_64-libc++ := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-libc++
PKG@@mingw-w64-x86_64-libc++: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libc++'

override VERSION_OF_mingw-w64-x86_64-libcaca := 0.99.beta20-8
.PHONY: PKG@@mingw-w64-x86_64-libcaca
PKG@@mingw-w64-x86_64-libcaca: $(if $(__deps),PKG@@mingw-w64-x86_64-fontconfig PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libcaca'

override VERSION_OF_mingw-w64-x86_64-libcacard := 2.8.1-2
.PHONY: PKG@@mingw-w64-x86_64-libcacard
PKG@@mingw-w64-x86_64-libcacard: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-nspr PKG@@mingw-w64-x86_64-nss) ; @echo 'PKG@@mingw-w64-x86_64-libcacard'

override VERSION_OF_mingw-w64-x86_64-libcbor := 0.14.0-1
.PHONY: PKG@@mingw-w64-x86_64-libcbor
PKG@@mingw-w64-x86_64-libcbor: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libcbor'

override VERSION_OF_mingw-w64-x86_64-libcddb := 1.3.2-6
.PHONY: PKG@@mingw-w64-x86_64-libcddb
PKG@@mingw-w64-x86_64-libcddb: $(if $(__deps),PKG@@mingw-w64-x86_64-libsystre) ; @echo 'PKG@@mingw-w64-x86_64-libcddb'

override VERSION_OF_mingw-w64-x86_64-libcdio := 2.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-libcdio
PKG@@mingw-w64-x86_64-libcdio: $(if $(__deps),PKG@@mingw-w64-x86_64-libcddb PKG@@mingw-w64-x86_64-libiconv) ; @echo 'PKG@@mingw-w64-x86_64-libcdio'

override VERSION_OF_mingw-w64-x86_64-libcdio-paranoia := 10.2+2.0.2-2
.PHONY: PKG@@mingw-w64-x86_64-libcdio-paranoia
PKG@@mingw-w64-x86_64-libcdio-paranoia: $(if $(__deps),PKG@@mingw-w64-x86_64-libcdio) ; @echo 'PKG@@mingw-w64-x86_64-libcdio-paranoia'

override VERSION_OF_mingw-w64-x86_64-libcdr := 0.1.9-1
.PHONY: PKG@@mingw-w64-x86_64-libcdr
PKG@@mingw-w64-x86_64-libcdr: $(if $(__deps),PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-lcms2 PKG@@mingw-w64-x86_64-librevenge PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libcdr'

override VERSION_OF_mingw-w64-x86_64-libcello := 2.1.0.313.dfcd86c-1
.PHONY: PKG@@mingw-w64-x86_64-libcello PKG@@mingw-w64-x86_64-libcello-git
PKG@@mingw-w64-x86_64-libcello PKG@@mingw-w64-x86_64-libcello-git: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libcello'

override VERSION_OF_mingw-w64-x86_64-libcerf := 1~3.3-1
.PHONY: PKG@@mingw-w64-x86_64-libcerf
PKG@@mingw-w64-x86_64-libcerf: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libcerf'

override VERSION_OF_mingw-w64-x86_64-libclc := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-libclc
PKG@@mingw-w64-x86_64-libclc: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libclc'

override VERSION_OF_mingw-w64-x86_64-libcloudproviders := 0.4.2-1
.PHONY: PKG@@mingw-w64-x86_64-libcloudproviders
PKG@@mingw-w64-x86_64-libcloudproviders: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2) ; @echo 'PKG@@mingw-w64-x86_64-libcloudproviders'

override VERSION_OF_mingw-w64-x86_64-libconfig := 1.8.3-1
.PHONY: PKG@@mingw-w64-x86_64-libconfig
PKG@@mingw-w64-x86_64-libconfig: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libconfig'

override VERSION_OF_mingw-w64-x86_64-libconfini := 1.16.4-1
.PHONY: PKG@@mingw-w64-x86_64-libconfini
PKG@@mingw-w64-x86_64-libconfini: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libconfini'

override VERSION_OF_mingw-w64-x86_64-libcue := 2.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-libcue
PKG@@mingw-w64-x86_64-libcue: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libcue'

override VERSION_OF_mingw-w64-x86_64-libdatrie := 0.2.14-1
.PHONY: PKG@@mingw-w64-x86_64-libdatrie
PKG@@mingw-w64-x86_64-libdatrie: $(if $(__deps),PKG@@mingw-w64-x86_64-libiconv) ; @echo 'PKG@@mingw-w64-x86_64-libdatrie'

override VERSION_OF_mingw-w64-x86_64-libdatrie-docs := 0.2.14-1
.PHONY: PKG@@mingw-w64-x86_64-libdatrie-docs
PKG@@mingw-w64-x86_64-libdatrie-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libdatrie-docs'

override VERSION_OF_mingw-w64-x86_64-libdazzle := 3.44.0-3
.PHONY: PKG@@mingw-w64-x86_64-libdazzle
PKG@@mingw-w64-x86_64-libdazzle: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gtk3) ; @echo 'PKG@@mingw-w64-x86_64-libdazzle'

override VERSION_OF_mingw-w64-x86_64-libdca := 0.0.7-3
.PHONY: PKG@@mingw-w64-x86_64-libdca
PKG@@mingw-w64-x86_64-libdca: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libdca'

override VERSION_OF_mingw-w64-x86_64-libde265 := 1.1.3-1
.PHONY: PKG@@mingw-w64-x86_64-libde265
PKG@@mingw-w64-x86_64-libde265: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libde265'

override VERSION_OF_mingw-w64-x86_64-libdeflate := 1.26-1
.PHONY: PKG@@mingw-w64-x86_64-libdeflate
PKG@@mingw-w64-x86_64-libdeflate: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libdeflate'

override VERSION_OF_mingw-w64-x86_64-libdex := 0.8.1-3
.PHONY: PKG@@mingw-w64-x86_64-libdex
PKG@@mingw-w64-x86_64-libdex: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libatomic) ; @echo 'PKG@@mingw-w64-x86_64-libdex'

override VERSION_OF_mingw-w64-x86_64-libdicom := 1.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-libdicom
PKG@@mingw-w64-x86_64-libdicom: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libdicom'

override VERSION_OF_mingw-w64-x86_64-libdispatch := 6.1.1-4
.PHONY: PKG@@mingw-w64-x86_64-libdispatch
PKG@@mingw-w64-x86_64-libdispatch: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libblocksruntime-swift PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-libdispatch'

override VERSION_OF_mingw-w64-x86_64-libdmtx := 0.7.8-1
.PHONY: PKG@@mingw-w64-x86_64-libdmtx
PKG@@mingw-w64-x86_64-libdmtx: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libdmtx'

override VERSION_OF_mingw-w64-x86_64-libdovi := 3.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-libdovi
PKG@@mingw-w64-x86_64-libdovi: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libdovi'

override VERSION_OF_mingw-w64-x86_64-libdsm := 0.4.3-5
.PHONY: PKG@@mingw-w64-x86_64-libdsm
PKG@@mingw-w64-x86_64-libdsm: $(if $(__deps),PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libtasn1 PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-libdsm'

override VERSION_OF_mingw-w64-x86_64-libdvdcss := 1.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-libdvdcss
PKG@@mingw-w64-x86_64-libdvdcss: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libdvdcss'

override VERSION_OF_mingw-w64-x86_64-libdvdread := 7.1.1-1
.PHONY: PKG@@mingw-w64-x86_64-libdvdread
PKG@@mingw-w64-x86_64-libdvdread: $(if $(__deps),PKG@@mingw-w64-x86_64-libdvdcss) ; @echo 'PKG@@mingw-w64-x86_64-libdvdread'

override VERSION_OF_mingw-w64-x86_64-libebml := 1.4.7-1
.PHONY: PKG@@mingw-w64-x86_64-libebml
PKG@@mingw-w64-x86_64-libebml: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libebml'

override VERSION_OF_mingw-w64-x86_64-libelf := 0.8.13-7
.PHONY: PKG@@mingw-w64-x86_64-libelf
PKG@@mingw-w64-x86_64-libelf: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libelf'

override VERSION_OF_mingw-w64-x86_64-libepoxy := 1.5.10-7
.PHONY: PKG@@mingw-w64-x86_64-libepoxy
PKG@@mingw-w64-x86_64-libepoxy: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-egl-headers PKG@@mingw-w64-x86_64-gles-headers) ; @echo 'PKG@@mingw-w64-x86_64-libepoxy'

override VERSION_OF_mingw-w64-x86_64-libevent := 2.1.13-1
.PHONY: PKG@@mingw-w64-x86_64-libevent
PKG@@mingw-w64-x86_64-libevent: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-libevent'

override VERSION_OF_mingw-w64-x86_64-libexif := 0.6.26-1
.PHONY: PKG@@mingw-w64-x86_64-libexif
PKG@@mingw-w64-x86_64-libexif: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime) ; @echo 'PKG@@mingw-w64-x86_64-libexif'

override VERSION_OF_mingw-w64-x86_64-libffi := 3.8.0-1
.PHONY: PKG@@mingw-w64-x86_64-libffi
PKG@@mingw-w64-x86_64-libffi: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libffi'

override VERSION_OF_mingw-w64-x86_64-libfido2 := 1.17.0-3
.PHONY: PKG@@mingw-w64-x86_64-libfido2
PKG@@mingw-w64-x86_64-libfido2: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libcbor PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libfido2'

override VERSION_OF_mingw-w64-x86_64-libfreexl := 2.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-libfreexl
PKG@@mingw-w64-x86_64-libfreexl: $(if $(__deps),PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-minizip) ; @echo 'PKG@@mingw-w64-x86_64-libfreexl'

override VERSION_OF_mingw-w64-x86_64-libfyaml := 0.9.6-2
.PHONY: PKG@@mingw-w64-x86_64-libfyaml
PKG@@mingw-w64-x86_64-libfyaml: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libfyaml'

override VERSION_OF_mingw-w64-x86_64-libgadu := 1.12.2-4
.PHONY: PKG@@mingw-w64-x86_64-libgadu
PKG@@mingw-w64-x86_64-libgadu: $(if $(__deps),PKG@@mingw-w64-x86_64-gnutls PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-protobuf-c PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libgadu'

override VERSION_OF_mingw-w64-x86_64-libgcc := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-libgcc
PKG@@mingw-w64-x86_64-libgcc: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-libgcc'

override VERSION_OF_mingw-w64-x86_64-libgccjit := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-libgccjit
PKG@@mingw-w64-x86_64-libgccjit: $(if $(__deps),PKG@@mingw-w64-x86_64-gcc PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-isl PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-mpc PKG@@mingw-w64-x86_64-mpfr PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-libgccjit'

override VERSION_OF_mingw-w64-x86_64-libgcrypt := 1.12.4-2
.PHONY: PKG@@mingw-w64-x86_64-libgcrypt
PKG@@mingw-w64-x86_64-libgcrypt: $(if $(__deps),PKG@@mingw-w64-x86_64-libgpg-error) ; @echo 'PKG@@mingw-w64-x86_64-libgcrypt'

override VERSION_OF_mingw-w64-x86_64-libgd := 2.3.3-2
.PHONY: PKG@@mingw-w64-x86_64-libgd
PKG@@mingw-w64-x86_64-libgd: $(if $(__deps),PKG@@mingw-w64-x86_64-fontconfig PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-libavif PKG@@mingw-w64-x86_64-libheif PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libimagequant PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-xpm-nox PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libgd'

override VERSION_OF_mingw-w64-x86_64-libgeotiff := 1.7.4-2
.PHONY: PKG@@mingw-w64-x86_64-libgeotiff
PKG@@mingw-w64-x86_64-libgeotiff: $(if $(__deps),PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-proj PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libgeotiff'

override VERSION_OF_mingw-w64-x86_64-libgfortran := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-gcc-libgfortran PKG@@mingw-w64-x86_64-libgfortran
PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-gcc-libgfortran PKG@@mingw-w64-x86_64-libgfortran: $(if $(__deps),PKG@@mingw-w64-x86_64-libgcc PKG@@mingw-w64-x86_64-libquadmath PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-libgfortran'

override VERSION_OF_mingw-w64-x86_64-libgig := 4.5.2-1
.PHONY: PKG@@mingw-w64-x86_64-libgig
PKG@@mingw-w64-x86_64-libgig: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libsndfile) ; @echo 'PKG@@mingw-w64-x86_64-libgig'

override VERSION_OF_mingw-w64-x86_64-libgit2 := 1.9.7-2
.PHONY: PKG@@mingw-w64-x86_64-libgit2
PKG@@mingw-w64-x86_64-libgit2: $(if $(__deps),PKG@@mingw-w64-x86_64-ca-certificates PKG@@mingw-w64-x86_64-http-parser PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libssh2 PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-pcre2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libgit2'

override VERSION_OF_mingw-w64-x86_64-libgit2-glib := 1.2.0-8
.PHONY: PKG@@mingw-w64-x86_64-libgit2-glib
PKG@@mingw-w64-x86_64-libgit2-glib: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libgit2 PKG@@mingw-w64-x86_64-libssh2) ; @echo 'PKG@@mingw-w64-x86_64-libgit2-glib'

override VERSION_OF_mingw-w64-x86_64-libgit2-glib-docs := 1.2.0-8
.PHONY: PKG@@mingw-w64-x86_64-libgit2-glib-docs
PKG@@mingw-w64-x86_64-libgit2-glib-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libgit2-glib-docs'

override VERSION_OF_mingw-w64-x86_64-libgit2-winhttp := 1.9.7-2
.PHONY: PKG@@mingw-w64-x86_64-libgit2-winhttp
PKG@@mingw-w64-x86_64-libgit2-winhttp: $(if $(__deps),PKG@@mingw-w64-x86_64-http-parser PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libssh2 PKG@@mingw-w64-x86_64-pcre2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libgit2-winhttp'

override VERSION_OF_mingw-w64-x86_64-libgme := 0.6.5-2
.PHONY: PKG@@mingw-w64-x86_64-libgme
PKG@@mingw-w64-x86_64-libgme: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libgme'

override VERSION_OF_mingw-w64-x86_64-libgomp := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-libgomp PKG@@mingw-w64-x86_64-omp
PKG@@mingw-w64-x86_64-libgomp PKG@@mingw-w64-x86_64-omp: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-libgomp'

override VERSION_OF_mingw-w64-x86_64-libgoom2 := 2k4-6
.PHONY: PKG@@mingw-w64-x86_64-libgoom2
PKG@@mingw-w64-x86_64-libgoom2: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libgoom2'

override VERSION_OF_mingw-w64-x86_64-libgpg-error := 1.61-1
.PHONY: PKG@@mingw-w64-x86_64-libgpg-error
PKG@@mingw-w64-x86_64-libgpg-error: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gettext-runtime) ; @echo 'PKG@@mingw-w64-x86_64-libgpg-error'

override VERSION_OF_mingw-w64-x86_64-libgsf := 1.14.58-1
.PHONY: PKG@@mingw-w64-x86_64-libgsf
PKG@@mingw-w64-x86_64-libgsf: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-gdk-pixbuf2 PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libgsf'

override VERSION_OF_mingw-w64-x86_64-libguess := 1.2-6
.PHONY: PKG@@mingw-w64-x86_64-libguess
PKG@@mingw-w64-x86_64-libguess: $(if $(__deps),PKG@@mingw-w64-x86_64-libmowgli) ; @echo 'PKG@@mingw-w64-x86_64-libguess'

override VERSION_OF_mingw-w64-x86_64-libgweather := 4.4.4-1
.PHONY: PKG@@mingw-w64-x86_64-libgweather
PKG@@mingw-w64-x86_64-libgweather: $(if $(__deps),PKG@@mingw-w64-x86_64-geocode-glib PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-json-glib PKG@@mingw-w64-x86_64-libsoup3 PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-libgweather'

override VERSION_OF_mingw-w64-x86_64-libgxps := 0.3.2-6
.PHONY: PKG@@mingw-w64-x86_64-libgxps
PKG@@mingw-w64-x86_64-libgxps: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-lcms2 PKG@@mingw-w64-x86_64-libarchive PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff) ; @echo 'PKG@@mingw-w64-x86_64-libgxps'

override VERSION_OF_mingw-w64-x86_64-libharu := 2.4.6-1
.PHONY: PKG@@mingw-w64-x86_64-libharu
PKG@@mingw-w64-x86_64-libharu: $(if $(__deps),PKG@@mingw-w64-x86_64-libpng) ; @echo 'PKG@@mingw-w64-x86_64-libharu'

override VERSION_OF_mingw-w64-x86_64-libheif := 1.23.7-1
.PHONY: PKG@@mingw-w64-x86_64-libheif
PKG@@mingw-w64-x86_64-libheif: $(if $(__deps),PKG@@mingw-w64-x86_64-SDL2 PKG@@mingw-w64-x86_64-aom PKG@@mingw-w64-x86_64-brotli PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-dav1d PKG@@mingw-w64-x86_64-kvazaar PKG@@mingw-w64-x86_64-libde265 PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libsharpyuv PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libx264 PKG@@mingw-w64-x86_64-openh264 PKG@@mingw-w64-x86_64-openjpeg2 PKG@@mingw-w64-x86_64-openjph PKG@@mingw-w64-x86_64-rav1e PKG@@mingw-w64-x86_64-svt-av1 PKG@@mingw-w64-x86_64-x265 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libheif'

override VERSION_OF_mingw-w64-x86_64-libical := 3.0.20-8
.PHONY: PKG@@mingw-w64-x86_64-libical
PKG@@mingw-w64-x86_64-libical: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-db PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-libical'

override VERSION_OF_mingw-w64-x86_64-libiconv := 1.19.1-1
.PHONY: PKG@@mingw-w64-x86_64-libiconv
PKG@@mingw-w64-x86_64-libiconv: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libiconv'

override VERSION_OF_mingw-w64-x86_64-libicsneo := 1.1.0-4
.PHONY: PKG@@mingw-w64-x86_64-libicsneo
PKG@@mingw-w64-x86_64-libicsneo: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libicsneo'

override VERSION_OF_mingw-w64-x86_64-libidl2 := 0.8.14-3
.PHONY: PKG@@mingw-w64-x86_64-libidl2
PKG@@mingw-w64-x86_64-libidl2: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2) ; @echo 'PKG@@mingw-w64-x86_64-libidl2'

override VERSION_OF_mingw-w64-x86_64-libidn := 1.44-1
.PHONY: PKG@@mingw-w64-x86_64-libidn
PKG@@mingw-w64-x86_64-libidn: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-libiconv) ; @echo 'PKG@@mingw-w64-x86_64-libidn'

override VERSION_OF_mingw-w64-x86_64-libidn2 := 2.3.8-4
.PHONY: PKG@@mingw-w64-x86_64-libidn2
PKG@@mingw-w64-x86_64-libidn2: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libidn2'

override VERSION_OF_mingw-w64-x86_64-libiff := 0.1.r60.g0290be4-2
.PHONY: PKG@@mingw-w64-x86_64-libiff
PKG@@mingw-w64-x86_64-libiff: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libiff'

override VERSION_OF_mingw-w64-x86_64-libilbc := 3.0.4-3
.PHONY: PKG@@mingw-w64-x86_64-libilbc
PKG@@mingw-w64-x86_64-libilbc: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libilbc'

override VERSION_OF_mingw-w64-x86_64-libilbm := 0.1.r68.g170c269-2
.PHONY: PKG@@mingw-w64-x86_64-libilbm
PKG@@mingw-w64-x86_64-libilbm: $(if $(__deps),PKG@@mingw-w64-x86_64-libiff) ; @echo 'PKG@@mingw-w64-x86_64-libilbm'

override VERSION_OF_mingw-w64-x86_64-libimagequant := 4.4.1-1
.PHONY: PKG@@mingw-w64-x86_64-libimagequant
PKG@@mingw-w64-x86_64-libimagequant: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libimagequant'

override VERSION_OF_mingw-w64-x86_64-libimobiledevice-glue := 1.3.2-1
.PHONY: PKG@@mingw-w64-x86_64-libimobiledevice-glue
PKG@@mingw-w64-x86_64-libimobiledevice-glue: $(if $(__deps),PKG@@mingw-w64-x86_64-libplist) ; @echo 'PKG@@mingw-w64-x86_64-libimobiledevice-glue'

override VERSION_OF_mingw-w64-x86_64-libinih := 63-1
.PHONY: PKG@@mingw-w64-x86_64-libinih
PKG@@mingw-w64-x86_64-libinih: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libinih'

override VERSION_OF_mingw-w64-x86_64-libipt := 2.1.2-1
.PHONY: PKG@@mingw-w64-x86_64-libipt
PKG@@mingw-w64-x86_64-libipt: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libipt'

override VERSION_OF_mingw-w64-x86_64-libiptcdata := 1.0.5-2
.PHONY: PKG@@mingw-w64-x86_64-libiptcdata
PKG@@mingw-w64-x86_64-libiptcdata: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime) ; @echo 'PKG@@mingw-w64-x86_64-libiptcdata'

override VERSION_OF_mingw-w64-x86_64-libjpeg-turbo := 3.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libjpeg-turbo
PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libjpeg-turbo: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libjpeg-turbo'

override VERSION_OF_mingw-w64-x86_64-libjxl := 0.12.0-2
.PHONY: PKG@@mingw-w64-x86_64-libjxl
PKG@@mingw-w64-x86_64-libjxl: $(if $(__deps),PKG@@mingw-w64-x86_64-brotli PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-giflib PKG@@mingw-w64-x86_64-highway PKG@@mingw-w64-x86_64-lcms2 PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-openexr) ; @echo 'PKG@@mingw-w64-x86_64-libjxl'

override VERSION_OF_mingw-w64-x86_64-libkml := 1.3.0-12
.PHONY: PKG@@mingw-w64-x86_64-libkml
PKG@@mingw-w64-x86_64-libkml: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-minizip PKG@@mingw-w64-x86_64-uriparser PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libkml'

override VERSION_OF_mingw-w64-x86_64-libksba := 1.6.8-1
.PHONY: PKG@@mingw-w64-x86_64-libksba
PKG@@mingw-w64-x86_64-libksba: $(if $(__deps),PKG@@mingw-w64-x86_64-libgpg-error) ; @echo 'PKG@@mingw-w64-x86_64-libksba'

override VERSION_OF_mingw-w64-x86_64-liblas := 1.8.1-8
.PHONY: PKG@@mingw-w64-x86_64-liblas
PKG@@mingw-w64-x86_64-liblas: $(if $(__deps),PKG@@mingw-w64-x86_64-boost-libs PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libgeotiff) ; @echo 'PKG@@mingw-w64-x86_64-liblas'

override VERSION_OF_mingw-w64-x86_64-liblc3 := 1.1.3-1
.PHONY: PKG@@mingw-w64-x86_64-liblc3
PKG@@mingw-w64-x86_64-liblc3: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-liblc3'

override VERSION_OF_mingw-w64-x86_64-liblcf := 0.8.1-3
.PHONY: PKG@@mingw-w64-x86_64-liblcf
PKG@@mingw-w64-x86_64-liblcf: $(if $(__deps),PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-libinih) ; @echo 'PKG@@mingw-w64-x86_64-liblcf'

override VERSION_OF_mingw-w64-x86_64-liblo := 0.34-1
.PHONY: PKG@@mingw-w64-x86_64-liblo
PKG@@mingw-w64-x86_64-liblo: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-liblo'

override VERSION_OF_mingw-w64-x86_64-liblqr := 0.4.3-3
.PHONY: PKG@@mingw-w64-x86_64-liblqr
PKG@@mingw-w64-x86_64-liblqr: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2) ; @echo 'PKG@@mingw-w64-x86_64-liblqr'

override VERSION_OF_mingw-w64-x86_64-libltdl := 2.6.2-1
.PHONY: PKG@@mingw-w64-x86_64-libltdl
PKG@@mingw-w64-x86_64-libltdl: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libltdl'

override VERSION_OF_mingw-w64-x86_64-libmad := 0.15.1b-5
.PHONY: PKG@@mingw-w64-x86_64-libmad
PKG@@mingw-w64-x86_64-libmad: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libmad'

override VERSION_OF_mingw-w64-x86_64-libmariadbclient := 3.4.11-1
.PHONY: PKG@@mingw-w64-x86_64-libmariadbclient
PKG@@mingw-w64-x86_64-libmariadbclient: $(if $(__deps),PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-libmariadbclient'

override VERSION_OF_mingw-w64-x86_64-libmfx := 1.35.1-2
.PHONY: PKG@@mingw-w64-x86_64-libmfx
PKG@@mingw-w64-x86_64-libmfx: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libmfx'

override VERSION_OF_mingw-w64-x86_64-libmicrodns := 0.2.0-4
.PHONY: PKG@@mingw-w64-x86_64-libmicrodns
PKG@@mingw-w64-x86_64-libmicrodns: $(if $(__deps),PKG@@mingw-w64-x86_64-libtasn1) ; @echo 'PKG@@mingw-w64-x86_64-libmicrodns'

override VERSION_OF_mingw-w64-x86_64-libmikmod := 3.3.16-1
.PHONY: PKG@@mingw-w64-x86_64-libmikmod
PKG@@mingw-w64-x86_64-libmikmod: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libmikmod'

override VERSION_OF_mingw-w64-x86_64-libmimic := 1.0.4-4
.PHONY: PKG@@mingw-w64-x86_64-libmimic
PKG@@mingw-w64-x86_64-libmimic: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2) ; @echo 'PKG@@mingw-w64-x86_64-libmimic'

override VERSION_OF_mingw-w64-x86_64-libmng := 2.0.3-7
.PHONY: PKG@@mingw-w64-x86_64-libmng
PKG@@mingw-w64-x86_64-libmng: $(if $(__deps),PKG@@mingw-w64-x86_64-lcms2 PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libmng'

override VERSION_OF_mingw-w64-x86_64-libmodbus := 782.ebc4f47-1
.PHONY: PKG@@mingw-w64-x86_64-libmodbus PKG@@mingw-w64-x86_64-libmodbus-git
PKG@@mingw-w64-x86_64-libmodbus PKG@@mingw-w64-x86_64-libmodbus-git: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libmodbus'

override VERSION_OF_mingw-w64-x86_64-libmodplug := 0.8.9.0-5
.PHONY: PKG@@mingw-w64-x86_64-libmodplug
PKG@@mingw-w64-x86_64-libmodplug: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libmodplug'

override VERSION_OF_mingw-w64-x86_64-libmongocrypt := 1.20.4-1
.PHONY: PKG@@mingw-w64-x86_64-libmongocrypt
PKG@@mingw-w64-x86_64-libmongocrypt: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libmongocrypt'

override VERSION_OF_mingw-w64-x86_64-libmongoose := 7.23-2
.PHONY: PKG@@mingw-w64-x86_64-libmongoose
PKG@@mingw-w64-x86_64-libmongoose: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libmongoose'

override VERSION_OF_mingw-w64-x86_64-libmongoose-docs := 7.23-2
.PHONY: PKG@@mingw-w64-x86_64-libmongoose-docs
PKG@@mingw-w64-x86_64-libmongoose-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libmongoose-docs'

override VERSION_OF_mingw-w64-x86_64-libmowgli := 2.1.3-7
.PHONY: PKG@@mingw-w64-x86_64-libmowgli
PKG@@mingw-w64-x86_64-libmowgli: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-libmowgli'

override VERSION_OF_mingw-w64-x86_64-libmpack := 1.0.5-5
.PHONY: PKG@@mingw-w64-x86_64-libmpack
PKG@@mingw-w64-x86_64-libmpack: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libmpack'

override VERSION_OF_mingw-w64-x86_64-libmysofa := 1.3.5-1
.PHONY: PKG@@mingw-w64-x86_64-libmysofa
PKG@@mingw-w64-x86_64-libmysofa: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libmysofa'

override VERSION_OF_mingw-w64-x86_64-libnfs := 6.0.2-1
.PHONY: PKG@@mingw-w64-x86_64-libnfs
PKG@@mingw-w64-x86_64-libnfs: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libnfs'

override VERSION_OF_mingw-w64-x86_64-libnice := 0.1.24-1
.PHONY: PKG@@mingw-w64-x86_64-libnice
PKG@@mingw-w64-x86_64-libnice: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gnutls) ; @echo 'PKG@@mingw-w64-x86_64-libnice'

override VERSION_OF_mingw-w64-x86_64-libntlm := 1.8-2
.PHONY: PKG@@mingw-w64-x86_64-libntlm
PKG@@mingw-w64-x86_64-libntlm: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libntlm'

override VERSION_OF_mingw-w64-x86_64-libnumbertext := 1.0.11-2
.PHONY: PKG@@mingw-w64-x86_64-libnumbertext
PKG@@mingw-w64-x86_64-libnumbertext: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libnumbertext'

override VERSION_OF_mingw-w64-x86_64-liboauth := 1.0.3-7
.PHONY: PKG@@mingw-w64-x86_64-liboauth
PKG@@mingw-w64-x86_64-liboauth: $(if $(__deps),PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-nss) ; @echo 'PKG@@mingw-w64-x86_64-liboauth'

override VERSION_OF_mingw-w64-x86_64-libobjc := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-libobjc
PKG@@mingw-w64-x86_64-libobjc: $(if $(__deps),PKG@@mingw-w64-x86_64-libgcc PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-libobjc'

override VERSION_OF_mingw-w64-x86_64-libobjc2 := 2.3-1
.PHONY: PKG@@mingw-w64-x86_64-libobjc2
PKG@@mingw-w64-x86_64-libobjc2: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libobjc2'

override VERSION_OF_mingw-w64-x86_64-libodfgen := 0.1.8-8
.PHONY: PKG@@mingw-w64-x86_64-libodfgen
PKG@@mingw-w64-x86_64-libodfgen: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-librevenge PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-libodfgen'

override VERSION_OF_mingw-w64-x86_64-libogg := 1.3.6-1
.PHONY: PKG@@mingw-w64-x86_64-libogg
PKG@@mingw-w64-x86_64-libogg: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libogg'

override VERSION_OF_mingw-w64-x86_64-libomemo-c := 0.5.1-1
.PHONY: PKG@@mingw-w64-x86_64-libomemo-c
PKG@@mingw-w64-x86_64-libomemo-c: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-protobuf-c) ; @echo 'PKG@@mingw-w64-x86_64-libomemo-c'

override VERSION_OF_mingw-w64-x86_64-libopenmpt := 0.8.10-1
.PHONY: PKG@@mingw-w64-x86_64-libopenmpt
PKG@@mingw-w64-x86_64-libopenmpt: $(if $(__deps),PKG@@mingw-w64-x86_64-flac PKG@@mingw-w64-x86_64-libogg PKG@@mingw-w64-x86_64-libsndfile PKG@@mingw-w64-x86_64-libvorbis PKG@@mingw-w64-x86_64-mpg123 PKG@@mingw-w64-x86_64-portaudio PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libopenmpt'

override VERSION_OF_mingw-w64-x86_64-libopenmpt-modplug := 0.8.9.0-4
.PHONY: PKG@@mingw-w64-x86_64-libopenmpt-modplug
PKG@@mingw-w64-x86_64-libopenmpt-modplug: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libopenmpt) ; @echo 'PKG@@mingw-w64-x86_64-libopenmpt-modplug'

override VERSION_OF_mingw-w64-x86_64-libopusenc := 0.3-1
.PHONY: PKG@@mingw-w64-x86_64-libopusenc
PKG@@mingw-w64-x86_64-libopusenc: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-opus) ; @echo 'PKG@@mingw-w64-x86_64-libopusenc'

override VERSION_OF_mingw-w64-x86_64-liboqs := 0.16.0-1
.PHONY: PKG@@mingw-w64-x86_64-liboqs
PKG@@mingw-w64-x86_64-liboqs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-liboqs'

override VERSION_OF_mingw-w64-x86_64-libosmpbf := 1.5.1-6
.PHONY: PKG@@mingw-w64-x86_64-libosmpbf
PKG@@mingw-w64-x86_64-libosmpbf: $(if $(__deps),PKG@@mingw-w64-x86_64-protobuf PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libosmpbf'

override VERSION_OF_mingw-w64-x86_64-libotr := 4.1.1-6
.PHONY: PKG@@mingw-w64-x86_64-libotr
PKG@@mingw-w64-x86_64-libotr: $(if $(__deps),PKG@@mingw-w64-x86_64-libgcrypt) ; @echo 'PKG@@mingw-w64-x86_64-libotr'

override VERSION_OF_mingw-w64-x86_64-libp11 := 0.4.18-1
.PHONY: PKG@@mingw-w64-x86_64-libp11
PKG@@mingw-w64-x86_64-libp11: $(if $(__deps),PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-libp11'

override VERSION_OF_mingw-w64-x86_64-libpaper := 2.2.8-1
.PHONY: PKG@@mingw-w64-x86_64-libpaper
PKG@@mingw-w64-x86_64-libpaper: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libpaper'

override VERSION_OF_mingw-w64-x86_64-libpcap := 1.11.0-1
.PHONY: PKG@@mingw-w64-x86_64-libpcap
PKG@@mingw-w64-x86_64-libpcap: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-libpcap'

override VERSION_OF_mingw-w64-x86_64-libplacebo := 7.387.0-1
.PHONY: PKG@@mingw-w64-x86_64-libplacebo
PKG@@mingw-w64-x86_64-libplacebo: $(if $(__deps),PKG@@mingw-w64-x86_64-lcms2 PKG@@mingw-w64-x86_64-libdovi PKG@@mingw-w64-x86_64-shaderc PKG@@mingw-w64-x86_64-spirv-cross PKG@@mingw-w64-x86_64-vulkan) ; @echo 'PKG@@mingw-w64-x86_64-libplacebo'

override VERSION_OF_mingw-w64-x86_64-libplacebo-demos := 7.387.0-1
.PHONY: PKG@@mingw-w64-x86_64-libplacebo-demos
PKG@@mingw-w64-x86_64-libplacebo-demos: $(if $(__deps),PKG@@mingw-w64-x86_64-SDL2 PKG@@mingw-w64-x86_64-ffmpeg PKG@@mingw-w64-x86_64-libplacebo) ; @echo 'PKG@@mingw-w64-x86_64-libplacebo-demos'

override VERSION_OF_mingw-w64-x86_64-libplist := 2.7.0-4
.PHONY: PKG@@mingw-w64-x86_64-libplist
PKG@@mingw-w64-x86_64-libplist: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libplist'

override VERSION_OF_mingw-w64-x86_64-libpng := 1.6.59-2
.PHONY: PKG@@mingw-w64-x86_64-libpng
PKG@@mingw-w64-x86_64-libpng: $(if $(__deps),PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libpng'

override VERSION_OF_mingw-w64-x86_64-libpqxx := 8.0.2-1
.PHONY: PKG@@mingw-w64-x86_64-libpqxx
PKG@@mingw-w64-x86_64-libpqxx: $(if $(__deps),PKG@@mingw-w64-x86_64-postgresql) ; @echo 'PKG@@mingw-w64-x86_64-libpqxx'

override VERSION_OF_mingw-w64-x86_64-libpreferencepanes := 1.2.1-9
.PHONY: PKG@@mingw-w64-x86_64-libpreferencepanes
PKG@@mingw-w64-x86_64-libpreferencepanes: $(if $(__deps),PKG@@mingw-w64-x86_64-gnustep-gui) ; @echo 'PKG@@mingw-w64-x86_64-libpreferencepanes'

override VERSION_OF_mingw-w64-x86_64-libprojectm := 4.1.8-1
.PHONY: PKG@@mingw-w64-x86_64-libprojectm
PKG@@mingw-w64-x86_64-libprojectm: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-glew) ; @echo 'PKG@@mingw-w64-x86_64-libprojectm'

override VERSION_OF_mingw-w64-x86_64-libproxy := 0.5.12-1
.PHONY: PKG@@mingw-w64-x86_64-libproxy
PKG@@mingw-w64-x86_64-libproxy: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-duktape PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gsettings-desktop-schemas) ; @echo 'PKG@@mingw-w64-x86_64-libproxy'

override VERSION_OF_mingw-w64-x86_64-libpsl := 0.21.5-3
.PHONY: PKG@@mingw-w64-x86_64-libpsl
PKG@@mingw-w64-x86_64-libpsl: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libidn2 PKG@@mingw-w64-x86_64-libunistring) ; @echo 'PKG@@mingw-w64-x86_64-libpsl'

override VERSION_OF_mingw-w64-x86_64-libpst := 0.6.76.r79.gcc600ee-1
.PHONY: PKG@@mingw-w64-x86_64-libpst
PKG@@mingw-w64-x86_64-libpst: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libgd PKG@@mingw-w64-x86_64-libgsf PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libsystre PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libpst'

override VERSION_OF_mingw-w64-x86_64-libquadmath := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-libquadmath
PKG@@mingw-w64-x86_64-libquadmath: $(if $(__deps),PKG@@mingw-w64-x86_64-libgcc) ; @echo 'PKG@@mingw-w64-x86_64-libquadmath'

override VERSION_OF_mingw-w64-x86_64-libraqm := 0.11.0-1
.PHONY: PKG@@mingw-w64-x86_64-libraqm
PKG@@mingw-w64-x86_64-libraqm: $(if $(__deps),PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-fribidi PKG@@mingw-w64-x86_64-harfbuzz) ; @echo 'PKG@@mingw-w64-x86_64-libraqm'

override VERSION_OF_mingw-w64-x86_64-libraw := 0.22.3-1
.PHONY: PKG@@mingw-w64-x86_64-libraw
PKG@@mingw-w64-x86_64-libraw: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-lcms2 PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libraw'

override VERSION_OF_mingw-w64-x86_64-librdkafka := 2.15.0-1
.PHONY: PKG@@mingw-w64-x86_64-librdkafka
PKG@@mingw-w64-x86_64-librdkafka: $(if $(__deps),PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-cyrus-sasl PKG@@mingw-w64-x86_64-lz4 PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-librdkafka'

override VERSION_OF_mingw-w64-x86_64-libreplaygain := 475-3
.PHONY: PKG@@mingw-w64-x86_64-libreplaygain
PKG@@mingw-w64-x86_64-libreplaygain: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libreplaygain'

override VERSION_OF_mingw-w64-x86_64-libresidfp := 1.2.2-2
.PHONY: PKG@@mingw-w64-x86_64-libresidfp
PKG@@mingw-w64-x86_64-libresidfp: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-libresidfp'

override VERSION_OF_mingw-w64-x86_64-librevenge := 0.0.6-1
.PHONY: PKG@@mingw-w64-x86_64-librevenge
PKG@@mingw-w64-x86_64-librevenge: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-librevenge'

override VERSION_OF_mingw-w64-x86_64-librime-data := 0.0.0.20251229-1
.PHONY: PKG@@mingw-w64-x86_64-librime-data
PKG@@mingw-w64-x86_64-librime-data: $(if $(__deps),PKG@@mingw-w64-x86_64-rime-bopomofo PKG@@mingw-w64-x86_64-rime-cangjie PKG@@mingw-w64-x86_64-rime-essay PKG@@mingw-w64-x86_64-rime-luna-pinyin PKG@@mingw-w64-x86_64-rime-prelude PKG@@mingw-w64-x86_64-rime-stroke PKG@@mingw-w64-x86_64-rime-terra-pinyin) ; @echo 'PKG@@mingw-w64-x86_64-librime-data'

override VERSION_OF_mingw-w64-x86_64-librsvg := 2.63.4-1
.PHONY: PKG@@mingw-w64-x86_64-librsvg
PKG@@mingw-w64-x86_64-librsvg: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-gdk-pixbuf2 PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-pango) ; @echo 'PKG@@mingw-w64-x86_64-librsvg'

override VERSION_OF_mingw-w64-x86_64-librsvg-docs := 2.63.4-1
.PHONY: PKG@@mingw-w64-x86_64-librsvg-docs
PKG@@mingw-w64-x86_64-librsvg-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-librsvg-docs'

override VERSION_OF_mingw-w64-x86_64-librttopo := 1.1.0-2
.PHONY: PKG@@mingw-w64-x86_64-librttopo
PKG@@mingw-w64-x86_64-librttopo: $(if $(__deps),PKG@@mingw-w64-x86_64-geos) ; @echo 'PKG@@mingw-w64-x86_64-librttopo'

override VERSION_OF_mingw-w64-x86_64-libsamplerate := 0.2.2-1
.PHONY: PKG@@mingw-w64-x86_64-libsamplerate
PKG@@mingw-w64-x86_64-libsamplerate: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libsamplerate'

override VERSION_OF_mingw-w64-x86_64-libsass := 3.6.6-2
.PHONY: PKG@@mingw-w64-x86_64-libsass
PKG@@mingw-w64-x86_64-libsass: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libsass'

override VERSION_OF_mingw-w64-x86_64-libsbml := 5.20.5-6
.PHONY: PKG@@mingw-w64-x86_64-libsbml
PKG@@mingw-w64-x86_64-libsbml: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libsbml'

override VERSION_OF_mingw-w64-x86_64-libsbsms := 2.3.0-2
.PHONY: PKG@@mingw-w64-x86_64-libsbsms
PKG@@mingw-w64-x86_64-libsbsms: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libsbsms'

override VERSION_OF_mingw-w64-x86_64-libsecret := 0.21.8.2-1
.PHONY: PKG@@mingw-w64-x86_64-libsecret
PKG@@mingw-w64-x86_64-libsecret: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libgcrypt) ; @echo 'PKG@@mingw-w64-x86_64-libsecret'

override VERSION_OF_mingw-w64-x86_64-libserdes := 8.0.0-4
.PHONY: PKG@@mingw-w64-x86_64-libserdes
PKG@@mingw-w64-x86_64-libserdes: $(if $(__deps),PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-jansson PKG@@mingw-w64-x86_64-libavro) ; @echo 'PKG@@mingw-w64-x86_64-libserdes'

override VERSION_OF_mingw-w64-x86_64-libshout := 2.4.6-6
.PHONY: PKG@@mingw-w64-x86_64-libshout
PKG@@mingw-w64-x86_64-libshout: $(if $(__deps),PKG@@mingw-w64-x86_64-libtheora PKG@@mingw-w64-x86_64-libvorbis PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-speex) ; @echo 'PKG@@mingw-w64-x86_64-libshout'

override VERSION_OF_mingw-w64-x86_64-libsignal-protocol-c := 2.3.3-4
.PHONY: PKG@@mingw-w64-x86_64-libsignal-protocol-c
PKG@@mingw-w64-x86_64-libsignal-protocol-c: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libsignal-protocol-c'

override VERSION_OF_mingw-w64-x86_64-libsmi := 0.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-libsmi
PKG@@mingw-w64-x86_64-libsmi: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libsmi'

override VERSION_OF_mingw-w64-x86_64-libsndfile := 1.2.2-1
.PHONY: PKG@@mingw-w64-x86_64-libsndfile
PKG@@mingw-w64-x86_64-libsndfile: $(if $(__deps),PKG@@mingw-w64-x86_64-flac PKG@@mingw-w64-x86_64-lame PKG@@mingw-w64-x86_64-libogg PKG@@mingw-w64-x86_64-libvorbis PKG@@mingw-w64-x86_64-mpg123 PKG@@mingw-w64-x86_64-opus) ; @echo 'PKG@@mingw-w64-x86_64-libsndfile'

override VERSION_OF_mingw-w64-x86_64-libsodium := 1.0.22-3
.PHONY: PKG@@mingw-w64-x86_64-libsodium
PKG@@mingw-w64-x86_64-libsodium: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libsodium'

override VERSION_OF_mingw-w64-x86_64-libsoup3 := 3.8.1-1
.PHONY: PKG@@mingw-w64-x86_64-libsoup3
PKG@@mingw-w64-x86_64-libsoup3: $(if $(__deps),PKG@@mingw-w64-x86_64-brotli PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-glib-networking PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libpsl PKG@@mingw-w64-x86_64-nghttp2 PKG@@mingw-w64-x86_64-sqlite3 PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-libsoup3'

override VERSION_OF_mingw-w64-x86_64-libsoup3-docs := 3.8.1-1
.PHONY: PKG@@mingw-w64-x86_64-libsoup3-docs
PKG@@mingw-w64-x86_64-libsoup3-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libsoup3-docs'

override VERSION_OF_mingw-w64-x86_64-libsoxr := 0.1.3-6
.PHONY: PKG@@mingw-w64-x86_64-libsoxr
PKG@@mingw-w64-x86_64-libsoxr: $(if $(__deps),PKG@@mingw-w64-x86_64-omp) ; @echo 'PKG@@mingw-w64-x86_64-libsoxr'

override VERSION_OF_mingw-w64-x86_64-libspatialite := 5.1.0-5
.PHONY: PKG@@mingw-w64-x86_64-libspatialite
PKG@@mingw-w64-x86_64-libspatialite: $(if $(__deps),PKG@@mingw-w64-x86_64-geos PKG@@mingw-w64-x86_64-libfreexl PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-librttopo PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-minizip PKG@@mingw-w64-x86_64-proj PKG@@mingw-w64-x86_64-sqlite3) ; @echo 'PKG@@mingw-w64-x86_64-libspatialite'

override VERSION_OF_mingw-w64-x86_64-libspiro := 1~20240903-1
.PHONY: PKG@@mingw-w64-x86_64-libspiro
PKG@@mingw-w64-x86_64-libspiro: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libspiro'

override VERSION_OF_mingw-w64-x86_64-libspng := 0.7.4-1
.PHONY: PKG@@mingw-w64-x86_64-libspng
PKG@@mingw-w64-x86_64-libspng: $(if $(__deps),PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libspng'

override VERSION_OF_mingw-w64-x86_64-libsquish := 1.15-5
.PHONY: PKG@@mingw-w64-x86_64-libsquish
PKG@@mingw-w64-x86_64-libsquish: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libsquish'

override VERSION_OF_mingw-w64-x86_64-libssh := 0.12.2-1
.PHONY: PKG@@mingw-w64-x86_64-libssh
PKG@@mingw-w64-x86_64-libssh: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libssh'

override VERSION_OF_mingw-w64-x86_64-libssh2 := 1.11.1-2
.PHONY: PKG@@mingw-w64-x86_64-libssh2
PKG@@mingw-w64-x86_64-libssh2: $(if $(__deps),PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libssh2'

override VERSION_OF_mingw-w64-x86_64-libssh2-wincng := 1.11.1-2
.PHONY: PKG@@mingw-w64-x86_64-libssh2-wincng
PKG@@mingw-w64-x86_64-libssh2-wincng: $(if $(__deps),PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libssh2-wincng'

override VERSION_OF_mingw-w64-x86_64-libstdc++ := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-libstdc++
PKG@@mingw-w64-x86_64-libstdc++: $(if $(__deps),PKG@@mingw-w64-x86_64-libgcc PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-tzdata) ; @echo 'PKG@@mingw-w64-x86_64-libstdc++'

override VERSION_OF_mingw-w64-x86_64-libstemmer := 3.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-libstemmer
PKG@@mingw-w64-x86_64-libstemmer: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libstemmer'

override VERSION_OF_mingw-w64-x86_64-libsystre := 1.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-libsystre
PKG@@mingw-w64-x86_64-libsystre: $(if $(__deps),PKG@@mingw-w64-x86_64-libtre) ; @echo 'PKG@@mingw-w64-x86_64-libsystre'

override VERSION_OF_mingw-w64-x86_64-libtasn1 := 4.21.0-1
.PHONY: PKG@@mingw-w64-x86_64-libtasn1
PKG@@mingw-w64-x86_64-libtasn1: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libtasn1'

override VERSION_OF_mingw-w64-x86_64-libtermkey := 0.22-2
.PHONY: PKG@@mingw-w64-x86_64-libtermkey
PKG@@mingw-w64-x86_64-libtermkey: $(if $(__deps),PKG@@mingw-w64-x86_64-unibilium) ; @echo 'PKG@@mingw-w64-x86_64-libtermkey'

override VERSION_OF_mingw-w64-x86_64-libthai := 0.1.30-1
.PHONY: PKG@@mingw-w64-x86_64-libthai
PKG@@mingw-w64-x86_64-libthai: $(if $(__deps),PKG@@mingw-w64-x86_64-libdatrie) ; @echo 'PKG@@mingw-w64-x86_64-libthai'

override VERSION_OF_mingw-w64-x86_64-libthai-docs := 0.1.30-1
.PHONY: PKG@@mingw-w64-x86_64-libthai-docs
PKG@@mingw-w64-x86_64-libthai-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libthai-docs'

override VERSION_OF_mingw-w64-x86_64-libtheora := 1.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-libtheora
PKG@@mingw-w64-x86_64-libtheora: $(if $(__deps),PKG@@mingw-w64-x86_64-libogg PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libvorbis) ; @echo 'PKG@@mingw-w64-x86_64-libtheora'

override VERSION_OF_mingw-w64-x86_64-libtiff := 4.7.2-2
.PHONY: PKG@@mingw-w64-x86_64-libtiff
PKG@@mingw-w64-x86_64-libtiff: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-jbigkit PKG@@mingw-w64-x86_64-lerc PKG@@mingw-w64-x86_64-libdeflate PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-libtiff'

override VERSION_OF_mingw-w64-x86_64-libtiff-docs := 4.7.2-2
.PHONY: PKG@@mingw-w64-x86_64-libtiff-docs
PKG@@mingw-w64-x86_64-libtiff-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libtiff-docs'

override VERSION_OF_mingw-w64-x86_64-libtimidity := 0.2.8-1
.PHONY: PKG@@mingw-w64-x86_64-libtimidity
PKG@@mingw-w64-x86_64-libtimidity: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libtimidity'

override VERSION_OF_mingw-w64-x86_64-libtomcrypt := 1.18.2-6
.PHONY: PKG@@mingw-w64-x86_64-libtomcrypt
PKG@@mingw-w64-x86_64-libtomcrypt: $(if $(__deps),PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-libtommath) ; @echo 'PKG@@mingw-w64-x86_64-libtomcrypt'

override VERSION_OF_mingw-w64-x86_64-libtommath := 1.3.0-2
.PHONY: PKG@@mingw-w64-x86_64-libtommath
PKG@@mingw-w64-x86_64-libtommath: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libtommath'

override VERSION_OF_mingw-w64-x86_64-libtool := 2.6.2-1
.PHONY: PKG@@mingw-w64-x86_64-libtool
PKG@@mingw-w64-x86_64-libtool: $(if $(__deps),PKG@@bash PKG@@mingw-w64-x86_64-libltdl) ; @echo 'PKG@@mingw-w64-x86_64-libtool'

override VERSION_OF_mingw-w64-x86_64-libtorrent-rasterbar := 2.1.2-1
.PHONY: PKG@@mingw-w64-x86_64-libtorrent-rasterbar
PKG@@mingw-w64-x86_64-libtorrent-rasterbar: $(if $(__deps),PKG@@mingw-w64-x86_64-boost PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-libtorrent-rasterbar'

override VERSION_OF_mingw-w64-x86_64-libtre := 0.9.0-2
.PHONY: PKG@@mingw-w64-x86_64-libtre
PKG@@mingw-w64-x86_64-libtre: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime) ; @echo 'PKG@@mingw-w64-x86_64-libtre'

override VERSION_OF_mingw-w64-x86_64-libu2f-emu := 0.0.0.r148.gd1c4b9c-1
.PHONY: PKG@@mingw-w64-x86_64-libu2f-emu
PKG@@mingw-w64-x86_64-libu2f-emu: $(if $(__deps),PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-libu2f-emu'

override VERSION_OF_mingw-w64-x86_64-libultrahdr := 1.5.1-1
.PHONY: PKG@@mingw-w64-x86_64-libultrahdr
PKG@@mingw-w64-x86_64-libultrahdr: $(if $(__deps),PKG@@mingw-w64-x86_64-angleproject PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-libultrahdr'

override VERSION_OF_mingw-w64-x86_64-libunibreak := 7.0-1
.PHONY: PKG@@mingw-w64-x86_64-libunibreak
PKG@@mingw-w64-x86_64-libunibreak: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libunibreak'

override VERSION_OF_mingw-w64-x86_64-libunifex := 0.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-libunifex
PKG@@mingw-w64-x86_64-libunifex: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libunifex'

override VERSION_OF_mingw-w64-x86_64-libuninameslist := 20260107-1
.PHONY: PKG@@mingw-w64-x86_64-libuninameslist
PKG@@mingw-w64-x86_64-libuninameslist: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libuninameslist'

override VERSION_OF_mingw-w64-x86_64-libunistring := 1.4.2-1
.PHONY: PKG@@mingw-w64-x86_64-libunistring
PKG@@mingw-w64-x86_64-libunistring: $(if $(__deps),PKG@@mingw-w64-x86_64-libiconv) ; @echo 'PKG@@mingw-w64-x86_64-libunistring'

override VERSION_OF_mingw-w64-x86_64-libunwind := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-libunwind
PKG@@mingw-w64-x86_64-libunwind: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libunwind'

override VERSION_OF_mingw-w64-x86_64-libusb-win32 := 1.4.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-libusb-win32
PKG@@mingw-w64-x86_64-libusb-win32: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libusb-win32'

override VERSION_OF_mingw-w64-x86_64-libusbmuxd := 2.1.1-1
.PHONY: PKG@@mingw-w64-x86_64-libusbmuxd
PKG@@mingw-w64-x86_64-libusbmuxd: $(if $(__deps),PKG@@mingw-w64-x86_64-libimobiledevice-glue PKG@@mingw-w64-x86_64-libplist) ; @echo 'PKG@@mingw-w64-x86_64-libusbmuxd'

override VERSION_OF_mingw-w64-x86_64-libutf8proc := 2.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-libutf8proc
PKG@@mingw-w64-x86_64-libutf8proc: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libutf8proc'

override VERSION_OF_mingw-w64-x86_64-libutp := r175.9cb9f9c-2
.PHONY: PKG@@mingw-w64-x86_64-libutp
PKG@@mingw-w64-x86_64-libutp: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libutp'

override VERSION_OF_mingw-w64-x86_64-libuv := 1.53.0-1
.PHONY: PKG@@mingw-w64-x86_64-libuv
PKG@@mingw-w64-x86_64-libuv: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libuv'

override VERSION_OF_mingw-w64-x86_64-libva := 2.24.1-1
.PHONY: PKG@@mingw-w64-x86_64-libva
PKG@@mingw-w64-x86_64-libva: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libva'

override VERSION_OF_mingw-w64-x86_64-libva-utils := 2.24.0-1
.PHONY: PKG@@mingw-w64-x86_64-libva-utils
PKG@@mingw-w64-x86_64-libva-utils: $(if $(__deps),PKG@@mingw-w64-x86_64-libva) ; @echo 'PKG@@mingw-w64-x86_64-libva-utils'

override VERSION_OF_mingw-w64-x86_64-libvisio := 0.1.11-2
.PHONY: PKG@@mingw-w64-x86_64-libvisio
PKG@@mingw-w64-x86_64-libvisio: $(if $(__deps),PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-librevenge PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-libvisio'

override VERSION_OF_mingw-w64-x86_64-libvoikko := 4.3.3-3
.PHONY: PKG@@mingw-w64-x86_64-libvoikko
PKG@@mingw-w64-x86_64-libvoikko: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-libvoikko'

override VERSION_OF_mingw-w64-x86_64-libvorbis := 1.3.7-3
.PHONY: PKG@@mingw-w64-x86_64-libvorbis
PKG@@mingw-w64-x86_64-libvorbis: $(if $(__deps),PKG@@mingw-w64-x86_64-libogg) ; @echo 'PKG@@mingw-w64-x86_64-libvorbis'

override VERSION_OF_mingw-w64-x86_64-libvpl := 2.17.0-1
.PHONY: PKG@@mingw-w64-x86_64-libvpl PKG@@mingw-w64-x86_64-onevpl
PKG@@mingw-w64-x86_64-libvpl PKG@@mingw-w64-x86_64-onevpl: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libvpl'

override VERSION_OF_mingw-w64-x86_64-libvpx := 1.17.0-1
.PHONY: PKG@@mingw-w64-x86_64-libvpx
PKG@@mingw-w64-x86_64-libvpx: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-libvpx'

override VERSION_OF_mingw-w64-x86_64-libwebm := 1.0.0.32-1
.PHONY: PKG@@mingw-w64-x86_64-libwebm
PKG@@mingw-w64-x86_64-libwebm: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libwebm'

override VERSION_OF_mingw-w64-x86_64-libwebp := 1.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-libsharpyuv PKG@@mingw-w64-x86_64-libwebp
PKG@@mingw-w64-x86_64-libsharpyuv PKG@@mingw-w64-x86_64-libwebp: $(if $(__deps),PKG@@mingw-w64-x86_64-giflib PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff) ; @echo 'PKG@@mingw-w64-x86_64-libwebp'

override VERSION_OF_mingw-w64-x86_64-libwebsockets := 4.5.8-1
.PHONY: PKG@@mingw-w64-x86_64-libwebsockets
PKG@@mingw-w64-x86_64-libwebsockets: $(if $(__deps),PKG@@mingw-w64-x86_64-libuv PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-libwebsockets'

override VERSION_OF_mingw-w64-x86_64-libwinpthread := 14.0.0.r598.ge3d18511f-1
.PHONY: PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-libwinpthread-git
PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-libwinpthread-git: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libwinpthread'

override VERSION_OF_mingw-w64-x86_64-libwmf := 0.2.16-1
.PHONY: PKG@@mingw-w64-x86_64-libwmf
PKG@@mingw-w64-x86_64-libwmf: $(if $(__deps),PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-gdk-pixbuf2 PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libwmf'

override VERSION_OF_mingw-w64-x86_64-libwpd := 0.10.3-3
.PHONY: PKG@@mingw-w64-x86_64-libwpd
PKG@@mingw-w64-x86_64-libwpd: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-librevenge) ; @echo 'PKG@@mingw-w64-x86_64-libwpd'

override VERSION_OF_mingw-w64-x86_64-libwpg := 0.3.4-2
.PHONY: PKG@@mingw-w64-x86_64-libwpg
PKG@@mingw-w64-x86_64-libwpg: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-librevenge PKG@@mingw-w64-x86_64-libwpd) ; @echo 'PKG@@mingw-w64-x86_64-libwpg'

override VERSION_OF_mingw-w64-x86_64-libx264 := 0.165.r3222.b35605a-3
.PHONY: PKG@@mingw-w64-x86_64-libx264
PKG@@mingw-w64-x86_64-libx264: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libx264'

override VERSION_OF_mingw-w64-x86_64-libxisf := 0.2.17-1
.PHONY: PKG@@mingw-w64-x86_64-libxisf
PKG@@mingw-w64-x86_64-libxisf: $(if $(__deps),PKG@@mingw-w64-x86_64-lz4 PKG@@mingw-w64-x86_64-pugixml PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-libxisf'

override VERSION_OF_mingw-w64-x86_64-libxlsxwriter := 1.2.4-1
.PHONY: PKG@@mingw-w64-x86_64-libxlsxwriter
PKG@@mingw-w64-x86_64-libxlsxwriter: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libxlsxwriter'

override VERSION_OF_mingw-w64-x86_64-libxml++-5.0 := 5.6.1-1
.PHONY: PKG@@mingw-w64-x86_64-libxml++-5.0
PKG@@mingw-w64-x86_64-libxml++-5.0: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-libxml++-5.0'

override VERSION_OF_mingw-w64-x86_64-libxml2 := 2.15.4-3
.PHONY: PKG@@mingw-w64-x86_64-libxml2
PKG@@mingw-w64-x86_64-libxml2: $(if $(__deps),PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-libxml2'

override VERSION_OF_mingw-w64-x86_64-libxmlb := 0.3.29-1
.PHONY: PKG@@mingw-w64-x86_64-libxmlb
PKG@@mingw-w64-x86_64-libxmlb: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-libxmlb'

override VERSION_OF_mingw-w64-x86_64-libxmp := 4.7.4-1
.PHONY: PKG@@mingw-w64-x86_64-libxmp
PKG@@mingw-w64-x86_64-libxmp: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libxmp'

override VERSION_OF_mingw-w64-x86_64-libxslt := 1.1.45-1
.PHONY: PKG@@mingw-w64-x86_64-libxslt
PKG@@mingw-w64-x86_64-libxslt: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-libxslt'

override VERSION_OF_mingw-w64-x86_64-libyaml := 0.2.5-2
.PHONY: PKG@@mingw-w64-x86_64-libyaml
PKG@@mingw-w64-x86_64-libyaml: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-libyaml'

override VERSION_OF_mingw-w64-x86_64-libyuv := 1924.r2921.644251f25-2
.PHONY: PKG@@mingw-w64-x86_64-libyuv
PKG@@mingw-w64-x86_64-libyuv: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libjpeg-turbo) ; @echo 'PKG@@mingw-w64-x86_64-libyuv'

override VERSION_OF_mingw-w64-x86_64-libzen := 0.4.41-3
.PHONY: PKG@@mingw-w64-x86_64-libzen
PKG@@mingw-w64-x86_64-libzen: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-libzen'

override VERSION_OF_mingw-w64-x86_64-libzip := 1.12.1-1
.PHONY: PKG@@mingw-w64-x86_64-libzip
PKG@@mingw-w64-x86_64-libzip: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-gnutls PKG@@mingw-w64-x86_64-nettle PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-libzip'

override VERSION_OF_mingw-w64-x86_64-lis := 2.1.23-1
.PHONY: PKG@@mingw-w64-x86_64-lis
PKG@@mingw-w64-x86_64-lis: $(if $(__deps),PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-omp) ; @echo 'PKG@@mingw-w64-x86_64-lis'

override VERSION_OF_mingw-w64-x86_64-litehtml := 0.10-2
.PHONY: PKG@@mingw-w64-x86_64-litehtml
PKG@@mingw-w64-x86_64-litehtml: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gumbo-parser) ; @echo 'PKG@@mingw-w64-x86_64-litehtml'

override VERSION_OF_mingw-w64-x86_64-llama.cpp := 1~0.9.0-1
.PHONY: PKG@@mingw-w64-x86_64-llama.cpp
PKG@@mingw-w64-x86_64-llama.cpp: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-ggml PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-llama.cpp'

override VERSION_OF_mingw-w64-x86_64-lld := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-lld
PKG@@mingw-w64-x86_64-lld: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-llvm-libs PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-lld'

override VERSION_OF_mingw-w64-x86_64-lldb := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-lldb
PKG@@mingw-w64-x86_64-lldb: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-clang-libs PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-llvm-libs PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-lldb'

override VERSION_OF_mingw-w64-x86_64-lldb-mi := r102.1aa9e8f-2
.PHONY: PKG@@mingw-w64-x86_64-lldb-mi
PKG@@mingw-w64-x86_64-lldb-mi: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-lldb PKG@@mingw-w64-x86_64-llvm-libs) ; @echo 'PKG@@mingw-w64-x86_64-lldb-mi'

override VERSION_OF_mingw-w64-x86_64-llvm := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-llvm
PKG@@mingw-w64-x86_64-llvm: $(if $(__deps),PKG@@mingw-w64-x86_64-llvm-libs PKG@@mingw-w64-x86_64-llvm-tools) ; @echo 'PKG@@mingw-w64-x86_64-llvm'

override VERSION_OF_mingw-w64-x86_64-llvm-libs := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-llvm-libs
PKG@@mingw-w64-x86_64-llvm-libs: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libffi PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-llvm-libs'

override VERSION_OF_mingw-w64-x86_64-llvm-tools := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-llvm-tools
PKG@@mingw-w64-x86_64-llvm-tools: $(if $(__deps),PKG@@mingw-w64-x86_64-lld PKG@@mingw-w64-x86_64-llvm-libs) ; @echo 'PKG@@mingw-w64-x86_64-llvm-tools'

override VERSION_OF_mingw-w64-x86_64-lsp-dsp-lib := 1.0.29-1
.PHONY: PKG@@mingw-w64-x86_64-lsp-dsp-lib
PKG@@mingw-w64-x86_64-lsp-dsp-lib: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-lsp-dsp-lib'

override VERSION_OF_mingw-w64-x86_64-lto-dump := 16.2.0-6
.PHONY: PKG@@mingw-w64-x86_64-gcc-lto-dump PKG@@mingw-w64-x86_64-lto-dump
PKG@@mingw-w64-x86_64-gcc-lto-dump PKG@@mingw-w64-x86_64-lto-dump: $(if $(__deps),PKG@@mingw-w64-x86_64-gcc PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-isl PKG@@mingw-w64-x86_64-libgcc PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-mpc PKG@@mingw-w64-x86_64-mpfr PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-lto-dump'

override VERSION_OF_mingw-w64-x86_64-lua := 5.5.1-1
.PHONY: PKG@@mingw-w64-x86_64-lua
PKG@@mingw-w64-x86_64-lua: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-lua'

override VERSION_OF_mingw-w64-x86_64-lua51 := 5.1.5-6
.PHONY: PKG@@mingw-w64-x86_64-lua51
PKG@@mingw-w64-x86_64-lua51: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-lua51'

override VERSION_OF_mingw-w64-x86_64-lua53 := 5.3.6-6
.PHONY: PKG@@mingw-w64-x86_64-lua53
PKG@@mingw-w64-x86_64-lua53: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-lua53'

override VERSION_OF_mingw-w64-x86_64-lua54 := 5.4.9-1
.PHONY: PKG@@mingw-w64-x86_64-lua54
PKG@@mingw-w64-x86_64-lua54: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-lua54'

override VERSION_OF_mingw-w64-x86_64-lz4 := 1.11.0-1
.PHONY: PKG@@mingw-w64-x86_64-lz4
PKG@@mingw-w64-x86_64-lz4: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-lz4'

override VERSION_OF_mingw-w64-x86_64-lzo2 := 2.10-3
.PHONY: PKG@@mingw-w64-x86_64-lzo2
PKG@@mingw-w64-x86_64-lzo2: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-lzo2'

override VERSION_OF_mingw-w64-x86_64-make := 4.4.1-5
.PHONY: PKG@@mingw-w64-x86_64-make
PKG@@mingw-w64-x86_64-make: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime) ; @echo 'PKG@@mingw-w64-x86_64-make'

override VERSION_OF_mingw-w64-x86_64-mathjax2 := 2.7.9-1
.PHONY: PKG@@mingw-w64-x86_64-mathjax2
PKG@@mingw-w64-x86_64-mathjax2: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-mathjax2'

override VERSION_OF_mingw-w64-x86_64-matio := 1.5.30-1
.PHONY: PKG@@mingw-w64-x86_64-matio
PKG@@mingw-w64-x86_64-matio: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-hdf5 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-matio'

override VERSION_OF_mingw-w64-x86_64-maturin := 1.16.0-1
.PHONY: PKG@@mingw-w64-x86_64-maturin
PKG@@mingw-w64-x86_64-maturin: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-maturin'

override VERSION_OF_mingw-w64-x86_64-maxflow := 3.0.5-3
.PHONY: PKG@@mingw-w64-x86_64-maxflow
PKG@@mingw-w64-x86_64-maxflow: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-maxflow'

override VERSION_OF_mingw-w64-x86_64-mbedtls := 3.6.7-1
.PHONY: PKG@@mingw-w64-x86_64-mbedtls
PKG@@mingw-w64-x86_64-mbedtls: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-mbedtls'

override VERSION_OF_mingw-w64-x86_64-mcfgthread := 2.5.1-1
.PHONY: PKG@@mingw-w64-x86_64-mcfgthread
PKG@@mingw-w64-x86_64-mcfgthread: $(if $(__deps),PKG@@mingw-w64-x86_64-mcfgthread-libs) ; @echo 'PKG@@mingw-w64-x86_64-mcfgthread'

override VERSION_OF_mingw-w64-x86_64-mcfgthread-libs := 2.5.1-1
.PHONY: PKG@@mingw-w64-x86_64-mcfgthread-libs
PKG@@mingw-w64-x86_64-mcfgthread-libs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-mcfgthread-libs'

override VERSION_OF_mingw-w64-x86_64-md4c := 0.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-md4c
PKG@@mingw-w64-x86_64-md4c: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-md4c'

override VERSION_OF_mingw-w64-x86_64-mdbook := 0.5.4-1
.PHONY: PKG@@mingw-w64-x86_64-mdbook
PKG@@mingw-w64-x86_64-mdbook: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-mdbook'

override VERSION_OF_mingw-w64-x86_64-mdbook-admonish := 1.20.0-1
.PHONY: PKG@@mingw-w64-x86_64-mdbook-admonish
PKG@@mingw-w64-x86_64-mdbook-admonish: $(if $(__deps),PKG@@mingw-w64-x86_64-mdbook) ; @echo 'PKG@@mingw-w64-x86_64-mdbook-admonish'

override VERSION_OF_mingw-w64-x86_64-mdbook-mermaid := 0.17.1-1
.PHONY: PKG@@mingw-w64-x86_64-mdbook-mermaid
PKG@@mingw-w64-x86_64-mdbook-mermaid: $(if $(__deps),PKG@@mingw-w64-x86_64-mdbook) ; @echo 'PKG@@mingw-w64-x86_64-mdbook-mermaid'

override VERSION_OF_mingw-w64-x86_64-mdbook-pikchr := 0.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-mdbook-pikchr
PKG@@mingw-w64-x86_64-mdbook-pikchr: $(if $(__deps),PKG@@mingw-w64-x86_64-mdbook) ; @echo 'PKG@@mingw-w64-x86_64-mdbook-pikchr'

override VERSION_OF_mingw-w64-x86_64-mdloader := 1.0.7-1
.PHONY: PKG@@mingw-w64-x86_64-mdloader
PKG@@mingw-w64-x86_64-mdloader: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-mdloader'

override VERSION_OF_mingw-w64-x86_64-meanwhile := 1.1.1-2
.PHONY: PKG@@mingw-w64-x86_64-meanwhile
PKG@@mingw-w64-x86_64-meanwhile: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-glib2) ; @echo 'PKG@@mingw-w64-x86_64-meanwhile'

override VERSION_OF_mingw-w64-x86_64-mecab := 0.996-7
.PHONY: PKG@@mingw-w64-x86_64-mecab
PKG@@mingw-w64-x86_64-mecab: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libiconv) ; @echo 'PKG@@mingw-w64-x86_64-mecab'

override VERSION_OF_mingw-w64-x86_64-mesa := 26.2.6-1
.PHONY: PKG@@mingw-w64-x86_64-mesa
PKG@@mingw-w64-x86_64-mesa: $(if $(__deps),PKG@@mingw-w64-x86_64-clang-libs PKG@@mingw-w64-x86_64-libsystre PKG@@mingw-w64-x86_64-libva PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-spirv-llvm-translator PKG@@mingw-w64-x86_64-spirv-tools PKG@@mingw-w64-x86_64-vulkan-loader PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-mesa'

override VERSION_OF_mingw-w64-x86_64-meson := 1.12.1-2
.PHONY: PKG@@mingw-w64-x86_64-meson
PKG@@mingw-w64-x86_64-meson: $(if $(__deps),PKG@@mingw-w64-x86_64-ninja PKG@@mingw-w64-x86_64-pkgconf PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-meson'

override VERSION_OF_mingw-w64-x86_64-meson-python := 0.22.1-1
.PHONY: PKG@@mingw-w64-x86_64-meson-python
PKG@@mingw-w64-x86_64-meson-python: $(if $(__deps),PKG@@mingw-w64-x86_64-meson PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-pyproject-metadata) ; @echo 'PKG@@mingw-w64-x86_64-meson-python'

override VERSION_OF_mingw-w64-x86_64-metis := 5.1.0-7
.PHONY: PKG@@mingw-w64-x86_64-metis
PKG@@mingw-w64-x86_64-metis: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-metis'

override VERSION_OF_mingw-w64-x86_64-minisign := 0.12-1
.PHONY: PKG@@mingw-w64-x86_64-minisign
PKG@@mingw-w64-x86_64-minisign: $(if $(__deps),PKG@@mingw-w64-x86_64-libsodium) ; @echo 'PKG@@mingw-w64-x86_64-minisign'

override VERSION_OF_mingw-w64-x86_64-miniupnpc := 2.3.3-4
.PHONY: PKG@@mingw-w64-x86_64-miniupnpc
PKG@@mingw-w64-x86_64-miniupnpc: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-miniupnpc'

override VERSION_OF_mingw-w64-x86_64-minizip := 1.4-2
.PHONY: PKG@@mingw-w64-x86_64-minizip
PKG@@mingw-w64-x86_64-minizip: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-minizip'

override VERSION_OF_mingw-w64-x86_64-minizip-ng := 4.2.2-1
.PHONY: PKG@@mingw-w64-x86_64-minizip-ng
PKG@@mingw-w64-x86_64-minizip-ng: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-minizip-ng'

override VERSION_OF_mingw-w64-x86_64-mman-win32 := r31.fedbdbf-1
.PHONY: PKG@@mingw-w64-x86_64-mman-win32
PKG@@mingw-w64-x86_64-mman-win32: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-mman-win32'

override VERSION_OF_mingw-w64-x86_64-moab := 5.6.0-2
.PHONY: PKG@@mingw-w64-x86_64-moab
PKG@@mingw-w64-x86_64-moab: $(if $(__deps),PKG@@mingw-w64-x86_64-msmpi PKG@@mingw-w64-x86_64-openblas) ; @echo 'PKG@@mingw-w64-x86_64-moab'

override VERSION_OF_mingw-w64-x86_64-mongo-c-driver := 2.5.8-1
.PHONY: PKG@@mingw-w64-x86_64-mongo-c-driver
PKG@@mingw-w64-x86_64-mongo-c-driver: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libutf8proc PKG@@mingw-w64-x86_64-snappy PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-mongo-c-driver'

override VERSION_OF_mingw-w64-x86_64-mpc := 1.4.1-1
.PHONY: PKG@@mingw-w64-x86_64-mpc
PKG@@mingw-w64-x86_64-mpc: $(if $(__deps),PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-mpfr) ; @echo 'PKG@@mingw-w64-x86_64-mpc'

override VERSION_OF_mingw-w64-x86_64-mpdecimal := 4.0.1-3
.PHONY: PKG@@mingw-w64-x86_64-mpdecimal
PKG@@mingw-w64-x86_64-mpdecimal: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-mpdecimal'

override VERSION_OF_mingw-w64-x86_64-mpfr := 4.2.2-3
.PHONY: PKG@@mingw-w64-x86_64-mpfr
PKG@@mingw-w64-x86_64-mpfr: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gmp) ; @echo 'PKG@@mingw-w64-x86_64-mpfr'

override VERSION_OF_mingw-w64-x86_64-mpg123 := 1.33.7-1
.PHONY: PKG@@mingw-w64-x86_64-mpg123
PKG@@mingw-w64-x86_64-mpg123: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-mpg123'

override VERSION_OF_mingw-w64-x86_64-mruby := 3.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-mruby
PKG@@mingw-w64-x86_64-mruby: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-mruby'

override VERSION_OF_mingw-w64-x86_64-mscgen := 0.20-3
.PHONY: PKG@@mingw-w64-x86_64-mscgen
PKG@@mingw-w64-x86_64-mscgen: $(if $(__deps),PKG@@mingw-w64-x86_64-libgd) ; @echo 'PKG@@mingw-w64-x86_64-mscgen'

override VERSION_OF_mingw-w64-x86_64-msgpack-c := 7.0.2-1
.PHONY: PKG@@mingw-w64-x86_64-msgpack-c
PKG@@mingw-w64-x86_64-msgpack-c: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-msgpack-c'

override VERSION_OF_mingw-w64-x86_64-msitools := 0.106-4
.PHONY: PKG@@mingw-w64-x86_64-msitools
PKG@@mingw-w64-x86_64-msitools: $(if $(__deps),PKG@@mingw-w64-x86_64-gcab PKG@@mingw-w64-x86_64-libgsf) ; @echo 'PKG@@mingw-w64-x86_64-msitools'

override VERSION_OF_mingw-w64-x86_64-msmpi := 10.1.1-27
.PHONY: PKG@@mingw-w64-x86_64-msmpi
PKG@@mingw-w64-x86_64-msmpi: $(if $(__deps),PKG@@mingw-w64-x86_64-libuv) ; @echo 'PKG@@mingw-w64-x86_64-msmpi'

override VERSION_OF_mingw-w64-x86_64-mtex2MML := 1.3.1-3
.PHONY: PKG@@mingw-w64-x86_64-mtex2MML
PKG@@mingw-w64-x86_64-mtex2MML: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-mtex2MML'

override VERSION_OF_mingw-w64-x86_64-mtn := 3.5.0-4
.PHONY: PKG@@mingw-w64-x86_64-mtn
PKG@@mingw-w64-x86_64-mtn: $(if $(__deps),PKG@@mingw-w64-x86_64-ffmpeg PKG@@mingw-w64-x86_64-libgd) ; @echo 'PKG@@mingw-w64-x86_64-mtn'

override VERSION_OF_mingw-w64-x86_64-mtools := 4.0.50-1
.PHONY: PKG@@mingw-w64-x86_64-mtools
PKG@@mingw-w64-x86_64-mtools: $(if $(__deps),PKG@@mingw-w64-x86_64-libiconv) ; @echo 'PKG@@mingw-w64-x86_64-mtools'

override VERSION_OF_mingw-w64-x86_64-muparser := 2.3.5-2
.PHONY: PKG@@mingw-w64-x86_64-muparser
PKG@@mingw-w64-x86_64-muparser: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-omp) ; @echo 'PKG@@mingw-w64-x86_64-muparser'

override VERSION_OF_mingw-w64-x86_64-mxml := 3.3.1-4
.PHONY: PKG@@mingw-w64-x86_64-mxml
PKG@@mingw-w64-x86_64-mxml: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-mxml'

override VERSION_OF_mingw-w64-x86_64-mypy := 2.4.1-1
.PHONY: PKG@@mingw-w64-x86_64-mypy
PKG@@mingw-w64-x86_64-mypy: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-ast-serialize PKG@@mingw-w64-x86_64-python-librt PKG@@mingw-w64-x86_64-python-mypy_extensions PKG@@mingw-w64-x86_64-python-pathspec PKG@@mingw-w64-x86_64-python-typing_extensions) ; @echo 'PKG@@mingw-w64-x86_64-mypy'

override VERSION_OF_mingw-w64-x86_64-nanobind := 2.15.0-1
.PHONY: PKG@@mingw-w64-x86_64-nanobind
PKG@@mingw-w64-x86_64-nanobind: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-nanobind'

override VERSION_OF_mingw-w64-x86_64-nasm := 3.02-1
.PHONY: PKG@@mingw-w64-x86_64-nasm
PKG@@mingw-w64-x86_64-nasm: $(if $(__deps),PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-nasm'

override VERSION_OF_mingw-w64-x86_64-ncurses := 6.6-4
.PHONY: PKG@@mingw-w64-x86_64-ncurses
PKG@@mingw-w64-x86_64-ncurses: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libsystre PKG@@mingw-w64-x86_64-pcre2) ; @echo 'PKG@@mingw-w64-x86_64-ncurses'

override VERSION_OF_mingw-w64-x86_64-netcdf := 4.10.1-4
.PHONY: PKG@@mingw-w64-x86_64-netcdf
PKG@@mingw-w64-x86_64-netcdf: $(if $(__deps),PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-hdf5 PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-libzip) ; @echo 'PKG@@mingw-w64-x86_64-netcdf'

override VERSION_OF_mingw-w64-x86_64-netcdf-cxx := 4.3.1-7
.PHONY: PKG@@mingw-w64-x86_64-netcdf-cxx
PKG@@mingw-w64-x86_64-netcdf-cxx: $(if $(__deps),PKG@@mingw-w64-x86_64-hdf5 PKG@@mingw-w64-x86_64-netcdf PKG@@sed) ; @echo 'PKG@@mingw-w64-x86_64-netcdf-cxx'

override VERSION_OF_mingw-w64-x86_64-netcdf-fortran := 4.6.4-1
.PHONY: PKG@@mingw-w64-x86_64-netcdf-fortran
PKG@@mingw-w64-x86_64-netcdf-fortran: $(if $(__deps),PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-netcdf) ; @echo 'PKG@@mingw-w64-x86_64-netcdf-fortran'

override VERSION_OF_mingw-w64-x86_64-nettle := 4.0-1
.PHONY: PKG@@mingw-w64-x86_64-nettle
PKG@@mingw-w64-x86_64-nettle: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gmp) ; @echo 'PKG@@mingw-w64-x86_64-nettle'

override VERSION_OF_mingw-w64-x86_64-ng-log := 0.8.2-5
.PHONY: PKG@@mingw-w64-x86_64-ng-log
PKG@@mingw-w64-x86_64-ng-log: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gflags PKG@@mingw-w64-x86_64-libunwind PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-ng-log'

override VERSION_OF_mingw-w64-x86_64-nghttp2 := 1.71.0-1
.PHONY: PKG@@mingw-w64-x86_64-nghttp2
PKG@@mingw-w64-x86_64-nghttp2: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-nghttp2'

override VERSION_OF_mingw-w64-x86_64-nghttp3 := 1.19.0-1
.PHONY: PKG@@mingw-w64-x86_64-nghttp3
PKG@@mingw-w64-x86_64-nghttp3: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-nghttp3'

override VERSION_OF_mingw-w64-x86_64-ngtcp2 := 1.26.0-2
.PHONY: PKG@@mingw-w64-x86_64-ngtcp2
PKG@@mingw-w64-x86_64-ngtcp2: $(if $(__deps),PKG@@mingw-w64-x86_64-gnutls PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-ngtcp2'

override VERSION_OF_mingw-w64-x86_64-nim := 2.2.14-1
.PHONY: PKG@@mingw-w64-x86_64-nim
PKG@@mingw-w64-x86_64-nim: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-nim'

override VERSION_OF_mingw-w64-x86_64-ninja := 1.13.2-1
.PHONY: PKG@@mingw-w64-x86_64-ninja
PKG@@mingw-w64-x86_64-ninja: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-ninja'

override VERSION_OF_mingw-w64-x86_64-nlohmann-json := 3.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-nlohmann-json PKG@@mingw-w64-x86_64-nlohmann_json
PKG@@mingw-w64-x86_64-nlohmann-json PKG@@mingw-w64-x86_64-nlohmann_json: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-nlohmann-json'

override VERSION_OF_mingw-w64-x86_64-nlopt := 2.11.0-1
.PHONY: PKG@@mingw-w64-x86_64-nlopt
PKG@@mingw-w64-x86_64-nlopt: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-nlopt'

override VERSION_OF_mingw-w64-x86_64-npth := 1.8-2
.PHONY: PKG@@mingw-w64-x86_64-npth
PKG@@mingw-w64-x86_64-npth: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-npth'

override VERSION_OF_mingw-w64-x86_64-nsis := 3.13-2
.PHONY: PKG@@mingw-w64-x86_64-nsis
PKG@@mingw-w64-x86_64-nsis: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-nsis'

override VERSION_OF_mingw-w64-x86_64-nspr := 4.40-1
.PHONY: PKG@@mingw-w64-x86_64-nspr
PKG@@mingw-w64-x86_64-nspr: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-nspr'

override VERSION_OF_mingw-w64-x86_64-nss := 3.133-1
.PHONY: PKG@@mingw-w64-x86_64-nss
PKG@@mingw-w64-x86_64-nss: $(if $(__deps),PKG@@mingw-w64-x86_64-nspr PKG@@mingw-w64-x86_64-sqlite PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-nss'

override VERSION_OF_mingw-w64-x86_64-ntldd := r19.7fb9365-4
.PHONY: PKG@@mingw-w64-x86_64-ntldd PKG@@mingw-w64-x86_64-ntldd-git
PKG@@mingw-w64-x86_64-ntldd PKG@@mingw-w64-x86_64-ntldd-git: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-ntldd'

override VERSION_OF_mingw-w64-x86_64-nuspell := 5.1.9-1
.PHONY: PKG@@mingw-w64-x86_64-nuspell
PKG@@mingw-w64-x86_64-nuspell: $(if $(__deps),PKG@@mingw-w64-x86_64-icu) ; @echo 'PKG@@mingw-w64-x86_64-nuspell'

override VERSION_OF_mingw-w64-x86_64-ogre3d := 14.6.3-1
.PHONY: PKG@@mingw-w64-x86_64-ogre3d
PKG@@mingw-w64-x86_64-ogre3d: $(if $(__deps),PKG@@mingw-w64-x86_64-SDL2 PKG@@mingw-w64-x86_64-bullet PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-glsl-optimizer PKG@@mingw-w64-x86_64-hlsl2glsl PKG@@mingw-w64-x86_64-tinyxml PKG@@mingw-w64-x86_64-winpthreads PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-ogre3d'

override VERSION_OF_mingw-w64-x86_64-openal := 1.25.2-1
.PHONY: PKG@@mingw-w64-x86_64-openal
PKG@@mingw-w64-x86_64-openal: $(if $(__deps),PKG@@mingw-w64-x86_64-libmysofa) ; @echo 'PKG@@mingw-w64-x86_64-openal'

override VERSION_OF_mingw-w64-x86_64-openblas := 0.3.34-1
.PHONY: PKG@@mingw-w64-x86_64-openblas
PKG@@mingw-w64-x86_64-openblas: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-omp) ; @echo 'PKG@@mingw-w64-x86_64-openblas'

override VERSION_OF_mingw-w64-x86_64-openblas64 := 0.3.34-1
.PHONY: PKG@@mingw-w64-x86_64-openblas64
PKG@@mingw-w64-x86_64-openblas64: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-omp) ; @echo 'PKG@@mingw-w64-x86_64-openblas64'

override VERSION_OF_mingw-w64-x86_64-opencascade := 7.9.3-3
.PHONY: PKG@@mingw-w64-x86_64-opencascade
PKG@@mingw-w64-x86_64-opencascade: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-ffmpeg PKG@@mingw-w64-x86_64-freeimage PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-openvr PKG@@mingw-w64-x86_64-tbb PKG@@mingw-w64-x86_64-tcl PKG@@mingw-w64-x86_64-tk PKG@@mingw-w64-x86_64-vtk) ; @echo 'PKG@@mingw-w64-x86_64-opencascade'

override VERSION_OF_mingw-w64-x86_64-opencascade-doc-overview := 7.9.3-3
.PHONY: PKG@@mingw-w64-x86_64-opencascade-doc-overview
PKG@@mingw-w64-x86_64-opencascade-doc-overview: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-opencascade-doc-overview'

override VERSION_OF_mingw-w64-x86_64-opencascade-doc-refman := 7.9.3-3
.PHONY: PKG@@mingw-w64-x86_64-opencascade-doc-refman
PKG@@mingw-w64-x86_64-opencascade-doc-refman: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-opencascade-doc-refman'

override VERSION_OF_mingw-w64-x86_64-opencl-headers := 2~2026.05.29-1
.PHONY: PKG@@mingw-w64-x86_64-opencl-headers
PKG@@mingw-w64-x86_64-opencl-headers: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-opencl-headers'

override VERSION_OF_mingw-w64-x86_64-opencl-icd := 2026.05.29-1
.PHONY: PKG@@mingw-w64-x86_64-opencl-icd
PKG@@mingw-w64-x86_64-opencl-icd: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-opencl-icd'

override VERSION_OF_mingw-w64-x86_64-opencore-amr := 0.1.6-1
.PHONY: PKG@@mingw-w64-x86_64-opencore-amr
PKG@@mingw-w64-x86_64-opencore-amr: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-opencore-amr'

override VERSION_OF_mingw-w64-x86_64-opencsg := 1.8.2-1
.PHONY: PKG@@mingw-w64-x86_64-opencsg
PKG@@mingw-w64-x86_64-opencsg: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-opencsg'

override VERSION_OF_mingw-w64-x86_64-opencv := 5.0.0-8
.PHONY: PKG@@mingw-w64-x86_64-opencv
PKG@@mingw-w64-x86_64-opencv: $(if $(__deps),PKG@@mingw-w64-x86_64-abseil-cpp PKG@@mingw-w64-x86_64-ceres-solver PKG@@mingw-w64-x86_64-ffmpeg PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-gflags PKG@@mingw-w64-x86_64-glog PKG@@mingw-w64-x86_64-gst-plugins-base PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-hdf5 PKG@@mingw-w64-x86_64-jasper PKG@@mingw-w64-x86_64-libavif PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-ogre3d PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-openblas PKG@@mingw-w64-x86_64-openexr PKG@@mingw-w64-x86_64-openjpeg2 PKG@@mingw-w64-x86_64-protobuf PKG@@mingw-w64-x86_64-tbb PKG@@mingw-w64-x86_64-tesseract-ocr PKG@@mingw-w64-x86_64-vulkan-loader PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-opencv'

override VERSION_OF_mingw-w64-x86_64-openexr := 3.5.3-1
.PHONY: PKG@@mingw-w64-x86_64-openexr
PKG@@mingw-w64-x86_64-openexr: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-imath PKG@@mingw-w64-x86_64-libdeflate PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-openjph PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-openexr'

override VERSION_OF_mingw-w64-x86_64-openfa := 20231011.0.2-2
.PHONY: PKG@@mingw-w64-x86_64-openfa
PKG@@mingw-w64-x86_64-openfa: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-openfa'

override VERSION_OF_mingw-w64-x86_64-opengl-man-pages := 20261006-1
.PHONY: PKG@@mingw-w64-x86_64-opengl-man-pages
PKG@@mingw-w64-x86_64-opengl-man-pages: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-opengl-man-pages'

override VERSION_OF_mingw-w64-x86_64-openh264 := 2.7.0-1
.PHONY: PKG@@mingw-w64-x86_64-openh264
PKG@@mingw-w64-x86_64-openh264: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-openh264'

override VERSION_OF_mingw-w64-x86_64-openjpeg2 := 2.5.4-2
.PHONY: PKG@@mingw-w64-x86_64-openjpeg2
PKG@@mingw-w64-x86_64-openjpeg2: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-lcms2 PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-openjpeg2'

override VERSION_OF_mingw-w64-x86_64-openjph := 0.32.0-1
.PHONY: PKG@@mingw-w64-x86_64-openjph
PKG@@mingw-w64-x86_64-openjph: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libtiff) ; @echo 'PKG@@mingw-w64-x86_64-openjph'

override VERSION_OF_mingw-w64-x86_64-openlibm := 0.8.7-1
.PHONY: PKG@@mingw-w64-x86_64-openlibm
PKG@@mingw-w64-x86_64-openlibm: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-openlibm'

override VERSION_OF_mingw-w64-x86_64-openslide := 4.0.1-1
.PHONY: PKG@@mingw-w64-x86_64-openslide
PKG@@mingw-w64-x86_64-openslide: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libdicom PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-openjpeg2 PKG@@mingw-w64-x86_64-sqlite3 PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-openslide'

override VERSION_OF_mingw-w64-x86_64-openssl := 3.6.5-1
.PHONY: PKG@@mingw-w64-x86_64-openssl
PKG@@mingw-w64-x86_64-openssl: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-openssl'

override VERSION_OF_mingw-w64-x86_64-openturns := 1.27-1
.PHONY: PKG@@mingw-w64-x86_64-openturns
PKG@@mingw-w64-x86_64-openturns: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-hdf5 PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-mpc PKG@@mingw-w64-x86_64-mpfr PKG@@mingw-w64-x86_64-muparser PKG@@mingw-w64-x86_64-nlopt PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-openblas PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-dill PKG@@mingw-w64-x86_64-python-psutil PKG@@mingw-w64-x86_64-tbb) ; @echo 'PKG@@mingw-w64-x86_64-openturns'

override VERSION_OF_mingw-w64-x86_64-openvdb := 13.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-openvdb
PKG@@mingw-w64-x86_64-openvdb: $(if $(__deps),PKG@@mingw-w64-x86_64-blosc PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-imath PKG@@mingw-w64-x86_64-jemalloc PKG@@mingw-w64-x86_64-tbb PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-openvdb'

override VERSION_OF_mingw-w64-x86_64-openvr := 2.15.6-1
.PHONY: PKG@@mingw-w64-x86_64-openvr
PKG@@mingw-w64-x86_64-openvr: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-openvr'

override VERSION_OF_mingw-w64-x86_64-openxr-sdk := 1.1.62-1
.PHONY: PKG@@mingw-w64-x86_64-openxr-sdk
PKG@@mingw-w64-x86_64-openxr-sdk: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-openxr-sdk'

override VERSION_OF_mingw-w64-x86_64-optipng := 7.9.1-1
.PHONY: PKG@@mingw-w64-x86_64-optipng
PKG@@mingw-w64-x86_64-optipng: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-optipng'

override VERSION_OF_mingw-w64-x86_64-opus := 1.6.1-2
.PHONY: PKG@@mingw-w64-x86_64-opus
PKG@@mingw-w64-x86_64-opus: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-opus'

override VERSION_OF_mingw-w64-x86_64-opusfile := 0.12-4
.PHONY: PKG@@mingw-w64-x86_64-opusfile
PKG@@mingw-w64-x86_64-opusfile: $(if $(__deps),PKG@@mingw-w64-x86_64-libogg PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-opus) ; @echo 'PKG@@mingw-w64-x86_64-opusfile'

override VERSION_OF_mingw-w64-x86_64-oqsprovider := 0.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-oqsprovider
PKG@@mingw-w64-x86_64-oqsprovider: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-oqsprovider'

override VERSION_OF_mingw-w64-x86_64-orc := 0.4.44-1
.PHONY: PKG@@mingw-w64-x86_64-orc
PKG@@mingw-w64-x86_64-orc: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-orc'

override VERSION_OF_mingw-w64-x86_64-osl := 0.9.7-1
.PHONY: PKG@@mingw-w64-x86_64-osl
PKG@@mingw-w64-x86_64-osl: $(if $(__deps),PKG@@mingw-w64-x86_64-gmp) ; @echo 'PKG@@mingw-w64-x86_64-osl'

override VERSION_OF_mingw-w64-x86_64-osslsigncode := 2.15-1
.PHONY: PKG@@mingw-w64-x86_64-osslsigncode
PKG@@mingw-w64-x86_64-osslsigncode: $(if $(__deps),PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-osslsigncode'

override VERSION_OF_mingw-w64-x86_64-p11-kit := 0.26.5-1
.PHONY: PKG@@mingw-w64-x86_64-p11-kit
PKG@@mingw-w64-x86_64-p11-kit: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-p11-kit'

override VERSION_OF_mingw-w64-x86_64-pango := 1.58.2-1
.PHONY: PKG@@mingw-w64-x86_64-pango
PKG@@mingw-w64-x86_64-pango: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-fontconfig PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-fribidi PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-libthai) ; @echo 'PKG@@mingw-w64-x86_64-pango'

override VERSION_OF_mingw-w64-x86_64-pango-docs := 1.58.2-1
.PHONY: PKG@@mingw-w64-x86_64-pango-docs
PKG@@mingw-w64-x86_64-pango-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-pango-docs'

override VERSION_OF_mingw-w64-x86_64-parpack := 3.9.1-9
.PHONY: PKG@@mingw-w64-x86_64-parpack
PKG@@mingw-w64-x86_64-parpack: $(if $(__deps),PKG@@mingw-w64-x86_64-arpack PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-msmpi PKG@@mingw-w64-x86_64-openblas) ; @echo 'PKG@@mingw-w64-x86_64-parpack'

override VERSION_OF_mingw-w64-x86_64-pcaudiolib := 1.3-1
.PHONY: PKG@@mingw-w64-x86_64-pcaudiolib
PKG@@mingw-w64-x86_64-pcaudiolib: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-pcaudiolib'

override VERSION_OF_mingw-w64-x86_64-pcre := 8.45-2
.PHONY: PKG@@mingw-w64-x86_64-pcre
PKG@@mingw-w64-x86_64-pcre: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-wineditline PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-pcre'

override VERSION_OF_mingw-w64-x86_64-pcre2 := 10.49-1
.PHONY: PKG@@mingw-w64-x86_64-pcre2
PKG@@mingw-w64-x86_64-pcre2: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-wineditline PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-pcre2'

override VERSION_OF_mingw-w64-x86_64-pdal := 2.10.2-1
.PHONY: PKG@@mingw-w64-x86_64-pdal
PKG@@mingw-w64-x86_64-pdal: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-gdal PKG@@mingw-w64-x86_64-libgeotiff PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-proj PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-pdal'

override VERSION_OF_mingw-w64-x86_64-pdcurses := 4.5.4-1
.PHONY: PKG@@mingw-w64-x86_64-curses PKG@@mingw-w64-x86_64-pdcurses
PKG@@mingw-w64-x86_64-curses PKG@@mingw-w64-x86_64-pdcurses: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-pdcurses'

override VERSION_OF_mingw-w64-x86_64-pdf2svg := 0.2.4-1
.PHONY: PKG@@mingw-w64-x86_64-pdf2svg
PKG@@mingw-w64-x86_64-pdf2svg: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-poppler) ; @echo 'PKG@@mingw-w64-x86_64-pdf2svg'

override VERSION_OF_mingw-w64-x86_64-pelican := 4.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-pelican
PKG@@mingw-w64-x86_64-pelican: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-blinker PKG@@mingw-w64-x86_64-python-dateutil PKG@@mingw-w64-x86_64-python-docutils PKG@@mingw-w64-x86_64-python-feedgenerator PKG@@mingw-w64-x86_64-python-jinja PKG@@mingw-w64-x86_64-python-ordered-set PKG@@mingw-w64-x86_64-python-pygments PKG@@mingw-w64-x86_64-python-rich PKG@@mingw-w64-x86_64-python-tzdata PKG@@mingw-w64-x86_64-python-unidecode PKG@@mingw-w64-x86_64-python-watchfiles) ; @echo 'PKG@@mingw-w64-x86_64-pelican'

override VERSION_OF_mingw-w64-x86_64-perl := 5.44.0-3
.PHONY: PKG@@mingw-w64-x86_64-perl PKG@@mingw-w64-x86_64-perl-_charnames PKG@@mingw-w64-x86_64-perl-amiga-arexx PKG@@mingw-w64-x86_64-perl-amiga-exec PKG@@mingw-w64-x86_64-perl-anydbm_file PKG@@mingw-w64-x86_64-perl-app-cpan PKG@@mingw-w64-x86_64-perl-app-prove PKG@@mingw-w64-x86_64-perl-app-prove-state PKG@@mingw-w64-x86_64-perl-app-prove-state-result PKG@@mingw-w64-x86_64-perl-app-prove-state-result-test PKG@@mingw-w64-x86_64-perl-archive-tar PKG@@mingw-w64-x86_64-perl-archive-tar-constant PKG@@mingw-w64-x86_64-perl-archive-tar-file PKG@@mingw-w64-x86_64-perl-attribute-handlers PKG@@mingw-w64-x86_64-perl-attributes PKG@@mingw-w64-x86_64-perl-autodie PKG@@mingw-w64-x86_64-perl-autodie-exception PKG@@mingw-w64-x86_64-perl-autodie-exception-system PKG@@mingw-w64-x86_64-perl-autodie-hints PKG@@mingw-w64-x86_64-perl-autodie-scope-guard PKG@@mingw-w64-x86_64-perl-autodie-scope-guardstack PKG@@mingw-w64-x86_64-perl-autodie-skip PKG@@mingw-w64-x86_64-perl-autodie-util PKG@@mingw-w64-x86_64-perl-autoloader PKG@@mingw-w64-x86_64-perl-autosplit PKG@@mingw-w64-x86_64-perl-autouse PKG@@mingw-w64-x86_64-perl-b PKG@@mingw-w64-x86_64-perl-b-concise PKG@@mingw-w64-x86_64-perl-b-deparse PKG@@mingw-w64-x86_64-perl-b-op_private PKG@@mingw-w64-x86_64-perl-b-showlex PKG@@mingw-w64-x86_64-perl-b-terse PKG@@mingw-w64-x86_64-perl-b-xref PKG@@mingw-w64-x86_64-perl-base PKG@@mingw-w64-x86_64-perl-benchmark PKG@@mingw-w64-x86_64-perl-bigfloat PKG@@mingw-w64-x86_64-perl-bigint PKG@@mingw-w64-x86_64-perl-bignum PKG@@mingw-w64-x86_64-perl-bigrat PKG@@mingw-w64-x86_64-perl-blib PKG@@mingw-w64-x86_64-perl-builtin PKG@@mingw-w64-x86_64-perl-bytes PKG@@mingw-w64-x86_64-perl-carp PKG@@mingw-w64-x86_64-perl-carp-heavy PKG@@mingw-w64-x86_64-perl-charnames PKG@@mingw-w64-x86_64-perl-class-struct PKG@@mingw-w64-x86_64-perl-compress-raw-bzip2 PKG@@mingw-w64-x86_64-perl-compress-raw-zlib PKG@@mingw-w64-x86_64-perl-compress-zlib PKG@@mingw-w64-x86_64-perl-config PKG@@mingw-w64-x86_64-perl-config-extensions PKG@@mingw-w64-x86_64-perl-config-perl-v PKG@@mingw-w64-x86_64-perl-constant PKG@@mingw-w64-x86_64-perl-cpan PKG@@mingw-w64-x86_64-perl-cpan-author PKG@@mingw-w64-x86_64-perl-cpan-bundle PKG@@mingw-w64-x86_64-perl-cpan-cachemgr PKG@@mingw-w64-x86_64-perl-cpan-complete PKG@@mingw-w64-x86_64-perl-cpan-debug PKG@@mingw-w64-x86_64-perl-cpan-deferredcode PKG@@mingw-w64-x86_64-perl-cpan-distribution PKG@@mingw-w64-x86_64-perl-cpan-distroprefs PKG@@mingw-w64-x86_64-perl-cpan-distrostatus PKG@@mingw-w64-x86_64-perl-cpan-exception-blocked_urllist PKG@@mingw-w64-x86_64-perl-cpan-exception-recursivedependency PKG@@mingw-w64-x86_64-perl-cpan-exception-yaml_not_installed PKG@@mingw-w64-x86_64-perl-cpan-exception-yaml_process_error PKG@@mingw-w64-x86_64-perl-cpan-firsttime PKG@@mingw-w64-x86_64-perl-cpan-ftp PKG@@mingw-w64-x86_64-perl-cpan-ftp-netrc PKG@@mingw-w64-x86_64-perl-cpan-handleconfig PKG@@mingw-w64-x86_64-perl-cpan-http-client PKG@@mingw-w64-x86_64-perl-cpan-http-credentials PKG@@mingw-w64-x86_64-perl-cpan-index PKG@@mingw-w64-x86_64-perl-cpan-infoobj PKG@@mingw-w64-x86_64-perl-cpan-kwalify PKG@@mingw-w64-x86_64-perl-cpan-lwp-useragent PKG@@mingw-w64-x86_64-perl-cpan-meta PKG@@mingw-w64-x86_64-perl-cpan-meta-converter PKG@@mingw-w64-x86_64-perl-cpan-meta-feature PKG@@mingw-w64-x86_64-perl-cpan-meta-history PKG@@mingw-w64-x86_64-perl-cpan-meta-merge PKG@@mingw-w64-x86_64-perl-cpan-meta-prereqs PKG@@mingw-w64-x86_64-perl-cpan-meta-requirements PKG@@mingw-w64-x86_64-perl-cpan-meta-requirements-range PKG@@mingw-w64-x86_64-perl-cpan-meta-spec PKG@@mingw-w64-x86_64-perl-cpan-meta-validator PKG@@mingw-w64-x86_64-perl-cpan-meta-yaml PKG@@mingw-w64-x86_64-perl-cpan-mirrors PKG@@mingw-w64-x86_64-perl-cpan-module PKG@@mingw-w64-x86_64-perl-cpan-nox PKG@@mingw-w64-x86_64-perl-cpan-plugin PKG@@mingw-w64-x86_64-perl-cpan-plugin-specfile PKG@@mingw-w64-x86_64-perl-cpan-prompt PKG@@mingw-w64-x86_64-perl-cpan-queue PKG@@mingw-w64-x86_64-perl-cpan-shell PKG@@mingw-w64-x86_64-perl-cpan-tarzip PKG@@mingw-w64-x86_64-perl-cpan-url PKG@@mingw-w64-x86_64-perl-cpan-version PKG@@mingw-w64-x86_64-perl-cwd PKG@@mingw-w64-x86_64-perl-data-dumper PKG@@mingw-w64-x86_64-perl-db PKG@@mingw-w64-x86_64-perl-db_file PKG@@mingw-w64-x86_64-perl-dbm_filter PKG@@mingw-w64-x86_64-perl-dbm_filter-compress PKG@@mingw-w64-x86_64-perl-dbm_filter-encode PKG@@mingw-w64-x86_64-perl-dbm_filter-int32 PKG@@mingw-w64-x86_64-perl-dbm_filter-null PKG@@mingw-w64-x86_64-perl-dbm_filter-utf8 PKG@@mingw-w64-x86_64-perl-deprecate PKG@@mingw-w64-x86_64-perl-devel-peek PKG@@mingw-w64-x86_64-perl-devel-ppport PKG@@mingw-w64-x86_64-perl-devel-selfstubber PKG@@mingw-w64-x86_64-perl-diagnostics PKG@@mingw-w64-x86_64-perl-digest PKG@@mingw-w64-x86_64-perl-digest-base PKG@@mingw-w64-x86_64-perl-digest-file PKG@@mingw-w64-x86_64-perl-digest-md5 PKG@@mingw-w64-x86_64-perl-dirhandle PKG@@mingw-w64-x86_64-perl-dumpvalue PKG@@mingw-w64-x86_64-perl-dynaloader PKG@@mingw-w64-x86_64-perl-encode PKG@@mingw-w64-x86_64-perl-encode-alias PKG@@mingw-w64-x86_64-perl-encode-byte PKG@@mingw-w64-x86_64-perl-encode-cjkconstants PKG@@mingw-w64-x86_64-perl-encode-cn PKG@@mingw-w64-x86_64-perl-encode-cn-hz PKG@@mingw-w64-x86_64-perl-encode-config PKG@@mingw-w64-x86_64-perl-encode-ebcdic PKG@@mingw-w64-x86_64-perl-encode-encoder PKG@@mingw-w64-x86_64-perl-encode-encoding PKG@@mingw-w64-x86_64-perl-encode-gsm0338 PKG@@mingw-w64-x86_64-perl-encode-guess PKG@@mingw-w64-x86_64-perl-encode-jp PKG@@mingw-w64-x86_64-perl-encode-jp-h2z PKG@@mingw-w64-x86_64-perl-encode-jp-jis7 PKG@@mingw-w64-x86_64-perl-encode-kr PKG@@mingw-w64-x86_64-perl-encode-kr-2022_kr PKG@@mingw-w64-x86_64-perl-encode-mime-header PKG@@mingw-w64-x86_64-perl-encode-mime-header-iso_2022_jp PKG@@mingw-w64-x86_64-perl-encode-mime-name PKG@@mingw-w64-x86_64-perl-encode-symbol PKG@@mingw-w64-x86_64-perl-encode-tw PKG@@mingw-w64-x86_64-perl-encode-unicode PKG@@mingw-w64-x86_64-perl-encode-unicode-utf7 PKG@@mingw-w64-x86_64-perl-encoding PKG@@mingw-w64-x86_64-perl-encoding-warnings PKG@@mingw-w64-x86_64-perl-english PKG@@mingw-w64-x86_64-perl-env PKG@@mingw-w64-x86_64-perl-errno PKG@@mingw-w64-x86_64-perl-experimental PKG@@mingw-w64-x86_64-perl-exporter PKG@@mingw-w64-x86_64-perl-exporter-heavy PKG@@mingw-w64-x86_64-perl-extutils-cbuilder PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-base PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-aix PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-android PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-cygwin PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-darwin PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-dec_osf PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-os2 PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-unix PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-vms PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-windows PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-windows-bcc PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-windows-gcc PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-windows-msvc PKG@@mingw-w64-x86_64-perl-extutils-command PKG@@mingw-w64-x86_64-perl-extutils-command-mm PKG@@mingw-w64-x86_64-perl-extutils-constant PKG@@mingw-w64-x86_64-perl-extutils-constant-base PKG@@mingw-w64-x86_64-perl-extutils-constant-proxysubs PKG@@mingw-w64-x86_64-perl-extutils-constant-utils PKG@@mingw-w64-x86_64-perl-extutils-constant-xs PKG@@mingw-w64-x86_64-perl-extutils-embed PKG@@mingw-w64-x86_64-perl-extutils-install PKG@@mingw-w64-x86_64-perl-extutils-installed PKG@@mingw-w64-x86_64-perl-extutils-liblist PKG@@mingw-w64-x86_64-perl-extutils-liblist-kid PKG@@mingw-w64-x86_64-perl-extutils-makemaker PKG@@mingw-w64-x86_64-perl-extutils-makemaker-config PKG@@mingw-w64-x86_64-perl-extutils-makemaker-locale PKG@@mingw-w64-x86_64-perl-extutils-makemaker-version PKG@@mingw-w64-x86_64-perl-extutils-makemaker-version-regex PKG@@mingw-w64-x86_64-perl-extutils-manifest PKG@@mingw-w64-x86_64-perl-extutils-miniperl PKG@@mingw-w64-x86_64-perl-extutils-mkbootstrap PKG@@mingw-w64-x86_64-perl-extutils-mksymlists PKG@@mingw-w64-x86_64-perl-extutils-mm PKG@@mingw-w64-x86_64-perl-extutils-mm_aix PKG@@mingw-w64-x86_64-perl-extutils-mm_any PKG@@mingw-w64-x86_64-perl-extutils-mm_beos PKG@@mingw-w64-x86_64-perl-extutils-mm_cygwin PKG@@mingw-w64-x86_64-perl-extutils-mm_darwin PKG@@mingw-w64-x86_64-perl-extutils-mm_dos PKG@@mingw-w64-x86_64-perl-extutils-mm_macos PKG@@mingw-w64-x86_64-perl-extutils-mm_nw5 PKG@@mingw-w64-x86_64-perl-extutils-mm_os2 PKG@@mingw-w64-x86_64-perl-extutils-mm_os390 PKG@@mingw-w64-x86_64-perl-extutils-mm_qnx PKG@@mingw-w64-x86_64-perl-extutils-mm_unix PKG@@mingw-w64-x86_64-perl-extutils-mm_uwin PKG@@mingw-w64-x86_64-perl-extutils-mm_vms PKG@@mingw-w64-x86_64-perl-extutils-mm_vos PKG@@mingw-w64-x86_64-perl-extutils-mm_win32 PKG@@mingw-w64-x86_64-perl-extutils-mm_win95 PKG@@mingw-w64-x86_64-perl-extutils-my PKG@@mingw-w64-x86_64-perl-extutils-packlist PKG@@mingw-w64-x86_64-perl-extutils-parsexs PKG@@mingw-w64-x86_64-perl-extutils-parsexs-constants PKG@@mingw-w64-x86_64-perl-extutils-parsexs-countlines PKG@@mingw-w64-x86_64-perl-extutils-parsexs-eval PKG@@mingw-w64-x86_64-perl-extutils-parsexs-node PKG@@mingw-w64-x86_64-perl-extutils-parsexs-utilities PKG@@mingw-w64-x86_64-perl-extutils-pl2bat PKG@@mingw-w64-x86_64-perl-extutils-testlib PKG@@mingw-w64-x86_64-perl-extutils-typemaps PKG@@mingw-w64-x86_64-perl-extutils-typemaps-cmd PKG@@mingw-w64-x86_64-perl-extutils-typemaps-inputmap PKG@@mingw-w64-x86_64-perl-extutils-typemaps-outputmap PKG@@mingw-w64-x86_64-perl-extutils-typemaps-type PKG@@mingw-w64-x86_64-perl-extutils-xssymset PKG@@mingw-w64-x86_64-perl-fatal PKG@@mingw-w64-x86_64-perl-fcntl PKG@@mingw-w64-x86_64-perl-feature PKG@@mingw-w64-x86_64-perl-fields PKG@@mingw-w64-x86_64-perl-file-basename PKG@@mingw-w64-x86_64-perl-file-compare PKG@@mingw-w64-x86_64-perl-file-copy PKG@@mingw-w64-x86_64-perl-file-dosglob PKG@@mingw-w64-x86_64-perl-file-fetch PKG@@mingw-w64-x86_64-perl-file-find PKG@@mingw-w64-x86_64-perl-file-glob PKG@@mingw-w64-x86_64-perl-file-globmapper PKG@@mingw-w64-x86_64-perl-file-path PKG@@mingw-w64-x86_64-perl-file-spec PKG@@mingw-w64-x86_64-perl-file-spec-amigaos PKG@@mingw-w64-x86_64-perl-file-spec-cygwin PKG@@mingw-w64-x86_64-perl-file-spec-epoc PKG@@mingw-w64-x86_64-perl-file-spec-functions PKG@@mingw-w64-x86_64-perl-file-spec-mac PKG@@mingw-w64-x86_64-perl-file-spec-os2 PKG@@mingw-w64-x86_64-perl-file-spec-unix PKG@@mingw-w64-x86_64-perl-file-spec-vms PKG@@mingw-w64-x86_64-perl-file-spec-win32 PKG@@mingw-w64-x86_64-perl-file-stat PKG@@mingw-w64-x86_64-perl-file-temp PKG@@mingw-w64-x86_64-perl-filecache PKG@@mingw-w64-x86_64-perl-filehandle PKG@@mingw-w64-x86_64-perl-filetest PKG@@mingw-w64-x86_64-perl-filter-simple PKG@@mingw-w64-x86_64-perl-filter-util-call PKG@@mingw-w64-x86_64-perl-findbin PKG@@mingw-w64-x86_64-perl-gdbm_file PKG@@mingw-w64-x86_64-perl-getopt-long PKG@@mingw-w64-x86_64-perl-getopt-long-parser PKG@@mingw-w64-x86_64-perl-getopt-std PKG@@mingw-w64-x86_64-perl-hash-util PKG@@mingw-w64-x86_64-perl-hash-util-fieldhash PKG@@mingw-w64-x86_64-perl-http-tiny PKG@@mingw-w64-x86_64-perl-i18n-collate PKG@@mingw-w64-x86_64-perl-i18n-langinfo PKG@@mingw-w64-x86_64-perl-i18n-langtags PKG@@mingw-w64-x86_64-perl-i18n-langtags-detect PKG@@mingw-w64-x86_64-perl-i18n-langtags-list PKG@@mingw-w64-x86_64-perl-if PKG@@mingw-w64-x86_64-perl-integer PKG@@mingw-w64-x86_64-perl-io PKG@@mingw-w64-x86_64-perl-io-compress PKG@@mingw-w64-x86_64-perl-io-compress-adapter-bzip2 PKG@@mingw-w64-x86_64-perl-io-compress-adapter-deflate PKG@@mingw-w64-x86_64-perl-io-compress-adapter-identity PKG@@mingw-w64-x86_64-perl-io-compress-base PKG@@mingw-w64-x86_64-perl-io-compress-base-common PKG@@mingw-w64-x86_64-perl-io-compress-bzip2 PKG@@mingw-w64-x86_64-perl-io-compress-deflate PKG@@mingw-w64-x86_64-perl-io-compress-gzip PKG@@mingw-w64-x86_64-perl-io-compress-gzip-constants PKG@@mingw-w64-x86_64-perl-io-compress-rawdeflate PKG@@mingw-w64-x86_64-perl-io-compress-zip PKG@@mingw-w64-x86_64-perl-io-compress-zip-constants PKG@@mingw-w64-x86_64-perl-io-compress-zlib-constants PKG@@mingw-w64-x86_64-perl-io-compress-zlib-extra PKG@@mingw-w64-x86_64-perl-io-dir PKG@@mingw-w64-x86_64-perl-io-file PKG@@mingw-w64-x86_64-perl-io-handle PKG@@mingw-w64-x86_64-perl-io-pipe PKG@@mingw-w64-x86_64-perl-io-poll PKG@@mingw-w64-x86_64-perl-io-seekable PKG@@mingw-w64-x86_64-perl-io-select PKG@@mingw-w64-x86_64-perl-io-socket PKG@@mingw-w64-x86_64-perl-io-socket-inet PKG@@mingw-w64-x86_64-perl-io-socket-ip PKG@@mingw-w64-x86_64-perl-io-socket-unix PKG@@mingw-w64-x86_64-perl-io-uncompress-adapter-bunzip2 PKG@@mingw-w64-x86_64-perl-io-uncompress-adapter-identity PKG@@mingw-w64-x86_64-perl-io-uncompress-adapter-inflate PKG@@mingw-w64-x86_64-perl-io-uncompress-anyinflate PKG@@mingw-w64-x86_64-perl-io-uncompress-anyuncompress PKG@@mingw-w64-x86_64-perl-io-uncompress-base PKG@@mingw-w64-x86_64-perl-io-uncompress-bunzip2 PKG@@mingw-w64-x86_64-perl-io-uncompress-gunzip PKG@@mingw-w64-x86_64-perl-io-uncompress-inflate PKG@@mingw-w64-x86_64-perl-io-uncompress-rawinflate PKG@@mingw-w64-x86_64-perl-io-uncompress-unzip PKG@@mingw-w64-x86_64-perl-io-zlib PKG@@mingw-w64-x86_64-perl-ipc-cmd PKG@@mingw-w64-x86_64-perl-ipc-msg PKG@@mingw-w64-x86_64-perl-ipc-open2 PKG@@mingw-w64-x86_64-perl-ipc-open3 PKG@@mingw-w64-x86_64-perl-ipc-semaphore PKG@@mingw-w64-x86_64-perl-ipc-sharedmem PKG@@mingw-w64-x86_64-perl-ipc-sysv PKG@@mingw-w64-x86_64-perl-json-pp PKG@@mingw-w64-x86_64-perl-json-pp-boolean PKG@@mingw-w64-x86_64-perl-less PKG@@mingw-w64-x86_64-perl-lib PKG@@mingw-w64-x86_64-perl-list-util PKG@@mingw-w64-x86_64-perl-list-util-xs PKG@@mingw-w64-x86_64-perl-locale PKG@@mingw-w64-x86_64-perl-locale-maketext PKG@@mingw-w64-x86_64-perl-locale-maketext-guts PKG@@mingw-w64-x86_64-perl-locale-maketext-gutsloader PKG@@mingw-w64-x86_64-perl-locale-maketext-simple PKG@@mingw-w64-x86_64-perl-math-bigfloat PKG@@mingw-w64-x86_64-perl-math-bigfloat-trace PKG@@mingw-w64-x86_64-perl-math-bigint PKG@@mingw-w64-x86_64-perl-math-bigint-calc PKG@@mingw-w64-x86_64-perl-math-bigint-fastcalc PKG@@mingw-w64-x86_64-perl-math-bigint-lib PKG@@mingw-w64-x86_64-perl-math-bigint-trace PKG@@mingw-w64-x86_64-perl-math-bigrat PKG@@mingw-w64-x86_64-perl-math-bigrat-trace PKG@@mingw-w64-x86_64-perl-math-complex PKG@@mingw-w64-x86_64-perl-math-trig PKG@@mingw-w64-x86_64-perl-memoize PKG@@mingw-w64-x86_64-perl-memoize-anydbm_file PKG@@mingw-w64-x86_64-perl-memoize-expire PKG@@mingw-w64-x86_64-perl-memoize-ndbm_file PKG@@mingw-w64-x86_64-perl-memoize-sdbm_file PKG@@mingw-w64-x86_64-perl-memoize-storable PKG@@mingw-w64-x86_64-perl-meta_notation PKG@@mingw-w64-x86_64-perl-mime-base64 PKG@@mingw-w64-x86_64-perl-mime-quotedprint PKG@@mingw-w64-x86_64-perl-module-corelist PKG@@mingw-w64-x86_64-perl-module-corelist-utils PKG@@mingw-w64-x86_64-perl-module-load PKG@@mingw-w64-x86_64-perl-module-load-conditional PKG@@mingw-w64-x86_64-perl-module-loaded PKG@@mingw-w64-x86_64-perl-module-metadata PKG@@mingw-w64-x86_64-perl-mro PKG@@mingw-w64-x86_64-perl-ndbm_file PKG@@mingw-w64-x86_64-perl-net-cmd PKG@@mingw-w64-x86_64-perl-net-config PKG@@mingw-w64-x86_64-perl-net-domain PKG@@mingw-w64-x86_64-perl-net-ftp PKG@@mingw-w64-x86_64-perl-net-ftp-a PKG@@mingw-w64-x86_64-perl-net-ftp-dataconn PKG@@mingw-w64-x86_64-perl-net-ftp-e PKG@@mingw-w64-x86_64-perl-net-ftp-i PKG@@mingw-w64-x86_64-perl-net-ftp-l PKG@@mingw-w64-x86_64-perl-net-hostent PKG@@mingw-w64-x86_64-perl-net-netent PKG@@mingw-w64-x86_64-perl-net-netrc PKG@@mingw-w64-x86_64-perl-net-nntp PKG@@mingw-w64-x86_64-perl-net-ping PKG@@mingw-w64-x86_64-perl-net-pop3 PKG@@mingw-w64-x86_64-perl-net-protoent PKG@@mingw-w64-x86_64-perl-net-servent PKG@@mingw-w64-x86_64-perl-net-smtp PKG@@mingw-w64-x86_64-perl-net-time PKG@@mingw-w64-x86_64-perl-next PKG@@mingw-w64-x86_64-perl-o PKG@@mingw-w64-x86_64-perl-odbm_file PKG@@mingw-w64-x86_64-perl-ok PKG@@mingw-w64-x86_64-perl-opcode PKG@@mingw-w64-x86_64-perl-open PKG@@mingw-w64-x86_64-perl-ops PKG@@mingw-w64-x86_64-perl-os2-dll PKG@@mingw-w64-x86_64-perl-os2-extattr PKG@@mingw-w64-x86_64-perl-os2-prfdb PKG@@mingw-w64-x86_64-perl-os2-process PKG@@mingw-w64-x86_64-perl-os2-rexx PKG@@mingw-w64-x86_64-perl-overload PKG@@mingw-w64-x86_64-perl-overload-numbers PKG@@mingw-w64-x86_64-perl-overloading PKG@@mingw-w64-x86_64-perl-params-check PKG@@mingw-w64-x86_64-perl-parent PKG@@mingw-w64-x86_64-perl-parse-cpan-meta PKG@@mingw-w64-x86_64-perl-perl-ostype PKG@@mingw-w64-x86_64-perl-perlfaq PKG@@mingw-w64-x86_64-perl-perlio PKG@@mingw-w64-x86_64-perl-perlio-encoding PKG@@mingw-w64-x86_64-perl-perlio-mmap PKG@@mingw-w64-x86_64-perl-perlio-scalar PKG@@mingw-w64-x86_64-perl-perlio-via PKG@@mingw-w64-x86_64-perl-perlio-via-quotedprint PKG@@mingw-w64-x86_64-perl-pod-checker PKG@@mingw-w64-x86_64-perl-pod-escapes PKG@@mingw-w64-x86_64-perl-pod-functions PKG@@mingw-w64-x86_64-perl-pod-html PKG@@mingw-w64-x86_64-perl-pod-html-util PKG@@mingw-w64-x86_64-perl-pod-man PKG@@mingw-w64-x86_64-perl-pod-parselink PKG@@mingw-w64-x86_64-perl-pod-perldoc PKG@@mingw-w64-x86_64-perl-pod-perldoc-baseto PKG@@mingw-w64-x86_64-perl-pod-perldoc-getoptsoo PKG@@mingw-w64-x86_64-perl-pod-perldoc-toansi PKG@@mingw-w64-x86_64-perl-pod-perldoc-tochecker PKG@@mingw-w64-x86_64-perl-pod-perldoc-toman PKG@@mingw-w64-x86_64-perl-pod-perldoc-tonroff PKG@@mingw-w64-x86_64-perl-pod-perldoc-topod PKG@@mingw-w64-x86_64-perl-pod-perldoc-tortf PKG@@mingw-w64-x86_64-perl-pod-perldoc-toterm PKG@@mingw-w64-x86_64-perl-pod-perldoc-totext PKG@@mingw-w64-x86_64-perl-pod-perldoc-totk PKG@@mingw-w64-x86_64-perl-pod-perldoc-toxml PKG@@mingw-w64-x86_64-perl-pod-simple PKG@@mingw-w64-x86_64-perl-pod-simple-blackbox PKG@@mingw-w64-x86_64-perl-pod-simple-checker PKG@@mingw-w64-x86_64-perl-pod-simple-debug PKG@@mingw-w64-x86_64-perl-pod-simple-dumpastext PKG@@mingw-w64-x86_64-perl-pod-simple-dumpasxml PKG@@mingw-w64-x86_64-perl-pod-simple-html PKG@@mingw-w64-x86_64-perl-pod-simple-htmlbatch PKG@@mingw-w64-x86_64-perl-pod-simple-htmllegacy PKG@@mingw-w64-x86_64-perl-pod-simple-justpod PKG@@mingw-w64-x86_64-perl-pod-simple-linksection PKG@@mingw-w64-x86_64-perl-pod-simple-methody PKG@@mingw-w64-x86_64-perl-pod-simple-progress PKG@@mingw-w64-x86_64-perl-pod-simple-pullparser PKG@@mingw-w64-x86_64-perl-pod-simple-pullparserendtoken PKG@@mingw-w64-x86_64-perl-pod-simple-pullparserstarttoken PKG@@mingw-w64-x86_64-perl-pod-simple-pullparsertexttoken PKG@@mingw-w64-x86_64-perl-pod-simple-pullparsertoken PKG@@mingw-w64-x86_64-perl-pod-simple-rtf PKG@@mingw-w64-x86_64-perl-pod-simple-search PKG@@mingw-w64-x86_64-perl-pod-simple-simpletree PKG@@mingw-w64-x86_64-perl-pod-simple-text PKG@@mingw-w64-x86_64-perl-pod-simple-textcontent PKG@@mingw-w64-x86_64-perl-pod-simple-tiedoutfh PKG@@mingw-w64-x86_64-perl-pod-simple-transcode PKG@@mingw-w64-x86_64-perl-pod-simple-transcodedumb PKG@@mingw-w64-x86_64-perl-pod-simple-transcodesmart PKG@@mingw-w64-x86_64-perl-pod-simple-xhtml PKG@@mingw-w64-x86_64-perl-pod-simple-xmloutstream PKG@@mingw-w64-x86_64-perl-pod-text PKG@@mingw-w64-x86_64-perl-pod-text-color PKG@@mingw-w64-x86_64-perl-pod-text-overstrike PKG@@mingw-w64-x86_64-perl-pod-text-termcap PKG@@mingw-w64-x86_64-perl-pod-usage PKG@@mingw-w64-x86_64-perl-posix PKG@@mingw-w64-x86_64-perl-re PKG@@mingw-w64-x86_64-perl-safe PKG@@mingw-w64-x86_64-perl-scalar-list-utils PKG@@mingw-w64-x86_64-perl-scalar-util PKG@@mingw-w64-x86_64-perl-sdbm_file PKG@@mingw-w64-x86_64-perl-search-dict PKG@@mingw-w64-x86_64-perl-selectsaver PKG@@mingw-w64-x86_64-perl-selfloader PKG@@mingw-w64-x86_64-perl-sigtrap PKG@@mingw-w64-x86_64-perl-socket PKG@@mingw-w64-x86_64-perl-sort PKG@@mingw-w64-x86_64-perl-source-encoding PKG@@mingw-w64-x86_64-perl-stable PKG@@mingw-w64-x86_64-perl-storable PKG@@mingw-w64-x86_64-perl-strict PKG@@mingw-w64-x86_64-perl-sub-util PKG@@mingw-w64-x86_64-perl-subs PKG@@mingw-w64-x86_64-perl-symbol PKG@@mingw-w64-x86_64-perl-sys-hostname PKG@@mingw-w64-x86_64-perl-sys-syslog PKG@@mingw-w64-x86_64-perl-sys-syslog-win32 PKG@@mingw-w64-x86_64-perl-tap-base PKG@@mingw-w64-x86_64-perl-tap-formatter-base PKG@@mingw-w64-x86_64-perl-tap-formatter-color PKG@@mingw-w64-x86_64-perl-tap-formatter-console PKG@@mingw-w64-x86_64-perl-tap-formatter-console-parallelsession PKG@@mingw-w64-x86_64-perl-tap-formatter-console-session PKG@@mingw-w64-x86_64-perl-tap-formatter-file PKG@@mingw-w64-x86_64-perl-tap-formatter-file-session PKG@@mingw-w64-x86_64-perl-tap-formatter-session PKG@@mingw-w64-x86_64-perl-tap-harness PKG@@mingw-w64-x86_64-perl-tap-harness-env PKG@@mingw-w64-x86_64-perl-tap-object PKG@@mingw-w64-x86_64-perl-tap-parser PKG@@mingw-w64-x86_64-perl-tap-parser-aggregator PKG@@mingw-w64-x86_64-perl-tap-parser-grammar PKG@@mingw-w64-x86_64-perl-tap-parser-iterator PKG@@mingw-w64-x86_64-perl-tap-parser-iterator-array PKG@@mingw-w64-x86_64-perl-tap-parser-iterator-process PKG@@mingw-w64-x86_64-perl-tap-parser-iterator-stream PKG@@mingw-w64-x86_64-perl-tap-parser-iteratorfactory PKG@@mingw-w64-x86_64-perl-tap-parser-multiplexer PKG@@mingw-w64-x86_64-perl-tap-parser-result PKG@@mingw-w64-x86_64-perl-tap-parser-result-bailout PKG@@mingw-w64-x86_64-perl-tap-parser-result-comment PKG@@mingw-w64-x86_64-perl-tap-parser-result-plan PKG@@mingw-w64-x86_64-perl-tap-parser-result-pragma PKG@@mingw-w64-x86_64-perl-tap-parser-result-test PKG@@mingw-w64-x86_64-perl-tap-parser-result-unknown PKG@@mingw-w64-x86_64-perl-tap-parser-result-version PKG@@mingw-w64-x86_64-perl-tap-parser-result-yaml PKG@@mingw-w64-x86_64-perl-tap-parser-resultfactory PKG@@mingw-w64-x86_64-perl-tap-parser-scheduler PKG@@mingw-w64-x86_64-perl-tap-parser-scheduler-job PKG@@mingw-w64-x86_64-perl-tap-parser-scheduler-spinner PKG@@mingw-w64-x86_64-perl-tap-parser-source PKG@@mingw-w64-x86_64-perl-tap-parser-sourcehandler PKG@@mingw-w64-x86_64-perl-tap-parser-sourcehandler-executable PKG@@mingw-w64-x86_64-perl-tap-parser-sourcehandler-file PKG@@mingw-w64-x86_64-perl-tap-parser-sourcehandler-handle PKG@@mingw-w64-x86_64-perl-tap-parser-sourcehandler-perl PKG@@mingw-w64-x86_64-perl-tap-parser-sourcehandler-rawtap PKG@@mingw-w64-x86_64-perl-tap-parser-yamlish-reader PKG@@mingw-w64-x86_64-perl-tap-parser-yamlish-writer PKG@@mingw-w64-x86_64-perl-term-ansicolor PKG@@mingw-w64-x86_64-perl-term-cap PKG@@mingw-w64-x86_64-perl-term-complete PKG@@mingw-w64-x86_64-perl-term-readline PKG@@mingw-w64-x86_64-perl-term-table PKG@@mingw-w64-x86_64-perl-term-table-cell PKG@@mingw-w64-x86_64-perl-term-table-cellstack PKG@@mingw-w64-x86_64-perl-term-table-hashbase PKG@@mingw-w64-x86_64-perl-term-table-linebreak PKG@@mingw-w64-x86_64-perl-term-table-spacer PKG@@mingw-w64-x86_64-perl-term-table-util PKG@@mingw-w64-x86_64-perl-test PKG@@mingw-w64-x86_64-perl-test-builder PKG@@mingw-w64-x86_64-perl-test-builder-formatter PKG@@mingw-w64-x86_64-perl-test-builder-module PKG@@mingw-w64-x86_64-perl-test-builder-tester PKG@@mingw-w64-x86_64-perl-test-builder-tester-color PKG@@mingw-w64-x86_64-perl-test-builder-tododiag PKG@@mingw-w64-x86_64-perl-test-harness PKG@@mingw-w64-x86_64-perl-test-more PKG@@mingw-w64-x86_64-perl-test-simple PKG@@mingw-w64-x86_64-perl-test-tester PKG@@mingw-w64-x86_64-perl-test-tester-capture PKG@@mingw-w64-x86_64-perl-test-tester-capturerunner PKG@@mingw-w64-x86_64-perl-test-tester-delegate PKG@@mingw-w64-x86_64-perl-test-use-ok PKG@@mingw-w64-x86_64-perl-test2 PKG@@mingw-w64-x86_64-perl-test2-api PKG@@mingw-w64-x86_64-perl-test2-api-breakage PKG@@mingw-w64-x86_64-perl-test2-api-context PKG@@mingw-w64-x86_64-perl-test2-api-instance PKG@@mingw-w64-x86_64-perl-test2-api-interceptresult PKG@@mingw-w64-x86_64-perl-test2-api-interceptresult-event PKG@@mingw-w64-x86_64-perl-test2-api-interceptresult-facet PKG@@mingw-w64-x86_64-perl-test2-api-interceptresult-hub PKG@@mingw-w64-x86_64-perl-test2-api-interceptresult-squasher PKG@@mingw-w64-x86_64-perl-test2-api-stack PKG@@mingw-w64-x86_64-perl-test2-asyncsubtest PKG@@mingw-w64-x86_64-perl-test2-asyncsubtest-event-attach PKG@@mingw-w64-x86_64-perl-test2-asyncsubtest-event-detach PKG@@mingw-w64-x86_64-perl-test2-asyncsubtest-formatter PKG@@mingw-w64-x86_64-perl-test2-asyncsubtest-hub PKG@@mingw-w64-x86_64-perl-test2-bundle PKG@@mingw-w64-x86_64-perl-test2-bundle-extended PKG@@mingw-w64-x86_64-perl-test2-bundle-more PKG@@mingw-w64-x86_64-perl-test2-bundle-simple PKG@@mingw-w64-x86_64-perl-test2-compare PKG@@mingw-w64-x86_64-perl-test2-compare-array PKG@@mingw-w64-x86_64-perl-test2-compare-bag PKG@@mingw-w64-x86_64-perl-test2-compare-base PKG@@mingw-w64-x86_64-perl-test2-compare-bool PKG@@mingw-w64-x86_64-perl-test2-compare-custom PKG@@mingw-w64-x86_64-perl-test2-compare-deepref PKG@@mingw-w64-x86_64-perl-test2-compare-delta PKG@@mingw-w64-x86_64-perl-test2-compare-event PKG@@mingw-w64-x86_64-perl-test2-compare-eventmeta PKG@@mingw-w64-x86_64-perl-test2-compare-float PKG@@mingw-w64-x86_64-perl-test2-compare-hash PKG@@mingw-w64-x86_64-perl-test2-compare-isa PKG@@mingw-w64-x86_64-perl-test2-compare-meta PKG@@mingw-w64-x86_64-perl-test2-compare-negatable PKG@@mingw-w64-x86_64-perl-test2-compare-number PKG@@mingw-w64-x86_64-perl-test2-compare-object PKG@@mingw-w64-x86_64-perl-test2-compare-orderedsubset PKG@@mingw-w64-x86_64-perl-test2-compare-pattern PKG@@mingw-w64-x86_64-perl-test2-compare-ref PKG@@mingw-w64-x86_64-perl-test2-compare-regex PKG@@mingw-w64-x86_64-perl-test2-compare-scalar PKG@@mingw-w64-x86_64-perl-test2-compare-set PKG@@mingw-w64-x86_64-perl-test2-compare-string PKG@@mingw-w64-x86_64-perl-test2-compare-undef PKG@@mingw-w64-x86_64-perl-test2-compare-wildcard PKG@@mingw-w64-x86_64-perl-test2-env PKG@@mingw-w64-x86_64-perl-test2-event PKG@@mingw-w64-x86_64-perl-test2-event-bail PKG@@mingw-w64-x86_64-perl-test2-event-diag PKG@@mingw-w64-x86_64-perl-test2-event-encoding PKG@@mingw-w64-x86_64-perl-test2-event-exception PKG@@mingw-w64-x86_64-perl-test2-event-fail PKG@@mingw-w64-x86_64-perl-test2-event-generic PKG@@mingw-w64-x86_64-perl-test2-event-note PKG@@mingw-w64-x86_64-perl-test2-event-ok PKG@@mingw-w64-x86_64-perl-test2-event-pass PKG@@mingw-w64-x86_64-perl-test2-event-plan PKG@@mingw-w64-x86_64-perl-test2-event-skip PKG@@mingw-w64-x86_64-perl-test2-event-subtest PKG@@mingw-w64-x86_64-perl-test2-event-tap-version PKG@@mingw-w64-x86_64-perl-test2-event-v2 PKG@@mingw-w64-x86_64-perl-test2-event-waiting PKG@@mingw-w64-x86_64-perl-test2-eventfacet PKG@@mingw-w64-x86_64-perl-test2-eventfacet-about PKG@@mingw-w64-x86_64-perl-test2-eventfacet-amnesty PKG@@mingw-w64-x86_64-perl-test2-eventfacet-assert PKG@@mingw-w64-x86_64-perl-test2-eventfacet-control PKG@@mingw-w64-x86_64-perl-test2-eventfacet-error PKG@@mingw-w64-x86_64-perl-test2-eventfacet-hub PKG@@mingw-w64-x86_64-perl-test2-eventfacet-info PKG@@mingw-w64-x86_64-perl-test2-eventfacet-info-table PKG@@mingw-w64-x86_64-perl-test2-eventfacet-meta PKG@@mingw-w64-x86_64-perl-test2-eventfacet-parent PKG@@mingw-w64-x86_64-perl-test2-eventfacet-plan PKG@@mingw-w64-x86_64-perl-test2-eventfacet-render PKG@@mingw-w64-x86_64-perl-test2-eventfacet-trace PKG@@mingw-w64-x86_64-perl-test2-formatter PKG@@mingw-w64-x86_64-perl-test2-formatter-tap PKG@@mingw-w64-x86_64-perl-test2-hub PKG@@mingw-w64-x86_64-perl-test2-hub-interceptor PKG@@mingw-w64-x86_64-perl-test2-hub-interceptor-terminator PKG@@mingw-w64-x86_64-perl-test2-hub-subtest PKG@@mingw-w64-x86_64-perl-test2-ipc PKG@@mingw-w64-x86_64-perl-test2-ipc-driver PKG@@mingw-w64-x86_64-perl-test2-ipc-driver-files PKG@@mingw-w64-x86_64-perl-test2-manual PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-api PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-context PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-endtoend PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-event PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-hubs PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-ipc PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-utilities PKG@@mingw-w64-x86_64-perl-test2-manual-concurrency PKG@@mingw-w64-x86_64-perl-test2-manual-contributing PKG@@mingw-w64-x86_64-perl-test2-manual-testing PKG@@mingw-w64-x86_64-perl-test2-manual-testing-introduction PKG@@mingw-w64-x86_64-perl-test2-manual-testing-migrating PKG@@mingw-w64-x86_64-perl-test2-manual-testing-planning PKG@@mingw-w64-x86_64-perl-test2-manual-testing-todo PKG@@mingw-w64-x86_64-perl-test2-manual-tooling PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-firsttool PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-formatter PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-nesting PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-plugin-testexit PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-plugin-testingdone PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-plugin-toolcompletes PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-plugin-toolstarts PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-subtest PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-testbuilder PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-testing PKG@@mingw-w64-x86_64-perl-test2-mock PKG@@mingw-w64-x86_64-perl-test2-plugin PKG@@mingw-w64-x86_64-perl-test2-plugin-bailonfail PKG@@mingw-w64-x86_64-perl-test2-plugin-dieonfail PKG@@mingw-w64-x86_64-perl-test2-plugin-exitsummary PKG@@mingw-w64-x86_64-perl-test2-plugin-srand PKG@@mingw-w64-x86_64-perl-test2-plugin-times PKG@@mingw-w64-x86_64-perl-test2-plugin-utf8 PKG@@mingw-w64-x86_64-perl-test2-require PKG@@mingw-w64-x86_64-perl-test2-require-authortesting PKG@@mingw-w64-x86_64-perl-test2-require-automatedtesting PKG@@mingw-w64-x86_64-perl-test2-require-envvar PKG@@mingw-w64-x86_64-perl-test2-require-extendedtesting PKG@@mingw-w64-x86_64-perl-test2-require-fork PKG@@mingw-w64-x86_64-perl-test2-require-module PKG@@mingw-w64-x86_64-perl-test2-require-noninteractivetesting PKG@@mingw-w64-x86_64-perl-test2-require-perl PKG@@mingw-w64-x86_64-perl-test2-require-realfork PKG@@mingw-w64-x86_64-perl-test2-require-releasetesting PKG@@mingw-w64-x86_64-perl-test2-require-threads PKG@@mingw-w64-x86_64-perl-test2-suite PKG@@mingw-w64-x86_64-perl-test2-todo PKG@@mingw-w64-x86_64-perl-test2-tools PKG@@mingw-w64-x86_64-perl-test2-tools-asyncsubtest PKG@@mingw-w64-x86_64-perl-test2-tools-basic PKG@@mingw-w64-x86_64-perl-test2-tools-class PKG@@mingw-w64-x86_64-perl-test2-tools-classiccompare PKG@@mingw-w64-x86_64-perl-test2-tools-compare PKG@@mingw-w64-x86_64-perl-test2-tools-defer PKG@@mingw-w64-x86_64-perl-test2-tools-encoding PKG@@mingw-w64-x86_64-perl-test2-tools-event PKG@@mingw-w64-x86_64-perl-test2-tools-exception PKG@@mingw-w64-x86_64-perl-test2-tools-exports PKG@@mingw-w64-x86_64-perl-test2-tools-gentemp PKG@@mingw-w64-x86_64-perl-test2-tools-grab PKG@@mingw-w64-x86_64-perl-test2-tools-mock PKG@@mingw-w64-x86_64-perl-test2-tools-ref PKG@@mingw-w64-x86_64-perl-test2-tools-refcount PKG@@mingw-w64-x86_64-perl-test2-tools-spec PKG@@mingw-w64-x86_64-perl-test2-tools-subtest PKG@@mingw-w64-x86_64-perl-test2-tools-target PKG@@mingw-w64-x86_64-perl-test2-tools-tester PKG@@mingw-w64-x86_64-perl-test2-tools-tiny PKG@@mingw-w64-x86_64-perl-test2-tools-warnings PKG@@mingw-w64-x86_64-perl-test2-util PKG@@mingw-w64-x86_64-perl-test2-util-externalmeta PKG@@mingw-w64-x86_64-perl-test2-util-facets2legacy PKG@@mingw-w64-x86_64-perl-test2-util-grabber PKG@@mingw-w64-x86_64-perl-test2-util-guard PKG@@mingw-w64-x86_64-perl-test2-util-hashbase PKG@@mingw-w64-x86_64-perl-test2-util-importer PKG@@mingw-w64-x86_64-perl-test2-util-ref PKG@@mingw-w64-x86_64-perl-test2-util-sig PKG@@mingw-w64-x86_64-perl-test2-util-stash PKG@@mingw-w64-x86_64-perl-test2-util-sub PKG@@mingw-w64-x86_64-perl-test2-util-table PKG@@mingw-w64-x86_64-perl-test2-util-table-cell PKG@@mingw-w64-x86_64-perl-test2-util-table-linebreak PKG@@mingw-w64-x86_64-perl-test2-util-term PKG@@mingw-w64-x86_64-perl-test2-util-times PKG@@mingw-w64-x86_64-perl-test2-util-trace PKG@@mingw-w64-x86_64-perl-test2-v0 PKG@@mingw-w64-x86_64-perl-test2-workflow PKG@@mingw-w64-x86_64-perl-test2-workflow-blockbase PKG@@mingw-w64-x86_64-perl-test2-workflow-build PKG@@mingw-w64-x86_64-perl-test2-workflow-runner PKG@@mingw-w64-x86_64-perl-test2-workflow-task PKG@@mingw-w64-x86_64-perl-test2-workflow-task-action PKG@@mingw-w64-x86_64-perl-test2-workflow-task-group PKG@@mingw-w64-x86_64-perl-text-abbrev PKG@@mingw-w64-x86_64-perl-text-balanced PKG@@mingw-w64-x86_64-perl-text-parsewords PKG@@mingw-w64-x86_64-perl-text-tabs PKG@@mingw-w64-x86_64-perl-text-wrap PKG@@mingw-w64-x86_64-perl-thread PKG@@mingw-w64-x86_64-perl-thread-queue PKG@@mingw-w64-x86_64-perl-thread-semaphore PKG@@mingw-w64-x86_64-perl-threads PKG@@mingw-w64-x86_64-perl-threads-shared PKG@@mingw-w64-x86_64-perl-tie-array PKG@@mingw-w64-x86_64-perl-tie-file PKG@@mingw-w64-x86_64-perl-tie-handle PKG@@mingw-w64-x86_64-perl-tie-hash PKG@@mingw-w64-x86_64-perl-tie-hash-namedcapture PKG@@mingw-w64-x86_64-perl-tie-memoize PKG@@mingw-w64-x86_64-perl-tie-refhash PKG@@mingw-w64-x86_64-perl-tie-scalar PKG@@mingw-w64-x86_64-perl-tie-stdhandle PKG@@mingw-w64-x86_64-perl-tie-substrhash PKG@@mingw-w64-x86_64-perl-time-gmtime PKG@@mingw-w64-x86_64-perl-time-hires PKG@@mingw-w64-x86_64-perl-time-local PKG@@mingw-w64-x86_64-perl-time-localtime PKG@@mingw-w64-x86_64-perl-time-piece PKG@@mingw-w64-x86_64-perl-time-seconds PKG@@mingw-w64-x86_64-perl-time-tm PKG@@mingw-w64-x86_64-perl-unicode PKG@@mingw-w64-x86_64-perl-unicode-collate PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-big5 PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-gb2312 PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-jisx0208 PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-korean PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-pinyin PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-stroke PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-zhuyin PKG@@mingw-w64-x86_64-perl-unicode-collate-locale PKG@@mingw-w64-x86_64-perl-unicode-normalize PKG@@mingw-w64-x86_64-perl-unicode-ucd PKG@@mingw-w64-x86_64-perl-unicore-name PKG@@mingw-w64-x86_64-perl-universal PKG@@mingw-w64-x86_64-perl-user-grent PKG@@mingw-w64-x86_64-perl-user-pwent PKG@@mingw-w64-x86_64-perl-utf8 PKG@@mingw-w64-x86_64-perl-vars PKG@@mingw-w64-x86_64-perl-version PKG@@mingw-w64-x86_64-perl-version-regex PKG@@mingw-w64-x86_64-perl-vms-dclsym PKG@@mingw-w64-x86_64-perl-vms-filespec PKG@@mingw-w64-x86_64-perl-vms-stdio PKG@@mingw-w64-x86_64-perl-vmsish PKG@@mingw-w64-x86_64-perl-warnings PKG@@mingw-w64-x86_64-perl-warnings-register PKG@@mingw-w64-x86_64-perl-win32 PKG@@mingw-w64-x86_64-perl-win32api-file PKG@@mingw-w64-x86_64-perl-win32core PKG@@mingw-w64-x86_64-perl-xs-apitest PKG@@mingw-w64-x86_64-perl-xs-typemap PKG@@mingw-w64-x86_64-perl-xsloader
PKG@@mingw-w64-x86_64-perl PKG@@mingw-w64-x86_64-perl-_charnames PKG@@mingw-w64-x86_64-perl-amiga-arexx PKG@@mingw-w64-x86_64-perl-amiga-exec PKG@@mingw-w64-x86_64-perl-anydbm_file PKG@@mingw-w64-x86_64-perl-app-cpan PKG@@mingw-w64-x86_64-perl-app-prove PKG@@mingw-w64-x86_64-perl-app-prove-state PKG@@mingw-w64-x86_64-perl-app-prove-state-result PKG@@mingw-w64-x86_64-perl-app-prove-state-result-test PKG@@mingw-w64-x86_64-perl-archive-tar PKG@@mingw-w64-x86_64-perl-archive-tar-constant PKG@@mingw-w64-x86_64-perl-archive-tar-file PKG@@mingw-w64-x86_64-perl-attribute-handlers PKG@@mingw-w64-x86_64-perl-attributes PKG@@mingw-w64-x86_64-perl-autodie PKG@@mingw-w64-x86_64-perl-autodie-exception PKG@@mingw-w64-x86_64-perl-autodie-exception-system PKG@@mingw-w64-x86_64-perl-autodie-hints PKG@@mingw-w64-x86_64-perl-autodie-scope-guard PKG@@mingw-w64-x86_64-perl-autodie-scope-guardstack PKG@@mingw-w64-x86_64-perl-autodie-skip PKG@@mingw-w64-x86_64-perl-autodie-util PKG@@mingw-w64-x86_64-perl-autoloader PKG@@mingw-w64-x86_64-perl-autosplit PKG@@mingw-w64-x86_64-perl-autouse PKG@@mingw-w64-x86_64-perl-b PKG@@mingw-w64-x86_64-perl-b-concise PKG@@mingw-w64-x86_64-perl-b-deparse PKG@@mingw-w64-x86_64-perl-b-op_private PKG@@mingw-w64-x86_64-perl-b-showlex PKG@@mingw-w64-x86_64-perl-b-terse PKG@@mingw-w64-x86_64-perl-b-xref PKG@@mingw-w64-x86_64-perl-base PKG@@mingw-w64-x86_64-perl-benchmark PKG@@mingw-w64-x86_64-perl-bigfloat PKG@@mingw-w64-x86_64-perl-bigint PKG@@mingw-w64-x86_64-perl-bignum PKG@@mingw-w64-x86_64-perl-bigrat PKG@@mingw-w64-x86_64-perl-blib PKG@@mingw-w64-x86_64-perl-builtin PKG@@mingw-w64-x86_64-perl-bytes PKG@@mingw-w64-x86_64-perl-carp PKG@@mingw-w64-x86_64-perl-carp-heavy PKG@@mingw-w64-x86_64-perl-charnames PKG@@mingw-w64-x86_64-perl-class-struct PKG@@mingw-w64-x86_64-perl-compress-raw-bzip2 PKG@@mingw-w64-x86_64-perl-compress-raw-zlib PKG@@mingw-w64-x86_64-perl-compress-zlib PKG@@mingw-w64-x86_64-perl-config PKG@@mingw-w64-x86_64-perl-config-extensions PKG@@mingw-w64-x86_64-perl-config-perl-v PKG@@mingw-w64-x86_64-perl-constant PKG@@mingw-w64-x86_64-perl-cpan PKG@@mingw-w64-x86_64-perl-cpan-author PKG@@mingw-w64-x86_64-perl-cpan-bundle PKG@@mingw-w64-x86_64-perl-cpan-cachemgr PKG@@mingw-w64-x86_64-perl-cpan-complete PKG@@mingw-w64-x86_64-perl-cpan-debug PKG@@mingw-w64-x86_64-perl-cpan-deferredcode PKG@@mingw-w64-x86_64-perl-cpan-distribution PKG@@mingw-w64-x86_64-perl-cpan-distroprefs PKG@@mingw-w64-x86_64-perl-cpan-distrostatus PKG@@mingw-w64-x86_64-perl-cpan-exception-blocked_urllist PKG@@mingw-w64-x86_64-perl-cpan-exception-recursivedependency PKG@@mingw-w64-x86_64-perl-cpan-exception-yaml_not_installed PKG@@mingw-w64-x86_64-perl-cpan-exception-yaml_process_error PKG@@mingw-w64-x86_64-perl-cpan-firsttime PKG@@mingw-w64-x86_64-perl-cpan-ftp PKG@@mingw-w64-x86_64-perl-cpan-ftp-netrc PKG@@mingw-w64-x86_64-perl-cpan-handleconfig PKG@@mingw-w64-x86_64-perl-cpan-http-client PKG@@mingw-w64-x86_64-perl-cpan-http-credentials PKG@@mingw-w64-x86_64-perl-cpan-index PKG@@mingw-w64-x86_64-perl-cpan-infoobj PKG@@mingw-w64-x86_64-perl-cpan-kwalify PKG@@mingw-w64-x86_64-perl-cpan-lwp-useragent PKG@@mingw-w64-x86_64-perl-cpan-meta PKG@@mingw-w64-x86_64-perl-cpan-meta-converter PKG@@mingw-w64-x86_64-perl-cpan-meta-feature PKG@@mingw-w64-x86_64-perl-cpan-meta-history PKG@@mingw-w64-x86_64-perl-cpan-meta-merge PKG@@mingw-w64-x86_64-perl-cpan-meta-prereqs PKG@@mingw-w64-x86_64-perl-cpan-meta-requirements PKG@@mingw-w64-x86_64-perl-cpan-meta-requirements-range PKG@@mingw-w64-x86_64-perl-cpan-meta-spec PKG@@mingw-w64-x86_64-perl-cpan-meta-validator PKG@@mingw-w64-x86_64-perl-cpan-meta-yaml PKG@@mingw-w64-x86_64-perl-cpan-mirrors PKG@@mingw-w64-x86_64-perl-cpan-module PKG@@mingw-w64-x86_64-perl-cpan-nox PKG@@mingw-w64-x86_64-perl-cpan-plugin PKG@@mingw-w64-x86_64-perl-cpan-plugin-specfile PKG@@mingw-w64-x86_64-perl-cpan-prompt PKG@@mingw-w64-x86_64-perl-cpan-queue PKG@@mingw-w64-x86_64-perl-cpan-shell PKG@@mingw-w64-x86_64-perl-cpan-tarzip PKG@@mingw-w64-x86_64-perl-cpan-url PKG@@mingw-w64-x86_64-perl-cpan-version PKG@@mingw-w64-x86_64-perl-cwd PKG@@mingw-w64-x86_64-perl-data-dumper PKG@@mingw-w64-x86_64-perl-db PKG@@mingw-w64-x86_64-perl-db_file PKG@@mingw-w64-x86_64-perl-dbm_filter PKG@@mingw-w64-x86_64-perl-dbm_filter-compress PKG@@mingw-w64-x86_64-perl-dbm_filter-encode PKG@@mingw-w64-x86_64-perl-dbm_filter-int32 PKG@@mingw-w64-x86_64-perl-dbm_filter-null PKG@@mingw-w64-x86_64-perl-dbm_filter-utf8 PKG@@mingw-w64-x86_64-perl-deprecate PKG@@mingw-w64-x86_64-perl-devel-peek PKG@@mingw-w64-x86_64-perl-devel-ppport PKG@@mingw-w64-x86_64-perl-devel-selfstubber PKG@@mingw-w64-x86_64-perl-diagnostics PKG@@mingw-w64-x86_64-perl-digest PKG@@mingw-w64-x86_64-perl-digest-base PKG@@mingw-w64-x86_64-perl-digest-file PKG@@mingw-w64-x86_64-perl-digest-md5 PKG@@mingw-w64-x86_64-perl-dirhandle PKG@@mingw-w64-x86_64-perl-dumpvalue PKG@@mingw-w64-x86_64-perl-dynaloader PKG@@mingw-w64-x86_64-perl-encode PKG@@mingw-w64-x86_64-perl-encode-alias PKG@@mingw-w64-x86_64-perl-encode-byte PKG@@mingw-w64-x86_64-perl-encode-cjkconstants PKG@@mingw-w64-x86_64-perl-encode-cn PKG@@mingw-w64-x86_64-perl-encode-cn-hz PKG@@mingw-w64-x86_64-perl-encode-config PKG@@mingw-w64-x86_64-perl-encode-ebcdic PKG@@mingw-w64-x86_64-perl-encode-encoder PKG@@mingw-w64-x86_64-perl-encode-encoding PKG@@mingw-w64-x86_64-perl-encode-gsm0338 PKG@@mingw-w64-x86_64-perl-encode-guess PKG@@mingw-w64-x86_64-perl-encode-jp PKG@@mingw-w64-x86_64-perl-encode-jp-h2z PKG@@mingw-w64-x86_64-perl-encode-jp-jis7 PKG@@mingw-w64-x86_64-perl-encode-kr PKG@@mingw-w64-x86_64-perl-encode-kr-2022_kr PKG@@mingw-w64-x86_64-perl-encode-mime-header PKG@@mingw-w64-x86_64-perl-encode-mime-header-iso_2022_jp PKG@@mingw-w64-x86_64-perl-encode-mime-name PKG@@mingw-w64-x86_64-perl-encode-symbol PKG@@mingw-w64-x86_64-perl-encode-tw PKG@@mingw-w64-x86_64-perl-encode-unicode PKG@@mingw-w64-x86_64-perl-encode-unicode-utf7 PKG@@mingw-w64-x86_64-perl-encoding PKG@@mingw-w64-x86_64-perl-encoding-warnings PKG@@mingw-w64-x86_64-perl-english PKG@@mingw-w64-x86_64-perl-env PKG@@mingw-w64-x86_64-perl-errno PKG@@mingw-w64-x86_64-perl-experimental PKG@@mingw-w64-x86_64-perl-exporter PKG@@mingw-w64-x86_64-perl-exporter-heavy PKG@@mingw-w64-x86_64-perl-extutils-cbuilder PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-base PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-aix PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-android PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-cygwin PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-darwin PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-dec_osf PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-os2 PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-unix PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-vms PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-windows PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-windows-bcc PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-windows-gcc PKG@@mingw-w64-x86_64-perl-extutils-cbuilder-platform-windows-msvc PKG@@mingw-w64-x86_64-perl-extutils-command PKG@@mingw-w64-x86_64-perl-extutils-command-mm PKG@@mingw-w64-x86_64-perl-extutils-constant PKG@@mingw-w64-x86_64-perl-extutils-constant-base PKG@@mingw-w64-x86_64-perl-extutils-constant-proxysubs PKG@@mingw-w64-x86_64-perl-extutils-constant-utils PKG@@mingw-w64-x86_64-perl-extutils-constant-xs PKG@@mingw-w64-x86_64-perl-extutils-embed PKG@@mingw-w64-x86_64-perl-extutils-install PKG@@mingw-w64-x86_64-perl-extutils-installed PKG@@mingw-w64-x86_64-perl-extutils-liblist PKG@@mingw-w64-x86_64-perl-extutils-liblist-kid PKG@@mingw-w64-x86_64-perl-extutils-makemaker PKG@@mingw-w64-x86_64-perl-extutils-makemaker-config PKG@@mingw-w64-x86_64-perl-extutils-makemaker-locale PKG@@mingw-w64-x86_64-perl-extutils-makemaker-version PKG@@mingw-w64-x86_64-perl-extutils-makemaker-version-regex PKG@@mingw-w64-x86_64-perl-extutils-manifest PKG@@mingw-w64-x86_64-perl-extutils-miniperl PKG@@mingw-w64-x86_64-perl-extutils-mkbootstrap PKG@@mingw-w64-x86_64-perl-extutils-mksymlists PKG@@mingw-w64-x86_64-perl-extutils-mm PKG@@mingw-w64-x86_64-perl-extutils-mm_aix PKG@@mingw-w64-x86_64-perl-extutils-mm_any PKG@@mingw-w64-x86_64-perl-extutils-mm_beos PKG@@mingw-w64-x86_64-perl-extutils-mm_cygwin PKG@@mingw-w64-x86_64-perl-extutils-mm_darwin PKG@@mingw-w64-x86_64-perl-extutils-mm_dos PKG@@mingw-w64-x86_64-perl-extutils-mm_macos PKG@@mingw-w64-x86_64-perl-extutils-mm_nw5 PKG@@mingw-w64-x86_64-perl-extutils-mm_os2 PKG@@mingw-w64-x86_64-perl-extutils-mm_os390 PKG@@mingw-w64-x86_64-perl-extutils-mm_qnx PKG@@mingw-w64-x86_64-perl-extutils-mm_unix PKG@@mingw-w64-x86_64-perl-extutils-mm_uwin PKG@@mingw-w64-x86_64-perl-extutils-mm_vms PKG@@mingw-w64-x86_64-perl-extutils-mm_vos PKG@@mingw-w64-x86_64-perl-extutils-mm_win32 PKG@@mingw-w64-x86_64-perl-extutils-mm_win95 PKG@@mingw-w64-x86_64-perl-extutils-my PKG@@mingw-w64-x86_64-perl-extutils-packlist PKG@@mingw-w64-x86_64-perl-extutils-parsexs PKG@@mingw-w64-x86_64-perl-extutils-parsexs-constants PKG@@mingw-w64-x86_64-perl-extutils-parsexs-countlines PKG@@mingw-w64-x86_64-perl-extutils-parsexs-eval PKG@@mingw-w64-x86_64-perl-extutils-parsexs-node PKG@@mingw-w64-x86_64-perl-extutils-parsexs-utilities PKG@@mingw-w64-x86_64-perl-extutils-pl2bat PKG@@mingw-w64-x86_64-perl-extutils-testlib PKG@@mingw-w64-x86_64-perl-extutils-typemaps PKG@@mingw-w64-x86_64-perl-extutils-typemaps-cmd PKG@@mingw-w64-x86_64-perl-extutils-typemaps-inputmap PKG@@mingw-w64-x86_64-perl-extutils-typemaps-outputmap PKG@@mingw-w64-x86_64-perl-extutils-typemaps-type PKG@@mingw-w64-x86_64-perl-extutils-xssymset PKG@@mingw-w64-x86_64-perl-fatal PKG@@mingw-w64-x86_64-perl-fcntl PKG@@mingw-w64-x86_64-perl-feature PKG@@mingw-w64-x86_64-perl-fields PKG@@mingw-w64-x86_64-perl-file-basename PKG@@mingw-w64-x86_64-perl-file-compare PKG@@mingw-w64-x86_64-perl-file-copy PKG@@mingw-w64-x86_64-perl-file-dosglob PKG@@mingw-w64-x86_64-perl-file-fetch PKG@@mingw-w64-x86_64-perl-file-find PKG@@mingw-w64-x86_64-perl-file-glob PKG@@mingw-w64-x86_64-perl-file-globmapper PKG@@mingw-w64-x86_64-perl-file-path PKG@@mingw-w64-x86_64-perl-file-spec PKG@@mingw-w64-x86_64-perl-file-spec-amigaos PKG@@mingw-w64-x86_64-perl-file-spec-cygwin PKG@@mingw-w64-x86_64-perl-file-spec-epoc PKG@@mingw-w64-x86_64-perl-file-spec-functions PKG@@mingw-w64-x86_64-perl-file-spec-mac PKG@@mingw-w64-x86_64-perl-file-spec-os2 PKG@@mingw-w64-x86_64-perl-file-spec-unix PKG@@mingw-w64-x86_64-perl-file-spec-vms PKG@@mingw-w64-x86_64-perl-file-spec-win32 PKG@@mingw-w64-x86_64-perl-file-stat PKG@@mingw-w64-x86_64-perl-file-temp PKG@@mingw-w64-x86_64-perl-filecache PKG@@mingw-w64-x86_64-perl-filehandle PKG@@mingw-w64-x86_64-perl-filetest PKG@@mingw-w64-x86_64-perl-filter-simple PKG@@mingw-w64-x86_64-perl-filter-util-call PKG@@mingw-w64-x86_64-perl-findbin PKG@@mingw-w64-x86_64-perl-gdbm_file PKG@@mingw-w64-x86_64-perl-getopt-long PKG@@mingw-w64-x86_64-perl-getopt-long-parser PKG@@mingw-w64-x86_64-perl-getopt-std PKG@@mingw-w64-x86_64-perl-hash-util PKG@@mingw-w64-x86_64-perl-hash-util-fieldhash PKG@@mingw-w64-x86_64-perl-http-tiny PKG@@mingw-w64-x86_64-perl-i18n-collate PKG@@mingw-w64-x86_64-perl-i18n-langinfo PKG@@mingw-w64-x86_64-perl-i18n-langtags PKG@@mingw-w64-x86_64-perl-i18n-langtags-detect PKG@@mingw-w64-x86_64-perl-i18n-langtags-list PKG@@mingw-w64-x86_64-perl-if PKG@@mingw-w64-x86_64-perl-integer PKG@@mingw-w64-x86_64-perl-io PKG@@mingw-w64-x86_64-perl-io-compress PKG@@mingw-w64-x86_64-perl-io-compress-adapter-bzip2 PKG@@mingw-w64-x86_64-perl-io-compress-adapter-deflate PKG@@mingw-w64-x86_64-perl-io-compress-adapter-identity PKG@@mingw-w64-x86_64-perl-io-compress-base PKG@@mingw-w64-x86_64-perl-io-compress-base-common PKG@@mingw-w64-x86_64-perl-io-compress-bzip2 PKG@@mingw-w64-x86_64-perl-io-compress-deflate PKG@@mingw-w64-x86_64-perl-io-compress-gzip PKG@@mingw-w64-x86_64-perl-io-compress-gzip-constants PKG@@mingw-w64-x86_64-perl-io-compress-rawdeflate PKG@@mingw-w64-x86_64-perl-io-compress-zip PKG@@mingw-w64-x86_64-perl-io-compress-zip-constants PKG@@mingw-w64-x86_64-perl-io-compress-zlib-constants PKG@@mingw-w64-x86_64-perl-io-compress-zlib-extra PKG@@mingw-w64-x86_64-perl-io-dir PKG@@mingw-w64-x86_64-perl-io-file PKG@@mingw-w64-x86_64-perl-io-handle PKG@@mingw-w64-x86_64-perl-io-pipe PKG@@mingw-w64-x86_64-perl-io-poll PKG@@mingw-w64-x86_64-perl-io-seekable PKG@@mingw-w64-x86_64-perl-io-select PKG@@mingw-w64-x86_64-perl-io-socket PKG@@mingw-w64-x86_64-perl-io-socket-inet PKG@@mingw-w64-x86_64-perl-io-socket-ip PKG@@mingw-w64-x86_64-perl-io-socket-unix PKG@@mingw-w64-x86_64-perl-io-uncompress-adapter-bunzip2 PKG@@mingw-w64-x86_64-perl-io-uncompress-adapter-identity PKG@@mingw-w64-x86_64-perl-io-uncompress-adapter-inflate PKG@@mingw-w64-x86_64-perl-io-uncompress-anyinflate PKG@@mingw-w64-x86_64-perl-io-uncompress-anyuncompress PKG@@mingw-w64-x86_64-perl-io-uncompress-base PKG@@mingw-w64-x86_64-perl-io-uncompress-bunzip2 PKG@@mingw-w64-x86_64-perl-io-uncompress-gunzip PKG@@mingw-w64-x86_64-perl-io-uncompress-inflate PKG@@mingw-w64-x86_64-perl-io-uncompress-rawinflate PKG@@mingw-w64-x86_64-perl-io-uncompress-unzip PKG@@mingw-w64-x86_64-perl-io-zlib PKG@@mingw-w64-x86_64-perl-ipc-cmd PKG@@mingw-w64-x86_64-perl-ipc-msg PKG@@mingw-w64-x86_64-perl-ipc-open2 PKG@@mingw-w64-x86_64-perl-ipc-open3 PKG@@mingw-w64-x86_64-perl-ipc-semaphore PKG@@mingw-w64-x86_64-perl-ipc-sharedmem PKG@@mingw-w64-x86_64-perl-ipc-sysv PKG@@mingw-w64-x86_64-perl-json-pp PKG@@mingw-w64-x86_64-perl-json-pp-boolean PKG@@mingw-w64-x86_64-perl-less PKG@@mingw-w64-x86_64-perl-lib PKG@@mingw-w64-x86_64-perl-list-util PKG@@mingw-w64-x86_64-perl-list-util-xs PKG@@mingw-w64-x86_64-perl-locale PKG@@mingw-w64-x86_64-perl-locale-maketext PKG@@mingw-w64-x86_64-perl-locale-maketext-guts PKG@@mingw-w64-x86_64-perl-locale-maketext-gutsloader PKG@@mingw-w64-x86_64-perl-locale-maketext-simple PKG@@mingw-w64-x86_64-perl-math-bigfloat PKG@@mingw-w64-x86_64-perl-math-bigfloat-trace PKG@@mingw-w64-x86_64-perl-math-bigint PKG@@mingw-w64-x86_64-perl-math-bigint-calc PKG@@mingw-w64-x86_64-perl-math-bigint-fastcalc PKG@@mingw-w64-x86_64-perl-math-bigint-lib PKG@@mingw-w64-x86_64-perl-math-bigint-trace PKG@@mingw-w64-x86_64-perl-math-bigrat PKG@@mingw-w64-x86_64-perl-math-bigrat-trace PKG@@mingw-w64-x86_64-perl-math-complex PKG@@mingw-w64-x86_64-perl-math-trig PKG@@mingw-w64-x86_64-perl-memoize PKG@@mingw-w64-x86_64-perl-memoize-anydbm_file PKG@@mingw-w64-x86_64-perl-memoize-expire PKG@@mingw-w64-x86_64-perl-memoize-ndbm_file PKG@@mingw-w64-x86_64-perl-memoize-sdbm_file PKG@@mingw-w64-x86_64-perl-memoize-storable PKG@@mingw-w64-x86_64-perl-meta_notation PKG@@mingw-w64-x86_64-perl-mime-base64 PKG@@mingw-w64-x86_64-perl-mime-quotedprint PKG@@mingw-w64-x86_64-perl-module-corelist PKG@@mingw-w64-x86_64-perl-module-corelist-utils PKG@@mingw-w64-x86_64-perl-module-load PKG@@mingw-w64-x86_64-perl-module-load-conditional PKG@@mingw-w64-x86_64-perl-module-loaded PKG@@mingw-w64-x86_64-perl-module-metadata PKG@@mingw-w64-x86_64-perl-mro PKG@@mingw-w64-x86_64-perl-ndbm_file PKG@@mingw-w64-x86_64-perl-net-cmd PKG@@mingw-w64-x86_64-perl-net-config PKG@@mingw-w64-x86_64-perl-net-domain PKG@@mingw-w64-x86_64-perl-net-ftp PKG@@mingw-w64-x86_64-perl-net-ftp-a PKG@@mingw-w64-x86_64-perl-net-ftp-dataconn PKG@@mingw-w64-x86_64-perl-net-ftp-e PKG@@mingw-w64-x86_64-perl-net-ftp-i PKG@@mingw-w64-x86_64-perl-net-ftp-l PKG@@mingw-w64-x86_64-perl-net-hostent PKG@@mingw-w64-x86_64-perl-net-netent PKG@@mingw-w64-x86_64-perl-net-netrc PKG@@mingw-w64-x86_64-perl-net-nntp PKG@@mingw-w64-x86_64-perl-net-ping PKG@@mingw-w64-x86_64-perl-net-pop3 PKG@@mingw-w64-x86_64-perl-net-protoent PKG@@mingw-w64-x86_64-perl-net-servent PKG@@mingw-w64-x86_64-perl-net-smtp PKG@@mingw-w64-x86_64-perl-net-time PKG@@mingw-w64-x86_64-perl-next PKG@@mingw-w64-x86_64-perl-o PKG@@mingw-w64-x86_64-perl-odbm_file PKG@@mingw-w64-x86_64-perl-ok PKG@@mingw-w64-x86_64-perl-opcode PKG@@mingw-w64-x86_64-perl-open PKG@@mingw-w64-x86_64-perl-ops PKG@@mingw-w64-x86_64-perl-os2-dll PKG@@mingw-w64-x86_64-perl-os2-extattr PKG@@mingw-w64-x86_64-perl-os2-prfdb PKG@@mingw-w64-x86_64-perl-os2-process PKG@@mingw-w64-x86_64-perl-os2-rexx PKG@@mingw-w64-x86_64-perl-overload PKG@@mingw-w64-x86_64-perl-overload-numbers PKG@@mingw-w64-x86_64-perl-overloading PKG@@mingw-w64-x86_64-perl-params-check PKG@@mingw-w64-x86_64-perl-parent PKG@@mingw-w64-x86_64-perl-parse-cpan-meta PKG@@mingw-w64-x86_64-perl-perl-ostype PKG@@mingw-w64-x86_64-perl-perlfaq PKG@@mingw-w64-x86_64-perl-perlio PKG@@mingw-w64-x86_64-perl-perlio-encoding PKG@@mingw-w64-x86_64-perl-perlio-mmap PKG@@mingw-w64-x86_64-perl-perlio-scalar PKG@@mingw-w64-x86_64-perl-perlio-via PKG@@mingw-w64-x86_64-perl-perlio-via-quotedprint PKG@@mingw-w64-x86_64-perl-pod-checker PKG@@mingw-w64-x86_64-perl-pod-escapes PKG@@mingw-w64-x86_64-perl-pod-functions PKG@@mingw-w64-x86_64-perl-pod-html PKG@@mingw-w64-x86_64-perl-pod-html-util PKG@@mingw-w64-x86_64-perl-pod-man PKG@@mingw-w64-x86_64-perl-pod-parselink PKG@@mingw-w64-x86_64-perl-pod-perldoc PKG@@mingw-w64-x86_64-perl-pod-perldoc-baseto PKG@@mingw-w64-x86_64-perl-pod-perldoc-getoptsoo PKG@@mingw-w64-x86_64-perl-pod-perldoc-toansi PKG@@mingw-w64-x86_64-perl-pod-perldoc-tochecker PKG@@mingw-w64-x86_64-perl-pod-perldoc-toman PKG@@mingw-w64-x86_64-perl-pod-perldoc-tonroff PKG@@mingw-w64-x86_64-perl-pod-perldoc-topod PKG@@mingw-w64-x86_64-perl-pod-perldoc-tortf PKG@@mingw-w64-x86_64-perl-pod-perldoc-toterm PKG@@mingw-w64-x86_64-perl-pod-perldoc-totext PKG@@mingw-w64-x86_64-perl-pod-perldoc-totk PKG@@mingw-w64-x86_64-perl-pod-perldoc-toxml PKG@@mingw-w64-x86_64-perl-pod-simple PKG@@mingw-w64-x86_64-perl-pod-simple-blackbox PKG@@mingw-w64-x86_64-perl-pod-simple-checker PKG@@mingw-w64-x86_64-perl-pod-simple-debug PKG@@mingw-w64-x86_64-perl-pod-simple-dumpastext PKG@@mingw-w64-x86_64-perl-pod-simple-dumpasxml PKG@@mingw-w64-x86_64-perl-pod-simple-html PKG@@mingw-w64-x86_64-perl-pod-simple-htmlbatch PKG@@mingw-w64-x86_64-perl-pod-simple-htmllegacy PKG@@mingw-w64-x86_64-perl-pod-simple-justpod PKG@@mingw-w64-x86_64-perl-pod-simple-linksection PKG@@mingw-w64-x86_64-perl-pod-simple-methody PKG@@mingw-w64-x86_64-perl-pod-simple-progress PKG@@mingw-w64-x86_64-perl-pod-simple-pullparser PKG@@mingw-w64-x86_64-perl-pod-simple-pullparserendtoken PKG@@mingw-w64-x86_64-perl-pod-simple-pullparserstarttoken PKG@@mingw-w64-x86_64-perl-pod-simple-pullparsertexttoken PKG@@mingw-w64-x86_64-perl-pod-simple-pullparsertoken PKG@@mingw-w64-x86_64-perl-pod-simple-rtf PKG@@mingw-w64-x86_64-perl-pod-simple-search PKG@@mingw-w64-x86_64-perl-pod-simple-simpletree PKG@@mingw-w64-x86_64-perl-pod-simple-text PKG@@mingw-w64-x86_64-perl-pod-simple-textcontent PKG@@mingw-w64-x86_64-perl-pod-simple-tiedoutfh PKG@@mingw-w64-x86_64-perl-pod-simple-transcode PKG@@mingw-w64-x86_64-perl-pod-simple-transcodedumb PKG@@mingw-w64-x86_64-perl-pod-simple-transcodesmart PKG@@mingw-w64-x86_64-perl-pod-simple-xhtml PKG@@mingw-w64-x86_64-perl-pod-simple-xmloutstream PKG@@mingw-w64-x86_64-perl-pod-text PKG@@mingw-w64-x86_64-perl-pod-text-color PKG@@mingw-w64-x86_64-perl-pod-text-overstrike PKG@@mingw-w64-x86_64-perl-pod-text-termcap PKG@@mingw-w64-x86_64-perl-pod-usage PKG@@mingw-w64-x86_64-perl-posix PKG@@mingw-w64-x86_64-perl-re PKG@@mingw-w64-x86_64-perl-safe PKG@@mingw-w64-x86_64-perl-scalar-list-utils PKG@@mingw-w64-x86_64-perl-scalar-util PKG@@mingw-w64-x86_64-perl-sdbm_file PKG@@mingw-w64-x86_64-perl-search-dict PKG@@mingw-w64-x86_64-perl-selectsaver PKG@@mingw-w64-x86_64-perl-selfloader PKG@@mingw-w64-x86_64-perl-sigtrap PKG@@mingw-w64-x86_64-perl-socket PKG@@mingw-w64-x86_64-perl-sort PKG@@mingw-w64-x86_64-perl-source-encoding PKG@@mingw-w64-x86_64-perl-stable PKG@@mingw-w64-x86_64-perl-storable PKG@@mingw-w64-x86_64-perl-strict PKG@@mingw-w64-x86_64-perl-sub-util PKG@@mingw-w64-x86_64-perl-subs PKG@@mingw-w64-x86_64-perl-symbol PKG@@mingw-w64-x86_64-perl-sys-hostname PKG@@mingw-w64-x86_64-perl-sys-syslog PKG@@mingw-w64-x86_64-perl-sys-syslog-win32 PKG@@mingw-w64-x86_64-perl-tap-base PKG@@mingw-w64-x86_64-perl-tap-formatter-base PKG@@mingw-w64-x86_64-perl-tap-formatter-color PKG@@mingw-w64-x86_64-perl-tap-formatter-console PKG@@mingw-w64-x86_64-perl-tap-formatter-console-parallelsession PKG@@mingw-w64-x86_64-perl-tap-formatter-console-session PKG@@mingw-w64-x86_64-perl-tap-formatter-file PKG@@mingw-w64-x86_64-perl-tap-formatter-file-session PKG@@mingw-w64-x86_64-perl-tap-formatter-session PKG@@mingw-w64-x86_64-perl-tap-harness PKG@@mingw-w64-x86_64-perl-tap-harness-env PKG@@mingw-w64-x86_64-perl-tap-object PKG@@mingw-w64-x86_64-perl-tap-parser PKG@@mingw-w64-x86_64-perl-tap-parser-aggregator PKG@@mingw-w64-x86_64-perl-tap-parser-grammar PKG@@mingw-w64-x86_64-perl-tap-parser-iterator PKG@@mingw-w64-x86_64-perl-tap-parser-iterator-array PKG@@mingw-w64-x86_64-perl-tap-parser-iterator-process PKG@@mingw-w64-x86_64-perl-tap-parser-iterator-stream PKG@@mingw-w64-x86_64-perl-tap-parser-iteratorfactory PKG@@mingw-w64-x86_64-perl-tap-parser-multiplexer PKG@@mingw-w64-x86_64-perl-tap-parser-result PKG@@mingw-w64-x86_64-perl-tap-parser-result-bailout PKG@@mingw-w64-x86_64-perl-tap-parser-result-comment PKG@@mingw-w64-x86_64-perl-tap-parser-result-plan PKG@@mingw-w64-x86_64-perl-tap-parser-result-pragma PKG@@mingw-w64-x86_64-perl-tap-parser-result-test PKG@@mingw-w64-x86_64-perl-tap-parser-result-unknown PKG@@mingw-w64-x86_64-perl-tap-parser-result-version PKG@@mingw-w64-x86_64-perl-tap-parser-result-yaml PKG@@mingw-w64-x86_64-perl-tap-parser-resultfactory PKG@@mingw-w64-x86_64-perl-tap-parser-scheduler PKG@@mingw-w64-x86_64-perl-tap-parser-scheduler-job PKG@@mingw-w64-x86_64-perl-tap-parser-scheduler-spinner PKG@@mingw-w64-x86_64-perl-tap-parser-source PKG@@mingw-w64-x86_64-perl-tap-parser-sourcehandler PKG@@mingw-w64-x86_64-perl-tap-parser-sourcehandler-executable PKG@@mingw-w64-x86_64-perl-tap-parser-sourcehandler-file PKG@@mingw-w64-x86_64-perl-tap-parser-sourcehandler-handle PKG@@mingw-w64-x86_64-perl-tap-parser-sourcehandler-perl PKG@@mingw-w64-x86_64-perl-tap-parser-sourcehandler-rawtap PKG@@mingw-w64-x86_64-perl-tap-parser-yamlish-reader PKG@@mingw-w64-x86_64-perl-tap-parser-yamlish-writer PKG@@mingw-w64-x86_64-perl-term-ansicolor PKG@@mingw-w64-x86_64-perl-term-cap PKG@@mingw-w64-x86_64-perl-term-complete PKG@@mingw-w64-x86_64-perl-term-readline PKG@@mingw-w64-x86_64-perl-term-table PKG@@mingw-w64-x86_64-perl-term-table-cell PKG@@mingw-w64-x86_64-perl-term-table-cellstack PKG@@mingw-w64-x86_64-perl-term-table-hashbase PKG@@mingw-w64-x86_64-perl-term-table-linebreak PKG@@mingw-w64-x86_64-perl-term-table-spacer PKG@@mingw-w64-x86_64-perl-term-table-util PKG@@mingw-w64-x86_64-perl-test PKG@@mingw-w64-x86_64-perl-test-builder PKG@@mingw-w64-x86_64-perl-test-builder-formatter PKG@@mingw-w64-x86_64-perl-test-builder-module PKG@@mingw-w64-x86_64-perl-test-builder-tester PKG@@mingw-w64-x86_64-perl-test-builder-tester-color PKG@@mingw-w64-x86_64-perl-test-builder-tododiag PKG@@mingw-w64-x86_64-perl-test-harness PKG@@mingw-w64-x86_64-perl-test-more PKG@@mingw-w64-x86_64-perl-test-simple PKG@@mingw-w64-x86_64-perl-test-tester PKG@@mingw-w64-x86_64-perl-test-tester-capture PKG@@mingw-w64-x86_64-perl-test-tester-capturerunner PKG@@mingw-w64-x86_64-perl-test-tester-delegate PKG@@mingw-w64-x86_64-perl-test-use-ok PKG@@mingw-w64-x86_64-perl-test2 PKG@@mingw-w64-x86_64-perl-test2-api PKG@@mingw-w64-x86_64-perl-test2-api-breakage PKG@@mingw-w64-x86_64-perl-test2-api-context PKG@@mingw-w64-x86_64-perl-test2-api-instance PKG@@mingw-w64-x86_64-perl-test2-api-interceptresult PKG@@mingw-w64-x86_64-perl-test2-api-interceptresult-event PKG@@mingw-w64-x86_64-perl-test2-api-interceptresult-facet PKG@@mingw-w64-x86_64-perl-test2-api-interceptresult-hub PKG@@mingw-w64-x86_64-perl-test2-api-interceptresult-squasher PKG@@mingw-w64-x86_64-perl-test2-api-stack PKG@@mingw-w64-x86_64-perl-test2-asyncsubtest PKG@@mingw-w64-x86_64-perl-test2-asyncsubtest-event-attach PKG@@mingw-w64-x86_64-perl-test2-asyncsubtest-event-detach PKG@@mingw-w64-x86_64-perl-test2-asyncsubtest-formatter PKG@@mingw-w64-x86_64-perl-test2-asyncsubtest-hub PKG@@mingw-w64-x86_64-perl-test2-bundle PKG@@mingw-w64-x86_64-perl-test2-bundle-extended PKG@@mingw-w64-x86_64-perl-test2-bundle-more PKG@@mingw-w64-x86_64-perl-test2-bundle-simple PKG@@mingw-w64-x86_64-perl-test2-compare PKG@@mingw-w64-x86_64-perl-test2-compare-array PKG@@mingw-w64-x86_64-perl-test2-compare-bag PKG@@mingw-w64-x86_64-perl-test2-compare-base PKG@@mingw-w64-x86_64-perl-test2-compare-bool PKG@@mingw-w64-x86_64-perl-test2-compare-custom PKG@@mingw-w64-x86_64-perl-test2-compare-deepref PKG@@mingw-w64-x86_64-perl-test2-compare-delta PKG@@mingw-w64-x86_64-perl-test2-compare-event PKG@@mingw-w64-x86_64-perl-test2-compare-eventmeta PKG@@mingw-w64-x86_64-perl-test2-compare-float PKG@@mingw-w64-x86_64-perl-test2-compare-hash PKG@@mingw-w64-x86_64-perl-test2-compare-isa PKG@@mingw-w64-x86_64-perl-test2-compare-meta PKG@@mingw-w64-x86_64-perl-test2-compare-negatable PKG@@mingw-w64-x86_64-perl-test2-compare-number PKG@@mingw-w64-x86_64-perl-test2-compare-object PKG@@mingw-w64-x86_64-perl-test2-compare-orderedsubset PKG@@mingw-w64-x86_64-perl-test2-compare-pattern PKG@@mingw-w64-x86_64-perl-test2-compare-ref PKG@@mingw-w64-x86_64-perl-test2-compare-regex PKG@@mingw-w64-x86_64-perl-test2-compare-scalar PKG@@mingw-w64-x86_64-perl-test2-compare-set PKG@@mingw-w64-x86_64-perl-test2-compare-string PKG@@mingw-w64-x86_64-perl-test2-compare-undef PKG@@mingw-w64-x86_64-perl-test2-compare-wildcard PKG@@mingw-w64-x86_64-perl-test2-env PKG@@mingw-w64-x86_64-perl-test2-event PKG@@mingw-w64-x86_64-perl-test2-event-bail PKG@@mingw-w64-x86_64-perl-test2-event-diag PKG@@mingw-w64-x86_64-perl-test2-event-encoding PKG@@mingw-w64-x86_64-perl-test2-event-exception PKG@@mingw-w64-x86_64-perl-test2-event-fail PKG@@mingw-w64-x86_64-perl-test2-event-generic PKG@@mingw-w64-x86_64-perl-test2-event-note PKG@@mingw-w64-x86_64-perl-test2-event-ok PKG@@mingw-w64-x86_64-perl-test2-event-pass PKG@@mingw-w64-x86_64-perl-test2-event-plan PKG@@mingw-w64-x86_64-perl-test2-event-skip PKG@@mingw-w64-x86_64-perl-test2-event-subtest PKG@@mingw-w64-x86_64-perl-test2-event-tap-version PKG@@mingw-w64-x86_64-perl-test2-event-v2 PKG@@mingw-w64-x86_64-perl-test2-event-waiting PKG@@mingw-w64-x86_64-perl-test2-eventfacet PKG@@mingw-w64-x86_64-perl-test2-eventfacet-about PKG@@mingw-w64-x86_64-perl-test2-eventfacet-amnesty PKG@@mingw-w64-x86_64-perl-test2-eventfacet-assert PKG@@mingw-w64-x86_64-perl-test2-eventfacet-control PKG@@mingw-w64-x86_64-perl-test2-eventfacet-error PKG@@mingw-w64-x86_64-perl-test2-eventfacet-hub PKG@@mingw-w64-x86_64-perl-test2-eventfacet-info PKG@@mingw-w64-x86_64-perl-test2-eventfacet-info-table PKG@@mingw-w64-x86_64-perl-test2-eventfacet-meta PKG@@mingw-w64-x86_64-perl-test2-eventfacet-parent PKG@@mingw-w64-x86_64-perl-test2-eventfacet-plan PKG@@mingw-w64-x86_64-perl-test2-eventfacet-render PKG@@mingw-w64-x86_64-perl-test2-eventfacet-trace PKG@@mingw-w64-x86_64-perl-test2-formatter PKG@@mingw-w64-x86_64-perl-test2-formatter-tap PKG@@mingw-w64-x86_64-perl-test2-hub PKG@@mingw-w64-x86_64-perl-test2-hub-interceptor PKG@@mingw-w64-x86_64-perl-test2-hub-interceptor-terminator PKG@@mingw-w64-x86_64-perl-test2-hub-subtest PKG@@mingw-w64-x86_64-perl-test2-ipc PKG@@mingw-w64-x86_64-perl-test2-ipc-driver PKG@@mingw-w64-x86_64-perl-test2-ipc-driver-files PKG@@mingw-w64-x86_64-perl-test2-manual PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-api PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-context PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-endtoend PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-event PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-hubs PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-ipc PKG@@mingw-w64-x86_64-perl-test2-manual-anatomy-utilities PKG@@mingw-w64-x86_64-perl-test2-manual-concurrency PKG@@mingw-w64-x86_64-perl-test2-manual-contributing PKG@@mingw-w64-x86_64-perl-test2-manual-testing PKG@@mingw-w64-x86_64-perl-test2-manual-testing-introduction PKG@@mingw-w64-x86_64-perl-test2-manual-testing-migrating PKG@@mingw-w64-x86_64-perl-test2-manual-testing-planning PKG@@mingw-w64-x86_64-perl-test2-manual-testing-todo PKG@@mingw-w64-x86_64-perl-test2-manual-tooling PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-firsttool PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-formatter PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-nesting PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-plugin-testexit PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-plugin-testingdone PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-plugin-toolcompletes PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-plugin-toolstarts PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-subtest PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-testbuilder PKG@@mingw-w64-x86_64-perl-test2-manual-tooling-testing PKG@@mingw-w64-x86_64-perl-test2-mock PKG@@mingw-w64-x86_64-perl-test2-plugin PKG@@mingw-w64-x86_64-perl-test2-plugin-bailonfail PKG@@mingw-w64-x86_64-perl-test2-plugin-dieonfail PKG@@mingw-w64-x86_64-perl-test2-plugin-exitsummary PKG@@mingw-w64-x86_64-perl-test2-plugin-srand PKG@@mingw-w64-x86_64-perl-test2-plugin-times PKG@@mingw-w64-x86_64-perl-test2-plugin-utf8 PKG@@mingw-w64-x86_64-perl-test2-require PKG@@mingw-w64-x86_64-perl-test2-require-authortesting PKG@@mingw-w64-x86_64-perl-test2-require-automatedtesting PKG@@mingw-w64-x86_64-perl-test2-require-envvar PKG@@mingw-w64-x86_64-perl-test2-require-extendedtesting PKG@@mingw-w64-x86_64-perl-test2-require-fork PKG@@mingw-w64-x86_64-perl-test2-require-module PKG@@mingw-w64-x86_64-perl-test2-require-noninteractivetesting PKG@@mingw-w64-x86_64-perl-test2-require-perl PKG@@mingw-w64-x86_64-perl-test2-require-realfork PKG@@mingw-w64-x86_64-perl-test2-require-releasetesting PKG@@mingw-w64-x86_64-perl-test2-require-threads PKG@@mingw-w64-x86_64-perl-test2-suite PKG@@mingw-w64-x86_64-perl-test2-todo PKG@@mingw-w64-x86_64-perl-test2-tools PKG@@mingw-w64-x86_64-perl-test2-tools-asyncsubtest PKG@@mingw-w64-x86_64-perl-test2-tools-basic PKG@@mingw-w64-x86_64-perl-test2-tools-class PKG@@mingw-w64-x86_64-perl-test2-tools-classiccompare PKG@@mingw-w64-x86_64-perl-test2-tools-compare PKG@@mingw-w64-x86_64-perl-test2-tools-defer PKG@@mingw-w64-x86_64-perl-test2-tools-encoding PKG@@mingw-w64-x86_64-perl-test2-tools-event PKG@@mingw-w64-x86_64-perl-test2-tools-exception PKG@@mingw-w64-x86_64-perl-test2-tools-exports PKG@@mingw-w64-x86_64-perl-test2-tools-gentemp PKG@@mingw-w64-x86_64-perl-test2-tools-grab PKG@@mingw-w64-x86_64-perl-test2-tools-mock PKG@@mingw-w64-x86_64-perl-test2-tools-ref PKG@@mingw-w64-x86_64-perl-test2-tools-refcount PKG@@mingw-w64-x86_64-perl-test2-tools-spec PKG@@mingw-w64-x86_64-perl-test2-tools-subtest PKG@@mingw-w64-x86_64-perl-test2-tools-target PKG@@mingw-w64-x86_64-perl-test2-tools-tester PKG@@mingw-w64-x86_64-perl-test2-tools-tiny PKG@@mingw-w64-x86_64-perl-test2-tools-warnings PKG@@mingw-w64-x86_64-perl-test2-util PKG@@mingw-w64-x86_64-perl-test2-util-externalmeta PKG@@mingw-w64-x86_64-perl-test2-util-facets2legacy PKG@@mingw-w64-x86_64-perl-test2-util-grabber PKG@@mingw-w64-x86_64-perl-test2-util-guard PKG@@mingw-w64-x86_64-perl-test2-util-hashbase PKG@@mingw-w64-x86_64-perl-test2-util-importer PKG@@mingw-w64-x86_64-perl-test2-util-ref PKG@@mingw-w64-x86_64-perl-test2-util-sig PKG@@mingw-w64-x86_64-perl-test2-util-stash PKG@@mingw-w64-x86_64-perl-test2-util-sub PKG@@mingw-w64-x86_64-perl-test2-util-table PKG@@mingw-w64-x86_64-perl-test2-util-table-cell PKG@@mingw-w64-x86_64-perl-test2-util-table-linebreak PKG@@mingw-w64-x86_64-perl-test2-util-term PKG@@mingw-w64-x86_64-perl-test2-util-times PKG@@mingw-w64-x86_64-perl-test2-util-trace PKG@@mingw-w64-x86_64-perl-test2-v0 PKG@@mingw-w64-x86_64-perl-test2-workflow PKG@@mingw-w64-x86_64-perl-test2-workflow-blockbase PKG@@mingw-w64-x86_64-perl-test2-workflow-build PKG@@mingw-w64-x86_64-perl-test2-workflow-runner PKG@@mingw-w64-x86_64-perl-test2-workflow-task PKG@@mingw-w64-x86_64-perl-test2-workflow-task-action PKG@@mingw-w64-x86_64-perl-test2-workflow-task-group PKG@@mingw-w64-x86_64-perl-text-abbrev PKG@@mingw-w64-x86_64-perl-text-balanced PKG@@mingw-w64-x86_64-perl-text-parsewords PKG@@mingw-w64-x86_64-perl-text-tabs PKG@@mingw-w64-x86_64-perl-text-wrap PKG@@mingw-w64-x86_64-perl-thread PKG@@mingw-w64-x86_64-perl-thread-queue PKG@@mingw-w64-x86_64-perl-thread-semaphore PKG@@mingw-w64-x86_64-perl-threads PKG@@mingw-w64-x86_64-perl-threads-shared PKG@@mingw-w64-x86_64-perl-tie-array PKG@@mingw-w64-x86_64-perl-tie-file PKG@@mingw-w64-x86_64-perl-tie-handle PKG@@mingw-w64-x86_64-perl-tie-hash PKG@@mingw-w64-x86_64-perl-tie-hash-namedcapture PKG@@mingw-w64-x86_64-perl-tie-memoize PKG@@mingw-w64-x86_64-perl-tie-refhash PKG@@mingw-w64-x86_64-perl-tie-scalar PKG@@mingw-w64-x86_64-perl-tie-stdhandle PKG@@mingw-w64-x86_64-perl-tie-substrhash PKG@@mingw-w64-x86_64-perl-time-gmtime PKG@@mingw-w64-x86_64-perl-time-hires PKG@@mingw-w64-x86_64-perl-time-local PKG@@mingw-w64-x86_64-perl-time-localtime PKG@@mingw-w64-x86_64-perl-time-piece PKG@@mingw-w64-x86_64-perl-time-seconds PKG@@mingw-w64-x86_64-perl-time-tm PKG@@mingw-w64-x86_64-perl-unicode PKG@@mingw-w64-x86_64-perl-unicode-collate PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-big5 PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-gb2312 PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-jisx0208 PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-korean PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-pinyin PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-stroke PKG@@mingw-w64-x86_64-perl-unicode-collate-cjk-zhuyin PKG@@mingw-w64-x86_64-perl-unicode-collate-locale PKG@@mingw-w64-x86_64-perl-unicode-normalize PKG@@mingw-w64-x86_64-perl-unicode-ucd PKG@@mingw-w64-x86_64-perl-unicore-name PKG@@mingw-w64-x86_64-perl-universal PKG@@mingw-w64-x86_64-perl-user-grent PKG@@mingw-w64-x86_64-perl-user-pwent PKG@@mingw-w64-x86_64-perl-utf8 PKG@@mingw-w64-x86_64-perl-vars PKG@@mingw-w64-x86_64-perl-version PKG@@mingw-w64-x86_64-perl-version-regex PKG@@mingw-w64-x86_64-perl-vms-dclsym PKG@@mingw-w64-x86_64-perl-vms-filespec PKG@@mingw-w64-x86_64-perl-vms-stdio PKG@@mingw-w64-x86_64-perl-vmsish PKG@@mingw-w64-x86_64-perl-warnings PKG@@mingw-w64-x86_64-perl-warnings-register PKG@@mingw-w64-x86_64-perl-win32 PKG@@mingw-w64-x86_64-perl-win32api-file PKG@@mingw-w64-x86_64-perl-win32core PKG@@mingw-w64-x86_64-perl-xs-apitest PKG@@mingw-w64-x86_64-perl-xs-typemap PKG@@mingw-w64-x86_64-perl-xsloader: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-perl'

override VERSION_OF_mingw-w64-x86_64-perl-digest-sha := 6.04-7
.PHONY: PKG@@mingw-w64-x86_64-perl-digest-sha
PKG@@mingw-w64-x86_64-perl-digest-sha: $(if $(__deps),PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-perl-digest-sha'

override VERSION_OF_mingw-w64-x86_64-perl-doc := 5.44.0-3
.PHONY: PKG@@mingw-w64-x86_64-perl-doc
PKG@@mingw-w64-x86_64-perl-doc: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-perl-doc'

override VERSION_OF_mingw-w64-x86_64-perl-encode-compat := 0.07-10
.PHONY: PKG@@mingw-w64-x86_64-perl-encode-compat
PKG@@mingw-w64-x86_64-perl-encode-compat: $(if $(__deps),PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-perl-encode-compat'

override VERSION_OF_mingw-w64-x86_64-perl-file-which := 1.27-7
.PHONY: PKG@@mingw-w64-x86_64-perl-file-which
PKG@@mingw-w64-x86_64-perl-file-which: $(if $(__deps),PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-perl-file-which'

override VERSION_OF_mingw-w64-x86_64-perl-html-tagset := 3.24-7
.PHONY: PKG@@mingw-w64-x86_64-perl-html-tagset
PKG@@mingw-w64-x86_64-perl-html-tagset: $(if $(__deps),PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-perl-html-tagset'

override VERSION_OF_mingw-w64-x86_64-perl-io-string := 1.08-10
.PHONY: PKG@@mingw-w64-x86_64-perl-io-string
PKG@@mingw-w64-x86_64-perl-io-string: $(if $(__deps),PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-perl-io-string'

override VERSION_OF_mingw-w64-x86_64-perl-math-int64 := 0.57-8
.PHONY: PKG@@mingw-w64-x86_64-perl-math-int64
PKG@@mingw-w64-x86_64-perl-math-int64: $(if $(__deps),PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-perl-math-int64'

override VERSION_OF_mingw-w64-x86_64-perl-mozilla-ca := 20261105-1
.PHONY: PKG@@mingw-w64-x86_64-perl-mozilla-ca
PKG@@mingw-w64-x86_64-perl-mozilla-ca: $(if $(__deps),PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-perl-mozilla-ca'

override VERSION_OF_mingw-w64-x86_64-perl-win32-api := 0.84-12
.PHONY: PKG@@mingw-w64-x86_64-perl-win32-api
PKG@@mingw-w64-x86_64-perl-win32-api: $(if $(__deps),PKG@@mingw-w64-x86_64-perl PKG@@mingw-w64-x86_64-perl-encode-compat PKG@@mingw-w64-x86_64-perl-math-int64) ; @echo 'PKG@@mingw-w64-x86_64-perl-win32-api'

override VERSION_OF_mingw-w64-x86_64-perl-win32-console := 0.10-11
.PHONY: PKG@@mingw-w64-x86_64-perl-win32-console
PKG@@mingw-w64-x86_64-perl-win32-console: $(if $(__deps),PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-perl-win32-console'

override VERSION_OF_mingw-w64-x86_64-perl-win32-shortcut := 0.08-11
.PHONY: PKG@@mingw-w64-x86_64-perl-win32-shortcut
PKG@@mingw-w64-x86_64-perl-win32-shortcut: $(if $(__deps),PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-perl-win32-shortcut'

override VERSION_OF_mingw-w64-x86_64-perl-win32-tieregistry := 0.30-10
.PHONY: PKG@@mingw-w64-x86_64-perl-win32-tieregistry
PKG@@mingw-w64-x86_64-perl-win32-tieregistry: $(if $(__deps),PKG@@mingw-w64-x86_64-perl PKG@@mingw-w64-x86_64-perl-win32-winerror PKG@@mingw-w64-x86_64-perl-win32api-registry) ; @echo 'PKG@@mingw-w64-x86_64-perl-win32-tieregistry'

override VERSION_OF_mingw-w64-x86_64-perl-win32-winerror := 0.04-11
.PHONY: PKG@@mingw-w64-x86_64-perl-win32-winerror
PKG@@mingw-w64-x86_64-perl-win32-winerror: $(if $(__deps),PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-perl-win32-winerror'

override VERSION_OF_mingw-w64-x86_64-perl-win32api-registry := 0.33-21
.PHONY: PKG@@mingw-w64-x86_64-perl-win32api-registry
PKG@@mingw-w64-x86_64-perl-win32api-registry: $(if $(__deps),PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-perl-win32api-registry'

override VERSION_OF_mingw-w64-x86_64-pg_probackup-16 := 16.15-1
.PHONY: PKG@@mingw-w64-x86_64-pg_probackup-16
PKG@@mingw-w64-x86_64-pg_probackup-16: $(if $(__deps),PKG@@mingw-w64-x86_64-postgresql-16) ; @echo 'PKG@@mingw-w64-x86_64-pg_probackup-16'

override VERSION_OF_mingw-w64-x86_64-phodav := 3.0-5
.PHONY: PKG@@mingw-w64-x86_64-phodav
PKG@@mingw-w64-x86_64-phodav: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libsoup3 PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-phodav'

override VERSION_OF_mingw-w64-x86_64-phonon := 4.12.0-3
.PHONY: PKG@@mingw-w64-x86_64-phonon PKG@@mingw-w64-x86_64-phonon-qt6
PKG@@mingw-w64-x86_64-phonon PKG@@mingw-w64-x86_64-phonon-qt6: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-qt6-5compat PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-phonon'

override VERSION_OF_mingw-w64-x86_64-picosha2 := 1.0.1-1
.PHONY: PKG@@mingw-w64-x86_64-picosha2
PKG@@mingw-w64-x86_64-picosha2: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-picosha2'

override VERSION_OF_mingw-w64-x86_64-pikchr := 1.0-5
.PHONY: PKG@@mingw-w64-x86_64-pikchr
PKG@@mingw-w64-x86_64-pikchr: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-pikchr'

override VERSION_OF_mingw-w64-x86_64-pinentry := 1.3.3-1
.PHONY: PKG@@mingw-w64-x86_64-pinentry
PKG@@mingw-w64-x86_64-pinentry: $(if $(__deps),PKG@@mingw-w64-x86_64-libassuan PKG@@mingw-w64-x86_64-libsecret PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-pinentry'

override VERSION_OF_mingw-w64-x86_64-pixman := 0.46.4-3
.PHONY: PKG@@mingw-w64-x86_64-pixman
PKG@@mingw-w64-x86_64-pixman: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-pixman'

override VERSION_OF_mingw-w64-x86_64-pkcs11-helper := 1.31.0-1
.PHONY: PKG@@mingw-w64-x86_64-pkcs11-helper
PKG@@mingw-w64-x86_64-pkcs11-helper: $(if $(__deps),PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-pkcs11-helper'

override VERSION_OF_mingw-w64-x86_64-pkgconf := 1~3.0.10-1
.PHONY: PKG@@mingw-w64-x86_64-pkg-config PKG@@mingw-w64-x86_64-pkgconf
PKG@@mingw-w64-x86_64-pkg-config PKG@@mingw-w64-x86_64-pkgconf: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-pkgconf'

override VERSION_OF_mingw-w64-x86_64-plfit := 1.0.1-1
.PHONY: PKG@@mingw-w64-x86_64-plfit
PKG@@mingw-w64-x86_64-plfit: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-plfit'

override VERSION_OF_mingw-w64-x86_64-plutovg := 1.3.3-1
.PHONY: PKG@@mingw-w64-x86_64-plutovg
PKG@@mingw-w64-x86_64-plutovg: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-plutovg'

override VERSION_OF_mingw-w64-x86_64-podofo := 1.1.1-2
.PHONY: PKG@@mingw-w64-x86_64-podofo
PKG@@mingw-w64-x86_64-podofo: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-fontconfig PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-podofo'

override VERSION_OF_mingw-w64-x86_64-polipo := 1.1.1-3
.PHONY: PKG@@mingw-w64-x86_64-polipo
PKG@@mingw-w64-x86_64-polipo: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-polipo'

override VERSION_OF_mingw-w64-x86_64-polly := 23.1.5-1
.PHONY: PKG@@mingw-w64-x86_64-polly
PKG@@mingw-w64-x86_64-polly: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-polly'

override VERSION_OF_mingw-w64-x86_64-poppler := 26.10.0-1
.PHONY: PKG@@mingw-w64-x86_64-poppler
PKG@@mingw-w64-x86_64-poppler: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-lcms2 PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-nss PKG@@mingw-w64-x86_64-openjpeg2 PKG@@mingw-w64-x86_64-poppler-data PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-poppler'

override VERSION_OF_mingw-w64-x86_64-poppler-data := 0.4.12-1
.PHONY: PKG@@mingw-w64-x86_64-poppler-data
PKG@@mingw-w64-x86_64-poppler-data: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-poppler-data'

override VERSION_OF_mingw-w64-x86_64-poppler-qt5 := 26.10.0-1
.PHONY: PKG@@mingw-w64-x86_64-poppler-qt5
PKG@@mingw-w64-x86_64-poppler-qt5: $(if $(__deps),PKG@@mingw-w64-x86_64-poppler PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-poppler-qt5'

override VERSION_OF_mingw-w64-x86_64-poppler-qt6 := 26.10.0-1
.PHONY: PKG@@mingw-w64-x86_64-poppler-qt6
PKG@@mingw-w64-x86_64-poppler-qt6: $(if $(__deps),PKG@@mingw-w64-x86_64-poppler PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-poppler-qt6'

override VERSION_OF_mingw-w64-x86_64-popt := 1.19-2
.PHONY: PKG@@mingw-w64-x86_64-popt
PKG@@mingw-w64-x86_64-popt: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime) ; @echo 'PKG@@mingw-w64-x86_64-popt'

override VERSION_OF_mingw-w64-x86_64-port-scanner := 1.3-8
.PHONY: PKG@@mingw-w64-x86_64-port-scanner
PKG@@mingw-w64-x86_64-port-scanner: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-port-scanner'

override VERSION_OF_mingw-w64-x86_64-portablexdr := 4.9.2.r27.94fb83c-6
.PHONY: PKG@@mingw-w64-x86_64-portablexdr
PKG@@mingw-w64-x86_64-portablexdr: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-portablexdr'

override VERSION_OF_mingw-w64-x86_64-portaudio := 1~19.7.0-7
.PHONY: PKG@@mingw-w64-x86_64-portaudio
PKG@@mingw-w64-x86_64-portaudio: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-portaudio'

override VERSION_OF_mingw-w64-x86_64-portaudio-asio := 1~19.7.0-7
.PHONY: PKG@@mingw-w64-x86_64-portaudio-asio
PKG@@mingw-w64-x86_64-portaudio-asio: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-portaudio-asio'

override VERSION_OF_mingw-w64-x86_64-postgresql-16 := 16.15-1
.PHONY: PKG@@mingw-w64-x86_64-postgresql-16
PKG@@mingw-w64-x86_64-postgresql-16: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-lz4 PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-readline PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd PKG@@winpty) ; @echo 'PKG@@mingw-w64-x86_64-postgresql-16'

override VERSION_OF_mingw-w64-x86_64-postgresql := 18.6-4
.PHONY: PKG@@mingw-w64-x86_64-postgresql
PKG@@mingw-w64-x86_64-postgresql: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-lz4 PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-readline PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd PKG@@winpty) ; @echo 'PKG@@mingw-w64-x86_64-postgresql'

override VERSION_OF_mingw-w64-x86_64-potrace := 1.16-3
.PHONY: PKG@@mingw-w64-x86_64-potrace
PKG@@mingw-w64-x86_64-potrace: $(if $(__deps),PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-potrace'

override VERSION_OF_mingw-w64-x86_64-pqp := 1.3-3
.PHONY: PKG@@mingw-w64-x86_64-pqp
PKG@@mingw-w64-x86_64-pqp: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-pqp'

override VERSION_OF_mingw-w64-x86_64-prjtrellis := 1.4-8
.PHONY: PKG@@mingw-w64-x86_64-prjtrellis
PKG@@mingw-w64-x86_64-prjtrellis: $(if $(__deps),PKG@@mingw-w64-x86_64-boost-libs PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-prjtrellis'

override VERSION_OF_mingw-w64-x86_64-proj := 9.8.1-1
.PHONY: PKG@@mingw-w64-x86_64-proj
PKG@@mingw-w64-x86_64-proj: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-sqlite3) ; @echo 'PKG@@mingw-w64-x86_64-proj'

override VERSION_OF_mingw-w64-x86_64-protobuf := 36.2-2
.PHONY: PKG@@mingw-w64-x86_64-protobuf
PKG@@mingw-w64-x86_64-protobuf: $(if $(__deps),PKG@@mingw-w64-x86_64-abseil-cpp PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-protobuf'

override VERSION_OF_mingw-w64-x86_64-protobuf-c := 1.5.2-11
.PHONY: PKG@@mingw-w64-x86_64-protobuf-c
PKG@@mingw-w64-x86_64-protobuf-c: $(if $(__deps),PKG@@mingw-w64-x86_64-abseil-cpp PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-protobuf) ; @echo 'PKG@@mingw-w64-x86_64-protobuf-c'

override VERSION_OF_mingw-w64-x86_64-pugixml := 1.16-1
.PHONY: PKG@@mingw-w64-x86_64-pugixml
PKG@@mingw-w64-x86_64-pugixml: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-pugixml'

override VERSION_OF_mingw-w64-x86_64-pupnp := 1.14.31-1
.PHONY: PKG@@mingw-w64-x86_64-pupnp
PKG@@mingw-w64-x86_64-pupnp: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-pupnp'

override VERSION_OF_mingw-w64-x86_64-putty := 0.85-1
.PHONY: PKG@@mingw-w64-x86_64-putty
PKG@@mingw-w64-x86_64-putty: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-putty'

override VERSION_OF_mingw-w64-x86_64-putty-ssh := 0.0-5
.PHONY: PKG@@mingw-w64-x86_64-putty-ssh
PKG@@mingw-w64-x86_64-putty-ssh: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-putty) ; @echo 'PKG@@mingw-w64-x86_64-putty-ssh'

override VERSION_OF_mingw-w64-x86_64-pybind11 := 3.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-pybind11
PKG@@mingw-w64-x86_64-pybind11: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-pybind11'

override VERSION_OF_mingw-w64-x86_64-pybind11-json := 0.2.15-1
.PHONY: PKG@@mingw-w64-x86_64-pybind11-json
PKG@@mingw-w64-x86_64-pybind11-json: $(if $(__deps),PKG@@mingw-w64-x86_64-nlohmann-json PKG@@mingw-w64-x86_64-pybind11) ; @echo 'PKG@@mingw-w64-x86_64-pybind11-json'

override VERSION_OF_mingw-w64-x86_64-pyinstaller := 6.22.3-1
.PHONY: PKG@@mingw-w64-x86_64-pyinstaller
PKG@@mingw-w64-x86_64-pyinstaller: $(if $(__deps),PKG@@mingw-w64-x86_64-binutils PKG@@mingw-w64-x86_64-pyinstaller-hooks-contrib PKG@@mingw-w64-x86_64-python-altgraph PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-pefile PKG@@mingw-w64-x86_64-python-pywin32-ctypes PKG@@mingw-w64-x86_64-python-setuptools) ; @echo 'PKG@@mingw-w64-x86_64-pyinstaller'

override VERSION_OF_mingw-w64-x86_64-pyinstaller-hooks-contrib := 2026.9-1
.PHONY: PKG@@mingw-w64-x86_64-pyinstaller-hooks-contrib
PKG@@mingw-w64-x86_64-pyinstaller-hooks-contrib: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging) ; @echo 'PKG@@mingw-w64-x86_64-pyinstaller-hooks-contrib'

override VERSION_OF_mingw-w64-x86_64-pyqt-builder := 1.21.0-1
.PHONY: PKG@@mingw-w64-x86_64-pyqt-builder
PKG@@mingw-w64-x86_64-pyqt-builder: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-sip) ; @echo 'PKG@@mingw-w64-x86_64-pyqt-builder'

override VERSION_OF_mingw-w64-x86_64-pyqt5-sip := 12.20.0-1
.PHONY: PKG@@mingw-w64-x86_64-pyqt5-sip
PKG@@mingw-w64-x86_64-pyqt5-sip: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-pyqt5-sip'

override VERSION_OF_mingw-w64-x86_64-pystring := 1.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-pystring
PKG@@mingw-w64-x86_64-pystring: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-pystring'

override VERSION_OF_mingw-w64-x86_64-python := 3.14.10-1
.PHONY: PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python3 PKG@@mingw-w64-x86_64-python3.14
PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python3 PKG@@mingw-w64-x86_64-python3.14: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-libb2 PKG@@mingw-w64-x86_64-libffi PKG@@mingw-w64-x86_64-mpdecimal PKG@@mingw-w64-x86_64-ncurses PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-sqlite3 PKG@@mingw-w64-x86_64-tcl PKG@@mingw-w64-x86_64-tk PKG@@mingw-w64-x86_64-tzdata PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-python'

override VERSION_OF_mingw-w64-x86_64-python-aiofiles := 1~25.1.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-aiofiles
PKG@@mingw-w64-x86_64-python-aiofiles: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-aiofiles'

override VERSION_OF_mingw-w64-x86_64-python-aiosmtpd := 1.4.6-5
.PHONY: PKG@@mingw-w64-x86_64-python-aiosmtpd
PKG@@mingw-w64-x86_64-python-aiosmtpd: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-atpublic PKG@@mingw-w64-x86_64-python-attrs) ; @echo 'PKG@@mingw-w64-x86_64-python-aiosmtpd'

override VERSION_OF_mingw-w64-x86_64-python-aiosqlite := 0.22.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-aiosqlite
PKG@@mingw-w64-x86_64-python-aiosqlite: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-typing_extensions) ; @echo 'PKG@@mingw-w64-x86_64-python-aiosqlite'

override VERSION_OF_mingw-w64-x86_64-python-altgraph := 0.17.5-3
.PHONY: PKG@@mingw-w64-x86_64-python-altgraph
PKG@@mingw-w64-x86_64-python-altgraph: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-setuptools) ; @echo 'PKG@@mingw-w64-x86_64-python-altgraph'

override VERSION_OF_mingw-w64-x86_64-python-annotated-doc := 0.0.5-1
.PHONY: PKG@@mingw-w64-x86_64-python-annotated-doc
PKG@@mingw-w64-x86_64-python-annotated-doc: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-annotated-doc'

override VERSION_OF_mingw-w64-x86_64-python-annotated-types := 0.8.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-annotated-types
PKG@@mingw-w64-x86_64-python-annotated-types: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-annotated-types'

override VERSION_OF_mingw-w64-x86_64-python-anyio := 1~4.16.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-anyio
PKG@@mingw-w64-x86_64-python-anyio: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-idna PKG@@mingw-w64-x86_64-python-typing_extensions) ; @echo 'PKG@@mingw-w64-x86_64-python-anyio'

override VERSION_OF_mingw-w64-x86_64-python-apipkg := 3.0.2-4
.PHONY: PKG@@mingw-w64-x86_64-python-apipkg
PKG@@mingw-w64-x86_64-python-apipkg: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-apipkg'

override VERSION_OF_mingw-w64-x86_64-python-appdirs := 1.4.4-8
.PHONY: PKG@@mingw-w64-x86_64-python-appdirs
PKG@@mingw-w64-x86_64-python-appdirs: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-appdirs'

override VERSION_OF_mingw-w64-x86_64-python-argcomplete := 3.7.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-argcomplete
PKG@@mingw-w64-x86_64-python-argcomplete: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-argcomplete'

override VERSION_OF_mingw-w64-x86_64-python-argon2-cffi-bindings := 26.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-argon2-cffi-bindings
PKG@@mingw-w64-x86_64-python-argon2-cffi-bindings: $(if $(__deps),PKG@@mingw-w64-x86_64-argon2 PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-cffi) ; @echo 'PKG@@mingw-w64-x86_64-python-argon2-cffi-bindings'

override VERSION_OF_mingw-w64-x86_64-python-argon2_cffi := 25.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-argon2_cffi
PKG@@mingw-w64-x86_64-python-argon2_cffi: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-argon2-cffi-bindings) ; @echo 'PKG@@mingw-w64-x86_64-python-argon2_cffi'

override VERSION_OF_mingw-w64-x86_64-python-asgiref := 3.13.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-asgiref
PKG@@mingw-w64-x86_64-python-asgiref: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-asgiref'

override VERSION_OF_mingw-w64-x86_64-python-asn1crypto := 1.5.1-7
.PHONY: PKG@@mingw-w64-x86_64-python-asn1crypto
PKG@@mingw-w64-x86_64-python-asn1crypto: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-pycparser) ; @echo 'PKG@@mingw-w64-x86_64-python-asn1crypto'

override VERSION_OF_mingw-w64-x86_64-python-ast-serialize := 0.13.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-ast-serialize
PKG@@mingw-w64-x86_64-python-ast-serialize: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-ast-serialize'

override VERSION_OF_mingw-w64-x86_64-python-astor := 0.8.1-9
.PHONY: PKG@@mingw-w64-x86_64-python-astor
PKG@@mingw-w64-x86_64-python-astor: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-astor'

override VERSION_OF_mingw-w64-x86_64-python-astroid := 4.3.4-1
.PHONY: PKG@@mingw-w64-x86_64-python-astroid
PKG@@mingw-w64-x86_64-python-astroid: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-astroid'

override VERSION_OF_mingw-w64-x86_64-python-asttokens := 3.0.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-asttokens
PKG@@mingw-w64-x86_64-python-asttokens: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-six) ; @echo 'PKG@@mingw-w64-x86_64-python-asttokens'

override VERSION_OF_mingw-w64-x86_64-python-astunparse := 1.6.3-8
.PHONY: PKG@@mingw-w64-x86_64-python-astunparse
PKG@@mingw-w64-x86_64-python-astunparse: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-six PKG@@mingw-w64-x86_64-python-wheel) ; @echo 'PKG@@mingw-w64-x86_64-python-astunparse'

override VERSION_OF_mingw-w64-x86_64-python-async-timeout := 5.0.1-4
.PHONY: PKG@@mingw-w64-x86_64-python-async-timeout
PKG@@mingw-w64-x86_64-python-async-timeout: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-async-timeout'

override VERSION_OF_mingw-w64-x86_64-python-async_generator := 1.10-8
.PHONY: PKG@@mingw-w64-x86_64-python-async_generator
PKG@@mingw-w64-x86_64-python-async_generator: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-async_generator'

override VERSION_OF_mingw-w64-x86_64-python-atomicwrites := 1.4.1-6
.PHONY: PKG@@mingw-w64-x86_64-python-atomicwrites
PKG@@mingw-w64-x86_64-python-atomicwrites: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-atomicwrites'

override VERSION_OF_mingw-w64-x86_64-python-atpublic := 9.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-atpublic
PKG@@mingw-w64-x86_64-python-atpublic: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-atpublic'

override VERSION_OF_mingw-w64-x86_64-python-attrs := 26.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-attrs
PKG@@mingw-w64-x86_64-python-attrs: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-attrs'

override VERSION_OF_mingw-w64-x86_64-python-audioop-lts := 0.2.2-2
.PHONY: PKG@@mingw-w64-x86_64-python-audioop-lts
PKG@@mingw-w64-x86_64-python-audioop-lts: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-audioop-lts'

override VERSION_OF_mingw-w64-x86_64-python-audioread := 3.1.0-5
.PHONY: PKG@@mingw-w64-x86_64-python-audioread
PKG@@mingw-w64-x86_64-python-audioread: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-standard-aifc PKG@@mingw-w64-x86_64-python-standard-sunau) ; @echo 'PKG@@mingw-w64-x86_64-python-audioread'

override VERSION_OF_mingw-w64-x86_64-python-automat := 25.4.16-3
.PHONY: PKG@@mingw-w64-x86_64-python-automat
PKG@@mingw-w64-x86_64-python-automat: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-automat'

override VERSION_OF_mingw-w64-x86_64-python-autopage := 0.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-autopage
PKG@@mingw-w64-x86_64-python-autopage: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-autopage'

override VERSION_OF_mingw-w64-x86_64-python-autopep8 := 2.3.2-3
.PHONY: PKG@@mingw-w64-x86_64-python-autopep8
PKG@@mingw-w64-x86_64-python-autopep8: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-pycodestyle) ; @echo 'PKG@@mingw-w64-x86_64-python-autopep8'

override VERSION_OF_mingw-w64-x86_64-python-awesomeversion := 25.8.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-awesomeversion
PKG@@mingw-w64-x86_64-python-awesomeversion: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-awesomeversion'

override VERSION_OF_mingw-w64-x86_64-python-babel := 2.18.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-babel
PKG@@mingw-w64-x86_64-python-babel: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-babel'

override VERSION_OF_mingw-w64-x86_64-python-bcrypt := 5.0.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-bcrypt
PKG@@mingw-w64-x86_64-python-bcrypt: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-bcrypt'

override VERSION_OF_mingw-w64-x86_64-python-beautifulsoup4 := 4.15.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-beautifulsoup4
PKG@@mingw-w64-x86_64-python-beautifulsoup4: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-soupsieve PKG@@mingw-w64-x86_64-python-typing_extensions) ; @echo 'PKG@@mingw-w64-x86_64-python-beautifulsoup4'

override VERSION_OF_mingw-w64-x86_64-python-beniget := 0.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-beniget
PKG@@mingw-w64-x86_64-python-beniget: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-gast) ; @echo 'PKG@@mingw-w64-x86_64-python-beniget'

override VERSION_OF_mingw-w64-x86_64-python-blinker := 1.9.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-blinker
PKG@@mingw-w64-x86_64-python-blinker: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-blinker'

override VERSION_OF_mingw-w64-x86_64-python-boolean.py := 5.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-boolean.py
PKG@@mingw-w64-x86_64-python-boolean.py: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-boolean.py'

override VERSION_OF_mingw-w64-x86_64-python-bootstrap-flask := 2.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-bootstrap-flask
PKG@@mingw-w64-x86_64-python-bootstrap-flask: $(if $(__deps),PKG@@mingw-w64-x86_64-python-flask PKG@@mingw-w64-x86_64-python-wtforms) ; @echo 'PKG@@mingw-w64-x86_64-python-bootstrap-flask'

override VERSION_OF_mingw-w64-x86_64-python-bootstrap-flask-docs := 2.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-bootstrap-flask-docs
PKG@@mingw-w64-x86_64-python-bootstrap-flask-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-python-bootstrap-flask-docs'

override VERSION_OF_mingw-w64-x86_64-python-bottle := 0.13.4-3
.PHONY: PKG@@mingw-w64-x86_64-python-bottle
PKG@@mingw-w64-x86_64-python-bottle: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-bottle'

override VERSION_OF_mingw-w64-x86_64-python-branca := 0.8.2-2
.PHONY: PKG@@mingw-w64-x86_64-python-branca
PKG@@mingw-w64-x86_64-python-branca: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-jinja) ; @echo 'PKG@@mingw-w64-x86_64-python-branca'

override VERSION_OF_mingw-w64-x86_64-python-breathe := 5.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-breathe
PKG@@mingw-w64-x86_64-python-breathe: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-sphinx) ; @echo 'PKG@@mingw-w64-x86_64-python-breathe'

override VERSION_OF_mingw-w64-x86_64-python-brotli := 1.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-brotli
PKG@@mingw-w64-x86_64-python-brotli: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-brotli'

override VERSION_OF_mingw-w64-x86_64-python-bsddb3 := 6.2.9-13
.PHONY: PKG@@mingw-w64-x86_64-python-bsddb3
PKG@@mingw-w64-x86_64-python-bsddb3: $(if $(__deps),PKG@@mingw-w64-x86_64-db PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-bsddb3'

override VERSION_OF_mingw-w64-x86_64-python-build := 1.6.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-build
PKG@@mingw-w64-x86_64-python-build: $(if $(__deps),PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-pyproject-hooks) ; @echo 'PKG@@mingw-w64-x86_64-python-build'

override VERSION_OF_mingw-w64-x86_64-python-cabarchive := 0.2.5-1
.PHONY: PKG@@mingw-w64-x86_64-python-cabarchive
PKG@@mingw-w64-x86_64-python-cabarchive: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-cabarchive'

override VERSION_OF_mingw-w64-x86_64-python-cachecontrol := 0.14.4-3
.PHONY: PKG@@mingw-w64-x86_64-python-cachecontrol
PKG@@mingw-w64-x86_64-python-cachecontrol: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-filelock PKG@@mingw-w64-x86_64-python-msgpack PKG@@mingw-w64-x86_64-python-requests) ; @echo 'PKG@@mingw-w64-x86_64-python-cachecontrol'

override VERSION_OF_mingw-w64-x86_64-python-cached-property := 2.0.1-5
.PHONY: PKG@@mingw-w64-x86_64-python-cached-property
PKG@@mingw-w64-x86_64-python-cached-property: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-cached-property'

override VERSION_OF_mingw-w64-x86_64-python-cachetools := 8.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-cachetools
PKG@@mingw-w64-x86_64-python-cachetools: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-cachetools'

override VERSION_OF_mingw-w64-x86_64-python-cachy := 0.3.0-5
.PHONY: PKG@@mingw-w64-x86_64-python-cachy
PKG@@mingw-w64-x86_64-python-cachy: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-cachy'

override VERSION_OF_mingw-w64-x86_64-python-cairo := 1.29.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-cairo
PKG@@mingw-w64-x86_64-python-cairo: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-cairo'

override VERSION_OF_mingw-w64-x86_64-python-calver := 2025.10.20-3
.PHONY: PKG@@mingw-w64-x86_64-python-calver
PKG@@mingw-w64-x86_64-python-calver: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-calver'

override VERSION_OF_mingw-w64-x86_64-python-capstone := 5.0.9-1
.PHONY: PKG@@mingw-w64-x86_64-python-capstone
PKG@@mingw-w64-x86_64-python-capstone: $(if $(__deps),PKG@@mingw-w64-x86_64-capstone PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-capstone'

override VERSION_OF_mingw-w64-x86_64-python-cattrs := 26.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-cattrs
PKG@@mingw-w64-x86_64-python-cattrs: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-attrs PKG@@mingw-w64-x86_64-python-typing_extensions) ; @echo 'PKG@@mingw-w64-x86_64-python-cattrs'

override VERSION_OF_mingw-w64-x86_64-python-certifi := 2026.10.23-1
.PHONY: PKG@@mingw-w64-x86_64-python-certifi
PKG@@mingw-w64-x86_64-python-certifi: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-certifi'

override VERSION_OF_mingw-w64-x86_64-python-cffi := 2.1.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-cffi
PKG@@mingw-w64-x86_64-python-cffi: $(if $(__deps),PKG@@mingw-w64-x86_64-libffi PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-pycparser) ; @echo 'PKG@@mingw-w64-x86_64-python-cffi'

override VERSION_OF_mingw-w64-x86_64-python-charset-normalizer := 3.5.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-charset-normalizer
PKG@@mingw-w64-x86_64-python-charset-normalizer: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-charset-normalizer'

override VERSION_OF_mingw-w64-x86_64-python-cleo := 2.2.1-4
.PHONY: PKG@@mingw-w64-x86_64-python-cleo
PKG@@mingw-w64-x86_64-python-cleo: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-cleo'

override VERSION_OF_mingw-w64-x86_64-python-click := 8.5.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-click
PKG@@mingw-w64-x86_64-python-click: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-colorama) ; @echo 'PKG@@mingw-w64-x86_64-python-click'

override VERSION_OF_mingw-w64-x86_64-python-click-docs := 8.5.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-click-docs
PKG@@mingw-w64-x86_64-python-click-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-python-click-docs'

override VERSION_OF_mingw-w64-x86_64-python-cliff := 4.17.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-cliff
PKG@@mingw-w64-x86_64-python-cliff: $(if $(__deps),PKG@@mingw-w64-x86_64-python-autopage PKG@@mingw-w64-x86_64-python-cmd2 PKG@@mingw-w64-x86_64-python-prettytable PKG@@mingw-w64-x86_64-python-stevedore PKG@@mingw-w64-x86_64-python-yaml) ; @echo 'PKG@@mingw-w64-x86_64-python-cliff'

override VERSION_OF_mingw-w64-x86_64-python-cloudpickle := 3.1.2-3
.PHONY: PKG@@mingw-w64-x86_64-python-cloudpickle
PKG@@mingw-w64-x86_64-python-cloudpickle: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-cloudpickle'

override VERSION_OF_mingw-w64-x86_64-python-cmd2 := 4.4.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-cmd2
PKG@@mingw-w64-x86_64-python-cmd2: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-prompt_toolkit PKG@@mingw-w64-x86_64-python-pyperclip PKG@@mingw-w64-x86_64-python-rich PKG@@mingw-w64-x86_64-python-rich-argparse) ; @echo 'PKG@@mingw-w64-x86_64-python-cmd2'

override VERSION_OF_mingw-w64-x86_64-python-codespell := 2.4.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-codespell
PKG@@mingw-w64-x86_64-python-codespell: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-codespell'

override VERSION_OF_mingw-w64-x86_64-python-colorama := 0.4.6-5
.PHONY: PKG@@mingw-w64-x86_64-python-colorama
PKG@@mingw-w64-x86_64-python-colorama: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-colorama'

override VERSION_OF_mingw-w64-x86_64-python-colored-traceback := 0.4.2-3
.PHONY: PKG@@mingw-w64-x86_64-python-colored-traceback
PKG@@mingw-w64-x86_64-python-colored-traceback: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-colorama PKG@@mingw-w64-x86_64-python-pygments) ; @echo 'PKG@@mingw-w64-x86_64-python-colored-traceback'

override VERSION_OF_mingw-w64-x86_64-python-colorlog := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-colorlog
PKG@@mingw-w64-x86_64-python-colorlog: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-colorama) ; @echo 'PKG@@mingw-w64-x86_64-python-colorlog'

override VERSION_OF_mingw-w64-x86_64-python-commonmark := 0.9.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-commonmark
PKG@@mingw-w64-x86_64-python-commonmark: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-commonmark'

override VERSION_OF_mingw-w64-x86_64-python-comtypes := 1.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-comtypes
PKG@@mingw-w64-x86_64-python-comtypes: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-comtypes'

override VERSION_OF_mingw-w64-x86_64-python-constantly := 23.10.4-4
.PHONY: PKG@@mingw-w64-x86_64-python-constantly
PKG@@mingw-w64-x86_64-python-constantly: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-constantly'

override VERSION_OF_mingw-w64-x86_64-python-contextlib2 := 21.6.0-7
.PHONY: PKG@@mingw-w64-x86_64-python-contextlib2
PKG@@mingw-w64-x86_64-python-contextlib2: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-contextlib2'

override VERSION_OF_mingw-w64-x86_64-python-contourpy := 1.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-contourpy
PKG@@mingw-w64-x86_64-python-contourpy: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-numpy) ; @echo 'PKG@@mingw-w64-x86_64-python-contourpy'

override VERSION_OF_mingw-w64-x86_64-python-coverage := 7.17.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-coverage
PKG@@mingw-w64-x86_64-python-coverage: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-coverage'

override VERSION_OF_mingw-w64-x86_64-python-cppy := 1.3.1-4
.PHONY: PKG@@mingw-w64-x86_64-python-cppy
PKG@@mingw-w64-x86_64-python-cppy: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-cppy'

override VERSION_OF_mingw-w64-x86_64-python-cryptography := 50.0.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-cryptography
PKG@@mingw-w64-x86_64-python-cryptography: $(if $(__deps),PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-cffi) ; @echo 'PKG@@mingw-w64-x86_64-python-cryptography'

override VERSION_OF_mingw-w64-x86_64-python-cssselect := 1.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-cssselect
PKG@@mingw-w64-x86_64-python-cssselect: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-cssselect'

override VERSION_OF_mingw-w64-x86_64-python-csv23 := 0.3.4-6
.PHONY: PKG@@mingw-w64-x86_64-python-csv23
PKG@@mingw-w64-x86_64-python-csv23: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-csv23'

override VERSION_OF_mingw-w64-x86_64-python-cvxopt := 1.3.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-cvxopt
PKG@@mingw-w64-x86_64-python-cvxopt: $(if $(__deps),PKG@@mingw-w64-x86_64-openblas PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-suitesparse) ; @echo 'PKG@@mingw-w64-x86_64-python-cvxopt'

override VERSION_OF_mingw-w64-x86_64-python-cx-logging := 3.2.1-5
.PHONY: PKG@@mingw-w64-x86_64-python-cx-logging
PKG@@mingw-w64-x86_64-python-cx-logging: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-cx-logging'

override VERSION_OF_mingw-w64-x86_64-python-cycler := 0.12.1-4
.PHONY: PKG@@mingw-w64-x86_64-python-cycler
PKG@@mingw-w64-x86_64-python-cycler: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-cycler'

override VERSION_OF_mingw-w64-x86_64-python-darkdetect := 0.8.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-darkdetect
PKG@@mingw-w64-x86_64-python-darkdetect: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-darkdetect'

override VERSION_OF_mingw-w64-x86_64-python-dateutil := 4.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-dateutil
PKG@@mingw-w64-x86_64-python-dateutil: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-tzdata) ; @echo 'PKG@@mingw-w64-x86_64-python-dateutil'

override VERSION_OF_mingw-w64-x86_64-python-decouple := 3.8-3
.PHONY: PKG@@mingw-w64-x86_64-python-decouple
PKG@@mingw-w64-x86_64-python-decouple: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-decouple'

override VERSION_OF_mingw-w64-x86_64-python-defusedxml := 0.7.1-8
.PHONY: PKG@@mingw-w64-x86_64-python-defusedxml
PKG@@mingw-w64-x86_64-python-defusedxml: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-defusedxml'

override VERSION_OF_mingw-w64-x86_64-python-deprecation := 2.1.0-6
.PHONY: PKG@@mingw-w64-x86_64-python-deprecation
PKG@@mingw-w64-x86_64-python-deprecation: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging) ; @echo 'PKG@@mingw-w64-x86_64-python-deprecation'

override VERSION_OF_mingw-w64-x86_64-python-diff-match-patch := 20241021-3
.PHONY: PKG@@mingw-w64-x86_64-python-diff-match-patch
PKG@@mingw-w64-x86_64-python-diff-match-patch: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-diff-match-patch'

override VERSION_OF_mingw-w64-x86_64-python-dill := 0.4.1-2
.PHONY: PKG@@mingw-w64-x86_64-python-dill
PKG@@mingw-w64-x86_64-python-dill: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-dill'

override VERSION_OF_mingw-w64-x86_64-python-distlib := 0.4.4-1
.PHONY: PKG@@mingw-w64-x86_64-python-distlib
PKG@@mingw-w64-x86_64-python-distlib: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-distlib'

override VERSION_OF_mingw-w64-x86_64-python-distro := 1.9.0-5
.PHONY: PKG@@mingw-w64-x86_64-python-distro
PKG@@mingw-w64-x86_64-python-distro: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-distro'

override VERSION_OF_mingw-w64-x86_64-python-django := 6.1.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-django
PKG@@mingw-w64-x86_64-python-django: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-asgiref PKG@@mingw-w64-x86_64-python-sqlparse PKG@@mingw-w64-x86_64-python-tzdata) ; @echo 'PKG@@mingw-w64-x86_64-python-django'

override VERSION_OF_mingw-w64-x86_64-python-dnspython := 2.9.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-dnspython
PKG@@mingw-w64-x86_64-python-dnspython: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-dnspython'

override VERSION_OF_mingw-w64-x86_64-python-docopt := 0.6.2-3
.PHONY: PKG@@mingw-w64-x86_64-python-docopt
PKG@@mingw-w64-x86_64-python-docopt: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-docopt'

override VERSION_OF_mingw-w64-x86_64-python-docutils := 0.22.4-1
.PHONY: PKG@@mingw-w64-x86_64-python-docutils
PKG@@mingw-w64-x86_64-python-docutils: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-docutils'

override VERSION_OF_mingw-w64-x86_64-python-dulwich := 1.2.21-1
.PHONY: PKG@@mingw-w64-x86_64-python-dulwich
PKG@@mingw-w64-x86_64-python-dulwich: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-urllib3) ; @echo 'PKG@@mingw-w64-x86_64-python-dulwich'

override VERSION_OF_mingw-w64-x86_64-python-e3-core := 22.10.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-e3-core
PKG@@mingw-w64-x86_64-python-e3-core: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-colorama PKG@@mingw-w64-x86_64-python-dateutil PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-psutil PKG@@mingw-w64-x86_64-python-requests PKG@@mingw-w64-x86_64-python-requests-cache PKG@@mingw-w64-x86_64-python-requests-toolbelt PKG@@mingw-w64-x86_64-python-resolvelib PKG@@mingw-w64-x86_64-python-setuptools PKG@@mingw-w64-x86_64-python-stevedore PKG@@mingw-w64-x86_64-python-tqdm PKG@@mingw-w64-x86_64-python-yaml) ; @echo 'PKG@@mingw-w64-x86_64-python-e3-core'

override VERSION_OF_mingw-w64-x86_64-python-ecdsa := 0.19.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-ecdsa
PKG@@mingw-w64-x86_64-python-ecdsa: $(if $(__deps),PKG@@mingw-w64-x86_64-python-six) ; @echo 'PKG@@mingw-w64-x86_64-python-ecdsa'

override VERSION_OF_mingw-w64-x86_64-python-editables := 0.6-1
.PHONY: PKG@@mingw-w64-x86_64-python-editables
PKG@@mingw-w64-x86_64-python-editables: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-editables'

override VERSION_OF_mingw-w64-x86_64-python-elementpath := 5.1.4-1
.PHONY: PKG@@mingw-w64-x86_64-python-elementpath
PKG@@mingw-w64-x86_64-python-elementpath: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-elementpath'

override VERSION_OF_mingw-w64-x86_64-python-email-validator := 2.3.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-email-validator
PKG@@mingw-w64-x86_64-python-email-validator: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-dnspython PKG@@mingw-w64-x86_64-python-idna) ; @echo 'PKG@@mingw-w64-x86_64-python-email-validator'

override VERSION_OF_mingw-w64-x86_64-python-emoji := 2.16.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-emoji
PKG@@mingw-w64-x86_64-python-emoji: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-emoji'

override VERSION_OF_mingw-w64-x86_64-python-et-xmlfile := 2.0.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-et-xmlfile
PKG@@mingw-w64-x86_64-python-et-xmlfile: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-lxml) ; @echo 'PKG@@mingw-w64-x86_64-python-et-xmlfile'

override VERSION_OF_mingw-w64-x86_64-python-execnet := 2.1.2-3
.PHONY: PKG@@mingw-w64-x86_64-python-execnet
PKG@@mingw-w64-x86_64-python-execnet: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-execnet'

override VERSION_OF_mingw-w64-x86_64-python-faker := 40.52.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-faker
PKG@@mingw-w64-x86_64-python-faker: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-tzdata) ; @echo 'PKG@@mingw-w64-x86_64-python-faker'

override VERSION_OF_mingw-w64-x86_64-python-fasteners := 0.20-4
.PHONY: PKG@@mingw-w64-x86_64-python-fasteners
PKG@@mingw-w64-x86_64-python-fasteners: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-fasteners'

override VERSION_OF_mingw-w64-x86_64-python-fastexcel := 0.21.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-fastexcel
PKG@@mingw-w64-x86_64-python-fastexcel: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-fastexcel'

override VERSION_OF_mingw-w64-x86_64-python-fastjsonschema := 2.22.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-fastjsonschema
PKG@@mingw-w64-x86_64-python-fastjsonschema: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-fastjsonschema'

override VERSION_OF_mingw-w64-x86_64-python-feedgenerator := 2.2.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-feedgenerator
PKG@@mingw-w64-x86_64-python-feedgenerator: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-tzdata) ; @echo 'PKG@@mingw-w64-x86_64-python-feedgenerator'

override VERSION_OF_mingw-w64-x86_64-python-filelock := 4.2.10-1
.PHONY: PKG@@mingw-w64-x86_64-python-filelock
PKG@@mingw-w64-x86_64-python-filelock: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-filelock'

override VERSION_OF_mingw-w64-x86_64-python-findpython := 0.8.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-findpython
PKG@@mingw-w64-x86_64-python-findpython: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-platformdirs) ; @echo 'PKG@@mingw-w64-x86_64-python-findpython'

override VERSION_OF_mingw-w64-x86_64-python-fixtures := 4.3.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-fixtures
PKG@@mingw-w64-x86_64-python-fixtures: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-fixtures'

override VERSION_OF_mingw-w64-x86_64-python-flake8 := 7.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-flake8
PKG@@mingw-w64-x86_64-python-flake8: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-mccabe PKG@@mingw-w64-x86_64-python-pycodestyle PKG@@mingw-w64-x86_64-python-pyflakes) ; @echo 'PKG@@mingw-w64-x86_64-python-flake8'

override VERSION_OF_mingw-w64-x86_64-python-flaky := 3.8.1-4
.PHONY: PKG@@mingw-w64-x86_64-python-flaky
PKG@@mingw-w64-x86_64-python-flaky: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-flaky'

override VERSION_OF_mingw-w64-x86_64-python-flask := 3.1.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-flask
PKG@@mingw-w64-x86_64-python-flask: $(if $(__deps),PKG@@mingw-w64-x86_64-python-blinker PKG@@mingw-w64-x86_64-python-click PKG@@mingw-w64-x86_64-python-itsdangerous PKG@@mingw-w64-x86_64-python-jinja PKG@@mingw-w64-x86_64-python-werkzeug) ; @echo 'PKG@@mingw-w64-x86_64-python-flask'

override VERSION_OF_mingw-w64-x86_64-python-flask-babel := 4.0.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-flask-babel
PKG@@mingw-w64-x86_64-python-flask-babel: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-babel PKG@@mingw-w64-x86_64-python-flask PKG@@mingw-w64-x86_64-python-jinja PKG@@mingw-w64-x86_64-python-pytz) ; @echo 'PKG@@mingw-w64-x86_64-python-flask-babel'

override VERSION_OF_mingw-w64-x86_64-python-flask-babel-docs := 4.0.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-flask-babel-docs
PKG@@mingw-w64-x86_64-python-flask-babel-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-python-flask-babel-docs'

override VERSION_OF_mingw-w64-x86_64-python-flatbuffers := 25.12.19-1
.PHONY: PKG@@mingw-w64-x86_64-python-flatbuffers
PKG@@mingw-w64-x86_64-python-flatbuffers: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-flatbuffers'

override VERSION_OF_mingw-w64-x86_64-python-flexmock := 0.13.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-flexmock
PKG@@mingw-w64-x86_64-python-flexmock: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-flexmock'

override VERSION_OF_mingw-w64-x86_64-python-flit := 4.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-flit
PKG@@mingw-w64-x86_64-python-flit: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-docutils PKG@@mingw-w64-x86_64-python-flit-core PKG@@mingw-w64-x86_64-python-requests PKG@@mingw-w64-x86_64-python-tomli-w) ; @echo 'PKG@@mingw-w64-x86_64-python-flit'

override VERSION_OF_mingw-w64-x86_64-python-flit-core := 4.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-flit-core
PKG@@mingw-w64-x86_64-python-flit-core: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-flit-core'

override VERSION_OF_mingw-w64-x86_64-python-fonttools := 4.68.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-fonttools
PKG@@mingw-w64-x86_64-python-fonttools: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-fonttools'

override VERSION_OF_mingw-w64-x86_64-python-ford := 7.0.13-1
.PHONY: PKG@@mingw-w64-x86_64-python-ford
PKG@@mingw-w64-x86_64-python-ford: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-beautifulsoup4 PKG@@mingw-w64-x86_64-python-graphviz PKG@@mingw-w64-x86_64-python-jinja PKG@@mingw-w64-x86_64-python-markdown PKG@@mingw-w64-x86_64-python-markdown-include PKG@@mingw-w64-x86_64-python-markdown-math PKG@@mingw-w64-x86_64-python-pcpp PKG@@mingw-w64-x86_64-python-pygments PKG@@mingw-w64-x86_64-python-rich PKG@@mingw-w64-x86_64-python-toposort PKG@@mingw-w64-x86_64-python-tqdm) ; @echo 'PKG@@mingw-w64-x86_64-python-ford'

override VERSION_OF_mingw-w64-x86_64-python-freetype-py := 2.5.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-freetype-py
PKG@@mingw-w64-x86_64-python-freetype-py: $(if $(__deps),PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-freetype-py'

override VERSION_OF_mingw-w64-x86_64-python-freezegun := 1.5.5-4
.PHONY: PKG@@mingw-w64-x86_64-python-freezegun
PKG@@mingw-w64-x86_64-python-freezegun: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-dateutil) ; @echo 'PKG@@mingw-w64-x86_64-python-freezegun'

override VERSION_OF_mingw-w64-x86_64-python-ftfy := 6.3.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-ftfy
PKG@@mingw-w64-x86_64-python-ftfy: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-wcwidth) ; @echo 'PKG@@mingw-w64-x86_64-python-ftfy'

override VERSION_OF_mingw-w64-x86_64-python-func-timeout := 4.3.5-7
.PHONY: PKG@@mingw-w64-x86_64-python-func-timeout
PKG@@mingw-w64-x86_64-python-func-timeout: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-func-timeout'

override VERSION_OF_mingw-w64-x86_64-python-future := 1.0.0-5
.PHONY: PKG@@mingw-w64-x86_64-python-future
PKG@@mingw-w64-x86_64-python-future: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-future'

override VERSION_OF_mingw-w64-x86_64-python-gast := 0.7.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-gast
PKG@@mingw-w64-x86_64-python-gast: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-gast'

override VERSION_OF_mingw-w64-x86_64-python-gdal := 3.13.3-5
.PHONY: PKG@@mingw-w64-x86_64-python-gdal
PKG@@mingw-w64-x86_64-python-gdal: $(if $(__deps),PKG@@mingw-w64-x86_64-gdal PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-numpy PKG@@mingw-w64-x86_64-python-psycopg2) ; @echo 'PKG@@mingw-w64-x86_64-python-gdal'

override VERSION_OF_mingw-w64-x86_64-python-geographiclib := 2.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-geographiclib
PKG@@mingw-w64-x86_64-python-geographiclib: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-geographiclib'

override VERSION_OF_mingw-w64-x86_64-python-geographiclib-docs := 2.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-geographiclib-docs
PKG@@mingw-w64-x86_64-python-geographiclib-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-python-geographiclib-docs'

override VERSION_OF_mingw-w64-x86_64-python-glad := 2.0.8-3
.PHONY: PKG@@mingw-w64-x86_64-python-glad
PKG@@mingw-w64-x86_64-python-glad: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-jinja) ; @echo 'PKG@@mingw-w64-x86_64-python-glad'

override VERSION_OF_mingw-w64-x86_64-python-gmpy2 := 2.3.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-gmpy2
PKG@@mingw-w64-x86_64-python-gmpy2: $(if $(__deps),PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-mpc PKG@@mingw-w64-x86_64-mpfr PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-gmpy2'

override VERSION_OF_mingw-w64-x86_64-python-gobject := 3.58.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-gobject
PKG@@mingw-w64-x86_64-python-gobject: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gobject-introspection-runtime PKG@@mingw-w64-x86_64-libffi PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-cairo) ; @echo 'PKG@@mingw-w64-x86_64-python-gobject'

override VERSION_OF_mingw-w64-x86_64-python-graphviz := 0.21-3
.PHONY: PKG@@mingw-w64-x86_64-python-graphviz
PKG@@mingw-w64-x86_64-python-graphviz: $(if $(__deps),PKG@@mingw-w64-x86_64-graphviz PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-graphviz'

override VERSION_OF_mingw-w64-x86_64-python-grpcio := 1.85.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-grpcio
PKG@@mingw-w64-x86_64-python-grpcio: $(if $(__deps),PKG@@mingw-w64-x86_64-abseil-cpp PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-typing_extensions PKG@@mingw-w64-x86_64-re2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-python-grpcio'

override VERSION_OF_mingw-w64-x86_64-python-grpcio-tools := 1.85.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-grpcio-tools
PKG@@mingw-w64-x86_64-python-grpcio-tools: $(if $(__deps),PKG@@mingw-w64-x86_64-abseil-cpp PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-grpcio PKG@@mingw-w64-x86_64-python-protobuf PKG@@mingw-w64-x86_64-python-setuptools PKG@@mingw-w64-x86_64-re2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-python-grpcio-tools'

override VERSION_OF_mingw-w64-x86_64-python-gym := 0.26.2-6
.PHONY: PKG@@mingw-w64-x86_64-python-gym
PKG@@mingw-w64-x86_64-python-gym: $(if $(__deps),PKG@@mingw-w64-x86_64-python-cloudpickle PKG@@mingw-w64-x86_64-python-gym-notices PKG@@mingw-w64-x86_64-python-numpy) ; @echo 'PKG@@mingw-w64-x86_64-python-gym'

override VERSION_OF_mingw-w64-x86_64-python-gym-notices := 0.1.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-gym-notices
PKG@@mingw-w64-x86_64-python-gym-notices: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-gym-notices'

override VERSION_OF_mingw-w64-x86_64-python-h11 := 0.16.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-h11
PKG@@mingw-w64-x86_64-python-h11: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-h11'

override VERSION_OF_mingw-w64-x86_64-python-h2 := 4.4.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-h2
PKG@@mingw-w64-x86_64-python-h2: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-hpack PKG@@mingw-w64-x86_64-python-hyperframe) ; @echo 'PKG@@mingw-w64-x86_64-python-h2'

override VERSION_OF_mingw-w64-x86_64-python-h5py := 3.16.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-h5py
PKG@@mingw-w64-x86_64-python-h5py: $(if $(__deps),PKG@@mingw-w64-x86_64-hdf5 PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-numpy) ; @echo 'PKG@@mingw-w64-x86_64-python-h5py'

override VERSION_OF_mingw-w64-x86_64-python-halo := 0.0.31-7
.PHONY: PKG@@mingw-w64-x86_64-python-halo
PKG@@mingw-w64-x86_64-python-halo: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-log_symbols PKG@@mingw-w64-x86_64-python-six PKG@@mingw-w64-x86_64-python-spinners PKG@@mingw-w64-x86_64-python-termcolor) ; @echo 'PKG@@mingw-w64-x86_64-python-halo'

override VERSION_OF_mingw-w64-x86_64-python-hatch-fancy-pypi-readme := 25.1.0-5
.PHONY: PKG@@mingw-w64-x86_64-python-hatch-fancy-pypi-readme
PKG@@mingw-w64-x86_64-python-hatch-fancy-pypi-readme: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-hatchling) ; @echo 'PKG@@mingw-w64-x86_64-python-hatch-fancy-pypi-readme'

override VERSION_OF_mingw-w64-x86_64-python-hatch-vcs := 0.5.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-hatch-vcs
PKG@@mingw-w64-x86_64-python-hatch-vcs: $(if $(__deps),PKG@@mingw-w64-x86_64-python-hatchling PKG@@mingw-w64-x86_64-python-setuptools-scm) ; @echo 'PKG@@mingw-w64-x86_64-python-hatch-vcs'

override VERSION_OF_mingw-w64-x86_64-python-hatchling := 1.33.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-hatchling
PKG@@mingw-w64-x86_64-python-hatchling: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-pathspec PKG@@mingw-w64-x86_64-python-pluggy PKG@@mingw-w64-x86_64-python-tomlkit PKG@@mingw-w64-x86_64-python-trove-classifiers) ; @echo 'PKG@@mingw-w64-x86_64-python-hatchling'

override VERSION_OF_mingw-w64-x86_64-python-hpack := 4.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-hpack
PKG@@mingw-w64-x86_64-python-hpack: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-hpack'

override VERSION_OF_mingw-w64-x86_64-python-html5lib := 1.1-9
.PHONY: PKG@@mingw-w64-x86_64-python-html5lib
PKG@@mingw-w64-x86_64-python-html5lib: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-six PKG@@mingw-w64-x86_64-python-webencodings) ; @echo 'PKG@@mingw-w64-x86_64-python-html5lib'

override VERSION_OF_mingw-w64-x86_64-python-httpcore := 1.0.9-3
.PHONY: PKG@@mingw-w64-x86_64-python-httpcore
PKG@@mingw-w64-x86_64-python-httpcore: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-certifi PKG@@mingw-w64-x86_64-python-h11) ; @echo 'PKG@@mingw-w64-x86_64-python-httpcore'

override VERSION_OF_mingw-w64-x86_64-python-httplib2 := 0.32.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-httplib2
PKG@@mingw-w64-x86_64-python-httplib2: $(if $(__deps),PKG@@mingw-w64-x86_64-ca-certificates PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-certifi PKG@@mingw-w64-x86_64-python-pyparsing) ; @echo 'PKG@@mingw-w64-x86_64-python-httplib2'

override VERSION_OF_mingw-w64-x86_64-python-httpretty := 1.1.4-7
.PHONY: PKG@@mingw-w64-x86_64-python-httpretty
PKG@@mingw-w64-x86_64-python-httpretty: $(if $(__deps),PKG@@mingw-w64-x86_64-python-urllib3) ; @echo 'PKG@@mingw-w64-x86_64-python-httpretty'

override VERSION_OF_mingw-w64-x86_64-python-httpx := 0.28.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-httpx
PKG@@mingw-w64-x86_64-python-httpx: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-anyio PKG@@mingw-w64-x86_64-python-certifi PKG@@mingw-w64-x86_64-python-httpcore PKG@@mingw-w64-x86_64-python-idna PKG@@mingw-w64-x86_64-python-sniffio) ; @echo 'PKG@@mingw-w64-x86_64-python-httpx'

override VERSION_OF_mingw-w64-x86_64-python-hyperframe := 6.1.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-hyperframe
PKG@@mingw-w64-x86_64-python-hyperframe: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-hyperframe'

override VERSION_OF_mingw-w64-x86_64-python-hyperlink := 21.0.0-7
.PHONY: PKG@@mingw-w64-x86_64-python-hyperlink
PKG@@mingw-w64-x86_64-python-hyperlink: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-idna) ; @echo 'PKG@@mingw-w64-x86_64-python-hyperlink'

override VERSION_OF_mingw-w64-x86_64-python-hypothesis := 6.170.15-1
.PHONY: PKG@@mingw-w64-x86_64-python-hypothesis
PKG@@mingw-w64-x86_64-python-hypothesis: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-attrs PKG@@mingw-w64-x86_64-python-sortedcontainers) ; @echo 'PKG@@mingw-w64-x86_64-python-hypothesis'

override VERSION_OF_mingw-w64-x86_64-python-icu := 2.17-1
.PHONY: PKG@@mingw-w64-x86_64-python-icu PKG@@mingw-w64-x86_64-python-pyicu
PKG@@mingw-w64-x86_64-python-icu PKG@@mingw-w64-x86_64-python-pyicu: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-icu'

override VERSION_OF_mingw-w64-x86_64-python-id := 1.6.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-id
PKG@@mingw-w64-x86_64-python-id: $(if $(__deps),PKG@@mingw-w64-x86_64-python-urllib3) ; @echo 'PKG@@mingw-w64-x86_64-python-id'

override VERSION_OF_mingw-w64-x86_64-python-identify := 2.6.20-1
.PHONY: PKG@@mingw-w64-x86_64-python-identify
PKG@@mingw-w64-x86_64-python-identify: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-identify'

override VERSION_OF_mingw-w64-x86_64-python-idna := 3.20-1
.PHONY: PKG@@mingw-w64-x86_64-python-idna
PKG@@mingw-w64-x86_64-python-idna: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-idna'

override VERSION_OF_mingw-w64-x86_64-python-ifstools := 3.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-ifstools
PKG@@mingw-w64-x86_64-python-ifstools: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-kbinxml PKG@@mingw-w64-x86_64-python-lxml PKG@@mingw-w64-x86_64-python-pillow PKG@@mingw-w64-x86_64-python-tqdm) ; @echo 'PKG@@mingw-w64-x86_64-python-ifstools'

override VERSION_OF_mingw-w64-x86_64-python-imagesize := 2.0.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-imagesize
PKG@@mingw-w64-x86_64-python-imagesize: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-imagesize'

override VERSION_OF_mingw-w64-x86_64-python-immutables := 0.21-4
.PHONY: PKG@@mingw-w64-x86_64-python-immutables
PKG@@mingw-w64-x86_64-python-immutables: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-immutables'

override VERSION_OF_mingw-w64-x86_64-python-importlib-metadata := 10.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-importlib-metadata
PKG@@mingw-w64-x86_64-python-importlib-metadata: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-lazy-importer PKG@@mingw-w64-x86_64-python-zipp) ; @echo 'PKG@@mingw-w64-x86_64-python-importlib-metadata'

override VERSION_OF_mingw-w64-x86_64-python-importlib_resources := 6.5.2-3
.PHONY: PKG@@mingw-w64-x86_64-python-importlib_resources
PKG@@mingw-w64-x86_64-python-importlib_resources: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-importlib_resources'

override VERSION_OF_mingw-w64-x86_64-python-incremental := 24.11.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-incremental
PKG@@mingw-w64-x86_64-python-incremental: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging) ; @echo 'PKG@@mingw-w64-x86_64-python-incremental'

override VERSION_OF_mingw-w64-x86_64-python-inflection := 0.5.1-7
.PHONY: PKG@@mingw-w64-x86_64-python-inflection
PKG@@mingw-w64-x86_64-python-inflection: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-inflection'

override VERSION_OF_mingw-w64-x86_64-python-iniconfig := 2.3.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-iniconfig
PKG@@mingw-w64-x86_64-python-iniconfig: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-iniconfig'

override VERSION_OF_mingw-w64-x86_64-python-installer := 1.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-installer
PKG@@mingw-w64-x86_64-python-installer: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-installer'

override VERSION_OF_mingw-w64-x86_64-python-intervaltree := 3.2.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-intervaltree
PKG@@mingw-w64-x86_64-python-intervaltree: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-sortedcontainers) ; @echo 'PKG@@mingw-w64-x86_64-python-intervaltree'

override VERSION_OF_mingw-w64-x86_64-python-iso8601 := 2.1.0-5
.PHONY: PKG@@mingw-w64-x86_64-python-iso8601
PKG@@mingw-w64-x86_64-python-iso8601: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-iso8601'

override VERSION_OF_mingw-w64-x86_64-python-isodate := 0.7.2-3
.PHONY: PKG@@mingw-w64-x86_64-python-isodate
PKG@@mingw-w64-x86_64-python-isodate: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-isodate'

override VERSION_OF_mingw-w64-x86_64-python-isosurfaces := 0.1.2-5
.PHONY: PKG@@mingw-w64-x86_64-python-isosurfaces
PKG@@mingw-w64-x86_64-python-isosurfaces: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-numpy) ; @echo 'PKG@@mingw-w64-x86_64-python-isosurfaces'

override VERSION_OF_mingw-w64-x86_64-python-itsdangerous := 2.2.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-itsdangerous
PKG@@mingw-w64-x86_64-python-itsdangerous: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-itsdangerous'

override VERSION_OF_mingw-w64-x86_64-python-jaraco.classes := 3.4.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-jaraco.classes
PKG@@mingw-w64-x86_64-python-jaraco.classes: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-more-itertools) ; @echo 'PKG@@mingw-w64-x86_64-python-jaraco.classes'

override VERSION_OF_mingw-w64-x86_64-python-jaraco.context := 6.1.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-jaraco.context
PKG@@mingw-w64-x86_64-python-jaraco.context: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-jaraco.context'

override VERSION_OF_mingw-w64-x86_64-python-jaraco.envs := 2.6.0-5
.PHONY: PKG@@mingw-w64-x86_64-python-jaraco.envs
PKG@@mingw-w64-x86_64-python-jaraco.envs: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-path PKG@@mingw-w64-x86_64-python-tox PKG@@mingw-w64-x86_64-python-virtualenv) ; @echo 'PKG@@mingw-w64-x86_64-python-jaraco.envs'

override VERSION_OF_mingw-w64-x86_64-python-jaraco.functools := 4.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-jaraco.functools
PKG@@mingw-w64-x86_64-python-jaraco.functools: $(if $(__deps),PKG@@mingw-w64-x86_64-python-more-itertools) ; @echo 'PKG@@mingw-w64-x86_64-python-jaraco.functools'

override VERSION_OF_mingw-w64-x86_64-python-jaraco.path := 3.7.2-3
.PHONY: PKG@@mingw-w64-x86_64-python-jaraco.path
PKG@@mingw-w64-x86_64-python-jaraco.path: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-jaraco.path'

override VERSION_OF_mingw-w64-x86_64-python-jinja := 3.1.6-4
.PHONY: PKG@@mingw-w64-x86_64-python-jinja
PKG@@mingw-w64-x86_64-python-jinja: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-markupsafe) ; @echo 'PKG@@mingw-w64-x86_64-python-jinja'

override VERSION_OF_mingw-w64-x86_64-python-jinja-docs := 3.1.6-1
.PHONY: PKG@@mingw-w64-x86_64-python-jinja-docs
PKG@@mingw-w64-x86_64-python-jinja-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-python-jinja-docs'

override VERSION_OF_mingw-w64-x86_64-python-json-logger := 4.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-json-logger
PKG@@mingw-w64-x86_64-python-json-logger: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-json-logger'

override VERSION_OF_mingw-w64-x86_64-python-json-rpc := 1.15.0-6
.PHONY: PKG@@mingw-w64-x86_64-python-json-rpc
PKG@@mingw-w64-x86_64-python-json-rpc: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-json-rpc'

override VERSION_OF_mingw-w64-x86_64-python-kbinxml := 2.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-kbinxml
PKG@@mingw-w64-x86_64-python-kbinxml: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-lxml) ; @echo 'PKG@@mingw-w64-x86_64-python-kbinxml'

override VERSION_OF_mingw-w64-x86_64-python-keyring := 25.7.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-keyring
PKG@@mingw-w64-x86_64-python-keyring: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-jaraco.classes PKG@@mingw-w64-x86_64-python-jaraco.context PKG@@mingw-w64-x86_64-python-jaraco.functools PKG@@mingw-w64-x86_64-python-pywin32-ctypes) ; @echo 'PKG@@mingw-w64-x86_64-python-keyring'

override VERSION_OF_mingw-w64-x86_64-python-keystone := 0.9.2-9
.PHONY: PKG@@mingw-w64-x86_64-python-keystone
PKG@@mingw-w64-x86_64-python-keystone: $(if $(__deps),PKG@@mingw-w64-x86_64-keystone PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-keystone'

override VERSION_OF_mingw-w64-x86_64-python-kiwisolver := 1.5.1-2
.PHONY: PKG@@mingw-w64-x86_64-python-kiwisolver
PKG@@mingw-w64-x86_64-python-kiwisolver: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-kiwisolver'

override VERSION_OF_mingw-w64-x86_64-python-lark-parser := 1.3.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-lark-parser
PKG@@mingw-w64-x86_64-python-lark-parser: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-lark-parser'

override VERSION_OF_mingw-w64-x86_64-python-lazy-importer := 0.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-lazy-importer
PKG@@mingw-w64-x86_64-python-lazy-importer: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-lazy-importer'

override VERSION_OF_mingw-w64-x86_64-python-lazy-object-proxy := 1.13.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-lazy-object-proxy
PKG@@mingw-w64-x86_64-python-lazy-object-proxy: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-lazy-object-proxy'

override VERSION_OF_mingw-w64-x86_64-python-levenshtein := 0.27.5-1
.PHONY: PKG@@mingw-w64-x86_64-python-levenshtein
PKG@@mingw-w64-x86_64-python-levenshtein: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-rapidfuzz) ; @echo 'PKG@@mingw-w64-x86_64-python-levenshtein'

override VERSION_OF_mingw-w64-x86_64-python-lhafile := 0.3.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-lhafile
PKG@@mingw-w64-x86_64-python-lhafile: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-lhafile'

override VERSION_OF_mingw-w64-x86_64-python-librt := 0.16.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-librt
PKG@@mingw-w64-x86_64-python-librt: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-librt'

override VERSION_OF_mingw-w64-x86_64-python-libsass := 0.23.0-5
.PHONY: PKG@@mingw-w64-x86_64-python-libsass
PKG@@mingw-w64-x86_64-python-libsass: $(if $(__deps),PKG@@mingw-w64-x86_64-libsass PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-libsass'

override VERSION_OF_mingw-w64-x86_64-python-license-expression := 30.4.4-3
.PHONY: PKG@@mingw-w64-x86_64-python-license-expression
PKG@@mingw-w64-x86_64-python-license-expression: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-boolean.py) ; @echo 'PKG@@mingw-w64-x86_64-python-license-expression'

override VERSION_OF_mingw-w64-x86_64-python-log_symbols := 0.0.14-7
.PHONY: PKG@@mingw-w64-x86_64-python-log_symbols
PKG@@mingw-w64-x86_64-python-log_symbols: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-colorama) ; @echo 'PKG@@mingw-w64-x86_64-python-log_symbols'

override VERSION_OF_mingw-w64-x86_64-python-lws := 1.2.8-6
.PHONY: PKG@@mingw-w64-x86_64-python-lws
PKG@@mingw-w64-x86_64-python-lws: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-numpy) ; @echo 'PKG@@mingw-w64-x86_64-python-lws'

override VERSION_OF_mingw-w64-x86_64-python-lxml := 6.1.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-lxml
PKG@@mingw-w64-x86_64-python-lxml: $(if $(__deps),PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-libxslt PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-lxml'

override VERSION_OF_mingw-w64-x86_64-python-lz4 := 4.4.5-3
.PHONY: PKG@@mingw-w64-x86_64-python-lz4
PKG@@mingw-w64-x86_64-python-lz4: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-lz4'

override VERSION_OF_mingw-w64-x86_64-python-lzo := 1.15-5
.PHONY: PKG@@mingw-w64-x86_64-python-lzo
PKG@@mingw-w64-x86_64-python-lzo: $(if $(__deps),PKG@@mingw-w64-x86_64-lzo2 PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-lzo'

override VERSION_OF_mingw-w64-x86_64-python-mako := 1.4.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-mako
PKG@@mingw-w64-x86_64-python-mako: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-markupsafe) ; @echo 'PKG@@mingw-w64-x86_64-python-mako'

override VERSION_OF_mingw-w64-x86_64-python-mallard-ducktype := 1.0.2-9
.PHONY: PKG@@mingw-w64-x86_64-python-mallard-ducktype
PKG@@mingw-w64-x86_64-python-mallard-ducktype: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-mallard-ducktype'

override VERSION_OF_mingw-w64-x86_64-python-manhole := 1.8.1-4
.PHONY: PKG@@mingw-w64-x86_64-python-manhole
PKG@@mingw-w64-x86_64-python-manhole: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-manhole'

override VERSION_OF_mingw-w64-x86_64-python-markdown := 3.11.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-markdown
PKG@@mingw-w64-x86_64-python-markdown: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-markdown'

override VERSION_OF_mingw-w64-x86_64-python-markdown-include := 0.8.1-7
.PHONY: PKG@@mingw-w64-x86_64-python-markdown-include
PKG@@mingw-w64-x86_64-python-markdown-include: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-markdown) ; @echo 'PKG@@mingw-w64-x86_64-python-markdown-include'

override VERSION_OF_mingw-w64-x86_64-python-markdown-it-py := 4.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-markdown-it-py
PKG@@mingw-w64-x86_64-python-markdown-it-py: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-mdurl) ; @echo 'PKG@@mingw-w64-x86_64-python-markdown-it-py'

override VERSION_OF_mingw-w64-x86_64-python-markdown-math := 0.9-3
.PHONY: PKG@@mingw-w64-x86_64-python-markdown-math
PKG@@mingw-w64-x86_64-python-markdown-math: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-markdown) ; @echo 'PKG@@mingw-w64-x86_64-python-markdown-math'

override VERSION_OF_mingw-w64-x86_64-python-markups := 4.1.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-markups
PKG@@mingw-w64-x86_64-python-markups: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-markdown-math) ; @echo 'PKG@@mingw-w64-x86_64-python-markups'

override VERSION_OF_mingw-w64-x86_64-python-markupsafe := 3.0.4-1
.PHONY: PKG@@mingw-w64-x86_64-python-markupsafe
PKG@@mingw-w64-x86_64-python-markupsafe: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-markupsafe'

override VERSION_OF_mingw-w64-x86_64-python-matplotlib := 3.11.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-matplotlib
PKG@@mingw-w64-x86_64-python-matplotlib: $(if $(__deps),PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-libraqm PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-contourpy PKG@@mingw-w64-x86_64-python-cycler PKG@@mingw-w64-x86_64-python-dateutil PKG@@mingw-w64-x86_64-python-fonttools PKG@@mingw-w64-x86_64-python-kiwisolver PKG@@mingw-w64-x86_64-python-numpy PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-pillow PKG@@mingw-w64-x86_64-python-pyparsing PKG@@mingw-w64-x86_64-qhull) ; @echo 'PKG@@mingw-w64-x86_64-python-matplotlib'

override VERSION_OF_mingw-w64-x86_64-python-maturin := 1.16.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-maturin
PKG@@mingw-w64-x86_64-python-maturin: $(if $(__deps),PKG@@mingw-w64-x86_64-maturin PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-maturin'

override VERSION_OF_mingw-w64-x86_64-python-mccabe := 0.7.0-7
.PHONY: PKG@@mingw-w64-x86_64-python-mccabe
PKG@@mingw-w64-x86_64-python-mccabe: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-mccabe'

override VERSION_OF_mingw-w64-x86_64-python-mdit_py_plugins := 0.6.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-mdit_py_plugins
PKG@@mingw-w64-x86_64-python-mdit_py_plugins: $(if $(__deps),PKG@@mingw-w64-x86_64-python-markdown-it-py) ; @echo 'PKG@@mingw-w64-x86_64-python-mdit_py_plugins'

override VERSION_OF_mingw-w64-x86_64-python-mdurl := 0.1.2-5
.PHONY: PKG@@mingw-w64-x86_64-python-mdurl
PKG@@mingw-w64-x86_64-python-mdurl: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-mdurl'

override VERSION_OF_mingw-w64-x86_64-python-meshio := 5.3.5-6
.PHONY: PKG@@mingw-w64-x86_64-python-meshio
PKG@@mingw-w64-x86_64-python-meshio: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-numpy PKG@@mingw-w64-x86_64-python-rich) ; @echo 'PKG@@mingw-w64-x86_64-python-meshio'

override VERSION_OF_mingw-w64-x86_64-python-mimeparse := 2.0.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-mimeparse
PKG@@mingw-w64-x86_64-python-mimeparse: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-mimeparse'

override VERSION_OF_mingw-w64-x86_64-python-mingw-ldd := 0.2.1-7
.PHONY: PKG@@mingw-w64-x86_64-python-mingw-ldd
PKG@@mingw-w64-x86_64-python-mingw-ldd: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-pefile) ; @echo 'PKG@@mingw-w64-x86_64-python-mingw-ldd'

override VERSION_OF_mingw-w64-x86_64-python-miniupnpc := 2.3.3-4
.PHONY: PKG@@mingw-w64-x86_64-python-miniupnpc
PKG@@mingw-w64-x86_64-python-miniupnpc: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-miniupnpc'

override VERSION_OF_mingw-w64-x86_64-python-mock := 5.2.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-mock
PKG@@mingw-w64-x86_64-python-mock: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-mock'

override VERSION_OF_mingw-w64-x86_64-python-more-itertools := 11.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-more-itertools
PKG@@mingw-w64-x86_64-python-more-itertools: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-more-itertools'

override VERSION_OF_mingw-w64-x86_64-python-mpi4py := 4.1.2-2
.PHONY: PKG@@mingw-w64-x86_64-python-mpi4py
PKG@@mingw-w64-x86_64-python-mpi4py: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-mpi4py'

override VERSION_OF_mingw-w64-x86_64-python-msgpack := 1.2.4-1
.PHONY: PKG@@mingw-w64-x86_64-python-msgpack
PKG@@mingw-w64-x86_64-python-msgpack: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-msgpack'

override VERSION_OF_mingw-w64-x86_64-python-multidict := 7.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-multidict
PKG@@mingw-w64-x86_64-python-multidict: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-multidict'

override VERSION_OF_mingw-w64-x86_64-python-multipledispatch := 1.0.0-5
.PHONY: PKG@@mingw-w64-x86_64-python-multipledispatch
PKG@@mingw-w64-x86_64-python-multipledispatch: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-multipledispatch'

override VERSION_OF_mingw-w64-x86_64-python-multiprocess := 0.70.19-2
.PHONY: PKG@@mingw-w64-x86_64-python-multiprocess
PKG@@mingw-w64-x86_64-python-multiprocess: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-dill) ; @echo 'PKG@@mingw-w64-x86_64-python-multiprocess'

override VERSION_OF_mingw-w64-x86_64-python-mypy_extensions := 1.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-mypy_extensions
PKG@@mingw-w64-x86_64-python-mypy_extensions: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-mypy_extensions'

override VERSION_OF_mingw-w64-x86_64-python-myst-parser := 5.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-myst-parser
PKG@@mingw-w64-x86_64-python-myst-parser: $(if $(__deps),PKG@@mingw-w64-x86_64-python-docutils PKG@@mingw-w64-x86_64-python-jinja PKG@@mingw-w64-x86_64-python-markdown-it-py PKG@@mingw-w64-x86_64-python-mdit_py_plugins PKG@@mingw-w64-x86_64-python-sphinx PKG@@mingw-w64-x86_64-python-typing_extensions PKG@@mingw-w64-x86_64-python-yaml) ; @echo 'PKG@@mingw-w64-x86_64-python-myst-parser'

override VERSION_OF_mingw-w64-x86_64-python-natsort := 8.4.0-6
.PHONY: PKG@@mingw-w64-x86_64-python-natsort
PKG@@mingw-w64-x86_64-python-natsort: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-natsort'

override VERSION_OF_mingw-w64-x86_64-python-nh3 := 0.3.7-1
.PHONY: PKG@@mingw-w64-x86_64-python-nh3
PKG@@mingw-w64-x86_64-python-nh3: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-nh3'

override VERSION_OF_mingw-w64-x86_64-python-node-semver := 0.9.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-node-semver
PKG@@mingw-w64-x86_64-python-node-semver: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-node-semver'

override VERSION_OF_mingw-w64-x86_64-python-nodeenv := 1.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-nodeenv
PKG@@mingw-w64-x86_64-python-nodeenv: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-nodeenv'

override VERSION_OF_mingw-w64-x86_64-python-numexpr := 2.14.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-numexpr
PKG@@mingw-w64-x86_64-python-numexpr: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-numpy) ; @echo 'PKG@@mingw-w64-x86_64-python-numexpr'

override VERSION_OF_mingw-w64-x86_64-python-numpy := 2.5.4-1
.PHONY: PKG@@mingw-w64-x86_64-python-numpy
PKG@@mingw-w64-x86_64-python-numpy: $(if $(__deps),PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-openblas PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-numpy'

override VERSION_OF_mingw-w64-x86_64-python-olefile := 0.47-4
.PHONY: PKG@@mingw-w64-x86_64-python-olefile
PKG@@mingw-w64-x86_64-python-olefile: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-olefile'

override VERSION_OF_mingw-w64-x86_64-python-opencv := 5.0.0-8
.PHONY: PKG@@mingw-w64-x86_64-python-opencv
PKG@@mingw-w64-x86_64-python-opencv: $(if $(__deps),PKG@@mingw-w64-x86_64-opencv PKG@@mingw-w64-x86_64-python-numpy PKG@@mingw-w64-x86_64-qt6-5compat PKG@@mingw-w64-x86_64-qt6-base PKG@@mingw-w64-x86_64-vtk) ; @echo 'PKG@@mingw-w64-x86_64-python-opencv'

override VERSION_OF_mingw-w64-x86_64-python-openpyxl := 3.1.5-5
.PHONY: PKG@@mingw-w64-x86_64-python-openpyxl
PKG@@mingw-w64-x86_64-python-openpyxl: $(if $(__deps),PKG@@mingw-w64-x86_64-python-defusedxml PKG@@mingw-w64-x86_64-python-et-xmlfile PKG@@mingw-w64-x86_64-python-pandas PKG@@mingw-w64-x86_64-python-pillow) ; @echo 'PKG@@mingw-w64-x86_64-python-openpyxl'

override VERSION_OF_mingw-w64-x86_64-python-opt_einsum := 3.4.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-opt_einsum
PKG@@mingw-w64-x86_64-python-opt_einsum: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-opt_einsum'

override VERSION_OF_mingw-w64-x86_64-python-ordered-set := 4.1.0-7
.PHONY: PKG@@mingw-w64-x86_64-python-ordered-set
PKG@@mingw-w64-x86_64-python-ordered-set: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-ordered-set'

override VERSION_OF_mingw-w64-x86_64-python-outcome := 1.3.0.post0-4
.PHONY: PKG@@mingw-w64-x86_64-python-outcome
PKG@@mingw-w64-x86_64-python-outcome: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-attrs) ; @echo 'PKG@@mingw-w64-x86_64-python-outcome'

override VERSION_OF_mingw-w64-x86_64-python-overrides := 7.7.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-overrides
PKG@@mingw-w64-x86_64-python-overrides: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-overrides'

override VERSION_OF_mingw-w64-x86_64-python-owslib := 0.36.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-owslib
PKG@@mingw-w64-x86_64-python-owslib: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-dateutil PKG@@mingw-w64-x86_64-python-lxml PKG@@mingw-w64-x86_64-python-pytz PKG@@mingw-w64-x86_64-python-requests PKG@@mingw-w64-x86_64-python-yaml) ; @echo 'PKG@@mingw-w64-x86_64-python-owslib'

override VERSION_OF_mingw-w64-x86_64-python-pacdb := 0.1.0-6
.PHONY: PKG@@mingw-w64-x86_64-python-pacdb
PKG@@mingw-w64-x86_64-python-pacdb: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-zstandard) ; @echo 'PKG@@mingw-w64-x86_64-python-pacdb'

override VERSION_OF_mingw-w64-x86_64-python-packageurl-python := 0.17.6-1
.PHONY: PKG@@mingw-w64-x86_64-python-packageurl-python
PKG@@mingw-w64-x86_64-python-packageurl-python: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-packageurl-python'

override VERSION_OF_mingw-w64-x86_64-python-packaging := 26.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-packaging
PKG@@mingw-w64-x86_64-python-packaging: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-packaging'

override VERSION_OF_mingw-w64-x86_64-python-pallets-sphinx-themes := 2.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pallets-sphinx-themes
PKG@@mingw-w64-x86_64-python-pallets-sphinx-themes: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-sphinx PKG@@mingw-w64-x86_64-python-sphinx-notfound-page) ; @echo 'PKG@@mingw-w64-x86_64-python-pallets-sphinx-themes'

override VERSION_OF_mingw-w64-x86_64-python-pandas := 3.1.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-pandas
PKG@@mingw-w64-x86_64-python-pandas: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-dateutil PKG@@mingw-w64-x86_64-python-numpy PKG@@mingw-w64-x86_64-python-pytz PKG@@mingw-w64-x86_64-python-tzdata) ; @echo 'PKG@@mingw-w64-x86_64-python-pandas'

override VERSION_OF_mingw-w64-x86_64-python-pandocfilters := 1.5.1-4
.PHONY: PKG@@mingw-w64-x86_64-python-pandocfilters
PKG@@mingw-w64-x86_64-python-pandocfilters: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pandocfilters'

override VERSION_OF_mingw-w64-x86_64-python-passlib := 1.7.4-5
.PHONY: PKG@@mingw-w64-x86_64-python-passlib
PKG@@mingw-w64-x86_64-python-passlib: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-passlib'

override VERSION_OF_mingw-w64-x86_64-python-pastel := 0.2.1-5
.PHONY: PKG@@mingw-w64-x86_64-python-pastel
PKG@@mingw-w64-x86_64-python-pastel: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pastel'

override VERSION_OF_mingw-w64-x86_64-python-patch-ng := 1.19.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-patch-ng
PKG@@mingw-w64-x86_64-python-patch-ng: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-patch-ng'

override VERSION_OF_mingw-w64-x86_64-python-path := 17.1.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-path PKG@@mingw-w64-x86_64-python-path.py
PKG@@mingw-w64-x86_64-python-path PKG@@mingw-w64-x86_64-python-path.py: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-path'

override VERSION_OF_mingw-w64-x86_64-python-pathspec := 1.1.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-pathspec
PKG@@mingw-w64-x86_64-python-pathspec: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pathspec'

override VERSION_OF_mingw-w64-x86_64-python-pbr := 7.1.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-pbr
PKG@@mingw-w64-x86_64-python-pbr: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pbr'

override VERSION_OF_mingw-w64-x86_64-python-pbs-installer := 2026.10.25-1
.PHONY: PKG@@mingw-w64-x86_64-python-pbs-installer
PKG@@mingw-w64-x86_64-python-pbs-installer: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pbs-installer'

override VERSION_OF_mingw-w64-x86_64-python-pcpp := 1.30-5
.PHONY: PKG@@mingw-w64-x86_64-python-pcpp
PKG@@mingw-w64-x86_64-python-pcpp: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pcpp'

override VERSION_OF_mingw-w64-x86_64-python-pdfminer := 20260107-2
.PHONY: PKG@@mingw-w64-x86_64-python-pdfminer
PKG@@mingw-w64-x86_64-python-pdfminer: $(if $(__deps),PKG@@mingw-w64-x86_64-python-charset-normalizer PKG@@mingw-w64-x86_64-python-cryptography) ; @echo 'PKG@@mingw-w64-x86_64-python-pdfminer'

override VERSION_OF_mingw-w64-x86_64-python-pdm-backend := 2.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pdm-backend
PKG@@mingw-w64-x86_64-python-pdm-backend: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-pyproject-metadata PKG@@mingw-w64-x86_64-python-tomli-w) ; @echo 'PKG@@mingw-w64-x86_64-python-pdm-backend'

override VERSION_OF_mingw-w64-x86_64-python-pefile := 2024.8.26-3
.PHONY: PKG@@mingw-w64-x86_64-python-pefile
PKG@@mingw-w64-x86_64-python-pefile: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-future) ; @echo 'PKG@@mingw-w64-x86_64-python-pefile'

override VERSION_OF_mingw-w64-x86_64-python-pesq := 0.0.4-8
.PHONY: PKG@@mingw-w64-x86_64-python-pesq
PKG@@mingw-w64-x86_64-python-pesq: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-numpy) ; @echo 'PKG@@mingw-w64-x86_64-python-pesq'

override VERSION_OF_mingw-w64-x86_64-python-pexpect := 4.9.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-pexpect
PKG@@mingw-w64-x86_64-python-pexpect: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-ptyprocess) ; @echo 'PKG@@mingw-w64-x86_64-python-pexpect'

override VERSION_OF_mingw-w64-x86_64-python-pgen2 := 0.1.1-7
.PHONY: PKG@@mingw-w64-x86_64-python-pgen2
PKG@@mingw-w64-x86_64-python-pgen2: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pgen2'

override VERSION_OF_mingw-w64-x86_64-python-pi-heif := 1.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pi-heif
PKG@@mingw-w64-x86_64-python-pi-heif: $(if $(__deps),PKG@@mingw-w64-x86_64-libheif PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-pillow) ; @echo 'PKG@@mingw-w64-x86_64-python-pi-heif'

override VERSION_OF_mingw-w64-x86_64-python-pika := 1.4.4-1
.PHONY: PKG@@mingw-w64-x86_64-python-pika
PKG@@mingw-w64-x86_64-python-pika: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pika'

override VERSION_OF_mingw-w64-x86_64-python-pillow := 13.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pillow
PKG@@mingw-w64-x86_64-python-pillow: $(if $(__deps),PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-lcms2 PKG@@mingw-w64-x86_64-libavif PKG@@mingw-w64-x86_64-libimagequant PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libraqm PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-openjpeg2 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-python-pillow'

override VERSION_OF_mingw-w64-x86_64-python-pip := 26.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-pip
PKG@@mingw-w64-x86_64-python-pip: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-distlib PKG@@mingw-w64-x86_64-python-setuptools) ; @echo 'PKG@@mingw-w64-x86_64-python-pip'

override VERSION_OF_mingw-w64-x86_64-python-pipenv := 2026.11.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pipenv
PKG@@mingw-w64-x86_64-python-pipenv: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-certifi PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-setuptools PKG@@mingw-w64-x86_64-python-virtualenv PKG@@mingw-w64-x86_64-python-virtualenv-clone) ; @echo 'PKG@@mingw-w64-x86_64-python-pipenv'

override VERSION_OF_mingw-w64-x86_64-python-pkg-resources := 81.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pkg-resources
PKG@@mingw-w64-x86_64-python-pkg-resources: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-setuptools) ; @echo 'PKG@@mingw-w64-x86_64-python-pkg-resources'

override VERSION_OF_mingw-w64-x86_64-python-pkgconfig := 1.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pkgconfig
PKG@@mingw-w64-x86_64-python-pkgconfig: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pkgconfig'

override VERSION_OF_mingw-w64-x86_64-python-pkginfo := 1.13.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-pkginfo
PKG@@mingw-w64-x86_64-python-pkginfo: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pkginfo'

override VERSION_OF_mingw-w64-x86_64-python-platformdirs := 4.14.5-1
.PHONY: PKG@@mingw-w64-x86_64-python-platformdirs
PKG@@mingw-w64-x86_64-python-platformdirs: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-platformdirs'

override VERSION_OF_mingw-w64-x86_64-python-pluggy := 1.7.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pluggy
PKG@@mingw-w64-x86_64-python-pluggy: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pluggy'

override VERSION_OF_mingw-w64-x86_64-python-pluginbase := 1.0.1-6
.PHONY: PKG@@mingw-w64-x86_64-python-pluginbase
PKG@@mingw-w64-x86_64-python-pluginbase: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pluginbase'

override VERSION_OF_mingw-w64-x86_64-python-ply := 3.11-8
.PHONY: PKG@@mingw-w64-x86_64-python-ply
PKG@@mingw-w64-x86_64-python-ply: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-ply'

override VERSION_OF_mingw-w64-x86_64-python-poetry := 2.5.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-poetry
PKG@@mingw-w64-x86_64-python-poetry: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-build PKG@@mingw-w64-x86_64-python-cachecontrol PKG@@mingw-w64-x86_64-python-cleo PKG@@mingw-w64-x86_64-python-dulwich PKG@@mingw-w64-x86_64-python-fastjsonschema PKG@@mingw-w64-x86_64-python-findpython PKG@@mingw-w64-x86_64-python-installer PKG@@mingw-w64-x86_64-python-keyring PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-pbs-installer PKG@@mingw-w64-x86_64-python-pkginfo PKG@@mingw-w64-x86_64-python-platformdirs PKG@@mingw-w64-x86_64-python-poetry-core PKG@@mingw-w64-x86_64-python-pyproject-hooks PKG@@mingw-w64-x86_64-python-requests PKG@@mingw-w64-x86_64-python-requests-toolbelt PKG@@mingw-w64-x86_64-python-shellingham PKG@@mingw-w64-x86_64-python-tomlkit PKG@@mingw-w64-x86_64-python-trove-classifiers PKG@@mingw-w64-x86_64-python-virtualenv) ; @echo 'PKG@@mingw-w64-x86_64-python-poetry'

override VERSION_OF_mingw-w64-x86_64-python-poetry-core := 2.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-poetry-core
PKG@@mingw-w64-x86_64-python-poetry-core: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-poetry-core'

override VERSION_OF_mingw-w64-x86_64-python-poetry-plugin-export := 1.10.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-poetry-plugin-export
PKG@@mingw-w64-x86_64-python-poetry-plugin-export: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-poetry PKG@@mingw-w64-x86_64-python-poetry-core) ; @echo 'PKG@@mingw-w64-x86_64-python-poetry-plugin-export'

override VERSION_OF_mingw-w64-x86_64-python-poetry-plugin-shell := 1.0.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-poetry-plugin-shell
PKG@@mingw-w64-x86_64-python-poetry-plugin-shell: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-pexpect PKG@@mingw-w64-x86_64-python-poetry PKG@@mingw-w64-x86_64-python-shellingham) ; @echo 'PKG@@mingw-w64-x86_64-python-poetry-plugin-shell'

override VERSION_OF_mingw-w64-x86_64-python-pptx := 1.0.2-4
.PHONY: PKG@@mingw-w64-x86_64-python-pptx
PKG@@mingw-w64-x86_64-python-pptx: $(if $(__deps),PKG@@mingw-w64-x86_64-python-lxml PKG@@mingw-w64-x86_64-python-pillow PKG@@mingw-w64-x86_64-python-typing_extensions PKG@@mingw-w64-x86_64-python-xlsxwriter) ; @echo 'PKG@@mingw-w64-x86_64-python-pptx'

override VERSION_OF_mingw-w64-x86_64-python-pretend := 1.0.9-8
.PHONY: PKG@@mingw-w64-x86_64-python-pretend
PKG@@mingw-w64-x86_64-python-pretend: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pretend'

override VERSION_OF_mingw-w64-x86_64-python-prettytable := 3.18.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-prettytable
PKG@@mingw-w64-x86_64-python-prettytable: $(if $(__deps),PKG@@mingw-w64-x86_64-python-wcwidth) ; @echo 'PKG@@mingw-w64-x86_64-python-prettytable'

override VERSION_OF_mingw-w64-x86_64-python-priority := 2.0.0-7
.PHONY: PKG@@mingw-w64-x86_64-python-priority
PKG@@mingw-w64-x86_64-python-priority: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-priority'

override VERSION_OF_mingw-w64-x86_64-python-progress := 1.6.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-progress
PKG@@mingw-w64-x86_64-python-progress: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-progress'

override VERSION_OF_mingw-w64-x86_64-python-prompt_toolkit := 3.0.53-1
.PHONY: PKG@@mingw-w64-x86_64-python-prompt_toolkit
PKG@@mingw-w64-x86_64-python-prompt_toolkit: $(if $(__deps),PKG@@mingw-w64-x86_64-python-wcwidth) ; @echo 'PKG@@mingw-w64-x86_64-python-prompt_toolkit'

override VERSION_OF_mingw-w64-x86_64-python-protobuf := 7.36.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-protobuf
PKG@@mingw-w64-x86_64-python-protobuf: $(if $(__deps),PKG@@mingw-w64-x86_64-protobuf PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-protobuf'

override VERSION_OF_mingw-w64-x86_64-python-psleak := 0.1.6-1
.PHONY: PKG@@mingw-w64-x86_64-python-psleak
PKG@@mingw-w64-x86_64-python-psleak: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-psutil) ; @echo 'PKG@@mingw-w64-x86_64-python-psleak'

override VERSION_OF_mingw-w64-x86_64-python-psutil := 7.2.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-psutil
PKG@@mingw-w64-x86_64-python-psutil: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-psutil'

override VERSION_OF_mingw-w64-x86_64-python-psycopg2 := 2.9.13-1
.PHONY: PKG@@mingw-w64-x86_64-python-psycopg2
PKG@@mingw-w64-x86_64-python-psycopg2: $(if $(__deps),PKG@@mingw-w64-x86_64-postgresql PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-psycopg2'

override VERSION_OF_mingw-w64-x86_64-python-ptyprocess := 0.7.0-8
.PHONY: PKG@@mingw-w64-x86_64-python-ptyprocess
PKG@@mingw-w64-x86_64-python-ptyprocess: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-ptyprocess'

override VERSION_OF_mingw-w64-x86_64-python-purl := 1.6-8
.PHONY: PKG@@mingw-w64-x86_64-python-purl
PKG@@mingw-w64-x86_64-python-purl: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-six) ; @echo 'PKG@@mingw-w64-x86_64-python-purl'

override VERSION_OF_mingw-w64-x86_64-python-py-cpuinfo := 9.0.0-7
.PHONY: PKG@@mingw-w64-x86_64-python-py-cpuinfo
PKG@@mingw-w64-x86_64-python-py-cpuinfo: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-py-cpuinfo'

override VERSION_OF_mingw-w64-x86_64-python-py-cpuinfo2 := 10.1.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-py-cpuinfo2
PKG@@mingw-w64-x86_64-python-py-cpuinfo2: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-py-cpuinfo2'

override VERSION_OF_mingw-w64-x86_64-python-pyaml := 26.7.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyaml
PKG@@mingw-w64-x86_64-python-pyaml: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-yaml) ; @echo 'PKG@@mingw-w64-x86_64-python-pyaml'

override VERSION_OF_mingw-w64-x86_64-python-pyarrow := 26.0.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyarrow
PKG@@mingw-w64-x86_64-python-pyarrow: $(if $(__deps),PKG@@mingw-w64-x86_64-arrow PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-numpy) ; @echo 'PKG@@mingw-w64-x86_64-python-pyarrow'

override VERSION_OF_mingw-w64-x86_64-python-pyasn1 := 0.6.5-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyasn1
PKG@@mingw-w64-x86_64-python-pyasn1: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyasn1'

override VERSION_OF_mingw-w64-x86_64-python-pyasn1-modules := 0.4.2-3
.PHONY: PKG@@mingw-w64-x86_64-python-pyasn1-modules
PKG@@mingw-w64-x86_64-python-pyasn1-modules: $(if $(__deps),PKG@@mingw-w64-x86_64-python-pyasn1) ; @echo 'PKG@@mingw-w64-x86_64-python-pyasn1-modules'

override VERSION_OF_mingw-w64-x86_64-python-pybullet := 3.25-11
.PHONY: PKG@@mingw-w64-x86_64-python-pybullet
PKG@@mingw-w64-x86_64-python-pybullet: $(if $(__deps),PKG@@mingw-w64-x86_64-bullet PKG@@mingw-w64-x86_64-python-numpy) ; @echo 'PKG@@mingw-w64-x86_64-python-pybullet'

override VERSION_OF_mingw-w64-x86_64-python-pycodestyle := 2.15.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pycodestyle
PKG@@mingw-w64-x86_64-python-pycodestyle: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pycodestyle'

override VERSION_OF_mingw-w64-x86_64-python-pycparser := 3.12-1
.PHONY: PKG@@mingw-w64-x86_64-python-pycparser
PKG@@mingw-w64-x86_64-python-pycparser: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pycparser'

override VERSION_OF_mingw-w64-x86_64-python-pydocstyle := 6.3.0-7
.PHONY: PKG@@mingw-w64-x86_64-python-pydocstyle
PKG@@mingw-w64-x86_64-python-pydocstyle: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-snowballstemmer) ; @echo 'PKG@@mingw-w64-x86_64-python-pydocstyle'

override VERSION_OF_mingw-w64-x86_64-python-pydub := 0.25.1-7
.PHONY: PKG@@mingw-w64-x86_64-python-pydub
PKG@@mingw-w64-x86_64-python-pydub: $(if $(__deps),PKG@@mingw-w64-x86_64-ffmpeg PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pydub'

override VERSION_OF_mingw-w64-x86_64-python-pyelftools := 0.33-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyelftools
PKG@@mingw-w64-x86_64-python-pyelftools: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyelftools'

override VERSION_OF_mingw-w64-x86_64-python-pyfastbti := 0.r3.da01c14-8
.PHONY: PKG@@mingw-w64-x86_64-python-pyfastbti PKG@@mingw-w64-x86_64-python-pyfastbti-git
PKG@@mingw-w64-x86_64-python-pyfastbti PKG@@mingw-w64-x86_64-python-pyfastbti-git: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyfastbti'

override VERSION_OF_mingw-w64-x86_64-python-pyfasttextureutils := 0.r6.786d7cd-3
.PHONY: PKG@@mingw-w64-x86_64-python-pyfasttextureutils PKG@@mingw-w64-x86_64-python-pyfasttextureutils-git
PKG@@mingw-w64-x86_64-python-pyfasttextureutils PKG@@mingw-w64-x86_64-python-pyfasttextureutils-git: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyfasttextureutils'

override VERSION_OF_mingw-w64-x86_64-python-pyfdt := 0.3-3
.PHONY: PKG@@mingw-w64-x86_64-python-pyfdt
PKG@@mingw-w64-x86_64-python-pyfdt: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyfdt'

override VERSION_OF_mingw-w64-x86_64-python-pyfilesystem2 := 2.4.16-6
.PHONY: PKG@@mingw-w64-x86_64-python-pyfilesystem2
PKG@@mingw-w64-x86_64-python-pyfilesystem2: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-appdirs PKG@@mingw-w64-x86_64-python-pytz PKG@@mingw-w64-x86_64-python-six) ; @echo 'PKG@@mingw-w64-x86_64-python-pyfilesystem2'

override VERSION_OF_mingw-w64-x86_64-python-pyflakes := 4.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyflakes
PKG@@mingw-w64-x86_64-python-pyflakes: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyflakes'

override VERSION_OF_mingw-w64-x86_64-python-pygccxml := 3.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-python-pygccxml
PKG@@mingw-w64-x86_64-python-pygccxml: $(if $(__deps),PKG@@mingw-w64-x86_64-castxml PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pygccxml'

override VERSION_OF_mingw-w64-x86_64-python-pygit2 := 1.21.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pygit2
PKG@@mingw-w64-x86_64-python-pygit2: $(if $(__deps),PKG@@mingw-w64-x86_64-libgit2 PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-cffi) ; @echo 'PKG@@mingw-w64-x86_64-python-pygit2'

override VERSION_OF_mingw-w64-x86_64-python-pyglm := 2.8.3-3
.PHONY: PKG@@mingw-w64-x86_64-python-pyglm
PKG@@mingw-w64-x86_64-python-pyglm: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyglm'

override VERSION_OF_mingw-w64-x86_64-python-pygments := 2.21.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pygments
PKG@@mingw-w64-x86_64-python-pygments: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pygments'

override VERSION_OF_mingw-w64-x86_64-python-pygments-github-lexers := 0.0.5-7
.PHONY: PKG@@mingw-w64-x86_64-python-pygments-github-lexers
PKG@@mingw-w64-x86_64-python-pygments-github-lexers: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-pygments) ; @echo 'PKG@@mingw-w64-x86_64-python-pygments-github-lexers'

override VERSION_OF_mingw-w64-x86_64-python-pykwalify := 1.8.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-pykwalify
PKG@@mingw-w64-x86_64-python-pykwalify: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-dateutil PKG@@mingw-w64-x86_64-python-docopt PKG@@mingw-w64-x86_64-python-ruamel-yaml) ; @echo 'PKG@@mingw-w64-x86_64-python-pykwalify'

override VERSION_OF_mingw-w64-x86_64-python-pylev := 1.4.0-5
.PHONY: PKG@@mingw-w64-x86_64-python-pylev
PKG@@mingw-w64-x86_64-python-pylev: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pylev'

override VERSION_OF_mingw-w64-x86_64-python-pymongocrypt := 1.20.4-1
.PHONY: PKG@@mingw-w64-x86_64-python-pymongocrypt
PKG@@mingw-w64-x86_64-python-pymongocrypt: $(if $(__deps),PKG@@mingw-w64-x86_64-libmongocrypt PKG@@mingw-w64-x86_64-python-cffi PKG@@mingw-w64-x86_64-python-cryptography) ; @echo 'PKG@@mingw-w64-x86_64-python-pymongocrypt'

override VERSION_OF_mingw-w64-x86_64-python-pynput := 1.8.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-pynput
PKG@@mingw-w64-x86_64-python-pynput: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-six) ; @echo 'PKG@@mingw-w64-x86_64-python-pynput'

override VERSION_OF_mingw-w64-x86_64-python-pyodbc := 5.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyodbc
PKG@@mingw-w64-x86_64-python-pyodbc: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyodbc'

override VERSION_OF_mingw-w64-x86_64-python-pyopengl := 3.1.10-3
.PHONY: PKG@@mingw-w64-x86_64-python-pyopengl
PKG@@mingw-w64-x86_64-python-pyopengl: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyopengl'

override VERSION_OF_mingw-w64-x86_64-python-pyopengl-accelerate := 3.1.10-3
.PHONY: PKG@@mingw-w64-x86_64-python-pyopengl-accelerate
PKG@@mingw-w64-x86_64-python-pyopengl-accelerate: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-pyopengl) ; @echo 'PKG@@mingw-w64-x86_64-python-pyopengl-accelerate'

override VERSION_OF_mingw-w64-x86_64-python-pyopenssl := 26.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyopenssl
PKG@@mingw-w64-x86_64-python-pyopenssl: $(if $(__deps),PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-python-cryptography PKG@@mingw-w64-x86_64-python-typing_extensions) ; @echo 'PKG@@mingw-w64-x86_64-python-pyopenssl'

override VERSION_OF_mingw-w64-x86_64-python-pyparsing := 3.3.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyparsing
PKG@@mingw-w64-x86_64-python-pyparsing: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyparsing'

override VERSION_OF_mingw-w64-x86_64-python-pyperclip := 1.11.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-pyperclip
PKG@@mingw-w64-x86_64-python-pyperclip: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyperclip'

override VERSION_OF_mingw-w64-x86_64-python-pyproject-api := 1.12.5-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyproject-api
PKG@@mingw-w64-x86_64-python-pyproject-api: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging) ; @echo 'PKG@@mingw-w64-x86_64-python-pyproject-api'

override VERSION_OF_mingw-w64-x86_64-python-pyproject-hooks := 1.3.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyproject-hooks
PKG@@mingw-w64-x86_64-python-pyproject-hooks: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyproject-hooks'

override VERSION_OF_mingw-w64-x86_64-python-pyproject-metadata := 0.12.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyproject-metadata
PKG@@mingw-w64-x86_64-python-pyproject-metadata: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging) ; @echo 'PKG@@mingw-w64-x86_64-python-pyproject-metadata'

override VERSION_OF_mingw-w64-x86_64-python-pyqt5 := 5.15.11-5
.PHONY: PKG@@mingw-w64-x86_64-python-pyqt5
PKG@@mingw-w64-x86_64-python-pyqt5: $(if $(__deps),PKG@@mingw-w64-x86_64-pyqt5-sip PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-python-pyqt5'

override VERSION_OF_mingw-w64-x86_64-python-pyqt6-sip := 13.13.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyqt6-sip
PKG@@mingw-w64-x86_64-python-pyqt6-sip: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyqt6-sip'

override VERSION_OF_mingw-w64-x86_64-python-pyrr := 0.10.3-8
.PHONY: PKG@@mingw-w64-x86_64-python-pyrr
PKG@@mingw-w64-x86_64-python-pyrr: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-multipledispatch PKG@@mingw-w64-x86_64-python-numpy) ; @echo 'PKG@@mingw-w64-x86_64-python-pyrr'

override VERSION_OF_mingw-w64-x86_64-python-pysfcgal := 2.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pysfcgal
PKG@@mingw-w64-x86_64-python-pysfcgal: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-cffi PKG@@mingw-w64-x86_64-sfcgal) ; @echo 'PKG@@mingw-w64-x86_64-python-pysfcgal'

override VERSION_OF_mingw-w64-x86_64-python-pyshp := 3.1.8-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyshp
PKG@@mingw-w64-x86_64-python-pyshp: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyshp'

override VERSION_OF_mingw-w64-x86_64-python-pysocks := 1.7.1-4
.PHONY: PKG@@mingw-w64-x86_64-python-pysocks
PKG@@mingw-w64-x86_64-python-pysocks: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pysocks'

override VERSION_OF_mingw-w64-x86_64-python-pystemmer := 3.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pystemmer
PKG@@mingw-w64-x86_64-python-pystemmer: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pystemmer'

override VERSION_OF_mingw-w64-x86_64-python-pytest := 9.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pytest
PKG@@mingw-w64-x86_64-python-pytest: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-colorama PKG@@mingw-w64-x86_64-python-iniconfig PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-pluggy PKG@@mingw-w64-x86_64-python-pygments) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest'

override VERSION_OF_mingw-w64-x86_64-python-pytest-asyncio := 1.4.1.post1-1
.PHONY: PKG@@mingw-w64-x86_64-python-pytest-asyncio
PKG@@mingw-w64-x86_64-python-pytest-asyncio: $(if $(__deps),PKG@@mingw-w64-x86_64-python-pytest) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest-asyncio'

override VERSION_OF_mingw-w64-x86_64-python-pytest-benchmark := 5.3.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-pytest-benchmark
PKG@@mingw-w64-x86_64-python-pytest-benchmark: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-py-cpuinfo2 PKG@@mingw-w64-x86_64-python-pytest) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest-benchmark'

override VERSION_OF_mingw-w64-x86_64-python-pytest-cov := 7.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pytest-cov
PKG@@mingw-w64-x86_64-python-pytest-cov: $(if $(__deps),PKG@@mingw-w64-x86_64-python-coverage PKG@@mingw-w64-x86_64-python-pluggy PKG@@mingw-w64-x86_64-python-pytest) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest-cov'

override VERSION_OF_mingw-w64-x86_64-python-pytest-fixture-config := 1.8.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-pytest-fixture-config
PKG@@mingw-w64-x86_64-python-pytest-fixture-config: $(if $(__deps),PKG@@mingw-w64-x86_64-python-pytest) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest-fixture-config'

override VERSION_OF_mingw-w64-x86_64-python-pytest-flake8 := 1.3.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-pytest-flake8
PKG@@mingw-w64-x86_64-python-pytest-flake8: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-flake8 PKG@@mingw-w64-x86_64-python-pytest) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest-flake8'

override VERSION_OF_mingw-w64-x86_64-python-pytest-localserver := 0.10.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pytest-localserver
PKG@@mingw-w64-x86_64-python-pytest-localserver: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-werkzeug) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest-localserver'

override VERSION_OF_mingw-w64-x86_64-python-pytest-mock := 3.17.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pytest-mock
PKG@@mingw-w64-x86_64-python-pytest-mock: $(if $(__deps),PKG@@mingw-w64-x86_64-python-pytest) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest-mock'

override VERSION_OF_mingw-w64-x86_64-python-pytest-rerunfailures := 16.7-1
.PHONY: PKG@@mingw-w64-x86_64-python-pytest-rerunfailures
PKG@@mingw-w64-x86_64-python-pytest-rerunfailures: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-pytest) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest-rerunfailures'

override VERSION_OF_mingw-w64-x86_64-python-pytest-shutil := 1.8.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-pytest-shutil
PKG@@mingw-w64-x86_64-python-pytest-shutil: $(if $(__deps),PKG@@mingw-w64-x86_64-python-execnet PKG@@mingw-w64-x86_64-python-pytest PKG@@mingw-w64-x86_64-python-six PKG@@mingw-w64-x86_64-python-termcolor) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest-shutil'

override VERSION_OF_mingw-w64-x86_64-python-pytest-subprocess := 1.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pytest-subprocess
PKG@@mingw-w64-x86_64-python-pytest-subprocess: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-pytest) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest-subprocess'

override VERSION_OF_mingw-w64-x86_64-python-pytest-timeout := 2.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pytest-timeout
PKG@@mingw-w64-x86_64-python-pytest-timeout: $(if $(__deps),PKG@@mingw-w64-x86_64-python-pytest) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest-timeout'

override VERSION_OF_mingw-w64-x86_64-python-pytest-xdist := 3.8.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-pytest-xdist
PKG@@mingw-w64-x86_64-python-pytest-xdist: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-execnet PKG@@mingw-w64-x86_64-python-pytest) ; @echo 'PKG@@mingw-w64-x86_64-python-pytest-xdist'

override VERSION_OF_mingw-w64-x86_64-python-python-discovery := 1.9.5-1
.PHONY: PKG@@mingw-w64-x86_64-python-python-discovery
PKG@@mingw-w64-x86_64-python-python-discovery: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-filelock PKG@@mingw-w64-x86_64-python-platformdirs) ; @echo 'PKG@@mingw-w64-x86_64-python-python-discovery'

override VERSION_OF_mingw-w64-x86_64-python-python-multipart := 0.0.33-1
.PHONY: PKG@@mingw-w64-x86_64-python-python-multipart
PKG@@mingw-w64-x86_64-python-python-multipart: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-python-multipart'

override VERSION_OF_mingw-w64-x86_64-python-python-sbom := 0.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-python-sbom
PKG@@mingw-w64-x86_64-python-python-sbom: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-installer PKG@@mingw-w64-x86_64-python-packageurl-python PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-zipremove) ; @echo 'PKG@@mingw-w64-x86_64-python-python-sbom'

override VERSION_OF_mingw-w64-x86_64-python-pythran := 0.19.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pythran
PKG@@mingw-w64-x86_64-python-pythran: $(if $(__deps),PKG@@mingw-w64-x86_64-boost PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-beniget PKG@@mingw-w64-x86_64-python-gast PKG@@mingw-w64-x86_64-python-numpy PKG@@mingw-w64-x86_64-python-ply PKG@@mingw-w64-x86_64-xsimd) ; @echo 'PKG@@mingw-w64-x86_64-python-pythran'

override VERSION_OF_mingw-w64-x86_64-python-pytoolconfig := 1.3.1-4
.PHONY: PKG@@mingw-w64-x86_64-python-pytoolconfig
PKG@@mingw-w64-x86_64-python-pytoolconfig: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging) ; @echo 'PKG@@mingw-w64-x86_64-python-pytoolconfig'

override VERSION_OF_mingw-w64-x86_64-python-pytz := 2026.6-1
.PHONY: PKG@@mingw-w64-x86_64-python-pytz
PKG@@mingw-w64-x86_64-python-pytz: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pytz'

override VERSION_OF_mingw-w64-x86_64-python-pyuca := 1.2-4
.PHONY: PKG@@mingw-w64-x86_64-python-pyuca
PKG@@mingw-w64-x86_64-python-pyuca: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyuca'

override VERSION_OF_mingw-w64-x86_64-python-pyvda := 0.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyvda
PKG@@mingw-w64-x86_64-python-pyvda: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-comtypes PKG@@mingw-w64-x86_64-python-pywin32) ; @echo 'PKG@@mingw-w64-x86_64-python-pyvda'

override VERSION_OF_mingw-w64-x86_64-python-pywin32 := 310-4
.PHONY: PKG@@mingw-w64-x86_64-python-pywin32
PKG@@mingw-w64-x86_64-python-pywin32: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pywin32'

override VERSION_OF_mingw-w64-x86_64-python-pywin32-ctypes := 0.2.3-4
.PHONY: PKG@@mingw-w64-x86_64-python-pywin32-ctypes
PKG@@mingw-w64-x86_64-python-pywin32-ctypes: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pywin32-ctypes'

override VERSION_OF_mingw-w64-x86_64-python-pyzopfli := 0.4.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-pyzopfli
PKG@@mingw-w64-x86_64-python-pyzopfli: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-pyzopfli'

override VERSION_OF_mingw-w64-x86_64-python-quart := 0.23.1-2
.PHONY: PKG@@mingw-w64-x86_64-python-quart
PKG@@mingw-w64-x86_64-python-quart: $(if $(__deps),PKG@@mingw-w64-x86_64-hypercorn PKG@@mingw-w64-x86_64-python-aiofiles PKG@@mingw-w64-x86_64-python-blinker PKG@@mingw-w64-x86_64-python-click PKG@@mingw-w64-x86_64-python-flask PKG@@mingw-w64-x86_64-python-itsdangerous PKG@@mingw-w64-x86_64-python-jinja PKG@@mingw-w64-x86_64-python-markupsafe PKG@@mingw-w64-x86_64-python-werkzeug) ; @echo 'PKG@@mingw-w64-x86_64-python-quart'

override VERSION_OF_mingw-w64-x86_64-python-rapidfuzz := 3.14.6-2
.PHONY: PKG@@mingw-w64-x86_64-python-rapidfuzz
PKG@@mingw-w64-x86_64-python-rapidfuzz: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-rapidfuzz'

override VERSION_OF_mingw-w64-x86_64-python-re-assert := 1.1.0-8
.PHONY: PKG@@mingw-w64-x86_64-python-re-assert
PKG@@mingw-w64-x86_64-python-re-assert: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-regex) ; @echo 'PKG@@mingw-w64-x86_64-python-re-assert'

override VERSION_OF_mingw-w64-x86_64-python-regex := 2026.11.9-1
.PHONY: PKG@@mingw-w64-x86_64-python-regex
PKG@@mingw-w64-x86_64-python-regex: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-regex'

override VERSION_OF_mingw-w64-x86_64-python-requests := 2.34.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-requests
PKG@@mingw-w64-x86_64-python-requests: $(if $(__deps),PKG@@mingw-w64-x86_64-python-certifi PKG@@mingw-w64-x86_64-python-charset-normalizer PKG@@mingw-w64-x86_64-python-idna PKG@@mingw-w64-x86_64-python-urllib3) ; @echo 'PKG@@mingw-w64-x86_64-python-requests'

override VERSION_OF_mingw-w64-x86_64-python-requests-cache := 1.3.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-requests-cache
PKG@@mingw-w64-x86_64-python-requests-cache: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-attrs PKG@@mingw-w64-x86_64-python-cattrs PKG@@mingw-w64-x86_64-python-platformdirs PKG@@mingw-w64-x86_64-python-requests PKG@@mingw-w64-x86_64-python-url-normalize PKG@@mingw-w64-x86_64-python-urllib3) ; @echo 'PKG@@mingw-w64-x86_64-python-requests-cache'

override VERSION_OF_mingw-w64-x86_64-python-requests-mock := 1.12.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-requests-mock
PKG@@mingw-w64-x86_64-python-requests-mock: $(if $(__deps),PKG@@mingw-w64-x86_64-python-requests PKG@@mingw-w64-x86_64-python-six) ; @echo 'PKG@@mingw-w64-x86_64-python-requests-mock'

override VERSION_OF_mingw-w64-x86_64-python-requests-toolbelt := 1.0.0-5
.PHONY: PKG@@mingw-w64-x86_64-python-requests-toolbelt
PKG@@mingw-w64-x86_64-python-requests-toolbelt: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-requests) ; @echo 'PKG@@mingw-w64-x86_64-python-requests-toolbelt'

override VERSION_OF_mingw-w64-x86_64-python-requests-unixsocket := 0.4.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-requests-unixsocket
PKG@@mingw-w64-x86_64-python-requests-unixsocket: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-requests PKG@@mingw-w64-x86_64-python-urllib3) ; @echo 'PKG@@mingw-w64-x86_64-python-requests-unixsocket'

override VERSION_OF_mingw-w64-x86_64-python-resolvelib := 1.2.1-3
.PHONY: PKG@@mingw-w64-x86_64-python-resolvelib
PKG@@mingw-w64-x86_64-python-resolvelib: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-resolvelib'

override VERSION_OF_mingw-w64-x86_64-python-responses := 0.26.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-responses
PKG@@mingw-w64-x86_64-python-responses: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-requests PKG@@mingw-w64-x86_64-python-urllib3 PKG@@mingw-w64-x86_64-python-yaml) ; @echo 'PKG@@mingw-w64-x86_64-python-responses'

override VERSION_OF_mingw-w64-x86_64-python-retrying := 1.4.2-3
.PHONY: PKG@@mingw-w64-x86_64-python-retrying
PKG@@mingw-w64-x86_64-python-retrying: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-six) ; @echo 'PKG@@mingw-w64-x86_64-python-retrying'

override VERSION_OF_mingw-w64-x86_64-python-rfc3986 := 2.0.0-8
.PHONY: PKG@@mingw-w64-x86_64-python-rfc3986
PKG@@mingw-w64-x86_64-python-rfc3986: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-rfc3986'

override VERSION_OF_mingw-w64-x86_64-python-rfc3987 := 1.3.8-8
.PHONY: PKG@@mingw-w64-x86_64-python-rfc3987
PKG@@mingw-w64-x86_64-python-rfc3987: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-rfc3987'

override VERSION_OF_mingw-w64-x86_64-python-rich := 15.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-rich
PKG@@mingw-w64-x86_64-python-rich: $(if $(__deps),PKG@@mingw-w64-x86_64-python-markdown-it-py PKG@@mingw-w64-x86_64-python-pygments) ; @echo 'PKG@@mingw-w64-x86_64-python-rich'

override VERSION_OF_mingw-w64-x86_64-python-rich-argparse := 1.8.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-rich-argparse
PKG@@mingw-w64-x86_64-python-rich-argparse: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-rich) ; @echo 'PKG@@mingw-w64-x86_64-python-rich-argparse'

override VERSION_OF_mingw-w64-x86_64-python-roman-numerals := 4.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-roman-numerals PKG@@mingw-w64-x86_64-python-roman-numerals-py
PKG@@mingw-w64-x86_64-python-roman-numerals PKG@@mingw-w64-x86_64-python-roman-numerals-py: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-roman-numerals'

override VERSION_OF_mingw-w64-x86_64-python-ruamel-yaml := 0.19.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-ruamel-yaml
PKG@@mingw-w64-x86_64-python-ruamel-yaml: $(if $(__deps),PKG@@mingw-w64-x86_64-python-ruamel.yaml.clib) ; @echo 'PKG@@mingw-w64-x86_64-python-ruamel-yaml'

override VERSION_OF_mingw-w64-x86_64-python-ruamel.yaml.clib := 0.2.15-3
.PHONY: PKG@@mingw-w64-x86_64-python-ruamel.yaml.clib
PKG@@mingw-w64-x86_64-python-ruamel.yaml.clib: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-ruamel.yaml.clib'

override VERSION_OF_mingw-w64-x86_64-python-s2geometry := 0.14.0-2
.PHONY: PKG@@mingw-w64-x86_64-python-s2geometry
PKG@@mingw-w64-x86_64-python-s2geometry: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-s2geometry) ; @echo 'PKG@@mingw-w64-x86_64-python-s2geometry'

override VERSION_OF_mingw-w64-x86_64-python-scikit-build-core := 1.1.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-scikit-build-core
PKG@@mingw-w64-x86_64-python-scikit-build-core: $(if $(__deps),PKG@@mingw-w64-x86_64-cmake PKG@@mingw-w64-x86_64-ninja PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-pathspec) ; @echo 'PKG@@mingw-w64-x86_64-python-scikit-build-core'

override VERSION_OF_mingw-w64-x86_64-python-scipy := 1.18.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-scipy
PKG@@mingw-w64-x86_64-python-scipy: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-fc-libs PKG@@mingw-w64-x86_64-openblas PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-numpy) ; @echo 'PKG@@mingw-w64-x86_64-python-scipy'

override VERSION_OF_mingw-w64-x86_64-python-semantic-version := 2.10.0-6
.PHONY: PKG@@mingw-w64-x86_64-python-semantic-version
PKG@@mingw-w64-x86_64-python-semantic-version: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-semantic-version'

override VERSION_OF_mingw-w64-x86_64-python-setuptools := 84.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-setuptools
PKG@@mingw-w64-x86_64-python-setuptools: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-setuptools'

override VERSION_OF_mingw-w64-x86_64-python-setuptools-rust := 1.13.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-setuptools-rust
PKG@@mingw-w64-x86_64-python-setuptools-rust: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-semantic-version PKG@@mingw-w64-x86_64-python-setuptools PKG@@mingw-w64-x86_64-rust) ; @echo 'PKG@@mingw-w64-x86_64-python-setuptools-rust'

override VERSION_OF_mingw-w64-x86_64-python-setuptools-scm := 10.3.4-1
.PHONY: PKG@@mingw-w64-x86_64-python-setuptools-scm
PKG@@mingw-w64-x86_64-python-setuptools-scm: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-setuptools PKG@@mingw-w64-x86_64-python-vcs-versioning) ; @echo 'PKG@@mingw-w64-x86_64-python-setuptools-scm'

override VERSION_OF_mingw-w64-x86_64-python-setuptools-scm-git-archive := 1.4.1-4
.PHONY: PKG@@mingw-w64-x86_64-python-setuptools-scm-git-archive
PKG@@mingw-w64-x86_64-python-setuptools-scm-git-archive: $(if $(__deps),PKG@@mingw-w64-x86_64-python-setuptools-scm) ; @echo 'PKG@@mingw-w64-x86_64-python-setuptools-scm-git-archive'

override VERSION_OF_mingw-w64-x86_64-python-shellingham := 1.5.4-4
.PHONY: PKG@@mingw-w64-x86_64-python-shellingham
PKG@@mingw-w64-x86_64-python-shellingham: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-shellingham'

override VERSION_OF_mingw-w64-x86_64-python-six := 1.17.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-six
PKG@@mingw-w64-x86_64-python-six: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-six'

override VERSION_OF_mingw-w64-x86_64-python-smartypants := 2.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-python-smartypants
PKG@@mingw-w64-x86_64-python-smartypants: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-smartypants'

override VERSION_OF_mingw-w64-x86_64-python-sniffio := 1.3.1-4
.PHONY: PKG@@mingw-w64-x86_64-python-sniffio
PKG@@mingw-w64-x86_64-python-sniffio: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-sniffio'

override VERSION_OF_mingw-w64-x86_64-python-snowballstemmer := 3.1.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-snowballstemmer
PKG@@mingw-w64-x86_64-python-snowballstemmer: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-snowballstemmer'

override VERSION_OF_mingw-w64-x86_64-python-sortedcontainers := 2.4.0-8
.PHONY: PKG@@mingw-w64-x86_64-python-sortedcontainers
PKG@@mingw-w64-x86_64-python-sortedcontainers: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-sortedcontainers'

override VERSION_OF_mingw-w64-x86_64-python-soupsieve := 4.0.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-soupsieve
PKG@@mingw-w64-x86_64-python-soupsieve: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-soupsieve'

override VERSION_OF_mingw-w64-x86_64-python-sphinx := 9.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-sphinx
PKG@@mingw-w64-x86_64-python-sphinx: $(if $(__deps),PKG@@mingw-w64-x86_64-python-babel PKG@@mingw-w64-x86_64-python-colorama PKG@@mingw-w64-x86_64-python-docutils PKG@@mingw-w64-x86_64-python-imagesize PKG@@mingw-w64-x86_64-python-jinja PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-pygments PKG@@mingw-w64-x86_64-python-requests PKG@@mingw-w64-x86_64-python-roman-numerals PKG@@mingw-w64-x86_64-python-snowballstemmer PKG@@mingw-w64-x86_64-python-sphinx-alabaster-theme PKG@@mingw-w64-x86_64-python-sphinxcontrib-applehelp PKG@@mingw-w64-x86_64-python-sphinxcontrib-devhelp PKG@@mingw-w64-x86_64-python-sphinxcontrib-htmlhelp PKG@@mingw-w64-x86_64-python-sphinxcontrib-jsmath PKG@@mingw-w64-x86_64-python-sphinxcontrib-qthelp PKG@@mingw-w64-x86_64-python-sphinxcontrib-serializinghtml) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinx'

override VERSION_OF_mingw-w64-x86_64-python-sphinx-alabaster-theme := 1.0.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-sphinx-alabaster-theme
PKG@@mingw-w64-x86_64-python-sphinx-alabaster-theme: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinx-alabaster-theme'

override VERSION_OF_mingw-w64-x86_64-python-sphinx-copybutton := 0.5.2-6
.PHONY: PKG@@mingw-w64-x86_64-python-sphinx-copybutton
PKG@@mingw-w64-x86_64-python-sphinx-copybutton: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-sphinx) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinx-copybutton'

override VERSION_OF_mingw-w64-x86_64-python-sphinx-issues := 6.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-sphinx-issues
PKG@@mingw-w64-x86_64-python-sphinx-issues: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-sphinx) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinx-issues'

override VERSION_OF_mingw-w64-x86_64-python-sphinx-notfound-page := 1.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-sphinx-notfound-page
PKG@@mingw-w64-x86_64-python-sphinx-notfound-page: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-docutils PKG@@mingw-w64-x86_64-python-sphinx) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinx-notfound-page'

override VERSION_OF_mingw-w64-x86_64-python-sphinx-tabs := 3.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-sphinx-tabs
PKG@@mingw-w64-x86_64-python-sphinx-tabs: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-docutils PKG@@mingw-w64-x86_64-python-pygments PKG@@mingw-w64-x86_64-python-sphinx) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinx-tabs'

override VERSION_OF_mingw-w64-x86_64-python-sphinx_rtd_theme := 3.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-sphinx_rtd_theme
PKG@@mingw-w64-x86_64-python-sphinx_rtd_theme: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-docutils PKG@@mingw-w64-x86_64-python-sphinx PKG@@mingw-w64-x86_64-python-sphinxcontrib-jquery) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinx_rtd_theme'

override VERSION_OF_mingw-w64-x86_64-python-sphinxcontrib-applehelp := 2.0.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-sphinxcontrib-applehelp
PKG@@mingw-w64-x86_64-python-sphinxcontrib-applehelp: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinxcontrib-applehelp'

override VERSION_OF_mingw-w64-x86_64-python-sphinxcontrib-devhelp := 2.0.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-sphinxcontrib-devhelp
PKG@@mingw-w64-x86_64-python-sphinxcontrib-devhelp: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinxcontrib-devhelp'

override VERSION_OF_mingw-w64-x86_64-python-sphinxcontrib-github-alt := 1.2-7
.PHONY: PKG@@mingw-w64-x86_64-python-sphinxcontrib-github-alt
PKG@@mingw-w64-x86_64-python-sphinxcontrib-github-alt: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-docutils PKG@@mingw-w64-x86_64-python-sphinx) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinxcontrib-github-alt'

override VERSION_OF_mingw-w64-x86_64-python-sphinxcontrib-htmlhelp := 2.1.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-sphinxcontrib-htmlhelp
PKG@@mingw-w64-x86_64-python-sphinxcontrib-htmlhelp: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinxcontrib-htmlhelp'

override VERSION_OF_mingw-w64-x86_64-python-sphinxcontrib-jquery := 4.1-5
.PHONY: PKG@@mingw-w64-x86_64-python-sphinxcontrib-jquery
PKG@@mingw-w64-x86_64-python-sphinxcontrib-jquery: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-sphinx) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinxcontrib-jquery'

override VERSION_OF_mingw-w64-x86_64-python-sphinxcontrib-jsmath := 2.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-sphinxcontrib-jsmath
PKG@@mingw-w64-x86_64-python-sphinxcontrib-jsmath: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinxcontrib-jsmath'

override VERSION_OF_mingw-w64-x86_64-python-sphinxcontrib-log-cabinet := 1.0.1-6
.PHONY: PKG@@mingw-w64-x86_64-python-sphinxcontrib-log-cabinet
PKG@@mingw-w64-x86_64-python-sphinxcontrib-log-cabinet: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-sphinx) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinxcontrib-log-cabinet'

override VERSION_OF_mingw-w64-x86_64-python-sphinxcontrib-moderncmakedomain := 4.4.3-1
.PHONY: PKG@@mingw-w64-x86_64-python-sphinxcontrib-moderncmakedomain
PKG@@mingw-w64-x86_64-python-sphinxcontrib-moderncmakedomain: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-sphinx) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinxcontrib-moderncmakedomain'

override VERSION_OF_mingw-w64-x86_64-python-sphinxcontrib-qthelp := 2.0.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-sphinxcontrib-qthelp
PKG@@mingw-w64-x86_64-python-sphinxcontrib-qthelp: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinxcontrib-qthelp'

override VERSION_OF_mingw-w64-x86_64-python-sphinxcontrib-serializinghtml := 2.0.0-4
.PHONY: PKG@@mingw-w64-x86_64-python-sphinxcontrib-serializinghtml
PKG@@mingw-w64-x86_64-python-sphinxcontrib-serializinghtml: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinxcontrib-serializinghtml'

override VERSION_OF_mingw-w64-x86_64-python-sphinxcontrib-svg2pdfconverter := 2.1.0-2
.PHONY: PKG@@mingw-w64-x86_64-python-sphinxcontrib-svg2pdfconverter
PKG@@mingw-w64-x86_64-python-sphinxcontrib-svg2pdfconverter: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-sphinx) ; @echo 'PKG@@mingw-w64-x86_64-python-sphinxcontrib-svg2pdfconverter'

override VERSION_OF_mingw-w64-x86_64-python-spinners := 0.0.24-7
.PHONY: PKG@@mingw-w64-x86_64-python-spinners
PKG@@mingw-w64-x86_64-python-spinners: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-spinners'

override VERSION_OF_mingw-w64-x86_64-python-sqlparse := 0.6.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-sqlparse
PKG@@mingw-w64-x86_64-python-sqlparse: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-sqlparse'

override VERSION_OF_mingw-w64-x86_64-python-standard-aifc := 3.13.0-2
.PHONY: PKG@@mingw-w64-x86_64-python-standard-aifc
PKG@@mingw-w64-x86_64-python-standard-aifc: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-audioop-lts PKG@@mingw-w64-x86_64-python-standard-chunk) ; @echo 'PKG@@mingw-w64-x86_64-python-standard-aifc'

override VERSION_OF_mingw-w64-x86_64-python-standard-chunk := 3.13.0-2
.PHONY: PKG@@mingw-w64-x86_64-python-standard-chunk
PKG@@mingw-w64-x86_64-python-standard-chunk: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-standard-chunk'

override VERSION_OF_mingw-w64-x86_64-python-standard-sunau := 3.13.0-2
.PHONY: PKG@@mingw-w64-x86_64-python-standard-sunau
PKG@@mingw-w64-x86_64-python-standard-sunau: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-audioop-lts) ; @echo 'PKG@@mingw-w64-x86_64-python-standard-sunau'

override VERSION_OF_mingw-w64-x86_64-python-stestr := 4.2.1-2
.PHONY: PKG@@mingw-w64-x86_64-python-stestr
PKG@@mingw-w64-x86_64-python-stestr: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-cliff PKG@@mingw-w64-x86_64-python-fixtures PKG@@mingw-w64-x86_64-python-pbr PKG@@mingw-w64-x86_64-python-subunit PKG@@mingw-w64-x86_64-python-testtools PKG@@mingw-w64-x86_64-python-tomlkit PKG@@mingw-w64-x86_64-python-voluptuous PKG@@mingw-w64-x86_64-python-yaml) ; @echo 'PKG@@mingw-w64-x86_64-python-stestr'

override VERSION_OF_mingw-w64-x86_64-python-stevedore := 5.9.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-stevedore
PKG@@mingw-w64-x86_64-python-stevedore: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-stevedore'

override VERSION_OF_mingw-w64-x86_64-python-subunit := 1.4.6-1
.PHONY: PKG@@mingw-w64-x86_64-python-subunit
PKG@@mingw-w64-x86_64-python-subunit: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-iso8601 PKG@@mingw-w64-x86_64-python-pyaml PKG@@mingw-w64-x86_64-python-testtools) ; @echo 'PKG@@mingw-w64-x86_64-python-subunit'

override VERSION_OF_mingw-w64-x86_64-python-termcolor := 3.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-termcolor
PKG@@mingw-w64-x86_64-python-termcolor: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-termcolor'

override VERSION_OF_mingw-w64-x86_64-python-testpath := 0.6.0-5
.PHONY: PKG@@mingw-w64-x86_64-python-testpath
PKG@@mingw-w64-x86_64-python-testpath: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-testpath'

override VERSION_OF_mingw-w64-x86_64-python-testrepository := 0.0.22-1
.PHONY: PKG@@mingw-w64-x86_64-python-testrepository
PKG@@mingw-w64-x86_64-python-testrepository: $(if $(__deps),PKG@@mingw-w64-x86_64-python-fixtures PKG@@mingw-w64-x86_64-python-subunit PKG@@mingw-w64-x86_64-python-testtools) ; @echo 'PKG@@mingw-w64-x86_64-python-testrepository'

override VERSION_OF_mingw-w64-x86_64-python-testresources := 2.1.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-testresources
PKG@@mingw-w64-x86_64-python-testresources: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-testresources'

override VERSION_OF_mingw-w64-x86_64-python-testscenarios := 0.7.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-testscenarios
PKG@@mingw-w64-x86_64-python-testscenarios: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-testtools) ; @echo 'PKG@@mingw-w64-x86_64-python-testscenarios'

override VERSION_OF_mingw-w64-x86_64-python-testtools := 2.9.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-testtools
PKG@@mingw-w64-x86_64-python-testtools: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-testtools'

override VERSION_OF_mingw-w64-x86_64-python-toml := 0.10.2-8
.PHONY: PKG@@mingw-w64-x86_64-python-toml
PKG@@mingw-w64-x86_64-python-toml: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-toml'

override VERSION_OF_mingw-w64-x86_64-python-tomli := 2.5.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-tomli
PKG@@mingw-w64-x86_64-python-tomli: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-tomli'

override VERSION_OF_mingw-w64-x86_64-python-tomli-w := 1.2.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-tomli-w
PKG@@mingw-w64-x86_64-python-tomli-w: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-tomli-w'

override VERSION_OF_mingw-w64-x86_64-python-tomlkit := 0.15.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-tomlkit
PKG@@mingw-w64-x86_64-python-tomlkit: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-tomlkit'

override VERSION_OF_mingw-w64-x86_64-python-toposort := 1.10-5
.PHONY: PKG@@mingw-w64-x86_64-python-toposort
PKG@@mingw-w64-x86_64-python-toposort: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-toposort'

override VERSION_OF_mingw-w64-x86_64-python-tornado := 6.6.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-tornado
PKG@@mingw-w64-x86_64-python-tornado: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-tornado'

override VERSION_OF_mingw-w64-x86_64-python-tox := 4.67.13-1
.PHONY: PKG@@mingw-w64-x86_64-python-tox
PKG@@mingw-w64-x86_64-python-tox: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-cachetools PKG@@mingw-w64-x86_64-python-colorama PKG@@mingw-w64-x86_64-python-filelock PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-platformdirs PKG@@mingw-w64-x86_64-python-pluggy PKG@@mingw-w64-x86_64-python-pyproject-api PKG@@mingw-w64-x86_64-python-python-discovery PKG@@mingw-w64-x86_64-python-tomli-w PKG@@mingw-w64-x86_64-python-virtualenv) ; @echo 'PKG@@mingw-w64-x86_64-python-tox'

override VERSION_OF_mingw-w64-x86_64-python-tqdm := 4.70.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-tqdm
PKG@@mingw-w64-x86_64-python-tqdm: $(if $(__deps),PKG@@mingw-w64-x86_64-python-colorama) ; @echo 'PKG@@mingw-w64-x86_64-python-tqdm'

override VERSION_OF_mingw-w64-x86_64-python-trio := 0.35.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-trio
PKG@@mingw-w64-x86_64-python-trio: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-attrs PKG@@mingw-w64-x86_64-python-cffi PKG@@mingw-w64-x86_64-python-idna PKG@@mingw-w64-x86_64-python-outcome PKG@@mingw-w64-x86_64-python-sniffio PKG@@mingw-w64-x86_64-python-sortedcontainers) ; @echo 'PKG@@mingw-w64-x86_64-python-trio'

override VERSION_OF_mingw-w64-x86_64-python-trove-classifiers := 2026.9.21.13-1
.PHONY: PKG@@mingw-w64-x86_64-python-trove-classifiers
PKG@@mingw-w64-x86_64-python-trove-classifiers: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-trove-classifiers'

override VERSION_OF_mingw-w64-x86_64-python-twisted := 26.10.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-twisted
PKG@@mingw-w64-x86_64-python-twisted: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-attrs PKG@@mingw-w64-x86_64-python-automat PKG@@mingw-w64-x86_64-python-constantly PKG@@mingw-w64-x86_64-python-hyperlink PKG@@mingw-w64-x86_64-python-incremental PKG@@mingw-w64-x86_64-python-typing_extensions PKG@@mingw-w64-x86_64-python-zope.interface) ; @echo 'PKG@@mingw-w64-x86_64-python-twisted'

override VERSION_OF_mingw-w64-x86_64-python-typing_extensions := 4.16.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-typing_extensions
PKG@@mingw-w64-x86_64-python-typing_extensions: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-typing_extensions'

override VERSION_OF_mingw-w64-x86_64-python-typogrify := 2.1.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-typogrify
PKG@@mingw-w64-x86_64-python-typogrify: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-smartypants) ; @echo 'PKG@@mingw-w64-x86_64-python-typogrify'

override VERSION_OF_mingw-w64-x86_64-python-tzdata := 2026.6-1
.PHONY: PKG@@mingw-w64-x86_64-python-tzdata
PKG@@mingw-w64-x86_64-python-tzdata: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-tzdata'

override VERSION_OF_mingw-w64-x86_64-python-ufolib2 := 0.19.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-ufolib2
PKG@@mingw-w64-x86_64-python-ufolib2: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-attrs PKG@@mingw-w64-x86_64-python-cattrs PKG@@mingw-w64-x86_64-python-fonttools PKG@@mingw-w64-x86_64-python-lxml PKG@@mingw-w64-x86_64-python-pyfilesystem2) ; @echo 'PKG@@mingw-w64-x86_64-python-ufolib2'

override VERSION_OF_mingw-w64-x86_64-python-ukpostcodeparser := 1.1.2-8
.PHONY: PKG@@mingw-w64-x86_64-python-ukpostcodeparser
PKG@@mingw-w64-x86_64-python-ukpostcodeparser: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-ukpostcodeparser'

override VERSION_OF_mingw-w64-x86_64-python-unidecode := 1.4.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-unidecode
PKG@@mingw-w64-x86_64-python-unidecode: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-unidecode'

override VERSION_OF_mingw-w64-x86_64-python-url-normalize := 3.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-url-normalize
PKG@@mingw-w64-x86_64-python-url-normalize: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-idna) ; @echo 'PKG@@mingw-w64-x86_64-python-url-normalize'

override VERSION_OF_mingw-w64-x86_64-python-urllib3 := 2.8.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-urllib3
PKG@@mingw-w64-x86_64-python-urllib3: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-urllib3'

override VERSION_OF_mingw-w64-x86_64-python-uv := 0.13.7-2
.PHONY: PKG@@mingw-w64-x86_64-python-uv
PKG@@mingw-w64-x86_64-python-uv: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-uv) ; @echo 'PKG@@mingw-w64-x86_64-python-uv'

override VERSION_OF_mingw-w64-x86_64-python-uv-build := 0.13.7-2
.PHONY: PKG@@mingw-w64-x86_64-python-uv-build
PKG@@mingw-w64-x86_64-python-uv-build: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-python-uv-build'

override VERSION_OF_mingw-w64-x86_64-python-vcs-versioning := 2.6.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-vcs-versioning
PKG@@mingw-w64-x86_64-python-vcs-versioning: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging) ; @echo 'PKG@@mingw-w64-x86_64-python-vcs-versioning'

override VERSION_OF_mingw-w64-x86_64-python-versioneer := 0.29-6
.PHONY: PKG@@mingw-w64-x86_64-python-versioneer
PKG@@mingw-w64-x86_64-python-versioneer: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-versioneer'

override VERSION_OF_mingw-w64-x86_64-python-virtualenv := 21.15.6-1
.PHONY: PKG@@mingw-w64-x86_64-python-virtualenv
PKG@@mingw-w64-x86_64-python-virtualenv: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-distlib PKG@@mingw-w64-x86_64-python-filelock PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-platformdirs PKG@@mingw-w64-x86_64-python-python-discovery) ; @echo 'PKG@@mingw-w64-x86_64-python-virtualenv'

override VERSION_OF_mingw-w64-x86_64-python-virtualenv-clone := 0.5.7-7
.PHONY: PKG@@mingw-w64-x86_64-python-virtualenv-clone
PKG@@mingw-w64-x86_64-python-virtualenv-clone: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-virtualenv) ; @echo 'PKG@@mingw-w64-x86_64-python-virtualenv-clone'

override VERSION_OF_mingw-w64-x86_64-python-voluptuous := 0.16.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-voluptuous
PKG@@mingw-w64-x86_64-python-voluptuous: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-voluptuous'

override VERSION_OF_mingw-w64-x86_64-python-watchfiles := 1.3.0-2
.PHONY: PKG@@mingw-w64-x86_64-python-watchfiles
PKG@@mingw-w64-x86_64-python-watchfiles: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-anyio) ; @echo 'PKG@@mingw-w64-x86_64-python-watchfiles'

override VERSION_OF_mingw-w64-x86_64-python-wcag-contrast-ratio := 0.9-7
.PHONY: PKG@@mingw-w64-x86_64-python-wcag-contrast-ratio
PKG@@mingw-w64-x86_64-python-wcag-contrast-ratio: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-wcag-contrast-ratio'

override VERSION_OF_mingw-w64-x86_64-python-wcwidth := 0.9.4-1
.PHONY: PKG@@mingw-w64-x86_64-python-wcwidth
PKG@@mingw-w64-x86_64-python-wcwidth: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-wcwidth'

override VERSION_OF_mingw-w64-x86_64-python-webencodings := 0.6.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-webencodings
PKG@@mingw-w64-x86_64-python-webencodings: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-webencodings'

override VERSION_OF_mingw-w64-x86_64-python-werkzeug := 3.1.9-1
.PHONY: PKG@@mingw-w64-x86_64-python-werkzeug
PKG@@mingw-w64-x86_64-python-werkzeug: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-markupsafe) ; @echo 'PKG@@mingw-w64-x86_64-python-werkzeug'

override VERSION_OF_mingw-w64-x86_64-python-wheel := 0.48.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-wheel
PKG@@mingw-w64-x86_64-python-wheel: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging) ; @echo 'PKG@@mingw-w64-x86_64-python-wheel'

override VERSION_OF_mingw-w64-x86_64-python-wmi := 1.5.1-5
.PHONY: PKG@@mingw-w64-x86_64-python-wmi
PKG@@mingw-w64-x86_64-python-wmi: $(if $(__deps),PKG@@mingw-w64-x86_64-python-pywin32) ; @echo 'PKG@@mingw-w64-x86_64-python-wmi'

override VERSION_OF_mingw-w64-x86_64-python-wsproto := 1.3.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-wsproto
PKG@@mingw-w64-x86_64-python-wsproto: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-h11) ; @echo 'PKG@@mingw-w64-x86_64-python-wsproto'

override VERSION_OF_mingw-w64-x86_64-python-wsproto-docs := 1.3.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-wsproto-docs
PKG@@mingw-w64-x86_64-python-wsproto-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-python-wsproto-docs'

override VERSION_OF_mingw-w64-x86_64-python-wtforms := 3.3.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-wtforms
PKG@@mingw-w64-x86_64-python-wtforms: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-markupsafe) ; @echo 'PKG@@mingw-w64-x86_64-python-wtforms'

override VERSION_OF_mingw-w64-x86_64-python-wtforms-docs := 3.3.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-wtforms-docs
PKG@@mingw-w64-x86_64-python-wtforms-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-python-wtforms-docs'

override VERSION_OF_mingw-w64-x86_64-python-xlsxwriter := 3.3.1-1
.PHONY: PKG@@mingw-w64-x86_64-python-xlsxwriter
PKG@@mingw-w64-x86_64-python-xlsxwriter: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-xlsxwriter'

override VERSION_OF_mingw-w64-x86_64-python-xmlschema := 4.3.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-xmlschema
PKG@@mingw-w64-x86_64-python-xmlschema: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-elementpath) ; @echo 'PKG@@mingw-w64-x86_64-python-xmlschema'

override VERSION_OF_mingw-w64-x86_64-python-yaml := 6.0.3-3
.PHONY: PKG@@mingw-w64-x86_64-python-yaml
PKG@@mingw-w64-x86_64-python-yaml: $(if $(__deps),PKG@@mingw-w64-x86_64-libyaml PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-yaml'

override VERSION_OF_mingw-w64-x86_64-python-zipp := 4.1.2-1
.PHONY: PKG@@mingw-w64-x86_64-python-zipp
PKG@@mingw-w64-x86_64-python-zipp: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-zipp'

override VERSION_OF_mingw-w64-x86_64-python-zipremove := 0.10.0-1
.PHONY: PKG@@mingw-w64-x86_64-python-zipremove
PKG@@mingw-w64-x86_64-python-zipremove: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-zipremove'

override VERSION_OF_mingw-w64-x86_64-python-zope.interface := 8.7-1
.PHONY: PKG@@mingw-w64-x86_64-python-zope.interface
PKG@@mingw-w64-x86_64-python-zope.interface: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-zope.interface'

override VERSION_OF_mingw-w64-x86_64-python-zstandard := 0.25.0-3
.PHONY: PKG@@mingw-w64-x86_64-python-zstandard
PKG@@mingw-w64-x86_64-python-zstandard: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-python-zstandard'

override VERSION_OF_mingw-w64-x86_64-qbs := 3.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-qbs
PKG@@mingw-w64-x86_64-qbs: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-qt6-5compat PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qbs'

override VERSION_OF_mingw-w64-x86_64-qdia := 0.66-1
.PHONY: PKG@@mingw-w64-x86_64-qdia
PKG@@mingw-w64-x86_64-qdia: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-svg) ; @echo 'PKG@@mingw-w64-x86_64-qdia'

override VERSION_OF_mingw-w64-x86_64-qhull := 2020.2-4
.PHONY: PKG@@mingw-w64-x86_64-qhull
PKG@@mingw-w64-x86_64-qhull: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-qhull'

override VERSION_OF_mingw-w64-x86_64-qrencode := 4.1.1-4
.PHONY: PKG@@mingw-w64-x86_64-qrencode
PKG@@mingw-w64-x86_64-qrencode: $(if $(__deps),PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-qrencode'

override VERSION_OF_mingw-w64-x86_64-qt5-3d := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-3d
PKG@@mingw-w64-x86_64-qt5-3d: $(if $(__deps),PKG@@mingw-w64-x86_64-assimp PKG@@mingw-w64-x86_64-qt5-declarative PKG@@mingw-w64-x86_64-qt5-gamepad) ; @echo 'PKG@@mingw-w64-x86_64-qt5-3d'

override VERSION_OF_mingw-w64-x86_64-qt5-3d-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-3d-debug
PKG@@mingw-w64-x86_64-qt5-3d-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-3d PKG@@mingw-w64-x86_64-qt5-declarative-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt5-3d-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-activeqt := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-activeqt
PKG@@mingw-w64-x86_64-qt5-activeqt: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-activeqt'

override VERSION_OF_mingw-w64-x86_64-qt5-activeqt-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-activeqt-debug
PKG@@mingw-w64-x86_64-qt5-activeqt-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-activeqt PKG@@mingw-w64-x86_64-qt5-base-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt5-activeqt-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-base := 5.15.19+kde+r96-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-base
PKG@@mingw-w64-x86_64-qt5-base: $(if $(__deps),PKG@@mingw-w64-x86_64-dbus PKG@@mingw-w64-x86_64-double-conversion PKG@@mingw-w64-x86_64-fontconfig PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-md4c PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-pcre2 PKG@@mingw-w64-x86_64-sqlite3 PKG@@mingw-w64-x86_64-vulkan PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-qt5-base'

override VERSION_OF_mingw-w64-x86_64-qt5-base-debug := 5.15.19+kde+r96-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-base-debug
PKG@@mingw-w64-x86_64-qt5-base-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-base-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-charts := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-charts
PKG@@mingw-w64-x86_64-qt5-charts: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-charts'

override VERSION_OF_mingw-w64-x86_64-qt5-charts-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-charts-debug
PKG@@mingw-w64-x86_64-qt5-charts-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-charts) ; @echo 'PKG@@mingw-w64-x86_64-qt5-charts-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-connectivity := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-connectivity
PKG@@mingw-w64-x86_64-qt5-connectivity: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-connectivity'

override VERSION_OF_mingw-w64-x86_64-qt5-connectivity-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-connectivity-debug
PKG@@mingw-w64-x86_64-qt5-connectivity-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-connectivity) ; @echo 'PKG@@mingw-w64-x86_64-qt5-connectivity-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-datavis3d := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-datavis3d
PKG@@mingw-w64-x86_64-qt5-datavis3d: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-datavis3d'

override VERSION_OF_mingw-w64-x86_64-qt5-datavis3d-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-datavis3d-debug
PKG@@mingw-w64-x86_64-qt5-datavis3d-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-datavis3d) ; @echo 'PKG@@mingw-w64-x86_64-qt5-datavis3d-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-declarative := 5.15.19+kde+r23-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-declarative
PKG@@mingw-w64-x86_64-qt5-declarative: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-declarative'

override VERSION_OF_mingw-w64-x86_64-qt5-declarative-debug := 5.15.19+kde+r23-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-declarative-debug
PKG@@mingw-w64-x86_64-qt5-declarative-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt5-declarative-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-gamepad := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-gamepad
PKG@@mingw-w64-x86_64-qt5-gamepad: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-gamepad'

override VERSION_OF_mingw-w64-x86_64-qt5-gamepad-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-gamepad-debug
PKG@@mingw-w64-x86_64-qt5-gamepad-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-gamepad) ; @echo 'PKG@@mingw-w64-x86_64-qt5-gamepad-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-graphicaleffects := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-graphicaleffects
PKG@@mingw-w64-x86_64-qt5-graphicaleffects: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt5-graphicaleffects'

override VERSION_OF_mingw-w64-x86_64-qt5-graphicaleffects-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-graphicaleffects-debug
PKG@@mingw-w64-x86_64-qt5-graphicaleffects-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative-debug PKG@@mingw-w64-x86_64-qt5-graphicaleffects) ; @echo 'PKG@@mingw-w64-x86_64-qt5-graphicaleffects-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-imageformats := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-imageformats
PKG@@mingw-w64-x86_64-qt5-imageformats: $(if $(__deps),PKG@@mingw-w64-x86_64-jasper PKG@@mingw-w64-x86_64-libmng PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-imageformats'

override VERSION_OF_mingw-w64-x86_64-qt5-imageformats-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-imageformats-debug
PKG@@mingw-w64-x86_64-qt5-imageformats-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-imageformats) ; @echo 'PKG@@mingw-w64-x86_64-qt5-imageformats-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-location := 5.15.19+kde+r7-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-location
PKG@@mingw-w64-x86_64-qt5-location: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt5-location'

override VERSION_OF_mingw-w64-x86_64-qt5-location-debug := 5.15.19+kde+r7-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-location-debug
PKG@@mingw-w64-x86_64-qt5-location-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative-debug PKG@@mingw-w64-x86_64-qt5-location) ; @echo 'PKG@@mingw-w64-x86_64-qt5-location-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-lottie := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-lottie
PKG@@mingw-w64-x86_64-qt5-lottie: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt5-lottie'

override VERSION_OF_mingw-w64-x86_64-qt5-lottie-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-lottie-debug
PKG@@mingw-w64-x86_64-qt5-lottie-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative-debug PKG@@mingw-w64-x86_64-qt5-lottie) ; @echo 'PKG@@mingw-w64-x86_64-qt5-lottie-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-multimedia := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-multimedia
PKG@@mingw-w64-x86_64-qt5-multimedia: $(if $(__deps),PKG@@mingw-w64-x86_64-openal PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-multimedia'

override VERSION_OF_mingw-w64-x86_64-qt5-multimedia-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-multimedia-debug
PKG@@mingw-w64-x86_64-qt5-multimedia-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-multimedia) ; @echo 'PKG@@mingw-w64-x86_64-qt5-multimedia-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-networkauth := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-networkauth
PKG@@mingw-w64-x86_64-qt5-networkauth: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-networkauth'

override VERSION_OF_mingw-w64-x86_64-qt5-networkauth-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-networkauth-debug
PKG@@mingw-w64-x86_64-qt5-networkauth-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-networkauth) ; @echo 'PKG@@mingw-w64-x86_64-qt5-networkauth-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-purchasing := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-purchasing
PKG@@mingw-w64-x86_64-qt5-purchasing: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-purchasing'

override VERSION_OF_mingw-w64-x86_64-qt5-purchasing-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-purchasing-debug
PKG@@mingw-w64-x86_64-qt5-purchasing-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-purchasing) ; @echo 'PKG@@mingw-w64-x86_64-qt5-purchasing-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-quick3d := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-quick3d
PKG@@mingw-w64-x86_64-qt5-quick3d: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt5-quick3d'

override VERSION_OF_mingw-w64-x86_64-qt5-quick3d-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-quick3d-debug
PKG@@mingw-w64-x86_64-qt5-quick3d-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative-debug PKG@@mingw-w64-x86_64-qt5-quick3d) ; @echo 'PKG@@mingw-w64-x86_64-qt5-quick3d-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-quickcontrols := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-quickcontrols
PKG@@mingw-w64-x86_64-qt5-quickcontrols: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt5-quickcontrols'

override VERSION_OF_mingw-w64-x86_64-qt5-quickcontrols-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-quickcontrols-debug
PKG@@mingw-w64-x86_64-qt5-quickcontrols-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative-debug PKG@@mingw-w64-x86_64-qt5-quickcontrols) ; @echo 'PKG@@mingw-w64-x86_64-qt5-quickcontrols-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-quickcontrols2 := 5.15.19+kde+r5-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-quickcontrols2
PKG@@mingw-w64-x86_64-qt5-quickcontrols2: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt5-quickcontrols2'

override VERSION_OF_mingw-w64-x86_64-qt5-quickcontrols2-debug := 5.15.19+kde+r5-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-quickcontrols2-debug
PKG@@mingw-w64-x86_64-qt5-quickcontrols2-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative-debug PKG@@mingw-w64-x86_64-qt5-quickcontrols2) ; @echo 'PKG@@mingw-w64-x86_64-qt5-quickcontrols2-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-quicktimeline := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-quicktimeline
PKG@@mingw-w64-x86_64-qt5-quicktimeline: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt5-quicktimeline'

override VERSION_OF_mingw-w64-x86_64-qt5-quicktimeline-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-quicktimeline-debug
PKG@@mingw-w64-x86_64-qt5-quicktimeline-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative-debug PKG@@mingw-w64-x86_64-qt5-quicktimeline) ; @echo 'PKG@@mingw-w64-x86_64-qt5-quicktimeline-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-remoteobjects := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-remoteobjects
PKG@@mingw-w64-x86_64-qt5-remoteobjects: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-remoteobjects'

override VERSION_OF_mingw-w64-x86_64-qt5-remoteobjects-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-remoteobjects-debug
PKG@@mingw-w64-x86_64-qt5-remoteobjects-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-remoteobjects) ; @echo 'PKG@@mingw-w64-x86_64-qt5-remoteobjects-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-script := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-script
PKG@@mingw-w64-x86_64-qt5-script: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-script'

override VERSION_OF_mingw-w64-x86_64-qt5-script-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-script-debug
PKG@@mingw-w64-x86_64-qt5-script-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-script) ; @echo 'PKG@@mingw-w64-x86_64-qt5-script-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-scxml := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-scxml
PKG@@mingw-w64-x86_64-qt5-scxml: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt5-scxml'

override VERSION_OF_mingw-w64-x86_64-qt5-scxml-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-scxml-debug
PKG@@mingw-w64-x86_64-qt5-scxml-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative-debug PKG@@mingw-w64-x86_64-qt5-scxml) ; @echo 'PKG@@mingw-w64-x86_64-qt5-scxml-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-sensors := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-sensors
PKG@@mingw-w64-x86_64-qt5-sensors: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-sensors'

override VERSION_OF_mingw-w64-x86_64-qt5-sensors-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-sensors-debug
PKG@@mingw-w64-x86_64-qt5-sensors-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-sensors) ; @echo 'PKG@@mingw-w64-x86_64-qt5-sensors-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-serialbus := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-serialbus
PKG@@mingw-w64-x86_64-qt5-serialbus: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-serialport) ; @echo 'PKG@@mingw-w64-x86_64-qt5-serialbus'

override VERSION_OF_mingw-w64-x86_64-qt5-serialbus-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-serialbus-debug
PKG@@mingw-w64-x86_64-qt5-serialbus-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-serialbus PKG@@mingw-w64-x86_64-qt5-serialport-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt5-serialbus-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-serialport := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-serialport
PKG@@mingw-w64-x86_64-qt5-serialport: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-serialport'

override VERSION_OF_mingw-w64-x86_64-qt5-serialport-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-serialport-debug
PKG@@mingw-w64-x86_64-qt5-serialport-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-serialport) ; @echo 'PKG@@mingw-w64-x86_64-qt5-serialport-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-speech := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-speech
PKG@@mingw-w64-x86_64-qt5-speech: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-speech'

override VERSION_OF_mingw-w64-x86_64-qt5-speech-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-speech-debug
PKG@@mingw-w64-x86_64-qt5-speech-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-speech) ; @echo 'PKG@@mingw-w64-x86_64-qt5-speech-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-static := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-static
PKG@@mingw-w64-x86_64-qt5-static: $(if $(__deps),PKG@@mingw-w64-x86_64-vulkan) ; @echo 'PKG@@mingw-w64-x86_64-qt5-static'

override VERSION_OF_mingw-w64-x86_64-qt5-svg := 5.15.19+kde+r5-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-svg
PKG@@mingw-w64-x86_64-qt5-svg: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-svg'

override VERSION_OF_mingw-w64-x86_64-qt5-svg-debug := 5.15.19+kde+r5-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-svg-debug
PKG@@mingw-w64-x86_64-qt5-svg-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-svg) ; @echo 'PKG@@mingw-w64-x86_64-qt5-svg-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-tools := 5.15.19-2
.PHONY: PKG@@mingw-w64-x86_64-qt5-tools
PKG@@mingw-w64-x86_64-qt5-tools: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-tools'

override VERSION_OF_mingw-w64-x86_64-qt5-tools-debug := 5.15.19-2
.PHONY: PKG@@mingw-w64-x86_64-qt5-tools-debug
PKG@@mingw-w64-x86_64-qt5-tools-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-tools) ; @echo 'PKG@@mingw-w64-x86_64-qt5-tools-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-translations := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-translations
PKG@@mingw-w64-x86_64-qt5-translations: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-translations'

override VERSION_OF_mingw-w64-x86_64-qt5-virtualkeyboard := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-virtualkeyboard
PKG@@mingw-w64-x86_64-qt5-virtualkeyboard: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative PKG@@mingw-w64-x86_64-qt5-svg) ; @echo 'PKG@@mingw-w64-x86_64-qt5-virtualkeyboard'

override VERSION_OF_mingw-w64-x86_64-qt5-virtualkeyboard-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-virtualkeyboard-debug
PKG@@mingw-w64-x86_64-qt5-virtualkeyboard-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-virtualkeyboard) ; @echo 'PKG@@mingw-w64-x86_64-qt5-virtualkeyboard-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-webchannel := 5.15.19+kde+r3-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-webchannel
PKG@@mingw-w64-x86_64-qt5-webchannel: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt5-webchannel'

override VERSION_OF_mingw-w64-x86_64-qt5-webchannel-debug := 5.15.19+kde+r3-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-webchannel-debug
PKG@@mingw-w64-x86_64-qt5-webchannel-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-declarative-debug PKG@@mingw-w64-x86_64-qt5-webchannel) ; @echo 'PKG@@mingw-w64-x86_64-qt5-webchannel-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-webglplugin := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-webglplugin
PKG@@mingw-w64-x86_64-qt5-webglplugin: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base PKG@@mingw-w64-x86_64-qt5-websockets) ; @echo 'PKG@@mingw-w64-x86_64-qt5-webglplugin'

override VERSION_OF_mingw-w64-x86_64-qt5-webglplugin-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-webglplugin-debug
PKG@@mingw-w64-x86_64-qt5-webglplugin-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-webglplugin PKG@@mingw-w64-x86_64-qt5-websockets-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt5-webglplugin-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-websockets := 5.15.19+kde+r2-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-websockets
PKG@@mingw-w64-x86_64-qt5-websockets: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-websockets'

override VERSION_OF_mingw-w64-x86_64-qt5-websockets-debug := 5.15.19+kde+r2-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-websockets-debug
PKG@@mingw-w64-x86_64-qt5-websockets-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-websockets) ; @echo 'PKG@@mingw-w64-x86_64-qt5-websockets-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-webview := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-webview
PKG@@mingw-w64-x86_64-qt5-webview: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-webchannel) ; @echo 'PKG@@mingw-w64-x86_64-qt5-webview'

override VERSION_OF_mingw-w64-x86_64-qt5-webview-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-webview-debug
PKG@@mingw-w64-x86_64-qt5-webview-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-webchannel-debug PKG@@mingw-w64-x86_64-qt5-webview) ; @echo 'PKG@@mingw-w64-x86_64-qt5-webview-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-winextras := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-winextras
PKG@@mingw-w64-x86_64-qt5-winextras: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-winextras'

override VERSION_OF_mingw-w64-x86_64-qt5-winextras-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-winextras-debug
PKG@@mingw-w64-x86_64-qt5-winextras-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-winextras) ; @echo 'PKG@@mingw-w64-x86_64-qt5-winextras-debug'

override VERSION_OF_mingw-w64-x86_64-qt5-xmlpatterns := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-xmlpatterns
PKG@@mingw-w64-x86_64-qt5-xmlpatterns: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-qt5-xmlpatterns'

override VERSION_OF_mingw-w64-x86_64-qt5-xmlpatterns-debug := 5.15.19-1
.PHONY: PKG@@mingw-w64-x86_64-qt5-xmlpatterns-debug
PKG@@mingw-w64-x86_64-qt5-xmlpatterns-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base-debug PKG@@mingw-w64-x86_64-qt5-xmlpatterns) ; @echo 'PKG@@mingw-w64-x86_64-qt5-xmlpatterns-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-3d := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-3d
PKG@@mingw-w64-x86_64-qt6-3d: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-3d'

override VERSION_OF_mingw-w64-x86_64-qt6-3d-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-3d-debug
PKG@@mingw-w64-x86_64-qt6-3d-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-3d PKG@@mingw-w64-x86_64-qt6-base-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt6-3d-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-5compat := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-5compat
PKG@@mingw-w64-x86_64-qt6-5compat: $(if $(__deps),PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-5compat'

override VERSION_OF_mingw-w64-x86_64-qt6-5compat-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-5compat-debug
PKG@@mingw-w64-x86_64-qt6-5compat-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-5compat PKG@@mingw-w64-x86_64-qt6-base-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt6-5compat-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-activeqt := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-activeqt
PKG@@mingw-w64-x86_64-qt6-activeqt: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-activeqt'

override VERSION_OF_mingw-w64-x86_64-qt6-activeqt-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-activeqt-debug
PKG@@mingw-w64-x86_64-qt6-activeqt-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-activeqt PKG@@mingw-w64-x86_64-qt6-base-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt6-activeqt-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-base := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-base
PKG@@mingw-w64-x86_64-qt6-base: $(if $(__deps),PKG@@mingw-w64-x86_64-dbus PKG@@mingw-w64-x86_64-double-conversion PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-libb2 PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-md4c PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-pcre2 PKG@@mingw-w64-x86_64-sqlite3 PKG@@mingw-w64-x86_64-vulkan-headers PKG@@mingw-w64-x86_64-vulkan-loader PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-qt6-base'

override VERSION_OF_mingw-w64-x86_64-qt6-base-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-base-debug
PKG@@mingw-w64-x86_64-qt6-base-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-base-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-charts := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-charts
PKG@@mingw-w64-x86_64-qt6-charts: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-charts'

override VERSION_OF_mingw-w64-x86_64-qt6-charts-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-charts-debug
PKG@@mingw-w64-x86_64-qt6-charts-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-charts) ; @echo 'PKG@@mingw-w64-x86_64-qt6-charts-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-connectivity := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-connectivity
PKG@@mingw-w64-x86_64-qt6-connectivity: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-connectivity'

override VERSION_OF_mingw-w64-x86_64-qt6-connectivity-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-connectivity-debug
PKG@@mingw-w64-x86_64-qt6-connectivity-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-connectivity) ; @echo 'PKG@@mingw-w64-x86_64-qt6-connectivity-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-datavis3d := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-datavis3d
PKG@@mingw-w64-x86_64-qt6-datavis3d: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-datavis3d'

override VERSION_OF_mingw-w64-x86_64-qt6-datavis3d-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-datavis3d-debug
PKG@@mingw-w64-x86_64-qt6-datavis3d-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-datavis3d) ; @echo 'PKG@@mingw-w64-x86_64-qt6-datavis3d-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-declarative := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-declarative
PKG@@mingw-w64-x86_64-qt6-declarative: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-declarative'

override VERSION_OF_mingw-w64-x86_64-qt6-declarative-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-declarative-debug
PKG@@mingw-w64-x86_64-qt6-declarative-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt6-declarative-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-grpc := 6.12.0-2
.PHONY: PKG@@mingw-w64-x86_64-qt6-grpc
PKG@@mingw-w64-x86_64-qt6-grpc: $(if $(__deps),PKG@@mingw-w64-x86_64-abseil-cpp PKG@@mingw-w64-x86_64-grpc PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-protobuf PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-grpc'

override VERSION_OF_mingw-w64-x86_64-qt6-grpc-debug := 6.12.0-2
.PHONY: PKG@@mingw-w64-x86_64-qt6-grpc-debug
PKG@@mingw-w64-x86_64-qt6-grpc-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-grpc) ; @echo 'PKG@@mingw-w64-x86_64-qt6-grpc-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-httpserver := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-httpserver
PKG@@mingw-w64-x86_64-qt6-httpserver: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-websockets) ; @echo 'PKG@@mingw-w64-x86_64-qt6-httpserver'

override VERSION_OF_mingw-w64-x86_64-qt6-httpserver-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-httpserver-debug
PKG@@mingw-w64-x86_64-qt6-httpserver-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-httpserver PKG@@mingw-w64-x86_64-qt6-websockets-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt6-httpserver-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-imageformats := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-imageformats
PKG@@mingw-w64-x86_64-qt6-imageformats: $(if $(__deps),PKG@@mingw-w64-x86_64-jasper PKG@@mingw-w64-x86_64-libmng PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-imageformats'

override VERSION_OF_mingw-w64-x86_64-qt6-imageformats-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-imageformats-debug
PKG@@mingw-w64-x86_64-qt6-imageformats-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-imageformats) ; @echo 'PKG@@mingw-w64-x86_64-qt6-imageformats-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-languageserver := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-languageserver
PKG@@mingw-w64-x86_64-qt6-languageserver: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-languageserver'

override VERSION_OF_mingw-w64-x86_64-qt6-location := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-location
PKG@@mingw-w64-x86_64-qt6-location: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative PKG@@mingw-w64-x86_64-qt6-positioning) ; @echo 'PKG@@mingw-w64-x86_64-qt6-location'

override VERSION_OF_mingw-w64-x86_64-qt6-location-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-location-debug
PKG@@mingw-w64-x86_64-qt6-location-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative-debug PKG@@mingw-w64-x86_64-qt6-location PKG@@mingw-w64-x86_64-qt6-positioning-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt6-location-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-lottie := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-lottie
PKG@@mingw-w64-x86_64-qt6-lottie: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-qt6-base PKG@@mingw-w64-x86_64-qt6-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt6-lottie'

override VERSION_OF_mingw-w64-x86_64-qt6-lottie-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-lottie-debug
PKG@@mingw-w64-x86_64-qt6-lottie-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative-debug PKG@@mingw-w64-x86_64-qt6-lottie) ; @echo 'PKG@@mingw-w64-x86_64-qt6-lottie-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-multimedia := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-multimedia
PKG@@mingw-w64-x86_64-qt6-multimedia: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base PKG@@mingw-w64-x86_64-qt6-multimedia-ffmpeg PKG@@mingw-w64-x86_64-qt6-shadertools) ; @echo 'PKG@@mingw-w64-x86_64-qt6-multimedia'

override VERSION_OF_mingw-w64-x86_64-qt6-multimedia-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-multimedia-debug
PKG@@mingw-w64-x86_64-qt6-multimedia-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-multimedia PKG@@mingw-w64-x86_64-qt6-shadertools-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt6-multimedia-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-multimedia-ffmpeg := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-multimedia-ffmpeg
PKG@@mingw-w64-x86_64-qt6-multimedia-ffmpeg: $(if $(__deps),PKG@@mingw-w64-x86_64-ffmpeg PKG@@mingw-w64-x86_64-qt6-multimedia) ; @echo 'PKG@@mingw-w64-x86_64-qt6-multimedia-ffmpeg'

override VERSION_OF_mingw-w64-x86_64-qt6-multimedia-wmf := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-multimedia-wmf
PKG@@mingw-w64-x86_64-qt6-multimedia-wmf: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-multimedia) ; @echo 'PKG@@mingw-w64-x86_64-qt6-multimedia-wmf'

override VERSION_OF_mingw-w64-x86_64-qt6-networkauth := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-networkauth
PKG@@mingw-w64-x86_64-qt6-networkauth: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-networkauth'

override VERSION_OF_mingw-w64-x86_64-qt6-networkauth-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-networkauth-debug
PKG@@mingw-w64-x86_64-qt6-networkauth-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-networkauth) ; @echo 'PKG@@mingw-w64-x86_64-qt6-networkauth-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-positioning := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-positioning
PKG@@mingw-w64-x86_64-qt6-positioning: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-positioning'

override VERSION_OF_mingw-w64-x86_64-qt6-positioning-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-positioning-debug
PKG@@mingw-w64-x86_64-qt6-positioning-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-positioning) ; @echo 'PKG@@mingw-w64-x86_64-qt6-positioning-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-quick3d := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-quick3d
PKG@@mingw-w64-x86_64-qt6-quick3d: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative PKG@@mingw-w64-x86_64-qt6-shadertools) ; @echo 'PKG@@mingw-w64-x86_64-qt6-quick3d'

override VERSION_OF_mingw-w64-x86_64-qt6-quick3d-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-quick3d-debug
PKG@@mingw-w64-x86_64-qt6-quick3d-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative-debug PKG@@mingw-w64-x86_64-qt6-quick3d PKG@@mingw-w64-x86_64-qt6-shadertools-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt6-quick3d-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-quick3dphysics := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-quick3dphysics
PKG@@mingw-w64-x86_64-qt6-quick3dphysics: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-quick3d PKG@@mingw-w64-x86_64-qt6-shadertools) ; @echo 'PKG@@mingw-w64-x86_64-qt6-quick3dphysics'

override VERSION_OF_mingw-w64-x86_64-qt6-quick3dphysics-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-quick3dphysics-debug
PKG@@mingw-w64-x86_64-qt6-quick3dphysics-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-quick3d-debug PKG@@mingw-w64-x86_64-qt6-quick3dphysics PKG@@mingw-w64-x86_64-qt6-shadertools-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt6-quick3dphysics-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-quickeffectmaker := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-quickeffectmaker
PKG@@mingw-w64-x86_64-qt6-quickeffectmaker: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative PKG@@mingw-w64-x86_64-qt6-quick3d PKG@@mingw-w64-x86_64-qt6-shadertools) ; @echo 'PKG@@mingw-w64-x86_64-qt6-quickeffectmaker'

override VERSION_OF_mingw-w64-x86_64-qt6-quickeffectmaker-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-quickeffectmaker-debug
PKG@@mingw-w64-x86_64-qt6-quickeffectmaker-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-quickeffectmaker) ; @echo 'PKG@@mingw-w64-x86_64-qt6-quickeffectmaker-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-quicktimeline := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-quicktimeline
PKG@@mingw-w64-x86_64-qt6-quicktimeline: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt6-quicktimeline'

override VERSION_OF_mingw-w64-x86_64-qt6-quicktimeline-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-quicktimeline-debug
PKG@@mingw-w64-x86_64-qt6-quicktimeline-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative-debug PKG@@mingw-w64-x86_64-qt6-quicktimeline) ; @echo 'PKG@@mingw-w64-x86_64-qt6-quicktimeline-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-remoteobjects := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-remoteobjects
PKG@@mingw-w64-x86_64-qt6-remoteobjects: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-remoteobjects'

override VERSION_OF_mingw-w64-x86_64-qt6-remoteobjects-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-remoteobjects-debug
PKG@@mingw-w64-x86_64-qt6-remoteobjects-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-remoteobjects) ; @echo 'PKG@@mingw-w64-x86_64-qt6-remoteobjects-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-scxml := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-scxml
PKG@@mingw-w64-x86_64-qt6-scxml: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-scxml'

override VERSION_OF_mingw-w64-x86_64-qt6-scxml-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-scxml-debug
PKG@@mingw-w64-x86_64-qt6-scxml-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative-debug PKG@@mingw-w64-x86_64-qt6-scxml) ; @echo 'PKG@@mingw-w64-x86_64-qt6-scxml-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-sensors := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-sensors
PKG@@mingw-w64-x86_64-qt6-sensors: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-sensors'

override VERSION_OF_mingw-w64-x86_64-qt6-sensors-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-sensors-debug
PKG@@mingw-w64-x86_64-qt6-sensors-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-sensors) ; @echo 'PKG@@mingw-w64-x86_64-qt6-sensors-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-serialbus := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-serialbus
PKG@@mingw-w64-x86_64-qt6-serialbus: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-serialport) ; @echo 'PKG@@mingw-w64-x86_64-qt6-serialbus'

override VERSION_OF_mingw-w64-x86_64-qt6-serialbus-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-serialbus-debug
PKG@@mingw-w64-x86_64-qt6-serialbus-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-serialbus PKG@@mingw-w64-x86_64-qt6-serialport-debug) ; @echo 'PKG@@mingw-w64-x86_64-qt6-serialbus-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-serialport := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-serialport
PKG@@mingw-w64-x86_64-qt6-serialport: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-serialport'

override VERSION_OF_mingw-w64-x86_64-qt6-serialport-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-serialport-debug
PKG@@mingw-w64-x86_64-qt6-serialport-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-serialport) ; @echo 'PKG@@mingw-w64-x86_64-qt6-serialport-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-shadertools := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-shadertools
PKG@@mingw-w64-x86_64-qt6-shadertools: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-shadertools'

override VERSION_OF_mingw-w64-x86_64-qt6-shadertools-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-shadertools-debug
PKG@@mingw-w64-x86_64-qt6-shadertools-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-shadertools) ; @echo 'PKG@@mingw-w64-x86_64-qt6-shadertools-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-speech := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-speech
PKG@@mingw-w64-x86_64-qt6-speech: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base PKG@@mingw-w64-x86_64-qt6-multimedia) ; @echo 'PKG@@mingw-w64-x86_64-qt6-speech'

override VERSION_OF_mingw-w64-x86_64-qt6-speech-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-speech-debug
PKG@@mingw-w64-x86_64-qt6-speech-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-multimedia-debug PKG@@mingw-w64-x86_64-qt6-speech) ; @echo 'PKG@@mingw-w64-x86_64-qt6-speech-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-svg := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-svg
PKG@@mingw-w64-x86_64-qt6-svg: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-svg'

override VERSION_OF_mingw-w64-x86_64-qt6-svg-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-svg-debug
PKG@@mingw-w64-x86_64-qt6-svg-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-svg) ; @echo 'PKG@@mingw-w64-x86_64-qt6-svg-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-tools := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-tools
PKG@@mingw-w64-x86_64-qt6-tools: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-qt6-base PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-qt6-tools'

override VERSION_OF_mingw-w64-x86_64-qt6-tools-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-tools-debug
PKG@@mingw-w64-x86_64-qt6-tools-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-tools) ; @echo 'PKG@@mingw-w64-x86_64-qt6-tools-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-translations := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-translations
PKG@@mingw-w64-x86_64-qt6-translations: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-translations'

override VERSION_OF_mingw-w64-x86_64-qt6-virtualkeyboard := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-virtualkeyboard
PKG@@mingw-w64-x86_64-qt6-virtualkeyboard: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative PKG@@mingw-w64-x86_64-qt6-svg) ; @echo 'PKG@@mingw-w64-x86_64-qt6-virtualkeyboard'

override VERSION_OF_mingw-w64-x86_64-qt6-virtualkeyboard-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-virtualkeyboard-debug
PKG@@mingw-w64-x86_64-qt6-virtualkeyboard-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative-debug PKG@@mingw-w64-x86_64-qt6-svg-debug PKG@@mingw-w64-x86_64-qt6-virtualkeyboard) ; @echo 'PKG@@mingw-w64-x86_64-qt6-virtualkeyboard-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-webchannel := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-webchannel
PKG@@mingw-w64-x86_64-qt6-webchannel: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative) ; @echo 'PKG@@mingw-w64-x86_64-qt6-webchannel'

override VERSION_OF_mingw-w64-x86_64-qt6-webchannel-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-webchannel-debug
PKG@@mingw-w64-x86_64-qt6-webchannel-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-declarative-debug PKG@@mingw-w64-x86_64-qt6-webchannel) ; @echo 'PKG@@mingw-w64-x86_64-qt6-webchannel-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-websockets := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-websockets
PKG@@mingw-w64-x86_64-qt6-websockets: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-websockets'

override VERSION_OF_mingw-w64-x86_64-qt6-websockets-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-websockets-debug
PKG@@mingw-w64-x86_64-qt6-websockets-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-websockets) ; @echo 'PKG@@mingw-w64-x86_64-qt6-websockets-debug'

override VERSION_OF_mingw-w64-x86_64-qt6-webview := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-webview
PKG@@mingw-w64-x86_64-qt6-webview: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qt6-webview'

override VERSION_OF_mingw-w64-x86_64-qt6-webview-debug := 6.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-qt6-webview-debug
PKG@@mingw-w64-x86_64-qt6-webview-debug: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base-debug PKG@@mingw-w64-x86_64-qt6-webview) ; @echo 'PKG@@mingw-w64-x86_64-qt6-webview-debug'

override VERSION_OF_mingw-w64-x86_64-qtkeychain := 0.17.0-1
.PHONY: PKG@@mingw-w64-x86_64-qtkeychain PKG@@mingw-w64-x86_64-qtkeychain-qt6
PKG@@mingw-w64-x86_64-qtkeychain PKG@@mingw-w64-x86_64-qtkeychain-qt6: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-qtkeychain'

override VERSION_OF_mingw-w64-x86_64-qtwebkit := 5.212.0alpha4-27
.PHONY: PKG@@mingw-w64-x86_64-qtwebkit
PKG@@mingw-w64-x86_64-qtwebkit: $(if $(__deps),PKG@@mingw-w64-x86_64-fontconfig PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-libxslt PKG@@mingw-w64-x86_64-qt5-location PKG@@mingw-w64-x86_64-qt5-multimedia PKG@@mingw-w64-x86_64-qt5-sensors PKG@@mingw-w64-x86_64-qt5-webchannel PKG@@mingw-w64-x86_64-sqlite3 PKG@@mingw-w64-x86_64-woff2) ; @echo 'PKG@@mingw-w64-x86_64-qtwebkit'

override VERSION_OF_mingw-w64-x86_64-qwt-qt5 := 6.3.0-2
.PHONY: PKG@@mingw-w64-x86_64-qwt-qt5
PKG@@mingw-w64-x86_64-qwt-qt5: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-qt5-base PKG@@mingw-w64-x86_64-qt5-svg) ; @echo 'PKG@@mingw-w64-x86_64-qwt-qt5'

override VERSION_OF_mingw-w64-x86_64-qwt-qt6 := 6.3.0-2
.PHONY: PKG@@mingw-w64-x86_64-qwt-qt6
PKG@@mingw-w64-x86_64-qwt-qt6: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-qt6-base PKG@@mingw-w64-x86_64-qt6-svg) ; @echo 'PKG@@mingw-w64-x86_64-qwt-qt6'

override VERSION_OF_mingw-w64-x86_64-rabbitmq-c := 0.16.0-1
.PHONY: PKG@@mingw-w64-x86_64-rabbitmq-c
PKG@@mingw-w64-x86_64-rabbitmq-c: $(if $(__deps),PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-popt) ; @echo 'PKG@@mingw-w64-x86_64-rabbitmq-c'

override VERSION_OF_mingw-w64-x86_64-ragel := 6.10-3
.PHONY: PKG@@mingw-w64-x86_64-ragel
PKG@@mingw-w64-x86_64-ragel: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-ragel'

override VERSION_OF_mingw-w64-x86_64-range-v3 := 0.12.0-1
.PHONY: PKG@@mingw-w64-x86_64-range-v3
PKG@@mingw-w64-x86_64-range-v3: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-range-v3'

override VERSION_OF_mingw-w64-x86_64-rapidfuzz-cpp := 3.3.4-1
.PHONY: PKG@@mingw-w64-x86_64-rapidfuzz-cpp
PKG@@mingw-w64-x86_64-rapidfuzz-cpp: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-rapidfuzz-cpp'

override VERSION_OF_mingw-w64-x86_64-rapidjson := 1.1.0-8
.PHONY: PKG@@mingw-w64-x86_64-rapidjson
PKG@@mingw-w64-x86_64-rapidjson: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-rapidjson'

override VERSION_OF_mingw-w64-x86_64-rav1e := 0.8.1-1
.PHONY: PKG@@mingw-w64-x86_64-rav1e
PKG@@mingw-w64-x86_64-rav1e: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-rav1e'

override VERSION_OF_mingw-w64-x86_64-rcedit := 2.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-rcedit
PKG@@mingw-w64-x86_64-rcedit: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-rcedit'

override VERSION_OF_mingw-w64-x86_64-re2 := 20251105-4
.PHONY: PKG@@mingw-w64-x86_64-re2
PKG@@mingw-w64-x86_64-re2: $(if $(__deps),PKG@@mingw-w64-x86_64-abseil-cpp PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-re2'

override VERSION_OF_mingw-w64-x86_64-readline := 8.3.003-1
.PHONY: PKG@@mingw-w64-x86_64-readline
PKG@@mingw-w64-x86_64-readline: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-termcap) ; @echo 'PKG@@mingw-w64-x86_64-readline'

override VERSION_OF_mingw-w64-x86_64-redis-plus-plus := 1.3.15-3
.PHONY: PKG@@mingw-w64-x86_64-redis-plus-plus
PKG@@mingw-w64-x86_64-redis-plus-plus: $(if $(__deps),PKG@@mingw-w64-x86_64-hiredis) ; @echo 'PKG@@mingw-w64-x86_64-redis-plus-plus'

override VERSION_OF_mingw-w64-x86_64-rhash := 1.4.6-1
.PHONY: PKG@@mingw-w64-x86_64-rhash
PKG@@mingw-w64-x86_64-rhash: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime) ; @echo 'PKG@@mingw-w64-x86_64-rhash'

override VERSION_OF_mingw-w64-x86_64-rime-bopomofo := 0.0.0.20260106-1
.PHONY: PKG@@mingw-w64-x86_64-rime-bopomofo
PKG@@mingw-w64-x86_64-rime-bopomofo: $(if $(__deps),PKG@@mingw-w64-x86_64-rime-cangjie PKG@@mingw-w64-x86_64-rime-terra-pinyin) ; @echo 'PKG@@mingw-w64-x86_64-rime-bopomofo'

override VERSION_OF_mingw-w64-x86_64-rime-cangjie := 0.0.0.20240325-1
.PHONY: PKG@@mingw-w64-x86_64-rime-cangjie
PKG@@mingw-w64-x86_64-rime-cangjie: $(if $(__deps),PKG@@mingw-w64-x86_64-rime-luna-pinyin) ; @echo 'PKG@@mingw-w64-x86_64-rime-cangjie'

override VERSION_OF_mingw-w64-x86_64-rime-double-pinyin := 0.0.0.20250923-1
.PHONY: PKG@@mingw-w64-x86_64-rime-double-pinyin
PKG@@mingw-w64-x86_64-rime-double-pinyin: $(if $(__deps),PKG@@mingw-w64-x86_64-rime-luna-pinyin PKG@@mingw-w64-x86_64-rime-stroke) ; @echo 'PKG@@mingw-w64-x86_64-rime-double-pinyin'

override VERSION_OF_mingw-w64-x86_64-rime-emoji := 0.0.0.20250401-1
.PHONY: PKG@@mingw-w64-x86_64-rime-emoji
PKG@@mingw-w64-x86_64-rime-emoji: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-rime-emoji'

override VERSION_OF_mingw-w64-x86_64-rime-essay := 0.0.0.20260106-1
.PHONY: PKG@@mingw-w64-x86_64-rime-essay
PKG@@mingw-w64-x86_64-rime-essay: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-rime-essay'

override VERSION_OF_mingw-w64-x86_64-rime-luna-pinyin := 0.0.0.20260106-1
.PHONY: PKG@@mingw-w64-x86_64-rime-luna-pinyin
PKG@@mingw-w64-x86_64-rime-luna-pinyin: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-rime-luna-pinyin'

override VERSION_OF_mingw-w64-x86_64-rime-pinyin-simp := 0.0.0.20241229-1
.PHONY: PKG@@mingw-w64-x86_64-rime-pinyin-simp
PKG@@mingw-w64-x86_64-rime-pinyin-simp: $(if $(__deps),PKG@@mingw-w64-x86_64-rime-stroke) ; @echo 'PKG@@mingw-w64-x86_64-rime-pinyin-simp'

override VERSION_OF_mingw-w64-x86_64-rime-prelude := 0.0.0.20251229-1
.PHONY: PKG@@mingw-w64-x86_64-rime-prelude
PKG@@mingw-w64-x86_64-rime-prelude: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-rime-prelude'

override VERSION_OF_mingw-w64-x86_64-rime-stroke := 0.0.0.20250923-1
.PHONY: PKG@@mingw-w64-x86_64-rime-stroke
PKG@@mingw-w64-x86_64-rime-stroke: $(if $(__deps),PKG@@mingw-w64-x86_64-rime-luna-pinyin) ; @echo 'PKG@@mingw-w64-x86_64-rime-stroke'

override VERSION_OF_mingw-w64-x86_64-rime-terra-pinyin := 0.0.0.20251206-1
.PHONY: PKG@@mingw-w64-x86_64-rime-terra-pinyin
PKG@@mingw-w64-x86_64-rime-terra-pinyin: $(if $(__deps),PKG@@mingw-w64-x86_64-rime-stroke) ; @echo 'PKG@@mingw-w64-x86_64-rime-terra-pinyin'

override VERSION_OF_mingw-w64-x86_64-rime-wubi := 0.0.0.20231025-1
.PHONY: PKG@@mingw-w64-x86_64-rime-wubi
PKG@@mingw-w64-x86_64-rime-wubi: $(if $(__deps),PKG@@mingw-w64-x86_64-rime-pinyin-simp) ; @echo 'PKG@@mingw-w64-x86_64-rime-wubi'

override VERSION_OF_mingw-w64-x86_64-robin-hood-hashing := 3.11.5-1
.PHONY: PKG@@mingw-w64-x86_64-robin-hood-hashing
PKG@@mingw-w64-x86_64-robin-hood-hashing: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-robin-hood-hashing'

override VERSION_OF_mingw-w64-x86_64-robin-map := 1.4.1-1
.PHONY: PKG@@mingw-w64-x86_64-robin-map
PKG@@mingw-w64-x86_64-robin-map: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-robin-map'

override VERSION_OF_mingw-w64-x86_64-rocksdb := 11.1.1-2
.PHONY: PKG@@mingw-w64-x86_64-rocksdb
PKG@@mingw-w64-x86_64-rocksdb: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-lz4 PKG@@mingw-w64-x86_64-snappy PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-rocksdb'

override VERSION_OF_mingw-w64-x86_64-rtmpdump := 2.6-2
.PHONY: PKG@@mingw-w64-x86_64-rtmpdump PKG@@mingw-w64-x86_64-rtmpdump-git
PKG@@mingw-w64-x86_64-rtmpdump PKG@@mingw-w64-x86_64-rtmpdump-git: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-gnutls PKG@@mingw-w64-x86_64-nettle PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-rtmpdump'

override VERSION_OF_mingw-w64-x86_64-ruby := 4.0.9-1
.PHONY: PKG@@mingw-w64-x86_64-ruby PKG@@mingw-w64-x86_64-ruby-abbrev PKG@@mingw-w64-x86_64-ruby-base64 PKG@@mingw-w64-x86_64-ruby-benchmark PKG@@mingw-w64-x86_64-ruby-bigdecimal PKG@@mingw-w64-x86_64-ruby-bundler PKG@@mingw-w64-x86_64-ruby-csv PKG@@mingw-w64-x86_64-ruby-date PKG@@mingw-w64-x86_64-ruby-debug PKG@@mingw-w64-x86_64-ruby-delegate PKG@@mingw-w64-x86_64-ruby-did_you_mean PKG@@mingw-w64-x86_64-ruby-digest PKG@@mingw-w64-x86_64-ruby-drb PKG@@mingw-w64-x86_64-ruby-english PKG@@mingw-w64-x86_64-ruby-erb PKG@@mingw-w64-x86_64-ruby-error_highlight PKG@@mingw-w64-x86_64-ruby-etc PKG@@mingw-w64-x86_64-ruby-fcntl PKG@@mingw-w64-x86_64-ruby-fiddle PKG@@mingw-w64-x86_64-ruby-fileutils PKG@@mingw-w64-x86_64-ruby-find PKG@@mingw-w64-x86_64-ruby-forwardable PKG@@mingw-w64-x86_64-ruby-getoptlong PKG@@mingw-w64-x86_64-ruby-io-console PKG@@mingw-w64-x86_64-ruby-io-nonblock PKG@@mingw-w64-x86_64-ruby-io-wait PKG@@mingw-w64-x86_64-ruby-ipaddr PKG@@mingw-w64-x86_64-ruby-irb PKG@@mingw-w64-x86_64-ruby-json PKG@@mingw-w64-x86_64-ruby-logger PKG@@mingw-w64-x86_64-ruby-matrix PKG@@mingw-w64-x86_64-ruby-minitest PKG@@mingw-w64-x86_64-ruby-mutex_m PKG@@mingw-w64-x86_64-ruby-net-ftp PKG@@mingw-w64-x86_64-ruby-net-http PKG@@mingw-w64-x86_64-ruby-net-imap PKG@@mingw-w64-x86_64-ruby-net-pop PKG@@mingw-w64-x86_64-ruby-net-protocol PKG@@mingw-w64-x86_64-ruby-net-smtp PKG@@mingw-w64-x86_64-ruby-nkf PKG@@mingw-w64-x86_64-ruby-observer PKG@@mingw-w64-x86_64-ruby-open-uri PKG@@mingw-w64-x86_64-ruby-open3 PKG@@mingw-w64-x86_64-ruby-openssl PKG@@mingw-w64-x86_64-ruby-optparse PKG@@mingw-w64-x86_64-ruby-ostruct PKG@@mingw-w64-x86_64-ruby-power_assert PKG@@mingw-w64-x86_64-ruby-pp PKG@@mingw-w64-x86_64-ruby-prettyprint PKG@@mingw-w64-x86_64-ruby-prime PKG@@mingw-w64-x86_64-ruby-prism PKG@@mingw-w64-x86_64-ruby-pstore PKG@@mingw-w64-x86_64-ruby-psych PKG@@mingw-w64-x86_64-ruby-racc PKG@@mingw-w64-x86_64-ruby-rake PKG@@mingw-w64-x86_64-ruby-rbs PKG@@mingw-w64-x86_64-ruby-rdoc PKG@@mingw-w64-x86_64-ruby-readline PKG@@mingw-w64-x86_64-ruby-reline PKG@@mingw-w64-x86_64-ruby-repl_type_completor PKG@@mingw-w64-x86_64-ruby-resolv PKG@@mingw-w64-x86_64-ruby-resolv-replace PKG@@mingw-w64-x86_64-ruby-rexml PKG@@mingw-w64-x86_64-ruby-rinda PKG@@mingw-w64-x86_64-ruby-rss PKG@@mingw-w64-x86_64-ruby-ruby2_keywords PKG@@mingw-w64-x86_64-ruby-securerandom PKG@@mingw-w64-x86_64-ruby-shellwords PKG@@mingw-w64-x86_64-ruby-singleton PKG@@mingw-w64-x86_64-ruby-stringio PKG@@mingw-w64-x86_64-ruby-strscan PKG@@mingw-w64-x86_64-ruby-syntax_suggest PKG@@mingw-w64-x86_64-ruby-tempfile PKG@@mingw-w64-x86_64-ruby-test-unit PKG@@mingw-w64-x86_64-ruby-time PKG@@mingw-w64-x86_64-ruby-timeout PKG@@mingw-w64-x86_64-ruby-tmpdir PKG@@mingw-w64-x86_64-ruby-tsort PKG@@mingw-w64-x86_64-ruby-typeprof PKG@@mingw-w64-x86_64-ruby-un PKG@@mingw-w64-x86_64-ruby-uri PKG@@mingw-w64-x86_64-ruby-weakref PKG@@mingw-w64-x86_64-ruby-win32-registry PKG@@mingw-w64-x86_64-ruby-win32ole PKG@@mingw-w64-x86_64-ruby-yaml PKG@@mingw-w64-x86_64-ruby-zlib
PKG@@mingw-w64-x86_64-ruby PKG@@mingw-w64-x86_64-ruby-abbrev PKG@@mingw-w64-x86_64-ruby-base64 PKG@@mingw-w64-x86_64-ruby-benchmark PKG@@mingw-w64-x86_64-ruby-bigdecimal PKG@@mingw-w64-x86_64-ruby-bundler PKG@@mingw-w64-x86_64-ruby-csv PKG@@mingw-w64-x86_64-ruby-date PKG@@mingw-w64-x86_64-ruby-debug PKG@@mingw-w64-x86_64-ruby-delegate PKG@@mingw-w64-x86_64-ruby-did_you_mean PKG@@mingw-w64-x86_64-ruby-digest PKG@@mingw-w64-x86_64-ruby-drb PKG@@mingw-w64-x86_64-ruby-english PKG@@mingw-w64-x86_64-ruby-erb PKG@@mingw-w64-x86_64-ruby-error_highlight PKG@@mingw-w64-x86_64-ruby-etc PKG@@mingw-w64-x86_64-ruby-fcntl PKG@@mingw-w64-x86_64-ruby-fiddle PKG@@mingw-w64-x86_64-ruby-fileutils PKG@@mingw-w64-x86_64-ruby-find PKG@@mingw-w64-x86_64-ruby-forwardable PKG@@mingw-w64-x86_64-ruby-getoptlong PKG@@mingw-w64-x86_64-ruby-io-console PKG@@mingw-w64-x86_64-ruby-io-nonblock PKG@@mingw-w64-x86_64-ruby-io-wait PKG@@mingw-w64-x86_64-ruby-ipaddr PKG@@mingw-w64-x86_64-ruby-irb PKG@@mingw-w64-x86_64-ruby-json PKG@@mingw-w64-x86_64-ruby-logger PKG@@mingw-w64-x86_64-ruby-matrix PKG@@mingw-w64-x86_64-ruby-minitest PKG@@mingw-w64-x86_64-ruby-mutex_m PKG@@mingw-w64-x86_64-ruby-net-ftp PKG@@mingw-w64-x86_64-ruby-net-http PKG@@mingw-w64-x86_64-ruby-net-imap PKG@@mingw-w64-x86_64-ruby-net-pop PKG@@mingw-w64-x86_64-ruby-net-protocol PKG@@mingw-w64-x86_64-ruby-net-smtp PKG@@mingw-w64-x86_64-ruby-nkf PKG@@mingw-w64-x86_64-ruby-observer PKG@@mingw-w64-x86_64-ruby-open-uri PKG@@mingw-w64-x86_64-ruby-open3 PKG@@mingw-w64-x86_64-ruby-openssl PKG@@mingw-w64-x86_64-ruby-optparse PKG@@mingw-w64-x86_64-ruby-ostruct PKG@@mingw-w64-x86_64-ruby-power_assert PKG@@mingw-w64-x86_64-ruby-pp PKG@@mingw-w64-x86_64-ruby-prettyprint PKG@@mingw-w64-x86_64-ruby-prime PKG@@mingw-w64-x86_64-ruby-prism PKG@@mingw-w64-x86_64-ruby-pstore PKG@@mingw-w64-x86_64-ruby-psych PKG@@mingw-w64-x86_64-ruby-racc PKG@@mingw-w64-x86_64-ruby-rake PKG@@mingw-w64-x86_64-ruby-rbs PKG@@mingw-w64-x86_64-ruby-rdoc PKG@@mingw-w64-x86_64-ruby-readline PKG@@mingw-w64-x86_64-ruby-reline PKG@@mingw-w64-x86_64-ruby-repl_type_completor PKG@@mingw-w64-x86_64-ruby-resolv PKG@@mingw-w64-x86_64-ruby-resolv-replace PKG@@mingw-w64-x86_64-ruby-rexml PKG@@mingw-w64-x86_64-ruby-rinda PKG@@mingw-w64-x86_64-ruby-rss PKG@@mingw-w64-x86_64-ruby-ruby2_keywords PKG@@mingw-w64-x86_64-ruby-securerandom PKG@@mingw-w64-x86_64-ruby-shellwords PKG@@mingw-w64-x86_64-ruby-singleton PKG@@mingw-w64-x86_64-ruby-stringio PKG@@mingw-w64-x86_64-ruby-strscan PKG@@mingw-w64-x86_64-ruby-syntax_suggest PKG@@mingw-w64-x86_64-ruby-tempfile PKG@@mingw-w64-x86_64-ruby-test-unit PKG@@mingw-w64-x86_64-ruby-time PKG@@mingw-w64-x86_64-ruby-timeout PKG@@mingw-w64-x86_64-ruby-tmpdir PKG@@mingw-w64-x86_64-ruby-tsort PKG@@mingw-w64-x86_64-ruby-typeprof PKG@@mingw-w64-x86_64-ruby-un PKG@@mingw-w64-x86_64-ruby-uri PKG@@mingw-w64-x86_64-ruby-weakref PKG@@mingw-w64-x86_64-ruby-win32-registry PKG@@mingw-w64-x86_64-ruby-win32ole PKG@@mingw-w64-x86_64-ruby-yaml PKG@@mingw-w64-x86_64-ruby-zlib: $(if $(__deps),PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-libffi PKG@@mingw-w64-x86_64-libyaml PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-ruby'

override VERSION_OF_mingw-w64-x86_64-ruby-dbus := 0.25.0-2
.PHONY: PKG@@mingw-w64-x86_64-ruby-dbus
PKG@@mingw-w64-x86_64-ruby-dbus: $(if $(__deps),PKG@@mingw-w64-x86_64-ruby) ; @echo 'PKG@@mingw-w64-x86_64-ruby-dbus'

override VERSION_OF_mingw-w64-x86_64-ruby-mustache := 1.1.1-7
.PHONY: PKG@@mingw-w64-x86_64-ruby-mustache
PKG@@mingw-w64-x86_64-ruby-mustache: $(if $(__deps),PKG@@mingw-w64-x86_64-ruby) ; @echo 'PKG@@mingw-w64-x86_64-ruby-mustache'

override VERSION_OF_mingw-w64-x86_64-ruby-native-package-installer := 1.1.9-3
.PHONY: PKG@@mingw-w64-x86_64-ruby-native-package-installer
PKG@@mingw-w64-x86_64-ruby-native-package-installer: $(if $(__deps),PKG@@mingw-w64-x86_64-ruby) ; @echo 'PKG@@mingw-w64-x86_64-ruby-native-package-installer'

override VERSION_OF_mingw-w64-x86_64-ruby-pkg-config := 1.6.6-1
.PHONY: PKG@@mingw-w64-x86_64-ruby-pkg-config
PKG@@mingw-w64-x86_64-ruby-pkg-config: $(if $(__deps),PKG@@mingw-w64-x86_64-ruby) ; @echo 'PKG@@mingw-w64-x86_64-ruby-pkg-config'

override VERSION_OF_mingw-w64-x86_64-ruby-rdiscount := 2.2.7.5-2
.PHONY: PKG@@mingw-w64-x86_64-ruby-rdiscount
PKG@@mingw-w64-x86_64-ruby-rdiscount: $(if $(__deps),PKG@@mingw-w64-x86_64-ruby) ; @echo 'PKG@@mingw-w64-x86_64-ruby-rdiscount'

override VERSION_OF_mingw-w64-x86_64-ruc-vm := 1.2-2
.PHONY: PKG@@mingw-w64-x86_64-ruc-vm
PKG@@mingw-w64-x86_64-ruc-vm: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-ruc-vm'

override VERSION_OF_mingw-w64-x86_64-rust := 1.99.1-1
.PHONY: PKG@@mingw-w64-x86_64-rust
PKG@@mingw-w64-x86_64-rust: $(if $(__deps),PKG@@mingw-w64-x86_64-cc PKG@@mingw-w64-x86_64-libgit2 PKG@@mingw-w64-x86_64-libssh2 PKG@@mingw-w64-x86_64-llvm-libs PKG@@mingw-w64-x86_64-sqlite3 PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-rust'

override VERSION_OF_mingw-w64-x86_64-rust-bindgen := 0.73.2-1
.PHONY: PKG@@mingw-w64-x86_64-rust-bindgen
PKG@@mingw-w64-x86_64-rust-bindgen: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-clang) ; @echo 'PKG@@mingw-w64-x86_64-rust-bindgen'

override VERSION_OF_mingw-w64-x86_64-rust-src := 1.99.1-1
.PHONY: PKG@@mingw-w64-x86_64-rust-src
PKG@@mingw-w64-x86_64-rust-src: $(if $(__deps),PKG@@mingw-w64-x86_64-rust) ; @echo 'PKG@@mingw-w64-x86_64-rust-src'

override VERSION_OF_mingw-w64-x86_64-rustup := 1.29.3-1
.PHONY: PKG@@mingw-w64-x86_64-rustup
PKG@@mingw-w64-x86_64-rustup: $(if $(__deps),PKG@@mingw-w64-x86_64-xz) ; @echo 'PKG@@mingw-w64-x86_64-rustup'

override VERSION_OF_mingw-w64-x86_64-rxspencer := 3.9.0-2
.PHONY: PKG@@mingw-w64-x86_64-rxspencer
PKG@@mingw-w64-x86_64-rxspencer: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-rxspencer'

override VERSION_OF_mingw-w64-x86_64-s2geometry := 0.14.0-2
.PHONY: PKG@@mingw-w64-x86_64-s2geometry
PKG@@mingw-w64-x86_64-s2geometry: $(if $(__deps),PKG@@mingw-w64-x86_64-abseil-cpp PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-s2geometry'

override VERSION_OF_mingw-w64-x86_64-samtools := 1.25-1
.PHONY: PKG@@mingw-w64-x86_64-samtools
PKG@@mingw-w64-x86_64-samtools: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-htslib PKG@@mingw-w64-x86_64-libdeflate PKG@@mingw-w64-x86_64-ncurses PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-samtools'

override VERSION_OF_mingw-w64-x86_64-sassc := 3.6.2-2
.PHONY: PKG@@mingw-w64-x86_64-sassc
PKG@@mingw-w64-x86_64-sassc: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-sassc'

override VERSION_OF_mingw-w64-x86_64-sbc := 2.2-1
.PHONY: PKG@@mingw-w64-x86_64-sbc
PKG@@mingw-w64-x86_64-sbc: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-sbc'

override VERSION_OF_mingw-w64-x86_64-sbcl := 2.6.9-1
.PHONY: PKG@@mingw-w64-x86_64-sbcl
PKG@@mingw-w64-x86_64-sbcl: $(if $(__deps),PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-sbcl'

override VERSION_OF_mingw-w64-x86_64-sccache := 0.19.0-2
.PHONY: PKG@@mingw-w64-x86_64-sccache
PKG@@mingw-w64-x86_64-sccache: $(if $(__deps),PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-sccache'

override VERSION_OF_mingw-w64-x86_64-scons := 4.11.1-1
.PHONY: PKG@@mingw-w64-x86_64-scons
PKG@@mingw-w64-x86_64-scons: $(if $(__deps),PKG@@mingw-w64-x86_64-python) ; @echo 'PKG@@mingw-w64-x86_64-scons'

override VERSION_OF_mingw-w64-x86_64-scour := 0.38.2-8
.PHONY: PKG@@mingw-w64-x86_64-scour
PKG@@mingw-w64-x86_64-scour: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-setuptools PKG@@mingw-w64-x86_64-python-six) ; @echo 'PKG@@mingw-w64-x86_64-scour'

override VERSION_OF_mingw-w64-x86_64-sdcv := 0.5.5-1
.PHONY: PKG@@mingw-w64-x86_64-sdcv
PKG@@mingw-w64-x86_64-sdcv: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-readline PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-sdcv'

override VERSION_OF_mingw-w64-x86_64-sdl-gfx := 2.0.27-2
.PHONY: PKG@@mingw-w64-x86_64-SDL_gfx PKG@@mingw-w64-x86_64-sdl-gfx
PKG@@mingw-w64-x86_64-SDL_gfx PKG@@mingw-w64-x86_64-sdl-gfx: $(if $(__deps),PKG@@mingw-w64-x86_64-sdl) ; @echo 'PKG@@mingw-w64-x86_64-sdl-gfx'

override VERSION_OF_mingw-w64-x86_64-sdl-image := 1.2.12-9
.PHONY: PKG@@mingw-w64-x86_64-SDL_image PKG@@mingw-w64-x86_64-sdl-image
PKG@@mingw-w64-x86_64-SDL_image PKG@@mingw-w64-x86_64-sdl-image: $(if $(__deps),PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-sdl PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-sdl-image'

override VERSION_OF_mingw-w64-x86_64-sdl-mixer := 1.2.12-11
.PHONY: PKG@@mingw-w64-x86_64-SDL_mixer PKG@@mingw-w64-x86_64-sdl-mixer
PKG@@mingw-w64-x86_64-SDL_mixer PKG@@mingw-w64-x86_64-sdl-mixer: $(if $(__deps),PKG@@mingw-w64-x86_64-flac PKG@@mingw-w64-x86_64-libmad PKG@@mingw-w64-x86_64-libmikmod PKG@@mingw-w64-x86_64-libvorbis PKG@@mingw-w64-x86_64-sdl) ; @echo 'PKG@@mingw-w64-x86_64-sdl-mixer'

override VERSION_OF_mingw-w64-x86_64-sdl-net := 1.2.8-5
.PHONY: PKG@@mingw-w64-x86_64-SDL_net PKG@@mingw-w64-x86_64-sdl-net
PKG@@mingw-w64-x86_64-SDL_net PKG@@mingw-w64-x86_64-sdl-net: $(if $(__deps),PKG@@mingw-w64-x86_64-sdl) ; @echo 'PKG@@mingw-w64-x86_64-sdl-net'

override VERSION_OF_mingw-w64-x86_64-sdl-ttf := 2.0.11-8
.PHONY: PKG@@mingw-w64-x86_64-SDL_ttf PKG@@mingw-w64-x86_64-sdl-ttf
PKG@@mingw-w64-x86_64-SDL_ttf PKG@@mingw-w64-x86_64-sdl-ttf: $(if $(__deps),PKG@@mingw-w64-x86_64-SDL PKG@@mingw-w64-x86_64-freetype) ; @echo 'PKG@@mingw-w64-x86_64-sdl-ttf'

override VERSION_OF_mingw-w64-x86_64-sdl12-compat := 1.2.74-2
.PHONY: PKG@@mingw-w64-x86_64-sdl PKG@@mingw-w64-x86_64-sdl12-compat
PKG@@mingw-w64-x86_64-sdl PKG@@mingw-w64-x86_64-sdl12-compat: $(if $(__deps),PKG@@mingw-w64-x86_64-sdl2) ; @echo 'PKG@@mingw-w64-x86_64-sdl12-compat'

override VERSION_OF_mingw-w64-x86_64-sdl2-compat := 2.32.74-2
.PHONY: PKG@@mingw-w64-x86_64-sdl2 PKG@@mingw-w64-x86_64-sdl2-compat
PKG@@mingw-w64-x86_64-sdl2 PKG@@mingw-w64-x86_64-sdl2-compat: $(if $(__deps),PKG@@mingw-w64-x86_64-sdl3) ; @echo 'PKG@@mingw-w64-x86_64-sdl2-compat'

override VERSION_OF_mingw-w64-x86_64-sdl2-gfx := 1.0.4-3
.PHONY: PKG@@mingw-w64-x86_64-SDL2_gfx PKG@@mingw-w64-x86_64-sdl2-gfx
PKG@@mingw-w64-x86_64-SDL2_gfx PKG@@mingw-w64-x86_64-sdl2-gfx: $(if $(__deps),PKG@@mingw-w64-x86_64-sdl2) ; @echo 'PKG@@mingw-w64-x86_64-sdl2-gfx'

override VERSION_OF_mingw-w64-x86_64-sdl2-image := 2.8.12-2
.PHONY: PKG@@mingw-w64-x86_64-SDL2_image PKG@@mingw-w64-x86_64-sdl2-image
PKG@@mingw-w64-x86_64-SDL2_image PKG@@mingw-w64-x86_64-sdl2-image: $(if $(__deps),PKG@@mingw-w64-x86_64-libavif PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libjxl PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-sdl2) ; @echo 'PKG@@mingw-w64-x86_64-sdl2-image'

override VERSION_OF_mingw-w64-x86_64-sdl2-mixer := 2.8.2-2
.PHONY: PKG@@mingw-w64-x86_64-SDL2_mixer PKG@@mingw-w64-x86_64-sdl2-mixer
PKG@@mingw-w64-x86_64-SDL2_mixer PKG@@mingw-w64-x86_64-sdl2-mixer: $(if $(__deps),PKG@@mingw-w64-x86_64-flac PKG@@mingw-w64-x86_64-fluidsynth PKG@@mingw-w64-x86_64-libgme PKG@@mingw-w64-x86_64-libvorbis PKG@@mingw-w64-x86_64-libxmp PKG@@mingw-w64-x86_64-mpg123 PKG@@mingw-w64-x86_64-opusfile PKG@@mingw-w64-x86_64-sdl2 PKG@@mingw-w64-x86_64-wavpack) ; @echo 'PKG@@mingw-w64-x86_64-sdl2-mixer'

override VERSION_OF_mingw-w64-x86_64-sdl2-net := 2.4.0-2
.PHONY: PKG@@mingw-w64-x86_64-SDL2_net PKG@@mingw-w64-x86_64-sdl2-net
PKG@@mingw-w64-x86_64-SDL2_net PKG@@mingw-w64-x86_64-sdl2-net: $(if $(__deps),PKG@@mingw-w64-x86_64-sdl2) ; @echo 'PKG@@mingw-w64-x86_64-sdl2-net'

override VERSION_OF_mingw-w64-x86_64-sdl2-pango := 2.1.5-3
.PHONY: PKG@@mingw-w64-x86_64-SDL2_pango PKG@@mingw-w64-x86_64-sdl2-pango
PKG@@mingw-w64-x86_64-SDL2_pango PKG@@mingw-w64-x86_64-sdl2-pango: $(if $(__deps),PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-pango PKG@@mingw-w64-x86_64-sdl2) ; @echo 'PKG@@mingw-w64-x86_64-sdl2-pango'

override VERSION_OF_mingw-w64-x86_64-sdl2-sound := 2.0.6-2
.PHONY: PKG@@mingw-w64-x86_64-SDL2_sound PKG@@mingw-w64-x86_64-sdl2-sound
PKG@@mingw-w64-x86_64-SDL2_sound PKG@@mingw-w64-x86_64-sdl2-sound: $(if $(__deps),PKG@@mingw-w64-x86_64-sdl2) ; @echo 'PKG@@mingw-w64-x86_64-sdl2-sound'

override VERSION_OF_mingw-w64-x86_64-sdl2-ttf := 2.24.0-2
.PHONY: PKG@@mingw-w64-x86_64-SDL2_ttf PKG@@mingw-w64-x86_64-sdl2-ttf
PKG@@mingw-w64-x86_64-SDL2_ttf PKG@@mingw-w64-x86_64-sdl2-ttf: $(if $(__deps),PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-sdl2) ; @echo 'PKG@@mingw-w64-x86_64-sdl2-ttf'

override VERSION_OF_mingw-w64-x86_64-sdl3 := 3.4.20-1
.PHONY: PKG@@mingw-w64-x86_64-sdl3
PKG@@mingw-w64-x86_64-sdl3: $(if $(__deps),PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-vulkan) ; @echo 'PKG@@mingw-w64-x86_64-sdl3'

override VERSION_OF_mingw-w64-x86_64-sdl3-image := 3.4.8-1
.PHONY: PKG@@mingw-w64-x86_64-sdl3-image
PKG@@mingw-w64-x86_64-sdl3-image: $(if $(__deps),PKG@@mingw-w64-x86_64-libavif PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libjxl PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-sdl3) ; @echo 'PKG@@mingw-w64-x86_64-sdl3-image'

override VERSION_OF_mingw-w64-x86_64-sdl3-mixer := 3.2.4-1
.PHONY: PKG@@mingw-w64-x86_64-sdl3-mixer
PKG@@mingw-w64-x86_64-sdl3-mixer: $(if $(__deps),PKG@@mingw-w64-x86_64-flac PKG@@mingw-w64-x86_64-fluidsynth PKG@@mingw-w64-x86_64-libgme PKG@@mingw-w64-x86_64-libvorbis PKG@@mingw-w64-x86_64-libxmp PKG@@mingw-w64-x86_64-mpg123 PKG@@mingw-w64-x86_64-opusfile PKG@@mingw-w64-x86_64-sdl3 PKG@@mingw-w64-x86_64-wavpack) ; @echo 'PKG@@mingw-w64-x86_64-sdl3-mixer'

override VERSION_OF_mingw-w64-x86_64-sdl3-sound := 3.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-sdl3-sound
PKG@@mingw-w64-x86_64-sdl3-sound: $(if $(__deps),PKG@@mingw-w64-x86_64-sdl3) ; @echo 'PKG@@mingw-w64-x86_64-sdl3-sound'

override VERSION_OF_mingw-w64-x86_64-sdl3-ttf := 3.2.2-3
.PHONY: PKG@@mingw-w64-x86_64-sdl3-ttf
PKG@@mingw-w64-x86_64-sdl3-ttf: $(if $(__deps),PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-sdl3) ; @echo 'PKG@@mingw-w64-x86_64-sdl3-ttf'

override VERSION_OF_mingw-w64-x86_64-seacas := 2026.08.11-1
.PHONY: PKG@@mingw-w64-x86_64-seacas
PKG@@mingw-w64-x86_64-seacas: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-cgns PKG@@mingw-w64-x86_64-fmt PKG@@mingw-w64-x86_64-hdf5 PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-matio PKG@@mingw-w64-x86_64-metis PKG@@mingw-w64-x86_64-netcdf) ; @echo 'PKG@@mingw-w64-x86_64-seacas'

override VERSION_OF_mingw-w64-x86_64-seexpr := 2.11-5
.PHONY: PKG@@mingw-w64-x86_64-seexpr
PKG@@mingw-w64-x86_64-seexpr: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-seexpr'

override VERSION_OF_mingw-w64-x86_64-sfcgal := 2.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-sfcgal
PKG@@mingw-w64-x86_64-sfcgal: $(if $(__deps),PKG@@mingw-w64-x86_64-boost-libs PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-cgal PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-mpfr) ; @echo 'PKG@@mingw-w64-x86_64-sfcgal'

override VERSION_OF_mingw-w64-x86_64-sgml-common := 0.6.3-4
.PHONY: PKG@@mingw-w64-x86_64-sgml-common
PKG@@mingw-w64-x86_64-sgml-common: $(if $(__deps),PKG@@sh) ; @echo 'PKG@@mingw-w64-x86_64-sgml-common'

override VERSION_OF_mingw-w64-x86_64-shaderc := 2026.4-1
.PHONY: PKG@@mingw-w64-x86_64-shaderc
PKG@@mingw-w64-x86_64-shaderc: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-glslang PKG@@mingw-w64-x86_64-spirv-tools) ; @echo 'PKG@@mingw-w64-x86_64-shaderc'

override VERSION_OF_mingw-w64-x86_64-shared-mime-info := 2.5.1-1
.PHONY: PKG@@mingw-w64-x86_64-shared-mime-info
PKG@@mingw-w64-x86_64-shared-mime-info: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-shared-mime-info'

override VERSION_OF_mingw-w64-x86_64-shim_executable := 2.3.r1.g65d5d07-1
.PHONY: PKG@@mingw-w64-x86_64-shim_executable
PKG@@mingw-w64-x86_64-shim_executable: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-shim_executable'

override VERSION_OF_mingw-w64-x86_64-shine := 3.1.1-2
.PHONY: PKG@@mingw-w64-x86_64-shine
PKG@@mingw-w64-x86_64-shine: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-shine'

override VERSION_OF_mingw-w64-x86_64-shishi := 1.0.3-2
.PHONY: PKG@@mingw-w64-x86_64-shishi
PKG@@mingw-w64-x86_64-shishi: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-gnutls PKG@@mingw-w64-x86_64-libgcrypt PKG@@mingw-w64-x86_64-libgpg-error PKG@@mingw-w64-x86_64-libidn PKG@@mingw-w64-x86_64-libtasn1) ; @echo 'PKG@@mingw-w64-x86_64-shishi'

override VERSION_OF_mingw-w64-x86_64-silc-toolkit := 1.1.12-6
.PHONY: PKG@@mingw-w64-x86_64-silc-toolkit
PKG@@mingw-w64-x86_64-silc-toolkit: $(if $(__deps),PKG@@mingw-w64-x86_64-libsystre) ; @echo 'PKG@@mingw-w64-x86_64-silc-toolkit'

override VERSION_OF_mingw-w64-x86_64-simdjson := 5.1.1-1
.PHONY: PKG@@mingw-w64-x86_64-simdjson
PKG@@mingw-w64-x86_64-simdjson: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-simdjson'

override VERSION_OF_mingw-w64-x86_64-simple-mail := 3.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-simple-mail
PKG@@mingw-w64-x86_64-simple-mail: $(if $(__deps),PKG@@mingw-w64-x86_64-qt6-base) ; @echo 'PKG@@mingw-w64-x86_64-simple-mail'

override VERSION_OF_mingw-w64-x86_64-sip := 6.17.2-1
.PHONY: PKG@@mingw-w64-x86_64-sip
PKG@@mingw-w64-x86_64-sip: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-packaging PKG@@mingw-w64-x86_64-python-setuptools) ; @echo 'PKG@@mingw-w64-x86_64-sip'

override VERSION_OF_mingw-w64-x86_64-skia := 143.r77119.da51f0d6-5
.PHONY: PKG@@mingw-w64-x86_64-skia
PKG@@mingw-w64-x86_64-skia: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-skia'

override VERSION_OF_mingw-w64-x86_64-skyr-url := 1.13.0-4
.PHONY: PKG@@mingw-w64-x86_64-skyr-url
PKG@@mingw-w64-x86_64-skyr-url: $(if $(__deps),PKG@@mingw-w64-x86_64-tl-expected) ; @echo 'PKG@@mingw-w64-x86_64-skyr-url'

override VERSION_OF_mingw-w64-x86_64-sl := 5.05-1
.PHONY: PKG@@mingw-w64-x86_64-sl
PKG@@mingw-w64-x86_64-sl: $(if $(__deps),PKG@@mingw-w64-x86_64-ncurses) ; @echo 'PKG@@mingw-w64-x86_64-sl'

override VERSION_OF_mingw-w64-x86_64-smpeg := 0.4.5-7
.PHONY: PKG@@mingw-w64-x86_64-smpeg
PKG@@mingw-w64-x86_64-smpeg: $(if $(__deps),PKG@@mingw-w64-x86_64-SDL PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-smpeg'

override VERSION_OF_mingw-w64-x86_64-snappy := 1.3.1-1
.PHONY: PKG@@mingw-w64-x86_64-snappy
PKG@@mingw-w64-x86_64-snappy: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-snappy'

override VERSION_OF_mingw-w64-x86_64-snoregrowl := 0.5.0-4
.PHONY: PKG@@mingw-w64-x86_64-snoregrowl
PKG@@mingw-w64-x86_64-snoregrowl: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-snoregrowl'

override VERSION_OF_mingw-w64-x86_64-snorenotify := 0.7.0-5
.PHONY: PKG@@mingw-w64-x86_64-Snorenotify PKG@@mingw-w64-x86_64-snorenotify
PKG@@mingw-w64-x86_64-Snorenotify PKG@@mingw-w64-x86_64-snorenotify: $(if $(__deps),PKG@@mingw-w64-x86_64-qt5-base PKG@@mingw-w64-x86_64-snoregrowl) ; @echo 'PKG@@mingw-w64-x86_64-snorenotify'

override VERSION_OF_mingw-w64-x86_64-snowball := 3.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-snowball
PKG@@mingw-w64-x86_64-snowball: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-snowball'

override VERSION_OF_mingw-w64-x86_64-soil := 1.16.0-4
.PHONY: PKG@@mingw-w64-x86_64-soil
PKG@@mingw-w64-x86_64-soil: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-soil'

override VERSION_OF_mingw-w64-x86_64-sol2 := 3.5.0-2
.PHONY: PKG@@mingw-w64-x86_64-sol2
PKG@@mingw-w64-x86_64-sol2: $(if $(__deps),PKG@@mingw-w64-x86_64-lua54) ; @echo 'PKG@@mingw-w64-x86_64-sol2'

override VERSION_OF_mingw-w64-x86_64-solid3 := 3.5.8-3
.PHONY: PKG@@mingw-w64-x86_64-solid3
PKG@@mingw-w64-x86_64-solid3: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-solid3'

override VERSION_OF_mingw-w64-x86_64-sonivox := 4.0.2-1
.PHONY: PKG@@mingw-w64-x86_64-sonivox
PKG@@mingw-w64-x86_64-sonivox: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-sonivox'

override VERSION_OF_mingw-w64-x86_64-soundtouch := 2.4.1-1
.PHONY: PKG@@mingw-w64-x86_64-soundtouch
PKG@@mingw-w64-x86_64-soundtouch: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-soundtouch'

override VERSION_OF_mingw-w64-x86_64-source-highlight := 3.1.9-6
.PHONY: PKG@@mingw-w64-x86_64-source-highlight
PKG@@mingw-w64-x86_64-source-highlight: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-source-highlight'

override VERSION_OF_mingw-w64-x86_64-spandsp := 0.0.6-6
.PHONY: PKG@@mingw-w64-x86_64-spandsp
PKG@@mingw-w64-x86_64-spandsp: $(if $(__deps),PKG@@mingw-w64-x86_64-libtiff) ; @echo 'PKG@@mingw-w64-x86_64-spandsp'

override VERSION_OF_mingw-w64-x86_64-sparsehash := 2.0.4-2
.PHONY: PKG@@mingw-w64-x86_64-sparsehash
PKG@@mingw-w64-x86_64-sparsehash: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-sparsehash'

override VERSION_OF_mingw-w64-x86_64-spatialindex := 2.1.0-3
.PHONY: PKG@@mingw-w64-x86_64-spatialindex
PKG@@mingw-w64-x86_64-spatialindex: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-spatialindex'

override VERSION_OF_mingw-w64-x86_64-spdlog := 1.17.0-3
.PHONY: PKG@@mingw-w64-x86_64-spdlog
PKG@@mingw-w64-x86_64-spdlog: $(if $(__deps),PKG@@mingw-w64-x86_64-fmt) ; @echo 'PKG@@mingw-w64-x86_64-spdlog'

override VERSION_OF_mingw-w64-x86_64-spdylay := 1.4.0-5
.PHONY: PKG@@mingw-w64-x86_64-spdylay
PKG@@mingw-w64-x86_64-spdylay: $(if $(__deps),PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-spdylay'

override VERSION_OF_mingw-w64-x86_64-spectra := 1.2.0-1
.PHONY: PKG@@mingw-w64-x86_64-spectra
PKG@@mingw-w64-x86_64-spectra: $(if $(__deps),PKG@@mingw-w64-x86_64-eigen3) ; @echo 'PKG@@mingw-w64-x86_64-spectra'

override VERSION_OF_mingw-w64-x86_64-speex := 1.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-speex
PKG@@mingw-w64-x86_64-speex: $(if $(__deps),PKG@@mingw-w64-x86_64-libogg PKG@@mingw-w64-x86_64-speexdsp) ; @echo 'PKG@@mingw-w64-x86_64-speex'

override VERSION_OF_mingw-w64-x86_64-speexdsp := 1.2.1-2
.PHONY: PKG@@mingw-w64-x86_64-speexdsp
PKG@@mingw-w64-x86_64-speexdsp: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-speexdsp'

override VERSION_OF_mingw-w64-x86_64-spirv-cross := 1~1.4.363.0-1
.PHONY: PKG@@mingw-w64-x86_64-spirv-cross
PKG@@mingw-w64-x86_64-spirv-cross: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-spirv-cross'

override VERSION_OF_mingw-w64-x86_64-spirv-headers := 2~1.4.363.0-1
.PHONY: PKG@@mingw-w64-x86_64-spirv-headers
PKG@@mingw-w64-x86_64-spirv-headers: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-spirv-headers'

override VERSION_OF_mingw-w64-x86_64-spirv-llvm-translator := 23.1.2-1
.PHONY: PKG@@mingw-w64-x86_64-spirv-llvm-translator
PKG@@mingw-w64-x86_64-spirv-llvm-translator: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-llvm-libs PKG@@mingw-w64-x86_64-spirv-tools) ; @echo 'PKG@@mingw-w64-x86_64-spirv-llvm-translator'

override VERSION_OF_mingw-w64-x86_64-spirv-tools := 3~1.4.363.0-1
.PHONY: PKG@@mingw-w64-x86_64-spirv-tools
PKG@@mingw-w64-x86_64-spirv-tools: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-spirv-tools'

override VERSION_OF_mingw-w64-x86_64-spooles := 2.2-5
.PHONY: PKG@@mingw-w64-x86_64-spooles
PKG@@mingw-w64-x86_64-spooles: $(if $(__deps),PKG@@mingw-w64-x86_64-gcc-libgfortran) ; @echo 'PKG@@mingw-w64-x86_64-spooles'

override VERSION_OF_mingw-w64-x86_64-sqlite := 3.54.1-2
.PHONY: PKG@@mingw-w64-x86_64-sqlite PKG@@mingw-w64-x86_64-sqlite3
PKG@@mingw-w64-x86_64-sqlite PKG@@mingw-w64-x86_64-sqlite3: $(if $(__deps),PKG@@mingw-w64-x86_64-tcl PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-sqlite'

override VERSION_OF_mingw-w64-x86_64-sqlite-docs := 3.54.1-2
.PHONY: PKG@@mingw-w64-x86_64-sqlite-docs
PKG@@mingw-w64-x86_64-sqlite-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-sqlite-docs'

override VERSION_OF_mingw-w64-x86_64-sqlite-orm := 1.9.1-1
.PHONY: PKG@@mingw-w64-x86_64-sqlite-orm
PKG@@mingw-w64-x86_64-sqlite-orm: $(if $(__deps),PKG@@mingw-w64-x86_64-sqlite3) ; @echo 'PKG@@mingw-w64-x86_64-sqlite-orm'

override VERSION_OF_mingw-w64-x86_64-sqlite3mc := 2.2.6-1
.PHONY: PKG@@mingw-w64-x86_64-sqlite3mc
PKG@@mingw-w64-x86_64-sqlite3mc: $(if $(__deps),PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-sqlite3mc'

override VERSION_OF_mingw-w64-x86_64-srt := 1.5.7-1
.PHONY: PKG@@mingw-w64-x86_64-srt
PKG@@mingw-w64-x86_64-srt: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-srt'

override VERSION_OF_mingw-w64-x86_64-stk := 5.0.1-2
.PHONY: PKG@@mingw-w64-x86_64-stk
PKG@@mingw-w64-x86_64-stk: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-jack2 PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-stk'

override VERSION_OF_mingw-w64-x86_64-stoken := 0.92-5
.PHONY: PKG@@mingw-w64-x86_64-stoken
PKG@@mingw-w64-x86_64-stoken: $(if $(__deps),PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-nettle) ; @echo 'PKG@@mingw-w64-x86_64-stoken'

override VERSION_OF_mingw-w64-x86_64-stxxl := 1.4.1.387.gb9e44f0e-2
.PHONY: PKG@@mingw-w64-x86_64-stxxl PKG@@mingw-w64-x86_64-stxxl-git
PKG@@mingw-w64-x86_64-stxxl PKG@@mingw-w64-x86_64-stxxl-git: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-stxxl'

override VERSION_OF_mingw-w64-x86_64-styrene := 0.3.0-11
.PHONY: PKG@@mingw-w64-x86_64-styrene
PKG@@mingw-w64-x86_64-styrene: $(if $(__deps),PKG@@mingw-w64-x86_64-binutils PKG@@mingw-w64-x86_64-cc PKG@@mingw-w64-x86_64-nsis PKG@@mingw-w64-x86_64-python PKG@@zip) ; @echo 'PKG@@mingw-w64-x86_64-styrene'

override VERSION_OF_mingw-w64-x86_64-suitesparse := 7.14.1-1
.PHONY: PKG@@mingw-w64-x86_64-suitesparse
PKG@@mingw-w64-x86_64-suitesparse: $(if $(__deps),PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-mpfr PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-openblas) ; @echo 'PKG@@mingw-w64-x86_64-suitesparse'

override VERSION_OF_mingw-w64-x86_64-sundials := 7.5.0-2
.PHONY: PKG@@mingw-w64-x86_64-sundials
PKG@@mingw-w64-x86_64-sundials: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gcc-libgfortran PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-openblas PKG@@mingw-w64-x86_64-suitesparse) ; @echo 'PKG@@mingw-w64-x86_64-sundials'

override VERSION_OF_mingw-w64-x86_64-superlu := 7.0.1-1
.PHONY: PKG@@mingw-w64-x86_64-superlu
PKG@@mingw-w64-x86_64-superlu: $(if $(__deps),PKG@@mingw-w64-x86_64-openblas) ; @echo 'PKG@@mingw-w64-x86_64-superlu'

override VERSION_OF_mingw-w64-x86_64-svt-av1 := 4.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-svt-av1
PKG@@mingw-w64-x86_64-svt-av1: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-svt-av1'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-archive := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-archive
PKG@@mingw-w64-x86_64-swi-prolog-archive: $(if $(__deps),PKG@@mingw-w64-x86_64-libarchive PKG@@mingw-w64-x86_64-swi-prolog-core) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-archive'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-bdb := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-bdb
PKG@@mingw-w64-x86_64-swi-prolog-bdb: $(if $(__deps),PKG@@mingw-w64-x86_64-db PKG@@mingw-w64-x86_64-swi-prolog-core) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-bdb'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-core := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-core
PKG@@mingw-w64-x86_64-swi-prolog-core: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gmp PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-core'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-doc := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-doc
PKG@@mingw-w64-x86_64-swi-prolog-doc: $(if $(__deps),PKG@@mingw-w64-x86_64-swi-prolog-core PKG@@mingw-w64-x86_64-swi-prolog-packages) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-doc'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-edit := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-edit
PKG@@mingw-w64-x86_64-swi-prolog-edit: $(if $(__deps),PKG@@mingw-w64-x86_64-swi-prolog-core) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-edit'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-examples := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-examples
PKG@@mingw-w64-x86_64-swi-prolog-examples: $(if $(__deps),PKG@@mingw-w64-x86_64-swi-prolog-core PKG@@mingw-w64-x86_64-swi-prolog-packages) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-examples'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-full := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog PKG@@mingw-w64-x86_64-swi-prolog-full
PKG@@mingw-w64-x86_64-swi-prolog PKG@@mingw-w64-x86_64-swi-prolog-full: $(if $(__deps),PKG@@mingw-w64-x86_64-swi-prolog-archive PKG@@mingw-w64-x86_64-swi-prolog-bdb PKG@@mingw-w64-x86_64-swi-prolog-core PKG@@mingw-w64-x86_64-swi-prolog-doc PKG@@mingw-w64-x86_64-swi-prolog-edit PKG@@mingw-w64-x86_64-swi-prolog-examples PKG@@mingw-w64-x86_64-swi-prolog-json PKG@@mingw-w64-x86_64-swi-prolog-odbc PKG@@mingw-w64-x86_64-swi-prolog-packages PKG@@mingw-w64-x86_64-swi-prolog-pcre2 PKG@@mingw-w64-x86_64-swi-prolog-python PKG@@mingw-w64-x86_64-swi-prolog-ssl PKG@@mingw-w64-x86_64-swi-prolog-tests PKG@@mingw-w64-x86_64-swi-prolog-x PKG@@mingw-w64-x86_64-swi-prolog-yaml) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-full'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-json := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-json
PKG@@mingw-w64-x86_64-swi-prolog-json: $(if $(__deps),PKG@@mingw-w64-x86_64-swi-prolog-core) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-json'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-odbc := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-odbc
PKG@@mingw-w64-x86_64-swi-prolog-odbc: $(if $(__deps),PKG@@mingw-w64-x86_64-swi-prolog-core PKG@@mingw-w64-x86_64-swi-prolog-packages PKG@@mingw-w64-x86_64-unixodbc) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-odbc'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-packages := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-packages
PKG@@mingw-w64-x86_64-swi-prolog-packages: $(if $(__deps),PKG@@mingw-w64-x86_64-swi-prolog-core) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-packages'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-pcre2 := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-pcre2
PKG@@mingw-w64-x86_64-swi-prolog-pcre2: $(if $(__deps),PKG@@mingw-w64-x86_64-pcre2 PKG@@mingw-w64-x86_64-swi-prolog-core) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-pcre2'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-python := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-python
PKG@@mingw-w64-x86_64-swi-prolog-python: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-swi-prolog-core) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-python'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-ssl := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-ssl
PKG@@mingw-w64-x86_64-swi-prolog-ssl: $(if $(__deps),PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-swi-prolog-core PKG@@mingw-w64-x86_64-swi-prolog-packages) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-ssl'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-tests := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-tests
PKG@@mingw-w64-x86_64-swi-prolog-tests: $(if $(__deps),PKG@@mingw-w64-x86_64-swi-prolog-core PKG@@mingw-w64-x86_64-swi-prolog-packages) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-tests'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-x := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-x
PKG@@mingw-w64-x86_64-swi-prolog-x: $(if $(__deps),PKG@@mingw-w64-x86_64-cairo PKG@@mingw-w64-x86_64-openjpeg2 PKG@@mingw-w64-x86_64-pango PKG@@mingw-w64-x86_64-sdl3-image PKG@@mingw-w64-x86_64-swi-prolog-core) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-x'

override VERSION_OF_mingw-w64-x86_64-swi-prolog-yaml := 10.0.2-3
.PHONY: PKG@@mingw-w64-x86_64-swi-prolog-yaml
PKG@@mingw-w64-x86_64-swi-prolog-yaml: $(if $(__deps),PKG@@mingw-w64-x86_64-libyaml PKG@@mingw-w64-x86_64-swi-prolog-core) ; @echo 'PKG@@mingw-w64-x86_64-swi-prolog-yaml'

override VERSION_OF_mingw-w64-x86_64-swig := 4.5.1-1
.PHONY: PKG@@mingw-w64-x86_64-swig
PKG@@mingw-w64-x86_64-swig: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-pcre2) ; @echo 'PKG@@mingw-w64-x86_64-swig'

override VERSION_OF_mingw-w64-x86_64-systemc := 3.0.2-1
.PHONY: PKG@@mingw-w64-x86_64-systemc
PKG@@mingw-w64-x86_64-systemc: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-systemc'

override VERSION_OF_mingw-w64-x86_64-t1utils := 1.42-2
.PHONY: PKG@@mingw-w64-x86_64-t1utils
PKG@@mingw-w64-x86_64-t1utils: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-t1utils'

override VERSION_OF_mingw-w64-x86_64-taglib := 2.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-taglib
PKG@@mingw-w64-x86_64-taglib: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-taglib'

override VERSION_OF_mingw-w64-x86_64-taucs := 2.2.1-2
.PHONY: PKG@@mingw-w64-x86_64-taucs
PKG@@mingw-w64-x86_64-taucs: $(if $(__deps),PKG@@mingw-w64-x86_64-gcc-libgfortran PKG@@mingw-w64-x86_64-metis PKG@@mingw-w64-x86_64-openblas) ; @echo 'PKG@@mingw-w64-x86_64-taucs'

override VERSION_OF_mingw-w64-x86_64-tbb := 2023.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-tbb
PKG@@mingw-w64-x86_64-tbb: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-hwloc) ; @echo 'PKG@@mingw-w64-x86_64-tbb'

override VERSION_OF_mingw-w64-x86_64-tcl := 8.6.18-1
.PHONY: PKG@@mingw-w64-x86_64-tcl
PKG@@mingw-w64-x86_64-tcl: $(if $(__deps),PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-tcl'

override VERSION_OF_mingw-w64-x86_64-tcllib := 2.0-1
.PHONY: PKG@@mingw-w64-x86_64-tcllib
PKG@@mingw-w64-x86_64-tcllib: $(if $(__deps),PKG@@mingw-w64-x86_64-tcl) ; @echo 'PKG@@mingw-w64-x86_64-tcllib'

override VERSION_OF_mingw-w64-x86_64-tclvfs-cvs := 20130425-4
.PHONY: PKG@@mingw-w64-x86_64-tclvfs PKG@@mingw-w64-x86_64-tclvfs-cvs
PKG@@mingw-w64-x86_64-tclvfs PKG@@mingw-w64-x86_64-tclvfs-cvs: $(if $(__deps),PKG@@mingw-w64-x86_64-tcl) ; @echo 'PKG@@mingw-w64-x86_64-tclvfs-cvs'

override VERSION_OF_mingw-w64-x86_64-tclx := 8.6.3-1
.PHONY: PKG@@mingw-w64-x86_64-tclx
PKG@@mingw-w64-x86_64-tclx: $(if $(__deps),PKG@@mingw-w64-x86_64-tcl) ; @echo 'PKG@@mingw-w64-x86_64-tclx'

override VERSION_OF_mingw-w64-x86_64-template-glib := 3.36.3-2
.PHONY: PKG@@mingw-w64-x86_64-template-glib
PKG@@mingw-w64-x86_64-template-glib: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-gobject-introspection-runtime) ; @echo 'PKG@@mingw-w64-x86_64-template-glib'

override VERSION_OF_mingw-w64-x86_64-termcap := 1.3.1-8
.PHONY: PKG@@mingw-w64-x86_64-termcap
PKG@@mingw-w64-x86_64-termcap: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-termcap'

override VERSION_OF_mingw-w64-x86_64-tesseract-ocr := 5.5.4-1
.PHONY: PKG@@mingw-w64-x86_64-tesseract-ocr
PKG@@mingw-w64-x86_64-tesseract-ocr: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-leptonica PKG@@mingw-w64-x86_64-libarchive PKG@@mingw-w64-x86_64-omp) ; @echo 'PKG@@mingw-w64-x86_64-tesseract-ocr'

override VERSION_OF_mingw-w64-x86_64-testu01 := 1.2.3-1
.PHONY: PKG@@mingw-w64-x86_64-testu01
PKG@@mingw-w64-x86_64-testu01: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-testu01'

override VERSION_OF_mingw-w64-x86_64-tetgen := 1.6.1-1
.PHONY: PKG@@mingw-w64-x86_64-tetgen
PKG@@mingw-w64-x86_64-tetgen: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-tetgen'

override VERSION_OF_mingw-w64-x86_64-texinfo := 7.3-2
.PHONY: PKG@@mingw-w64-x86_64-texinfo
PKG@@mingw-w64-x86_64-texinfo: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-perl) ; @echo 'PKG@@mingw-w64-x86_64-texinfo'

override VERSION_OF_mingw-w64-x86_64-tgui := 1.13.0-1
.PHONY: PKG@@mingw-w64-x86_64-tgui
PKG@@mingw-w64-x86_64-tgui: $(if $(__deps),PKG@@mingw-w64-x86_64-sdl3 PKG@@mingw-w64-x86_64-sdl3-ttf) ; @echo 'PKG@@mingw-w64-x86_64-tgui'

override VERSION_OF_mingw-w64-x86_64-thorvg := 1.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-thorvg
PKG@@mingw-w64-x86_64-thorvg: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libjpeg-turbo PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libwebp PKG@@mingw-w64-x86_64-omp) ; @echo 'PKG@@mingw-w64-x86_64-thorvg'

override VERSION_OF_mingw-w64-x86_64-thrift := 0.25.0-1
.PHONY: PKG@@mingw-w64-x86_64-thrift
PKG@@mingw-w64-x86_64-thrift: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-openssl PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-thrift'

override VERSION_OF_mingw-w64-x86_64-tiledb := 2.27.0-8
.PHONY: PKG@@mingw-w64-x86_64-tiledb
PKG@@mingw-w64-x86_64-tiledb: $(if $(__deps),PKG@@mingw-w64-x86_64-bzip2 PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-file PKG@@mingw-w64-x86_64-fmt PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-lz4 PKG@@mingw-w64-x86_64-spdlog PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-tiledb'

override VERSION_OF_mingw-w64-x86_64-tinyexpr := 1.1.1-1
.PHONY: PKG@@mingw-w64-x86_64-tinyexpr
PKG@@mingw-w64-x86_64-tinyexpr: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-tinyexpr'

override VERSION_OF_mingw-w64-x86_64-tinyformat := 2.3.0-2
.PHONY: PKG@@mingw-w64-x86_64-tinyformat
PKG@@mingw-w64-x86_64-tinyformat: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-tinyformat'

override VERSION_OF_mingw-w64-x86_64-tinyxml := 2.6.2-6
.PHONY: PKG@@mingw-w64-x86_64-tinyxml
PKG@@mingw-w64-x86_64-tinyxml: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-tinyxml'

override VERSION_OF_mingw-w64-x86_64-tinyxml2 := 11.0.0-2
.PHONY: PKG@@mingw-w64-x86_64-tinyxml2
PKG@@mingw-w64-x86_64-tinyxml2: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-tinyxml2'

override VERSION_OF_mingw-w64-x86_64-tixi := 3.3.0-5
.PHONY: PKG@@mingw-w64-x86_64-tixi PKG@@mingw-w64-x86_64-tixi3
PKG@@mingw-w64-x86_64-tixi PKG@@mingw-w64-x86_64-tixi3: $(if $(__deps),PKG@@mingw-w64-x86_64-curl PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-libxslt) ; @echo 'PKG@@mingw-w64-x86_64-tixi'

override VERSION_OF_mingw-w64-x86_64-tk := 8.6.18-1
.PHONY: PKG@@mingw-w64-x86_64-tk
PKG@@mingw-w64-x86_64-tk: $(if $(__deps),PKG@@mingw-w64-x86_64-tcl) ; @echo 'PKG@@mingw-w64-x86_64-tk'

override VERSION_OF_mingw-w64-x86_64-tklib := 0.9.0+2.8e1164d322-1
.PHONY: PKG@@mingw-w64-x86_64-tklib
PKG@@mingw-w64-x86_64-tklib: $(if $(__deps),PKG@@mingw-w64-x86_64-tcllib PKG@@mingw-w64-x86_64-tk) ; @echo 'PKG@@mingw-w64-x86_64-tklib'

override VERSION_OF_mingw-w64-x86_64-tktable := 2.12.1-1
.PHONY: PKG@@mingw-w64-x86_64-tktable
PKG@@mingw-w64-x86_64-tktable: $(if $(__deps),PKG@@mingw-w64-x86_64-tk) ; @echo 'PKG@@mingw-w64-x86_64-tktable'

override VERSION_OF_mingw-w64-x86_64-tl-expected := 1.3.1-1
.PHONY: PKG@@mingw-w64-x86_64-tl-expected
PKG@@mingw-w64-x86_64-tl-expected: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-tl-expected'

override VERSION_OF_mingw-w64-x86_64-tmxlite := 1.4.5-1
.PHONY: PKG@@mingw-w64-x86_64-tmxlite
PKG@@mingw-w64-x86_64-tmxlite: $(if $(__deps),PKG@@mingw-w64-x86_64-pugixml PKG@@mingw-w64-x86_64-zlib PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-tmxlite'

override VERSION_OF_mingw-w64-x86_64-tmxlite-docs := 1.4.5-1
.PHONY: PKG@@mingw-w64-x86_64-tmxlite-docs
PKG@@mingw-w64-x86_64-tmxlite-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-tmxlite-docs'

override VERSION_OF_mingw-w64-x86_64-toml11 := 4.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-toml11
PKG@@mingw-w64-x86_64-toml11: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-toml11'

override VERSION_OF_mingw-w64-x86_64-tomlc17 := 250712.r7.ge34eecb-1
.PHONY: PKG@@mingw-w64-x86_64-tomlc17
PKG@@mingw-w64-x86_64-tomlc17: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-tomlc17'

override VERSION_OF_mingw-w64-x86_64-tomlplusplus := 3.4.0-3
.PHONY: PKG@@mingw-w64-x86_64-tomlplusplus
PKG@@mingw-w64-x86_64-tomlplusplus: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-tomlplusplus'

override VERSION_OF_mingw-w64-x86_64-tools-xctest := 0.1.1-6
.PHONY: PKG@@mingw-w64-x86_64-tools-xctest
PKG@@mingw-w64-x86_64-tools-xctest: $(if $(__deps),PKG@@mingw-w64-x86_64-gnustep-base) ; @echo 'PKG@@mingw-w64-x86_64-tools-xctest'

override VERSION_OF_mingw-w64-x86_64-treedec := 0.9.3-2
.PHONY: PKG@@mingw-w64-x86_64-tdlib PKG@@mingw-w64-x86_64-treedec
PKG@@mingw-w64-x86_64-tdlib PKG@@mingw-w64-x86_64-treedec: $(if $(__deps),PKG@@mingw-w64-x86_64-boost) ; @echo 'PKG@@mingw-w64-x86_64-treedec'

override VERSION_OF_mingw-w64-x86_64-trompeloeil := 49-1
.PHONY: PKG@@mingw-w64-x86_64-trompeloeil
PKG@@mingw-w64-x86_64-trompeloeil: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-trompeloeil'

override VERSION_OF_mingw-w64-x86_64-tslib := 1.24-1
.PHONY: PKG@@mingw-w64-x86_64-tslib
PKG@@mingw-w64-x86_64-tslib: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-tslib'

override VERSION_OF_mingw-w64-x86_64-ttf-cm-unicode := 0.7.0-3
.PHONY: PKG@@mingw-w64-x86_64-ttf-cm-unicode
PKG@@mingw-w64-x86_64-ttf-cm-unicode: $(if $(__deps),PKG@@mingw-w64-x86_64-fontconfig) ; @echo 'PKG@@mingw-w64-x86_64-ttf-cm-unicode'

override VERSION_OF_mingw-w64-x86_64-ttf-dejavu := 2.37-4
.PHONY: PKG@@mingw-w64-x86_64-ttf-dejavu
PKG@@mingw-w64-x86_64-ttf-dejavu: $(if $(__deps),PKG@@mingw-w64-x86_64-fontconfig) ; @echo 'PKG@@mingw-w64-x86_64-ttf-dejavu'

override VERSION_OF_mingw-w64-x86_64-ttf-paratype := 2.005-1
.PHONY: PKG@@mingw-w64-x86_64-ttf-paratype
PKG@@mingw-w64-x86_64-ttf-paratype: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-ttf-paratype'

override VERSION_OF_mingw-w64-x86_64-ttfautohint := 1.8.4-2
.PHONY: PKG@@mingw-w64-x86_64-ttfautohint
PKG@@mingw-w64-x86_64-ttfautohint: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-harfbuzz PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-qt5-base) ; @echo 'PKG@@mingw-w64-x86_64-ttfautohint'

override VERSION_OF_mingw-w64-x86_64-twapi := 5.2-1
.PHONY: PKG@@mingw-w64-x86_64-twapi
PKG@@mingw-w64-x86_64-twapi: $(if $(__deps),PKG@@mingw-w64-x86_64-tcl) ; @echo 'PKG@@mingw-w64-x86_64-twapi'

override VERSION_OF_mingw-w64-x86_64-tzdata := 2026f-1
.PHONY: PKG@@mingw-w64-x86_64-tzdata
PKG@@mingw-w64-x86_64-tzdata: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-tzdata'

override VERSION_OF_mingw-w64-x86_64-uasm := 2.58-1
.PHONY: PKG@@mingw-w64-x86_64-uasm
PKG@@mingw-w64-x86_64-uasm: $(if $(__deps),PKG@@mingw-w64-x86_64-cc) ; @echo 'PKG@@mingw-w64-x86_64-uasm'

override VERSION_OF_mingw-w64-x86_64-uchardet := 0.0.8-2
.PHONY: PKG@@mingw-w64-x86_64-uchardet
PKG@@mingw-w64-x86_64-uchardet: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-uchardet'

override VERSION_OF_mingw-w64-x86_64-ucl := 1.03-3
.PHONY: PKG@@mingw-w64-x86_64-ucl
PKG@@mingw-w64-x86_64-ucl: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-ucl'

override VERSION_OF_mingw-w64-x86_64-udunits := 2.2.28-2
.PHONY: PKG@@mingw-w64-x86_64-udunits
PKG@@mingw-w64-x86_64-udunits: $(if $(__deps),PKG@@mingw-w64-x86_64-expat) ; @echo 'PKG@@mingw-w64-x86_64-udunits'

override VERSION_OF_mingw-w64-x86_64-uhttpmock := 0.11.0-2
.PHONY: PKG@@mingw-w64-x86_64-uhttpmock
PKG@@mingw-w64-x86_64-uhttpmock: $(if $(__deps),PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-libsoup3) ; @echo 'PKG@@mingw-w64-x86_64-uhttpmock'

override VERSION_OF_mingw-w64-x86_64-unbound := 1.26.2-1
.PHONY: PKG@@mingw-w64-x86_64-unbound
PKG@@mingw-w64-x86_64-unbound: $(if $(__deps),PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-ldns PKG@@mingw-w64-x86_64-openssl) ; @echo 'PKG@@mingw-w64-x86_64-unbound'

override VERSION_OF_mingw-w64-x86_64-uncrustify := 0.83.0-1
.PHONY: PKG@@mingw-w64-x86_64-uncrustify
PKG@@mingw-w64-x86_64-uncrustify: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-uncrustify'

override VERSION_OF_mingw-w64-x86_64-unibilium := 2.1.2-1
.PHONY: PKG@@mingw-w64-x86_64-unibilium
PKG@@mingw-w64-x86_64-unibilium: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-unibilium'

override VERSION_OF_mingw-w64-x86_64-unicode-character-database := 18.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-unicode-character-database
PKG@@mingw-w64-x86_64-unicode-character-database: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-unicode-character-database'

override VERSION_OF_mingw-w64-x86_64-unifdef := 2.12-1
.PHONY: PKG@@mingw-w64-x86_64-unifdef
PKG@@mingw-w64-x86_64-unifdef: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-unifdef'

override VERSION_OF_mingw-w64-x86_64-unittest-cpp := 2.0.0-3
.PHONY: PKG@@mingw-w64-x86_64-unittest-cpp
PKG@@mingw-w64-x86_64-unittest-cpp: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-unittest-cpp'

override VERSION_OF_mingw-w64-x86_64-unixodbc := 2.3.14-1
.PHONY: PKG@@mingw-w64-x86_64-unixodbc
PKG@@mingw-w64-x86_64-unixodbc: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libltdl PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-readline) ; @echo 'PKG@@mingw-w64-x86_64-unixodbc'

override VERSION_OF_mingw-w64-x86_64-unrar := 7.3.2-1
.PHONY: PKG@@mingw-w64-x86_64-unrar
PKG@@mingw-w64-x86_64-unrar: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-unrar'

override VERSION_OF_mingw-w64-x86_64-upx := 5.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-upx
PKG@@mingw-w64-x86_64-upx: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-upx'

override VERSION_OF_mingw-w64-x86_64-uriparser := 1.0.3-1
.PHONY: PKG@@mingw-w64-x86_64-uriparser
PKG@@mingw-w64-x86_64-uriparser: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-uriparser'

override VERSION_OF_mingw-w64-x86_64-usrsctp := 0.9.5.0-3
.PHONY: PKG@@mingw-w64-x86_64-usrsctp
PKG@@mingw-w64-x86_64-usrsctp: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-usrsctp'

override VERSION_OF_mingw-w64-x86_64-utf8cpp := 4.2.1-1
.PHONY: PKG@@mingw-w64-x86_64-utf8cpp
PKG@@mingw-w64-x86_64-utf8cpp: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-utf8cpp'

override VERSION_OF_mingw-w64-x86_64-uthash := 2.4.0-1
.PHONY: PKG@@mingw-w64-x86_64-uthash
PKG@@mingw-w64-x86_64-uthash: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-uthash'

override VERSION_OF_mingw-w64-x86_64-uv := 0.13.7-2
.PHONY: PKG@@mingw-w64-x86_64-uv
PKG@@mingw-w64-x86_64-uv: $(if $(__deps),PKG@@mingw-w64-x86_64-zstd) ; @echo 'PKG@@mingw-w64-x86_64-uv'

override VERSION_OF_mingw-w64-x86_64-uvatlas := 1.9.1-1
.PHONY: PKG@@mingw-w64-x86_64-uvatlas
PKG@@mingw-w64-x86_64-uvatlas: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-directx-headers PKG@@mingw-w64-x86_64-directxmath PKG@@mingw-w64-x86_64-directxmesh PKG@@mingw-w64-x86_64-eigen3 PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-spectra) ; @echo 'PKG@@mingw-w64-x86_64-uvatlas'

override VERSION_OF_mingw-w64-x86_64-uvicorn := 0.54.0-1
.PHONY: PKG@@mingw-w64-x86_64-uvicorn
PKG@@mingw-w64-x86_64-uvicorn: $(if $(__deps),PKG@@mingw-w64-x86_64-python PKG@@mingw-w64-x86_64-python-click PKG@@mingw-w64-x86_64-python-h11) ; @echo 'PKG@@mingw-w64-x86_64-uvicorn'

override VERSION_OF_mingw-w64-x86_64-vala := 0.56.19-3
.PHONY: PKG@@mingw-w64-x86_64-vala
PKG@@mingw-w64-x86_64-vala: $(if $(__deps),PKG@@mingw-w64-x86_64-cc PKG@@mingw-w64-x86_64-glib2 PKG@@mingw-w64-x86_64-graphviz PKG@@mingw-w64-x86_64-gtk-doc PKG@@mingw-w64-x86_64-pkgconf) ; @echo 'PKG@@mingw-w64-x86_64-vala'

override VERSION_OF_mingw-w64-x86_64-vid.stab := 1.1.2-1
.PHONY: PKG@@mingw-w64-x86_64-vid.stab
PKG@@mingw-w64-x86_64-vid.stab: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-omp) ; @echo 'PKG@@mingw-w64-x86_64-vid.stab'

override VERSION_OF_mingw-w64-x86_64-vtk := 9.6.2-8
.PHONY: PKG@@mingw-w64-x86_64-vtk
PKG@@mingw-w64-x86_64-vtk: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-double-conversion PKG@@mingw-w64-x86_64-expat PKG@@mingw-w64-x86_64-fmt PKG@@mingw-w64-x86_64-freetype PKG@@mingw-w64-x86_64-glew PKG@@mingw-w64-x86_64-jsoncpp PKG@@mingw-w64-x86_64-libarchive PKG@@mingw-w64-x86_64-libharu PKG@@mingw-w64-x86_64-libjpeg PKG@@mingw-w64-x86_64-libtiff PKG@@mingw-w64-x86_64-libwinpthread PKG@@mingw-w64-x86_64-lz4 PKG@@mingw-w64-x86_64-omp PKG@@mingw-w64-x86_64-openxr-sdk PKG@@mingw-w64-x86_64-pugixml PKG@@mingw-w64-x86_64-tbb PKG@@mingw-w64-x86_64-xz PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-vtk'

override VERSION_OF_mingw-w64-x86_64-vulkan-headers := 1~1.4.363.0-1
.PHONY: PKG@@mingw-w64-x86_64-vulkan-headers
PKG@@mingw-w64-x86_64-vulkan-headers: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-vulkan-headers'

override VERSION_OF_mingw-w64-x86_64-vulkan-loader := 1~1.4.363.0-1
.PHONY: PKG@@mingw-w64-x86_64-vulkan PKG@@mingw-w64-x86_64-vulkan-loader
PKG@@mingw-w64-x86_64-vulkan PKG@@mingw-w64-x86_64-vulkan-loader: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-vulkan-loader'

override VERSION_OF_mingw-w64-x86_64-vulkan-utility-libraries := 1.4.363.0-1
.PHONY: PKG@@mingw-w64-x86_64-vulkan-utility-libraries
PKG@@mingw-w64-x86_64-vulkan-utility-libraries: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-vulkan-utility-libraries'

override VERSION_OF_mingw-w64-x86_64-vulkan-validation-layers := 1.4.363.0-1
.PHONY: PKG@@mingw-w64-x86_64-vulkan-validation-layers
PKG@@mingw-w64-x86_64-vulkan-validation-layers: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-spirv-tools PKG@@mingw-w64-x86_64-vulkan-loader PKG@@mingw-w64-x86_64-vulkan-utility-libraries) ; @echo 'PKG@@mingw-w64-x86_64-vulkan-validation-layers'

override VERSION_OF_mingw-w64-x86_64-w3c-mathml2 := 2.0-4
.PHONY: PKG@@mingw-w64-x86_64-w3c-mathml2
PKG@@mingw-w64-x86_64-w3c-mathml2: $(if $(__deps),PKG@@mingw-w64-x86_64-libxml2) ; @echo 'PKG@@mingw-w64-x86_64-w3c-mathml2'

override VERSION_OF_mingw-w64-x86_64-wavpack := 5.9.0-1
.PHONY: PKG@@mingw-w64-x86_64-wavpack
PKG@@mingw-w64-x86_64-wavpack: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-wavpack'

override VERSION_OF_mingw-w64-x86_64-webp-pixbuf-loader := 0.2.7-2
.PHONY: PKG@@mingw-w64-x86_64-webp-pixbuf-loader
PKG@@mingw-w64-x86_64-webp-pixbuf-loader: $(if $(__deps),PKG@@mingw-w64-x86_64-gdk-pixbuf2 PKG@@mingw-w64-x86_64-libwebp) ; @echo 'PKG@@mingw-w64-x86_64-webp-pixbuf-loader'

override VERSION_OF_mingw-w64-x86_64-wget := 1.25.0-2
.PHONY: PKG@@mingw-w64-x86_64-wget
PKG@@mingw-w64-x86_64-wget: $(if $(__deps),PKG@@mingw-w64-x86_64-gnutls PKG@@mingw-w64-x86_64-libidn2 PKG@@mingw-w64-x86_64-pcre2) ; @echo 'PKG@@mingw-w64-x86_64-wget'

override VERSION_OF_mingw-w64-x86_64-whisper.cpp := 1~1.9.5.r258.g4da27c41c-1
.PHONY: PKG@@mingw-w64-x86_64-whisper.cpp
PKG@@mingw-w64-x86_64-whisper.cpp: $(if $(__deps),PKG@@mingw-w64-x86_64-SDL2 PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-ggml PKG@@mingw-w64-x86_64-llama.cpp) ; @echo 'PKG@@mingw-w64-x86_64-whisper.cpp'

override VERSION_OF_mingw-w64-x86_64-win7appid := 1.1-6
.PHONY: PKG@@mingw-w64-x86_64-win7appid
PKG@@mingw-w64-x86_64-win7appid: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-win7appid'

override VERSION_OF_mingw-w64-x86_64-windows-default-manifest := 20260815-1
.PHONY: PKG@@mingw-w64-x86_64-windows-default-manifest
PKG@@mingw-w64-x86_64-windows-default-manifest: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-windows-default-manifest'

override VERSION_OF_mingw-w64-x86_64-wineditline := 2.208-1
.PHONY: PKG@@mingw-w64-x86_64-wineditline
PKG@@mingw-w64-x86_64-wineditline: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-wineditline'

override VERSION_OF_mingw-w64-x86_64-winfile := 10.2.0.0-4
.PHONY: PKG@@mingw-w64-x86_64-winfile
PKG@@mingw-w64-x86_64-winfile: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-winfile'

override VERSION_OF_mingw-w64-x86_64-winico := 0.6-5
.PHONY: PKG@@mingw-w64-x86_64-winico
PKG@@mingw-w64-x86_64-winico: $(if $(__deps),PKG@@mingw-w64-x86_64-tk) ; @echo 'PKG@@mingw-w64-x86_64-winico'

override VERSION_OF_mingw-w64-x86_64-winpthreads := 14.0.0.r598.ge3d18511f-1
.PHONY: PKG@@mingw-w64-x86_64-winpthreads PKG@@mingw-w64-x86_64-winpthreads-git
PKG@@mingw-w64-x86_64-winpthreads PKG@@mingw-w64-x86_64-winpthreads-git: $(if $(__deps),PKG@@mingw-w64-x86_64-crt PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-winpthreads'

override VERSION_OF_mingw-w64-x86_64-winpthreads-stub := 1.0.0-1
.PHONY: PKG@@mingw-w64-x86_64-winpthreads-stub
PKG@@mingw-w64-x86_64-winpthreads-stub: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-winpthreads-stub'

override VERSION_OF_mingw-w64-x86_64-winpty := 0.4.3-4
.PHONY: PKG@@mingw-w64-x86_64-winpty
PKG@@mingw-w64-x86_64-winpty: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-winpty'

override VERSION_OF_mingw-w64-x86_64-wintab-sdk := 1.4-3
.PHONY: PKG@@mingw-w64-x86_64-wintab-sdk
PKG@@mingw-w64-x86_64-wintab-sdk: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-wintab-sdk'

override VERSION_OF_mingw-w64-x86_64-woff2 := 1.0.2-5
.PHONY: PKG@@mingw-w64-x86_64-woff2
PKG@@mingw-w64-x86_64-woff2: $(if $(__deps),PKG@@mingw-w64-x86_64-brotli PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-woff2'

override VERSION_OF_mingw-w64-x86_64-wolfssl := 5.9.4-1
.PHONY: PKG@@mingw-w64-x86_64-wolfssl
PKG@@mingw-w64-x86_64-wolfssl: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-wolfssl'

override VERSION_OF_mingw-w64-x86_64-x264 := 0.165.r3222.b35605a-3
.PHONY: PKG@@mingw-w64-x86_64-x264 PKG@@mingw-w64-x86_64-x264-git
PKG@@mingw-w64-x86_64-x264 PKG@@mingw-w64-x86_64-x264-git: $(if $(__deps),PKG@@mingw-w64-x86_64-ffmpeg PKG@@mingw-w64-x86_64-ffms2 PKG@@mingw-w64-x86_64-l-smash PKG@@mingw-w64-x86_64-libx264) ; @echo 'PKG@@mingw-w64-x86_64-x264'

override VERSION_OF_mingw-w64-x86_64-x265 := 4.3-1
.PHONY: PKG@@mingw-w64-x86_64-x265
PKG@@mingw-w64-x86_64-x265: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-x265'

override VERSION_OF_mingw-w64-x86_64-xapian-core := 1~2.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-xapian-core
PKG@@mingw-w64-x86_64-xapian-core: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-icu PKG@@mingw-w64-x86_64-zlib) ; @echo 'PKG@@mingw-w64-x86_64-xapian-core'

override VERSION_OF_mingw-w64-x86_64-xapian-core-docs := 1~2.1.0-1
.PHONY: PKG@@mingw-w64-x86_64-xapian-core-docs
PKG@@mingw-w64-x86_64-xapian-core-docs: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-xapian-core-docs'

override VERSION_OF_mingw-w64-x86_64-xerces-c := 3.3.0-4
.PHONY: PKG@@mingw-w64-x86_64-xerces-c
PKG@@mingw-w64-x86_64-xerces-c: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-icu) ; @echo 'PKG@@mingw-w64-x86_64-xerces-c'

override VERSION_OF_mingw-w64-x86_64-xmlstarlet := 1.6.1-5
.PHONY: PKG@@mingw-w64-x86_64-xmlstarlet PKG@@mingw-w64-x86_64-xmlstarlet-git
PKG@@mingw-w64-x86_64-xmlstarlet PKG@@mingw-w64-x86_64-xmlstarlet-git: $(if $(__deps),PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-libxslt) ; @echo 'PKG@@mingw-w64-x86_64-xmlstarlet'

override VERSION_OF_mingw-w64-x86_64-xmlto := 0.0.29-2
.PHONY: PKG@@mingw-w64-x86_64-xmlto
PKG@@mingw-w64-x86_64-xmlto: $(if $(__deps),PKG@@mingw-w64-x86_64-libxml2 PKG@@mingw-w64-x86_64-libxslt) ; @echo 'PKG@@mingw-w64-x86_64-xmlto'

override VERSION_OF_mingw-w64-x86_64-xpm-nox := 4.2.0-9
.PHONY: PKG@@mingw-w64-x86_64-xpm-nox
PKG@@mingw-w64-x86_64-xpm-nox: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-xpm-nox'

override VERSION_OF_mingw-w64-x86_64-xsimd := 14.3.0-1
.PHONY: PKG@@mingw-w64-x86_64-xsimd
PKG@@mingw-w64-x86_64-xsimd: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-xsimd'

override VERSION_OF_mingw-w64-x86_64-xvidcore := 1.3.7-5
.PHONY: PKG@@mingw-w64-x86_64-xvidcore
PKG@@mingw-w64-x86_64-xvidcore: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-xvidcore'

override VERSION_OF_mingw-w64-x86_64-xxhash := 0.8.4-1
.PHONY: PKG@@mingw-w64-x86_64-xxhash
PKG@@mingw-w64-x86_64-xxhash: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-xxhash'

override VERSION_OF_mingw-w64-x86_64-xz := 5.8.4-1
.PHONY: PKG@@mingw-w64-x86_64-xz
PKG@@mingw-w64-x86_64-xz: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime) ; @echo 'PKG@@mingw-w64-x86_64-xz'

override VERSION_OF_mingw-w64-x86_64-yaml-cpp := 0.9.0-1
.PHONY: PKG@@mingw-w64-x86_64-yaml-cpp
PKG@@mingw-w64-x86_64-yaml-cpp: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-yaml-cpp'

override VERSION_OF_mingw-w64-x86_64-z3 := 4.16.0-1
.PHONY: PKG@@mingw-w64-x86_64-z3
PKG@@mingw-w64-x86_64-z3: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-gmp) ; @echo 'PKG@@mingw-w64-x86_64-z3'

override VERSION_OF_mingw-w64-x86_64-zeromq := 4.3.5-5
.PHONY: PKG@@mingw-w64-x86_64-zeromq
PKG@@mingw-w64-x86_64-zeromq: $(if $(__deps),PKG@@mingw-w64-x86_64-libsodium PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-zeromq'

override VERSION_OF_mingw-w64-x86_64-zimg := 3.0.6-3
.PHONY: PKG@@mingw-w64-x86_64-zimg
PKG@@mingw-w64-x86_64-zimg: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-zimg'

override VERSION_OF_mingw-w64-x86_64-zlib := 1.4-2
.PHONY: PKG@@mingw-w64-x86_64-zlib
PKG@@mingw-w64-x86_64-zlib: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-zlib'

override VERSION_OF_mingw-w64-x86_64-zlib-ng := 2.3.3-2
.PHONY: PKG@@mingw-w64-x86_64-zlib-ng
PKG@@mingw-w64-x86_64-zlib-ng: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-zlib-ng'

override VERSION_OF_mingw-w64-x86_64-zlib-ng-compat := 2.3.3-2
.PHONY: PKG@@mingw-w64-x86_64-zlib-ng-compat
PKG@@mingw-w64-x86_64-zlib-ng-compat: $(if $(__deps),) ; @echo 'PKG@@mingw-w64-x86_64-zlib-ng-compat'

override VERSION_OF_mingw-w64-x86_64-zstd := 1.5.7-2
.PHONY: PKG@@mingw-w64-x86_64-zstd
PKG@@mingw-w64-x86_64-zstd: $(if $(__deps),PKG@@mingw-w64-x86_64-cc-libs) ; @echo 'PKG@@mingw-w64-x86_64-zstd'

override VERSION_OF_mingw-w64-x86_64-zvbi := 0.2.46-1
.PHONY: PKG@@mingw-w64-x86_64-zvbi
PKG@@mingw-w64-x86_64-zvbi: $(if $(__deps),PKG@@mingw-w64-x86_64-gettext-runtime PKG@@mingw-w64-x86_64-libiconv PKG@@mingw-w64-x86_64-libpng PKG@@mingw-w64-x86_64-libwinpthread) ; @echo 'PKG@@mingw-w64-x86_64-zvbi'

# Unknown deps:
.PHONY: autoconf automake make nano perl-dbi perl perl-error perl-libwww perl-termreadkey perl-authen-sasl perl-mime-tools perl-net-smtp-ssl subversion perl bash sed winpty winpty sh zip
PKG@@autoconf PKG@@automake PKG@@make PKG@@nano PKG@@perl-dbi PKG@@perl PKG@@perl-error PKG@@perl-libwww PKG@@perl-termreadkey PKG@@perl-authen-sasl PKG@@perl-mime-tools PKG@@perl-net-smtp-ssl PKG@@subversion PKG@@perl PKG@@bash PKG@@sed PKG@@winpty PKG@@winpty PKG@@sh PKG@@zip:

override FULL_PACKAGE_LIST := mingw-w64-x86_64-7zip mingw-w64-x86_64-SDL mingw-w64-x86_64-SDL2 mingw-w64-x86_64-abseil-cpp mingw-w64-x86_64-adios2 mingw-w64-x86_64-adobe-source-code-pro-fonts mingw-w64-x86_64-adwaita-icon-theme mingw-w64-x86_64-adwaita-icon-theme-legacy mingw-w64-x86_64-alembic mingw-w64-x86_64-amf-headers mingw-w64-x86_64-angleproject mingw-w64-x86_64-aom mingw-w64-x86_64-appstream-glib mingw-w64-x86_64-argon2 mingw-w64-x86_64-armadillo mingw-w64-x86_64-arpack mingw-w64-x86_64-arpack64 mingw-w64-x86_64-arrow mingw-w64-x86_64-asciidoc mingw-w64-x86_64-asciidoctor mingw-w64-x86_64-asio mingw-w64-x86_64-asiosdk mingw-w64-x86_64-assimp mingw-w64-x86_64-atk mingw-w64-x86_64-autotools mingw-w64-x86_64-aws-c-auth mingw-w64-x86_64-aws-c-cal mingw-w64-x86_64-aws-c-common mingw-w64-x86_64-aws-c-compression mingw-w64-x86_64-aws-c-event-stream mingw-w64-x86_64-aws-c-http mingw-w64-x86_64-aws-c-io mingw-w64-x86_64-aws-c-mqtt mingw-w64-x86_64-aws-c-s3 mingw-w64-x86_64-aws-c-sdkutils mingw-w64-x86_64-aws-checksums mingw-w64-x86_64-aws-crt-cpp mingw-w64-x86_64-aws-sdk-cpp mingw-w64-x86_64-b3sum mingw-w64-x86_64-bin2header mingw-w64-x86_64-binaryen mingw-w64-x86_64-binutils mingw-w64-x86_64-blas mingw-w64-x86_64-blas64 mingw-w64-x86_64-blosc mingw-w64-x86_64-blosc2 mingw-w64-x86_64-boost mingw-w64-x86_64-boost-libs mingw-w64-x86_64-brotli mingw-w64-x86_64-brotli-testdata mingw-w64-x86_64-bullet mingw-w64-x86_64-bullet-dp mingw-w64-x86_64-bzip2 mingw-w64-x86_64-c-ares mingw-w64-x86_64-ca-certificates mingw-w64-x86_64-cairo mingw-w64-x86_64-cantarell-fonts mingw-w64-x86_64-capstone mingw-w64-x86_64-cargo-c mingw-w64-x86_64-cargo-edit mingw-w64-x86_64-cargo-generate mingw-w64-x86_64-cargo-local-registry mingw-w64-x86_64-cas-offinder mingw-w64-x86_64-castxml mingw-w64-x86_64-catch mingw-w64-x86_64-cblas mingw-w64-x86_64-cblas64 mingw-w64-x86_64-cc-libs mingw-w64-x86_64-ccache mingw-w64-x86_64-cccl mingw-w64-x86_64-ccmake mingw-w64-x86_64-cddlib mingw-w64-x86_64-cereal mingw-w64-x86_64-ceres-solver mingw-w64-x86_64-cfitsio mingw-w64-x86_64-cgal mingw-w64-x86_64-cglm mingw-w64-x86_64-cglm-docs mingw-w64-x86_64-cgns mingw-w64-x86_64-chafa mingw-w64-x86_64-check mingw-w64-x86_64-chicken mingw-w64-x86_64-cjose mingw-w64-x86_64-cjson mingw-w64-x86_64-clang mingw-w64-x86_64-clang-analyzer mingw-w64-x86_64-clang-libs mingw-w64-x86_64-clang-tools-extra mingw-w64-x86_64-clblast mingw-w64-x86_64-cli11 mingw-w64-x86_64-clucene mingw-w64-x86_64-cmake mingw-w64-x86_64-cmake-bootstrap mingw-w64-x86_64-cmake-cmcldeps mingw-w64-x86_64-cmake-docs mingw-w64-x86_64-cmake-gui mingw-w64-x86_64-cmakelang mingw-w64-x86_64-cmakerc mingw-w64-x86_64-cmocka mingw-w64-x86_64-cninja mingw-w64-x86_64-codec2 mingw-w64-x86_64-collada-dom mingw-w64-x86_64-compiler-rt mingw-w64-x86_64-connect mingw-w64-x86_64-corrade mingw-w64-x86_64-cppcheck mingw-w64-x86_64-cppcoro mingw-w64-x86_64-cppdap mingw-w64-x86_64-cppreference-qt mingw-w64-x86_64-cpptest mingw-w64-x86_64-cppunit mingw-w64-x86_64-cpputest mingw-w64-x86_64-cppwinrt mingw-w64-x86_64-cppzmq mingw-w64-x86_64-crt mingw-w64-x86_64-crypto++ mingw-w64-x86_64-ctags mingw-w64-x86_64-ctpl mingw-w64-x86_64-cunit mingw-w64-x86_64-curl mingw-w64-x86_64-curl-gnutls mingw-w64-x86_64-curl-winssl mingw-w64-x86_64-cusp mingw-w64-x86_64-cxxopts mingw-w64-x86_64-cyrus-sasl mingw-w64-x86_64-cython mingw-w64-x86_64-d-feet mingw-w64-x86_64-dav1d mingw-w64-x86_64-db mingw-w64-x86_64-db-docs mingw-w64-x86_64-dbus mingw-w64-x86_64-dbus-c++ mingw-w64-x86_64-dbus-doc mingw-w64-x86_64-dbus-glib mingw-w64-x86_64-dcadec mingw-w64-x86_64-devil mingw-w64-x86_64-directx-headers mingw-w64-x86_64-directxmath mingw-w64-x86_64-directxmesh mingw-w64-x86_64-directxtex mingw-w64-x86_64-djvulibre mingw-w64-x86_64-dlfcn mingw-w64-x86_64-dnssec-anchors mingw-w64-x86_64-docbook-dsssl mingw-w64-x86_64-docbook-mathml mingw-w64-x86_64-docbook-xml mingw-w64-x86_64-docbook-xsl mingw-w64-x86_64-docbook5-xml mingw-w64-x86_64-doctest mingw-w64-x86_64-docx2txt-rs mingw-w64-x86_64-double-conversion mingw-w64-x86_64-doxygen mingw-w64-x86_64-draco mingw-w64-x86_64-dsdp mingw-w64-x86_64-duktape mingw-w64-x86_64-efxc2 mingw-w64-x86_64-egl-headers mingw-w64-x86_64-eigen3 mingw-w64-x86_64-expat mingw-w64-x86_64-exprtk mingw-w64-x86_64-extra-cmake-modules mingw-w64-x86_64-fast_float mingw-w64-x86_64-ffmpeg mingw-w64-x86_64-ffms2 mingw-w64-x86_64-ffnvcodec-headers mingw-w64-x86_64-fftw mingw-w64-x86_64-file mingw-w64-x86_64-firebird mingw-w64-x86_64-flac mingw-w64-x86_64-flatbuffers mingw-w64-x86_64-flif mingw-w64-x86_64-fluidsynth mingw-w64-x86_64-fmt mingw-w64-x86_64-fontconfig mingw-w64-x86_64-freeglut mingw-w64-x86_64-freeimage mingw-w64-x86_64-freetype mingw-w64-x86_64-frei0r-plugins mingw-w64-x86_64-fribidi mingw-w64-x86_64-fxc2 mingw-w64-x86_64-gavl mingw-w64-x86_64-gc mingw-w64-x86_64-gcab mingw-w64-x86_64-gcc mingw-w64-x86_64-gcc-ada mingw-w64-x86_64-gcc-fortran mingw-w64-x86_64-gcc-libs mingw-w64-x86_64-gcc-objc mingw-w64-x86_64-gcc-plugin mingw-w64-x86_64-gdal mingw-w64-x86_64-gdb mingw-w64-x86_64-gdb-multiarch mingw-w64-x86_64-gdbm mingw-w64-x86_64-gdk-pixbuf2 mingw-w64-x86_64-gdk-pixbuf2-docs mingw-w64-x86_64-gdl mingw-w64-x86_64-geocode-glib mingw-w64-x86_64-geographiclib mingw-w64-x86_64-geoip mingw-w64-x86_64-geos mingw-w64-x86_64-gettext-libtextstyle mingw-w64-x86_64-gettext-runtime mingw-w64-x86_64-gettext-tools mingw-w64-x86_64-gf2x mingw-w64-x86_64-gflags mingw-w64-x86_64-ggml mingw-w64-x86_64-ghostscript mingw-w64-x86_64-gi-docgen mingw-w64-x86_64-giflib mingw-w64-x86_64-git mingw-w64-x86_64-git-archimport mingw-w64-x86_64-git-credential-wincred mingw-w64-x86_64-git-cvs mingw-w64-x86_64-git-doc-html mingw-w64-x86_64-git-doc-man mingw-w64-x86_64-git-for-windows-addons mingw-w64-x86_64-git-gui mingw-w64-x86_64-git-lfs mingw-w64-x86_64-git-p4 mingw-w64-x86_64-git-perl mingw-w64-x86_64-git-send-email mingw-w64-x86_64-git-subtree mingw-w64-x86_64-git-svn mingw-w64-x86_64-gitk mingw-w64-x86_64-gitweb mingw-w64-x86_64-gl2ps mingw-w64-x86_64-glbinding mingw-w64-x86_64-gles-headers mingw-w64-x86_64-glew mingw-w64-x86_64-glfw mingw-w64-x86_64-glfw-docs mingw-w64-x86_64-glib-networking mingw-w64-x86_64-glib2 mingw-w64-x86_64-glm mingw-w64-x86_64-global mingw-w64-x86_64-globjects mingw-w64-x86_64-glog mingw-w64-x86_64-glpk mingw-w64-x86_64-glsl-optimizer mingw-w64-x86_64-glslang mingw-w64-x86_64-gmp mingw-w64-x86_64-gn mingw-w64-x86_64-gnome-common mingw-w64-x86_64-gnupg mingw-w64-x86_64-gnustep-back mingw-w64-x86_64-gnustep-base mingw-w64-x86_64-gnustep-gui mingw-w64-x86_64-gnustep-make mingw-w64-x86_64-gnutls mingw-w64-x86_64-go mingw-w64-x86_64-gobject-introspection mingw-w64-x86_64-gobject-introspection-runtime mingw-w64-x86_64-gperf mingw-w64-x86_64-gputils mingw-w64-x86_64-graphene mingw-w64-x86_64-graphite2 mingw-w64-x86_64-graphviz mingw-w64-x86_64-graphviz-docs mingw-w64-x86_64-grpc mingw-w64-x86_64-gsettings-desktop-schemas mingw-w64-x86_64-gsfonts mingw-w64-x86_64-gsl mingw-w64-x86_64-gsm mingw-w64-x86_64-gss mingw-w64-x86_64-gst-plugins-base mingw-w64-x86_64-gstreamer mingw-w64-x86_64-gtest mingw-w64-x86_64-gtk-doc mingw-w64-x86_64-gtk-update-icon-cache mingw-w64-x86_64-gtk3 mingw-w64-x86_64-gts mingw-w64-x86_64-gumbo-parser mingw-w64-x86_64-harfbuzz mingw-w64-x86_64-harfbuzz-cairo mingw-w64-x86_64-harfbuzz-docs mingw-w64-x86_64-harfbuzz-icu mingw-w64-x86_64-harfbuzz-utils mingw-w64-x86_64-hdf4 mingw-w64-x86_64-hdf5 mingw-w64-x86_64-headers mingw-w64-x86_64-hererocks mingw-w64-x86_64-hicolor-icon-theme mingw-w64-x86_64-highway mingw-w64-x86_64-hiredis mingw-w64-x86_64-hlsl2glsl mingw-w64-x86_64-htslib mingw-w64-x86_64-http-parser mingw-w64-x86_64-hunspell mingw-w64-x86_64-hunspell-en mingw-w64-x86_64-hwloc mingw-w64-x86_64-hypercorn mingw-w64-x86_64-i2pd mingw-w64-x86_64-iconv mingw-w64-x86_64-icoutils mingw-w64-x86_64-icu mingw-w64-x86_64-iir mingw-w64-x86_64-imagemagick mingw-w64-x86_64-imagemagick-docs mingw-w64-x86_64-imath mingw-w64-x86_64-iml mingw-w64-x86_64-include-what-you-use mingw-w64-x86_64-indent mingw-w64-x86_64-inja mingw-w64-x86_64-isa-l mingw-w64-x86_64-isl mingw-w64-x86_64-iso-codes mingw-w64-x86_64-itstool mingw-w64-x86_64-jack2 mingw-w64-x86_64-jansson mingw-w64-x86_64-jasper mingw-w64-x86_64-jasper-docs mingw-w64-x86_64-jbig2dec mingw-w64-x86_64-jbigkit mingw-w64-x86_64-jemalloc mingw-w64-x86_64-json-c mingw-w64-x86_64-json-glib mingw-w64-x86_64-jsoncpp mingw-w64-x86_64-jsonrpc-glib mingw-w64-x86_64-jsonrpc-glib-docs mingw-w64-x86_64-junitparser mingw-w64-x86_64-jxrlib mingw-w64-x86_64-keystone mingw-w64-x86_64-kiss_fft mingw-w64-x86_64-krpc-cpp mingw-w64-x86_64-kseexpr mingw-w64-x86_64-ktx-software mingw-w64-x86_64-kvazaar mingw-w64-x86_64-l-smash mingw-w64-x86_64-ladspa-sdk mingw-w64-x86_64-lager mingw-w64-x86_64-lame mingw-w64-x86_64-lapack mingw-w64-x86_64-lapack64 mingw-w64-x86_64-lapacke mingw-w64-x86_64-lapacke64 mingw-w64-x86_64-lcms2 mingw-w64-x86_64-lcov mingw-w64-x86_64-ldns mingw-w64-x86_64-ledger mingw-w64-x86_64-lensfun mingw-w64-x86_64-leptonica mingw-w64-x86_64-lerc mingw-w64-x86_64-level-zero-headers mingw-w64-x86_64-level-zero-loader mingw-w64-x86_64-lib2geom mingw-w64-x86_64-lib3mf mingw-w64-x86_64-libaacs mingw-w64-x86_64-libaec mingw-w64-x86_64-libarchive mingw-w64-x86_64-libaribcaption mingw-w64-x86_64-libart_lgpl mingw-w64-x86_64-libass mingw-w64-x86_64-libassuan mingw-w64-x86_64-libatomic mingw-w64-x86_64-libatomic_ops mingw-w64-x86_64-libavif mingw-w64-x86_64-libavro mingw-w64-x86_64-libb2 mingw-w64-x86_64-libb64 mingw-w64-x86_64-libbdplus mingw-w64-x86_64-libblake3 mingw-w64-x86_64-libblocksruntime-swift mingw-w64-x86_64-libbluray mingw-w64-x86_64-libbotan mingw-w64-x86_64-libbs2b mingw-w64-x86_64-libbsdf mingw-w64-x86_64-libc++ mingw-w64-x86_64-libcaca mingw-w64-x86_64-libcacard mingw-w64-x86_64-libcbor mingw-w64-x86_64-libcddb mingw-w64-x86_64-libcdio mingw-w64-x86_64-libcdio-paranoia mingw-w64-x86_64-libcdr mingw-w64-x86_64-libcello mingw-w64-x86_64-libcerf mingw-w64-x86_64-libclc mingw-w64-x86_64-libcloudproviders mingw-w64-x86_64-libconfig mingw-w64-x86_64-libconfini mingw-w64-x86_64-libcue mingw-w64-x86_64-libdatrie mingw-w64-x86_64-libdatrie-docs mingw-w64-x86_64-libdazzle mingw-w64-x86_64-libdca mingw-w64-x86_64-libde265 mingw-w64-x86_64-libdeflate mingw-w64-x86_64-libdex mingw-w64-x86_64-libdicom mingw-w64-x86_64-libdispatch mingw-w64-x86_64-libdmtx mingw-w64-x86_64-libdovi mingw-w64-x86_64-libdsm mingw-w64-x86_64-libdvdcss mingw-w64-x86_64-libdvdread mingw-w64-x86_64-libebml mingw-w64-x86_64-libelf mingw-w64-x86_64-libepoxy mingw-w64-x86_64-libevent mingw-w64-x86_64-libexif mingw-w64-x86_64-libffi mingw-w64-x86_64-libfido2 mingw-w64-x86_64-libfreexl mingw-w64-x86_64-libfyaml mingw-w64-x86_64-libgadu mingw-w64-x86_64-libgcc mingw-w64-x86_64-libgccjit mingw-w64-x86_64-libgcrypt mingw-w64-x86_64-libgd mingw-w64-x86_64-libgeotiff mingw-w64-x86_64-libgfortran mingw-w64-x86_64-libgig mingw-w64-x86_64-libgit2 mingw-w64-x86_64-libgit2-glib mingw-w64-x86_64-libgit2-glib-docs mingw-w64-x86_64-libgit2-winhttp mingw-w64-x86_64-libgme mingw-w64-x86_64-libgomp mingw-w64-x86_64-libgoom2 mingw-w64-x86_64-libgpg-error mingw-w64-x86_64-libgsf mingw-w64-x86_64-libguess mingw-w64-x86_64-libgweather mingw-w64-x86_64-libgxps mingw-w64-x86_64-libharu mingw-w64-x86_64-libheif mingw-w64-x86_64-libical mingw-w64-x86_64-libiconv mingw-w64-x86_64-libicsneo mingw-w64-x86_64-libidl2 mingw-w64-x86_64-libidn mingw-w64-x86_64-libidn2 mingw-w64-x86_64-libiff mingw-w64-x86_64-libilbc mingw-w64-x86_64-libilbm mingw-w64-x86_64-libimagequant mingw-w64-x86_64-libimobiledevice-glue mingw-w64-x86_64-libinih mingw-w64-x86_64-libipt mingw-w64-x86_64-libiptcdata mingw-w64-x86_64-libjpeg-turbo mingw-w64-x86_64-libjxl mingw-w64-x86_64-libkml mingw-w64-x86_64-libksba mingw-w64-x86_64-liblas mingw-w64-x86_64-liblc3 mingw-w64-x86_64-liblcf mingw-w64-x86_64-liblo mingw-w64-x86_64-liblqr mingw-w64-x86_64-libltdl mingw-w64-x86_64-libmad mingw-w64-x86_64-libmariadbclient mingw-w64-x86_64-libmfx mingw-w64-x86_64-libmicrodns mingw-w64-x86_64-libmikmod mingw-w64-x86_64-libmimic mingw-w64-x86_64-libmng mingw-w64-x86_64-libmodbus mingw-w64-x86_64-libmodplug mingw-w64-x86_64-libmongocrypt mingw-w64-x86_64-libmongoose mingw-w64-x86_64-libmongoose-docs mingw-w64-x86_64-libmowgli mingw-w64-x86_64-libmpack mingw-w64-x86_64-libmysofa mingw-w64-x86_64-libnfs mingw-w64-x86_64-libnice mingw-w64-x86_64-libntlm mingw-w64-x86_64-libnumbertext mingw-w64-x86_64-liboauth mingw-w64-x86_64-libobjc mingw-w64-x86_64-libobjc2 mingw-w64-x86_64-libodfgen mingw-w64-x86_64-libogg mingw-w64-x86_64-libomemo-c mingw-w64-x86_64-libopenmpt mingw-w64-x86_64-libopenmpt-modplug mingw-w64-x86_64-libopusenc mingw-w64-x86_64-liboqs mingw-w64-x86_64-libosmpbf mingw-w64-x86_64-libotr mingw-w64-x86_64-libp11 mingw-w64-x86_64-libpaper mingw-w64-x86_64-libpcap mingw-w64-x86_64-libplacebo mingw-w64-x86_64-libplacebo-demos mingw-w64-x86_64-libplist mingw-w64-x86_64-libpng mingw-w64-x86_64-libpqxx mingw-w64-x86_64-libpreferencepanes mingw-w64-x86_64-libprojectm mingw-w64-x86_64-libproxy mingw-w64-x86_64-libpsl mingw-w64-x86_64-libpst mingw-w64-x86_64-libquadmath mingw-w64-x86_64-libraqm mingw-w64-x86_64-libraw mingw-w64-x86_64-librdkafka mingw-w64-x86_64-libreplaygain mingw-w64-x86_64-libresidfp mingw-w64-x86_64-librevenge mingw-w64-x86_64-librime-data mingw-w64-x86_64-librsvg mingw-w64-x86_64-librsvg-docs mingw-w64-x86_64-librttopo mingw-w64-x86_64-libsamplerate mingw-w64-x86_64-libsass mingw-w64-x86_64-libsbml mingw-w64-x86_64-libsbsms mingw-w64-x86_64-libsecret mingw-w64-x86_64-libserdes mingw-w64-x86_64-libshout mingw-w64-x86_64-libsignal-protocol-c mingw-w64-x86_64-libsmi mingw-w64-x86_64-libsndfile mingw-w64-x86_64-libsodium mingw-w64-x86_64-libsoup3 mingw-w64-x86_64-libsoup3-docs mingw-w64-x86_64-libsoxr mingw-w64-x86_64-libspatialite mingw-w64-x86_64-libspiro mingw-w64-x86_64-libspng mingw-w64-x86_64-libsquish mingw-w64-x86_64-libssh mingw-w64-x86_64-libssh2 mingw-w64-x86_64-libssh2-wincng mingw-w64-x86_64-libstdc++ mingw-w64-x86_64-libstemmer mingw-w64-x86_64-libsystre mingw-w64-x86_64-libtasn1 mingw-w64-x86_64-libtermkey mingw-w64-x86_64-libthai mingw-w64-x86_64-libthai-docs mingw-w64-x86_64-libtheora mingw-w64-x86_64-libtiff mingw-w64-x86_64-libtiff-docs mingw-w64-x86_64-libtimidity mingw-w64-x86_64-libtomcrypt mingw-w64-x86_64-libtommath mingw-w64-x86_64-libtool mingw-w64-x86_64-libtorrent-rasterbar mingw-w64-x86_64-libtre mingw-w64-x86_64-libu2f-emu mingw-w64-x86_64-libultrahdr mingw-w64-x86_64-libunibreak mingw-w64-x86_64-libunifex mingw-w64-x86_64-libuninameslist mingw-w64-x86_64-libunistring mingw-w64-x86_64-libunwind mingw-w64-x86_64-libusb-win32 mingw-w64-x86_64-libusbmuxd mingw-w64-x86_64-libutf8proc mingw-w64-x86_64-libutp mingw-w64-x86_64-libuv mingw-w64-x86_64-libva mingw-w64-x86_64-libva-utils mingw-w64-x86_64-libvisio mingw-w64-x86_64-libvoikko mingw-w64-x86_64-libvorbis mingw-w64-x86_64-libvpl mingw-w64-x86_64-libvpx mingw-w64-x86_64-libwebm mingw-w64-x86_64-libwebp mingw-w64-x86_64-libwebsockets mingw-w64-x86_64-libwinpthread mingw-w64-x86_64-libwmf mingw-w64-x86_64-libwpd mingw-w64-x86_64-libwpg mingw-w64-x86_64-libx264 mingw-w64-x86_64-libxisf mingw-w64-x86_64-libxlsxwriter mingw-w64-x86_64-libxml++-5.0 mingw-w64-x86_64-libxml2 mingw-w64-x86_64-libxmlb mingw-w64-x86_64-libxmp mingw-w64-x86_64-libxslt mingw-w64-x86_64-libyaml mingw-w64-x86_64-libyuv mingw-w64-x86_64-libzen mingw-w64-x86_64-libzip mingw-w64-x86_64-lis mingw-w64-x86_64-litehtml mingw-w64-x86_64-llama.cpp mingw-w64-x86_64-lld mingw-w64-x86_64-lldb mingw-w64-x86_64-lldb-mi mingw-w64-x86_64-llvm mingw-w64-x86_64-llvm-libs mingw-w64-x86_64-llvm-tools mingw-w64-x86_64-lsp-dsp-lib mingw-w64-x86_64-lto-dump mingw-w64-x86_64-lua mingw-w64-x86_64-lua51 mingw-w64-x86_64-lua53 mingw-w64-x86_64-lua54 mingw-w64-x86_64-lz4 mingw-w64-x86_64-lzo2 mingw-w64-x86_64-make mingw-w64-x86_64-mathjax2 mingw-w64-x86_64-matio mingw-w64-x86_64-maturin mingw-w64-x86_64-maxflow mingw-w64-x86_64-mbedtls mingw-w64-x86_64-mcfgthread mingw-w64-x86_64-mcfgthread-libs mingw-w64-x86_64-md4c mingw-w64-x86_64-mdbook mingw-w64-x86_64-mdbook-admonish mingw-w64-x86_64-mdbook-mermaid mingw-w64-x86_64-mdbook-pikchr mingw-w64-x86_64-mdloader mingw-w64-x86_64-meanwhile mingw-w64-x86_64-mecab mingw-w64-x86_64-mesa mingw-w64-x86_64-meson mingw-w64-x86_64-meson-python mingw-w64-x86_64-metis mingw-w64-x86_64-minisign mingw-w64-x86_64-miniupnpc mingw-w64-x86_64-minizip mingw-w64-x86_64-minizip-ng mingw-w64-x86_64-mman-win32 mingw-w64-x86_64-moab mingw-w64-x86_64-mongo-c-driver mingw-w64-x86_64-mpc mingw-w64-x86_64-mpdecimal mingw-w64-x86_64-mpfr mingw-w64-x86_64-mpg123 mingw-w64-x86_64-mruby mingw-w64-x86_64-mscgen mingw-w64-x86_64-msgpack-c mingw-w64-x86_64-msitools mingw-w64-x86_64-msmpi mingw-w64-x86_64-mtex2MML mingw-w64-x86_64-mtn mingw-w64-x86_64-mtools mingw-w64-x86_64-muparser mingw-w64-x86_64-mxml mingw-w64-x86_64-mypy mingw-w64-x86_64-nanobind mingw-w64-x86_64-nasm mingw-w64-x86_64-ncurses mingw-w64-x86_64-netcdf mingw-w64-x86_64-netcdf-cxx mingw-w64-x86_64-netcdf-fortran mingw-w64-x86_64-nettle mingw-w64-x86_64-ng-log mingw-w64-x86_64-nghttp2 mingw-w64-x86_64-nghttp3 mingw-w64-x86_64-ngtcp2 mingw-w64-x86_64-nim mingw-w64-x86_64-ninja mingw-w64-x86_64-nlohmann-json mingw-w64-x86_64-nlopt mingw-w64-x86_64-npth mingw-w64-x86_64-nsis mingw-w64-x86_64-nspr mingw-w64-x86_64-nss mingw-w64-x86_64-ntldd mingw-w64-x86_64-nuspell mingw-w64-x86_64-ogre3d mingw-w64-x86_64-openal mingw-w64-x86_64-openblas mingw-w64-x86_64-openblas64 mingw-w64-x86_64-opencascade mingw-w64-x86_64-opencascade-doc-overview mingw-w64-x86_64-opencascade-doc-refman mingw-w64-x86_64-opencl-headers mingw-w64-x86_64-opencl-icd mingw-w64-x86_64-opencore-amr mingw-w64-x86_64-opencsg mingw-w64-x86_64-opencv mingw-w64-x86_64-openexr mingw-w64-x86_64-openfa mingw-w64-x86_64-opengl-man-pages mingw-w64-x86_64-openh264 mingw-w64-x86_64-openjpeg2 mingw-w64-x86_64-openjph mingw-w64-x86_64-openlibm mingw-w64-x86_64-openslide mingw-w64-x86_64-openssl mingw-w64-x86_64-openturns mingw-w64-x86_64-openvdb mingw-w64-x86_64-openvr mingw-w64-x86_64-openxr-sdk mingw-w64-x86_64-optipng mingw-w64-x86_64-opus mingw-w64-x86_64-opusfile mingw-w64-x86_64-oqsprovider mingw-w64-x86_64-orc mingw-w64-x86_64-osl mingw-w64-x86_64-osslsigncode mingw-w64-x86_64-p11-kit mingw-w64-x86_64-pango mingw-w64-x86_64-pango-docs mingw-w64-x86_64-parpack mingw-w64-x86_64-pcaudiolib mingw-w64-x86_64-pcre mingw-w64-x86_64-pcre2 mingw-w64-x86_64-pdal mingw-w64-x86_64-pdcurses mingw-w64-x86_64-pdf2svg mingw-w64-x86_64-pelican mingw-w64-x86_64-perl mingw-w64-x86_64-perl-digest-sha mingw-w64-x86_64-perl-doc mingw-w64-x86_64-perl-encode-compat mingw-w64-x86_64-perl-file-which mingw-w64-x86_64-perl-html-tagset mingw-w64-x86_64-perl-io-string mingw-w64-x86_64-perl-math-int64 mingw-w64-x86_64-perl-mozilla-ca mingw-w64-x86_64-perl-win32-api mingw-w64-x86_64-perl-win32-console mingw-w64-x86_64-perl-win32-shortcut mingw-w64-x86_64-perl-win32-tieregistry mingw-w64-x86_64-perl-win32-winerror mingw-w64-x86_64-perl-win32api-registry mingw-w64-x86_64-pg_probackup-16 mingw-w64-x86_64-phodav mingw-w64-x86_64-phonon mingw-w64-x86_64-picosha2 mingw-w64-x86_64-pikchr mingw-w64-x86_64-pinentry mingw-w64-x86_64-pixman mingw-w64-x86_64-pkcs11-helper mingw-w64-x86_64-pkgconf mingw-w64-x86_64-plfit mingw-w64-x86_64-plutovg mingw-w64-x86_64-podofo mingw-w64-x86_64-polipo mingw-w64-x86_64-polly mingw-w64-x86_64-poppler mingw-w64-x86_64-poppler-data mingw-w64-x86_64-poppler-qt5 mingw-w64-x86_64-poppler-qt6 mingw-w64-x86_64-popt mingw-w64-x86_64-port-scanner mingw-w64-x86_64-portablexdr mingw-w64-x86_64-portaudio mingw-w64-x86_64-portaudio-asio mingw-w64-x86_64-postgresql mingw-w64-x86_64-postgresql-16 mingw-w64-x86_64-potrace mingw-w64-x86_64-pqp mingw-w64-x86_64-prjtrellis mingw-w64-x86_64-proj mingw-w64-x86_64-protobuf mingw-w64-x86_64-protobuf-c mingw-w64-x86_64-pugixml mingw-w64-x86_64-pupnp mingw-w64-x86_64-putty mingw-w64-x86_64-putty-ssh mingw-w64-x86_64-pybind11 mingw-w64-x86_64-pybind11-json mingw-w64-x86_64-pyinstaller mingw-w64-x86_64-pyinstaller-hooks-contrib mingw-w64-x86_64-pyqt-builder mingw-w64-x86_64-pyqt5-sip mingw-w64-x86_64-pystring mingw-w64-x86_64-python mingw-w64-x86_64-python-aiofiles mingw-w64-x86_64-python-aiosmtpd mingw-w64-x86_64-python-aiosqlite mingw-w64-x86_64-python-altgraph mingw-w64-x86_64-python-annotated-doc mingw-w64-x86_64-python-annotated-types mingw-w64-x86_64-python-anyio mingw-w64-x86_64-python-apipkg mingw-w64-x86_64-python-appdirs mingw-w64-x86_64-python-argcomplete mingw-w64-x86_64-python-argon2-cffi-bindings mingw-w64-x86_64-python-argon2_cffi mingw-w64-x86_64-python-asgiref mingw-w64-x86_64-python-asn1crypto mingw-w64-x86_64-python-ast-serialize mingw-w64-x86_64-python-astor mingw-w64-x86_64-python-astroid mingw-w64-x86_64-python-asttokens mingw-w64-x86_64-python-astunparse mingw-w64-x86_64-python-async-timeout mingw-w64-x86_64-python-async_generator mingw-w64-x86_64-python-atomicwrites mingw-w64-x86_64-python-atpublic mingw-w64-x86_64-python-attrs mingw-w64-x86_64-python-audioop-lts mingw-w64-x86_64-python-audioread mingw-w64-x86_64-python-automat mingw-w64-x86_64-python-autopage mingw-w64-x86_64-python-autopep8 mingw-w64-x86_64-python-awesomeversion mingw-w64-x86_64-python-babel mingw-w64-x86_64-python-bcrypt mingw-w64-x86_64-python-beautifulsoup4 mingw-w64-x86_64-python-beniget mingw-w64-x86_64-python-blinker mingw-w64-x86_64-python-boolean.py mingw-w64-x86_64-python-bootstrap-flask mingw-w64-x86_64-python-bootstrap-flask-docs mingw-w64-x86_64-python-bottle mingw-w64-x86_64-python-branca mingw-w64-x86_64-python-breathe mingw-w64-x86_64-python-brotli mingw-w64-x86_64-python-bsddb3 mingw-w64-x86_64-python-build mingw-w64-x86_64-python-cabarchive mingw-w64-x86_64-python-cachecontrol mingw-w64-x86_64-python-cached-property mingw-w64-x86_64-python-cachetools mingw-w64-x86_64-python-cachy mingw-w64-x86_64-python-cairo mingw-w64-x86_64-python-calver mingw-w64-x86_64-python-capstone mingw-w64-x86_64-python-cattrs mingw-w64-x86_64-python-certifi mingw-w64-x86_64-python-cffi mingw-w64-x86_64-python-charset-normalizer mingw-w64-x86_64-python-cleo mingw-w64-x86_64-python-click mingw-w64-x86_64-python-click-docs mingw-w64-x86_64-python-cliff mingw-w64-x86_64-python-cloudpickle mingw-w64-x86_64-python-cmd2 mingw-w64-x86_64-python-codespell mingw-w64-x86_64-python-colorama mingw-w64-x86_64-python-colored-traceback mingw-w64-x86_64-python-colorlog mingw-w64-x86_64-python-commonmark mingw-w64-x86_64-python-comtypes mingw-w64-x86_64-python-constantly mingw-w64-x86_64-python-contextlib2 mingw-w64-x86_64-python-contourpy mingw-w64-x86_64-python-coverage mingw-w64-x86_64-python-cppy mingw-w64-x86_64-python-cryptography mingw-w64-x86_64-python-cssselect mingw-w64-x86_64-python-csv23 mingw-w64-x86_64-python-cvxopt mingw-w64-x86_64-python-cx-logging mingw-w64-x86_64-python-cycler mingw-w64-x86_64-python-darkdetect mingw-w64-x86_64-python-dateutil mingw-w64-x86_64-python-decouple mingw-w64-x86_64-python-defusedxml mingw-w64-x86_64-python-deprecation mingw-w64-x86_64-python-diff-match-patch mingw-w64-x86_64-python-dill mingw-w64-x86_64-python-distlib mingw-w64-x86_64-python-distro mingw-w64-x86_64-python-django mingw-w64-x86_64-python-dnspython mingw-w64-x86_64-python-docopt mingw-w64-x86_64-python-docutils mingw-w64-x86_64-python-dulwich mingw-w64-x86_64-python-e3-core mingw-w64-x86_64-python-ecdsa mingw-w64-x86_64-python-editables mingw-w64-x86_64-python-elementpath mingw-w64-x86_64-python-email-validator mingw-w64-x86_64-python-emoji mingw-w64-x86_64-python-et-xmlfile mingw-w64-x86_64-python-execnet mingw-w64-x86_64-python-faker mingw-w64-x86_64-python-fasteners mingw-w64-x86_64-python-fastexcel mingw-w64-x86_64-python-fastjsonschema mingw-w64-x86_64-python-feedgenerator mingw-w64-x86_64-python-filelock mingw-w64-x86_64-python-findpython mingw-w64-x86_64-python-fixtures mingw-w64-x86_64-python-flake8 mingw-w64-x86_64-python-flaky mingw-w64-x86_64-python-flask mingw-w64-x86_64-python-flask-babel mingw-w64-x86_64-python-flask-babel-docs mingw-w64-x86_64-python-flatbuffers mingw-w64-x86_64-python-flexmock mingw-w64-x86_64-python-flit mingw-w64-x86_64-python-flit-core mingw-w64-x86_64-python-fonttools mingw-w64-x86_64-python-ford mingw-w64-x86_64-python-freetype-py mingw-w64-x86_64-python-freezegun mingw-w64-x86_64-python-ftfy mingw-w64-x86_64-python-func-timeout mingw-w64-x86_64-python-future mingw-w64-x86_64-python-gast mingw-w64-x86_64-python-gdal mingw-w64-x86_64-python-geographiclib mingw-w64-x86_64-python-geographiclib-docs mingw-w64-x86_64-python-glad mingw-w64-x86_64-python-gmpy2 mingw-w64-x86_64-python-gobject mingw-w64-x86_64-python-graphviz mingw-w64-x86_64-python-grpcio mingw-w64-x86_64-python-grpcio-tools mingw-w64-x86_64-python-gym mingw-w64-x86_64-python-gym-notices mingw-w64-x86_64-python-h11 mingw-w64-x86_64-python-h2 mingw-w64-x86_64-python-h5py mingw-w64-x86_64-python-halo mingw-w64-x86_64-python-hatch-fancy-pypi-readme mingw-w64-x86_64-python-hatch-vcs mingw-w64-x86_64-python-hatchling mingw-w64-x86_64-python-hpack mingw-w64-x86_64-python-html5lib mingw-w64-x86_64-python-httpcore mingw-w64-x86_64-python-httplib2 mingw-w64-x86_64-python-httpretty mingw-w64-x86_64-python-httpx mingw-w64-x86_64-python-hyperframe mingw-w64-x86_64-python-hyperlink mingw-w64-x86_64-python-hypothesis mingw-w64-x86_64-python-icu mingw-w64-x86_64-python-id mingw-w64-x86_64-python-identify mingw-w64-x86_64-python-idna mingw-w64-x86_64-python-ifstools mingw-w64-x86_64-python-imagesize mingw-w64-x86_64-python-immutables mingw-w64-x86_64-python-importlib-metadata mingw-w64-x86_64-python-importlib_resources mingw-w64-x86_64-python-incremental mingw-w64-x86_64-python-inflection mingw-w64-x86_64-python-iniconfig mingw-w64-x86_64-python-installer mingw-w64-x86_64-python-intervaltree mingw-w64-x86_64-python-iso8601 mingw-w64-x86_64-python-isodate mingw-w64-x86_64-python-isosurfaces mingw-w64-x86_64-python-itsdangerous mingw-w64-x86_64-python-jaraco.classes mingw-w64-x86_64-python-jaraco.context mingw-w64-x86_64-python-jaraco.envs mingw-w64-x86_64-python-jaraco.functools mingw-w64-x86_64-python-jaraco.path mingw-w64-x86_64-python-jinja mingw-w64-x86_64-python-jinja-docs mingw-w64-x86_64-python-json-logger mingw-w64-x86_64-python-json-rpc mingw-w64-x86_64-python-kbinxml mingw-w64-x86_64-python-keyring mingw-w64-x86_64-python-keystone mingw-w64-x86_64-python-kiwisolver mingw-w64-x86_64-python-lark-parser mingw-w64-x86_64-python-lazy-importer mingw-w64-x86_64-python-lazy-object-proxy mingw-w64-x86_64-python-levenshtein mingw-w64-x86_64-python-lhafile mingw-w64-x86_64-python-librt mingw-w64-x86_64-python-libsass mingw-w64-x86_64-python-license-expression mingw-w64-x86_64-python-log_symbols mingw-w64-x86_64-python-lws mingw-w64-x86_64-python-lxml mingw-w64-x86_64-python-lz4 mingw-w64-x86_64-python-lzo mingw-w64-x86_64-python-mako mingw-w64-x86_64-python-mallard-ducktype mingw-w64-x86_64-python-manhole mingw-w64-x86_64-python-markdown mingw-w64-x86_64-python-markdown-include mingw-w64-x86_64-python-markdown-it-py mingw-w64-x86_64-python-markdown-math mingw-w64-x86_64-python-markups mingw-w64-x86_64-python-markupsafe mingw-w64-x86_64-python-matplotlib mingw-w64-x86_64-python-maturin mingw-w64-x86_64-python-mccabe mingw-w64-x86_64-python-mdit_py_plugins mingw-w64-x86_64-python-mdurl mingw-w64-x86_64-python-meshio mingw-w64-x86_64-python-mimeparse mingw-w64-x86_64-python-mingw-ldd mingw-w64-x86_64-python-miniupnpc mingw-w64-x86_64-python-mock mingw-w64-x86_64-python-more-itertools mingw-w64-x86_64-python-mpi4py mingw-w64-x86_64-python-msgpack mingw-w64-x86_64-python-multidict mingw-w64-x86_64-python-multipledispatch mingw-w64-x86_64-python-multiprocess mingw-w64-x86_64-python-mypy_extensions mingw-w64-x86_64-python-myst-parser mingw-w64-x86_64-python-natsort mingw-w64-x86_64-python-nh3 mingw-w64-x86_64-python-node-semver mingw-w64-x86_64-python-nodeenv mingw-w64-x86_64-python-numexpr mingw-w64-x86_64-python-numpy mingw-w64-x86_64-python-olefile mingw-w64-x86_64-python-opencv mingw-w64-x86_64-python-openpyxl mingw-w64-x86_64-python-opt_einsum mingw-w64-x86_64-python-ordered-set mingw-w64-x86_64-python-outcome mingw-w64-x86_64-python-overrides mingw-w64-x86_64-python-owslib mingw-w64-x86_64-python-pacdb mingw-w64-x86_64-python-packageurl-python mingw-w64-x86_64-python-packaging mingw-w64-x86_64-python-pallets-sphinx-themes mingw-w64-x86_64-python-pandas mingw-w64-x86_64-python-pandocfilters mingw-w64-x86_64-python-passlib mingw-w64-x86_64-python-pastel mingw-w64-x86_64-python-patch-ng mingw-w64-x86_64-python-path mingw-w64-x86_64-python-pathspec mingw-w64-x86_64-python-pbr mingw-w64-x86_64-python-pbs-installer mingw-w64-x86_64-python-pcpp mingw-w64-x86_64-python-pdfminer mingw-w64-x86_64-python-pdm-backend mingw-w64-x86_64-python-pefile mingw-w64-x86_64-python-pesq mingw-w64-x86_64-python-pexpect mingw-w64-x86_64-python-pgen2 mingw-w64-x86_64-python-pi-heif mingw-w64-x86_64-python-pika mingw-w64-x86_64-python-pillow mingw-w64-x86_64-python-pip mingw-w64-x86_64-python-pipenv mingw-w64-x86_64-python-pkg-resources mingw-w64-x86_64-python-pkgconfig mingw-w64-x86_64-python-pkginfo mingw-w64-x86_64-python-platformdirs mingw-w64-x86_64-python-pluggy mingw-w64-x86_64-python-pluginbase mingw-w64-x86_64-python-ply mingw-w64-x86_64-python-poetry mingw-w64-x86_64-python-poetry-core mingw-w64-x86_64-python-poetry-plugin-export mingw-w64-x86_64-python-poetry-plugin-shell mingw-w64-x86_64-python-pptx mingw-w64-x86_64-python-pretend mingw-w64-x86_64-python-prettytable mingw-w64-x86_64-python-priority mingw-w64-x86_64-python-progress mingw-w64-x86_64-python-prompt_toolkit mingw-w64-x86_64-python-protobuf mingw-w64-x86_64-python-psleak mingw-w64-x86_64-python-psutil mingw-w64-x86_64-python-psycopg2 mingw-w64-x86_64-python-ptyprocess mingw-w64-x86_64-python-purl mingw-w64-x86_64-python-py-cpuinfo mingw-w64-x86_64-python-py-cpuinfo2 mingw-w64-x86_64-python-pyaml mingw-w64-x86_64-python-pyarrow mingw-w64-x86_64-python-pyasn1 mingw-w64-x86_64-python-pyasn1-modules mingw-w64-x86_64-python-pybullet mingw-w64-x86_64-python-pycodestyle mingw-w64-x86_64-python-pycparser mingw-w64-x86_64-python-pydocstyle mingw-w64-x86_64-python-pydub mingw-w64-x86_64-python-pyelftools mingw-w64-x86_64-python-pyfastbti mingw-w64-x86_64-python-pyfasttextureutils mingw-w64-x86_64-python-pyfdt mingw-w64-x86_64-python-pyfilesystem2 mingw-w64-x86_64-python-pyflakes mingw-w64-x86_64-python-pygccxml mingw-w64-x86_64-python-pygit2 mingw-w64-x86_64-python-pyglm mingw-w64-x86_64-python-pygments mingw-w64-x86_64-python-pygments-github-lexers mingw-w64-x86_64-python-pykwalify mingw-w64-x86_64-python-pylev mingw-w64-x86_64-python-pymongocrypt mingw-w64-x86_64-python-pynput mingw-w64-x86_64-python-pyodbc mingw-w64-x86_64-python-pyopengl mingw-w64-x86_64-python-pyopengl-accelerate mingw-w64-x86_64-python-pyopenssl mingw-w64-x86_64-python-pyparsing mingw-w64-x86_64-python-pyperclip mingw-w64-x86_64-python-pyproject-api mingw-w64-x86_64-python-pyproject-hooks mingw-w64-x86_64-python-pyproject-metadata mingw-w64-x86_64-python-pyqt5 mingw-w64-x86_64-python-pyqt6-sip mingw-w64-x86_64-python-pyrr mingw-w64-x86_64-python-pysfcgal mingw-w64-x86_64-python-pyshp mingw-w64-x86_64-python-pysocks mingw-w64-x86_64-python-pystemmer mingw-w64-x86_64-python-pytest mingw-w64-x86_64-python-pytest-asyncio mingw-w64-x86_64-python-pytest-benchmark mingw-w64-x86_64-python-pytest-cov mingw-w64-x86_64-python-pytest-fixture-config mingw-w64-x86_64-python-pytest-flake8 mingw-w64-x86_64-python-pytest-localserver mingw-w64-x86_64-python-pytest-mock mingw-w64-x86_64-python-pytest-rerunfailures mingw-w64-x86_64-python-pytest-shutil mingw-w64-x86_64-python-pytest-subprocess mingw-w64-x86_64-python-pytest-timeout mingw-w64-x86_64-python-pytest-xdist mingw-w64-x86_64-python-python-discovery mingw-w64-x86_64-python-python-multipart mingw-w64-x86_64-python-python-sbom mingw-w64-x86_64-python-pythran mingw-w64-x86_64-python-pytoolconfig mingw-w64-x86_64-python-pytz mingw-w64-x86_64-python-pyuca mingw-w64-x86_64-python-pyvda mingw-w64-x86_64-python-pywin32 mingw-w64-x86_64-python-pywin32-ctypes mingw-w64-x86_64-python-pyzopfli mingw-w64-x86_64-python-quart mingw-w64-x86_64-python-rapidfuzz mingw-w64-x86_64-python-re-assert mingw-w64-x86_64-python-regex mingw-w64-x86_64-python-requests mingw-w64-x86_64-python-requests-cache mingw-w64-x86_64-python-requests-mock mingw-w64-x86_64-python-requests-toolbelt mingw-w64-x86_64-python-requests-unixsocket mingw-w64-x86_64-python-resolvelib mingw-w64-x86_64-python-responses mingw-w64-x86_64-python-retrying mingw-w64-x86_64-python-rfc3986 mingw-w64-x86_64-python-rfc3987 mingw-w64-x86_64-python-rich mingw-w64-x86_64-python-rich-argparse mingw-w64-x86_64-python-roman-numerals mingw-w64-x86_64-python-ruamel-yaml mingw-w64-x86_64-python-ruamel.yaml.clib mingw-w64-x86_64-python-s2geometry mingw-w64-x86_64-python-scikit-build-core mingw-w64-x86_64-python-scipy mingw-w64-x86_64-python-semantic-version mingw-w64-x86_64-python-setuptools mingw-w64-x86_64-python-setuptools-rust mingw-w64-x86_64-python-setuptools-scm mingw-w64-x86_64-python-setuptools-scm-git-archive mingw-w64-x86_64-python-shellingham mingw-w64-x86_64-python-six mingw-w64-x86_64-python-smartypants mingw-w64-x86_64-python-sniffio mingw-w64-x86_64-python-snowballstemmer mingw-w64-x86_64-python-sortedcontainers mingw-w64-x86_64-python-soupsieve mingw-w64-x86_64-python-sphinx mingw-w64-x86_64-python-sphinx-alabaster-theme mingw-w64-x86_64-python-sphinx-copybutton mingw-w64-x86_64-python-sphinx-issues mingw-w64-x86_64-python-sphinx-notfound-page mingw-w64-x86_64-python-sphinx-tabs mingw-w64-x86_64-python-sphinx_rtd_theme mingw-w64-x86_64-python-sphinxcontrib-applehelp mingw-w64-x86_64-python-sphinxcontrib-devhelp mingw-w64-x86_64-python-sphinxcontrib-github-alt mingw-w64-x86_64-python-sphinxcontrib-htmlhelp mingw-w64-x86_64-python-sphinxcontrib-jquery mingw-w64-x86_64-python-sphinxcontrib-jsmath mingw-w64-x86_64-python-sphinxcontrib-log-cabinet mingw-w64-x86_64-python-sphinxcontrib-moderncmakedomain mingw-w64-x86_64-python-sphinxcontrib-qthelp mingw-w64-x86_64-python-sphinxcontrib-serializinghtml mingw-w64-x86_64-python-sphinxcontrib-svg2pdfconverter mingw-w64-x86_64-python-spinners mingw-w64-x86_64-python-sqlparse mingw-w64-x86_64-python-standard-aifc mingw-w64-x86_64-python-standard-chunk mingw-w64-x86_64-python-standard-sunau mingw-w64-x86_64-python-stestr mingw-w64-x86_64-python-stevedore mingw-w64-x86_64-python-subunit mingw-w64-x86_64-python-termcolor mingw-w64-x86_64-python-testpath mingw-w64-x86_64-python-testrepository mingw-w64-x86_64-python-testresources mingw-w64-x86_64-python-testscenarios mingw-w64-x86_64-python-testtools mingw-w64-x86_64-python-toml mingw-w64-x86_64-python-tomli mingw-w64-x86_64-python-tomli-w mingw-w64-x86_64-python-tomlkit mingw-w64-x86_64-python-toposort mingw-w64-x86_64-python-tornado mingw-w64-x86_64-python-tox mingw-w64-x86_64-python-tqdm mingw-w64-x86_64-python-trio mingw-w64-x86_64-python-trove-classifiers mingw-w64-x86_64-python-twisted mingw-w64-x86_64-python-typing_extensions mingw-w64-x86_64-python-typogrify mingw-w64-x86_64-python-tzdata mingw-w64-x86_64-python-ufolib2 mingw-w64-x86_64-python-ukpostcodeparser mingw-w64-x86_64-python-unidecode mingw-w64-x86_64-python-url-normalize mingw-w64-x86_64-python-urllib3 mingw-w64-x86_64-python-uv mingw-w64-x86_64-python-uv-build mingw-w64-x86_64-python-vcs-versioning mingw-w64-x86_64-python-versioneer mingw-w64-x86_64-python-virtualenv mingw-w64-x86_64-python-virtualenv-clone mingw-w64-x86_64-python-voluptuous mingw-w64-x86_64-python-watchfiles mingw-w64-x86_64-python-wcag-contrast-ratio mingw-w64-x86_64-python-wcwidth mingw-w64-x86_64-python-webencodings mingw-w64-x86_64-python-werkzeug mingw-w64-x86_64-python-wheel mingw-w64-x86_64-python-wmi mingw-w64-x86_64-python-wsproto mingw-w64-x86_64-python-wsproto-docs mingw-w64-x86_64-python-wtforms mingw-w64-x86_64-python-wtforms-docs mingw-w64-x86_64-python-xlsxwriter mingw-w64-x86_64-python-xmlschema mingw-w64-x86_64-python-yaml mingw-w64-x86_64-python-zipp mingw-w64-x86_64-python-zipremove mingw-w64-x86_64-python-zope.interface mingw-w64-x86_64-python-zstandard mingw-w64-x86_64-qbs mingw-w64-x86_64-qdia mingw-w64-x86_64-qhull mingw-w64-x86_64-qrencode mingw-w64-x86_64-qt5-3d mingw-w64-x86_64-qt5-3d-debug mingw-w64-x86_64-qt5-activeqt mingw-w64-x86_64-qt5-activeqt-debug mingw-w64-x86_64-qt5-base mingw-w64-x86_64-qt5-base-debug mingw-w64-x86_64-qt5-charts mingw-w64-x86_64-qt5-charts-debug mingw-w64-x86_64-qt5-connectivity mingw-w64-x86_64-qt5-connectivity-debug mingw-w64-x86_64-qt5-datavis3d mingw-w64-x86_64-qt5-datavis3d-debug mingw-w64-x86_64-qt5-declarative mingw-w64-x86_64-qt5-declarative-debug mingw-w64-x86_64-qt5-gamepad mingw-w64-x86_64-qt5-gamepad-debug mingw-w64-x86_64-qt5-graphicaleffects mingw-w64-x86_64-qt5-graphicaleffects-debug mingw-w64-x86_64-qt5-imageformats mingw-w64-x86_64-qt5-imageformats-debug mingw-w64-x86_64-qt5-location mingw-w64-x86_64-qt5-location-debug mingw-w64-x86_64-qt5-lottie mingw-w64-x86_64-qt5-lottie-debug mingw-w64-x86_64-qt5-multimedia mingw-w64-x86_64-qt5-multimedia-debug mingw-w64-x86_64-qt5-networkauth mingw-w64-x86_64-qt5-networkauth-debug mingw-w64-x86_64-qt5-purchasing mingw-w64-x86_64-qt5-purchasing-debug mingw-w64-x86_64-qt5-quick3d mingw-w64-x86_64-qt5-quick3d-debug mingw-w64-x86_64-qt5-quickcontrols mingw-w64-x86_64-qt5-quickcontrols-debug mingw-w64-x86_64-qt5-quickcontrols2 mingw-w64-x86_64-qt5-quickcontrols2-debug mingw-w64-x86_64-qt5-quicktimeline mingw-w64-x86_64-qt5-quicktimeline-debug mingw-w64-x86_64-qt5-remoteobjects mingw-w64-x86_64-qt5-remoteobjects-debug mingw-w64-x86_64-qt5-script mingw-w64-x86_64-qt5-script-debug mingw-w64-x86_64-qt5-scxml mingw-w64-x86_64-qt5-scxml-debug mingw-w64-x86_64-qt5-sensors mingw-w64-x86_64-qt5-sensors-debug mingw-w64-x86_64-qt5-serialbus mingw-w64-x86_64-qt5-serialbus-debug mingw-w64-x86_64-qt5-serialport mingw-w64-x86_64-qt5-serialport-debug mingw-w64-x86_64-qt5-speech mingw-w64-x86_64-qt5-speech-debug mingw-w64-x86_64-qt5-static mingw-w64-x86_64-qt5-svg mingw-w64-x86_64-qt5-svg-debug mingw-w64-x86_64-qt5-tools mingw-w64-x86_64-qt5-tools-debug mingw-w64-x86_64-qt5-translations mingw-w64-x86_64-qt5-virtualkeyboard mingw-w64-x86_64-qt5-virtualkeyboard-debug mingw-w64-x86_64-qt5-webchannel mingw-w64-x86_64-qt5-webchannel-debug mingw-w64-x86_64-qt5-webglplugin mingw-w64-x86_64-qt5-webglplugin-debug mingw-w64-x86_64-qt5-websockets mingw-w64-x86_64-qt5-websockets-debug mingw-w64-x86_64-qt5-webview mingw-w64-x86_64-qt5-webview-debug mingw-w64-x86_64-qt5-winextras mingw-w64-x86_64-qt5-winextras-debug mingw-w64-x86_64-qt5-xmlpatterns mingw-w64-x86_64-qt5-xmlpatterns-debug mingw-w64-x86_64-qt6-3d mingw-w64-x86_64-qt6-3d-debug mingw-w64-x86_64-qt6-5compat mingw-w64-x86_64-qt6-5compat-debug mingw-w64-x86_64-qt6-activeqt mingw-w64-x86_64-qt6-activeqt-debug mingw-w64-x86_64-qt6-base mingw-w64-x86_64-qt6-base-debug mingw-w64-x86_64-qt6-charts mingw-w64-x86_64-qt6-charts-debug mingw-w64-x86_64-qt6-connectivity mingw-w64-x86_64-qt6-connectivity-debug mingw-w64-x86_64-qt6-datavis3d mingw-w64-x86_64-qt6-datavis3d-debug mingw-w64-x86_64-qt6-declarative mingw-w64-x86_64-qt6-declarative-debug mingw-w64-x86_64-qt6-grpc mingw-w64-x86_64-qt6-grpc-debug mingw-w64-x86_64-qt6-httpserver mingw-w64-x86_64-qt6-httpserver-debug mingw-w64-x86_64-qt6-imageformats mingw-w64-x86_64-qt6-imageformats-debug mingw-w64-x86_64-qt6-languageserver mingw-w64-x86_64-qt6-location mingw-w64-x86_64-qt6-location-debug mingw-w64-x86_64-qt6-lottie mingw-w64-x86_64-qt6-lottie-debug mingw-w64-x86_64-qt6-multimedia mingw-w64-x86_64-qt6-multimedia-debug mingw-w64-x86_64-qt6-multimedia-ffmpeg mingw-w64-x86_64-qt6-multimedia-wmf mingw-w64-x86_64-qt6-networkauth mingw-w64-x86_64-qt6-networkauth-debug mingw-w64-x86_64-qt6-positioning mingw-w64-x86_64-qt6-positioning-debug mingw-w64-x86_64-qt6-quick3d mingw-w64-x86_64-qt6-quick3d-debug mingw-w64-x86_64-qt6-quick3dphysics mingw-w64-x86_64-qt6-quick3dphysics-debug mingw-w64-x86_64-qt6-quickeffectmaker mingw-w64-x86_64-qt6-quickeffectmaker-debug mingw-w64-x86_64-qt6-quicktimeline mingw-w64-x86_64-qt6-quicktimeline-debug mingw-w64-x86_64-qt6-remoteobjects mingw-w64-x86_64-qt6-remoteobjects-debug mingw-w64-x86_64-qt6-scxml mingw-w64-x86_64-qt6-scxml-debug mingw-w64-x86_64-qt6-sensors mingw-w64-x86_64-qt6-sensors-debug mingw-w64-x86_64-qt6-serialbus mingw-w64-x86_64-qt6-serialbus-debug mingw-w64-x86_64-qt6-serialport mingw-w64-x86_64-qt6-serialport-debug mingw-w64-x86_64-qt6-shadertools mingw-w64-x86_64-qt6-shadertools-debug mingw-w64-x86_64-qt6-speech mingw-w64-x86_64-qt6-speech-debug mingw-w64-x86_64-qt6-svg mingw-w64-x86_64-qt6-svg-debug mingw-w64-x86_64-qt6-tools mingw-w64-x86_64-qt6-tools-debug mingw-w64-x86_64-qt6-translations mingw-w64-x86_64-qt6-virtualkeyboard mingw-w64-x86_64-qt6-virtualkeyboard-debug mingw-w64-x86_64-qt6-webchannel mingw-w64-x86_64-qt6-webchannel-debug mingw-w64-x86_64-qt6-websockets mingw-w64-x86_64-qt6-websockets-debug mingw-w64-x86_64-qt6-webview mingw-w64-x86_64-qt6-webview-debug mingw-w64-x86_64-qtkeychain mingw-w64-x86_64-qtwebkit mingw-w64-x86_64-qwt-qt5 mingw-w64-x86_64-qwt-qt6 mingw-w64-x86_64-rabbitmq-c mingw-w64-x86_64-ragel mingw-w64-x86_64-range-v3 mingw-w64-x86_64-rapidfuzz-cpp mingw-w64-x86_64-rapidjson mingw-w64-x86_64-rav1e mingw-w64-x86_64-rcedit mingw-w64-x86_64-re2 mingw-w64-x86_64-readline mingw-w64-x86_64-redis-plus-plus mingw-w64-x86_64-rhash mingw-w64-x86_64-rime-bopomofo mingw-w64-x86_64-rime-cangjie mingw-w64-x86_64-rime-double-pinyin mingw-w64-x86_64-rime-emoji mingw-w64-x86_64-rime-essay mingw-w64-x86_64-rime-luna-pinyin mingw-w64-x86_64-rime-pinyin-simp mingw-w64-x86_64-rime-prelude mingw-w64-x86_64-rime-stroke mingw-w64-x86_64-rime-terra-pinyin mingw-w64-x86_64-rime-wubi mingw-w64-x86_64-robin-hood-hashing mingw-w64-x86_64-robin-map mingw-w64-x86_64-rocksdb mingw-w64-x86_64-rtmpdump mingw-w64-x86_64-ruby mingw-w64-x86_64-ruby-dbus mingw-w64-x86_64-ruby-mustache mingw-w64-x86_64-ruby-native-package-installer mingw-w64-x86_64-ruby-pkg-config mingw-w64-x86_64-ruby-rdiscount mingw-w64-x86_64-ruc-vm mingw-w64-x86_64-rust mingw-w64-x86_64-rust-bindgen mingw-w64-x86_64-rust-src mingw-w64-x86_64-rustup mingw-w64-x86_64-rxspencer mingw-w64-x86_64-s2geometry mingw-w64-x86_64-samtools mingw-w64-x86_64-sassc mingw-w64-x86_64-sbc mingw-w64-x86_64-sbcl mingw-w64-x86_64-sccache mingw-w64-x86_64-scons mingw-w64-x86_64-scour mingw-w64-x86_64-sdcv mingw-w64-x86_64-sdl-gfx mingw-w64-x86_64-sdl-image mingw-w64-x86_64-sdl-mixer mingw-w64-x86_64-sdl-net mingw-w64-x86_64-sdl-ttf mingw-w64-x86_64-sdl12-compat mingw-w64-x86_64-sdl2-compat mingw-w64-x86_64-sdl2-gfx mingw-w64-x86_64-sdl2-image mingw-w64-x86_64-sdl2-mixer mingw-w64-x86_64-sdl2-net mingw-w64-x86_64-sdl2-pango mingw-w64-x86_64-sdl2-sound mingw-w64-x86_64-sdl2-ttf mingw-w64-x86_64-sdl3 mingw-w64-x86_64-sdl3-image mingw-w64-x86_64-sdl3-mixer mingw-w64-x86_64-sdl3-sound mingw-w64-x86_64-sdl3-ttf mingw-w64-x86_64-seacas mingw-w64-x86_64-seexpr mingw-w64-x86_64-sfcgal mingw-w64-x86_64-sgml-common mingw-w64-x86_64-shaderc mingw-w64-x86_64-shared-mime-info mingw-w64-x86_64-shim_executable mingw-w64-x86_64-shine mingw-w64-x86_64-shishi mingw-w64-x86_64-silc-toolkit mingw-w64-x86_64-simdjson mingw-w64-x86_64-simple-mail mingw-w64-x86_64-sip mingw-w64-x86_64-skia mingw-w64-x86_64-skyr-url mingw-w64-x86_64-sl mingw-w64-x86_64-smpeg mingw-w64-x86_64-snappy mingw-w64-x86_64-snoregrowl mingw-w64-x86_64-snorenotify mingw-w64-x86_64-snowball mingw-w64-x86_64-soil mingw-w64-x86_64-sol2 mingw-w64-x86_64-solid3 mingw-w64-x86_64-sonivox mingw-w64-x86_64-soundtouch mingw-w64-x86_64-source-highlight mingw-w64-x86_64-spandsp mingw-w64-x86_64-sparsehash mingw-w64-x86_64-spatialindex mingw-w64-x86_64-spdlog mingw-w64-x86_64-spdylay mingw-w64-x86_64-spectra mingw-w64-x86_64-speex mingw-w64-x86_64-speexdsp mingw-w64-x86_64-spirv-cross mingw-w64-x86_64-spirv-headers mingw-w64-x86_64-spirv-llvm-translator mingw-w64-x86_64-spirv-tools mingw-w64-x86_64-spooles mingw-w64-x86_64-sqlite mingw-w64-x86_64-sqlite-docs mingw-w64-x86_64-sqlite-orm mingw-w64-x86_64-sqlite3mc mingw-w64-x86_64-srt mingw-w64-x86_64-stk mingw-w64-x86_64-stoken mingw-w64-x86_64-stxxl mingw-w64-x86_64-styrene mingw-w64-x86_64-suitesparse mingw-w64-x86_64-sundials mingw-w64-x86_64-superlu mingw-w64-x86_64-svt-av1 mingw-w64-x86_64-swi-prolog-archive mingw-w64-x86_64-swi-prolog-bdb mingw-w64-x86_64-swi-prolog-core mingw-w64-x86_64-swi-prolog-doc mingw-w64-x86_64-swi-prolog-edit mingw-w64-x86_64-swi-prolog-examples mingw-w64-x86_64-swi-prolog-full mingw-w64-x86_64-swi-prolog-json mingw-w64-x86_64-swi-prolog-odbc mingw-w64-x86_64-swi-prolog-packages mingw-w64-x86_64-swi-prolog-pcre2 mingw-w64-x86_64-swi-prolog-python mingw-w64-x86_64-swi-prolog-ssl mingw-w64-x86_64-swi-prolog-tests mingw-w64-x86_64-swi-prolog-x mingw-w64-x86_64-swi-prolog-yaml mingw-w64-x86_64-swig mingw-w64-x86_64-systemc mingw-w64-x86_64-t1utils mingw-w64-x86_64-taglib mingw-w64-x86_64-taucs mingw-w64-x86_64-tbb mingw-w64-x86_64-tcl mingw-w64-x86_64-tcllib mingw-w64-x86_64-tclvfs-cvs mingw-w64-x86_64-tclx mingw-w64-x86_64-template-glib mingw-w64-x86_64-termcap mingw-w64-x86_64-tesseract-ocr mingw-w64-x86_64-testu01 mingw-w64-x86_64-tetgen mingw-w64-x86_64-texinfo mingw-w64-x86_64-tgui mingw-w64-x86_64-thorvg mingw-w64-x86_64-thrift mingw-w64-x86_64-tiledb mingw-w64-x86_64-tinyexpr mingw-w64-x86_64-tinyformat mingw-w64-x86_64-tinyxml mingw-w64-x86_64-tinyxml2 mingw-w64-x86_64-tixi mingw-w64-x86_64-tk mingw-w64-x86_64-tklib mingw-w64-x86_64-tktable mingw-w64-x86_64-tl-expected mingw-w64-x86_64-tmxlite mingw-w64-x86_64-tmxlite-docs mingw-w64-x86_64-toml11 mingw-w64-x86_64-tomlc17 mingw-w64-x86_64-tomlplusplus mingw-w64-x86_64-tools-xctest mingw-w64-x86_64-treedec mingw-w64-x86_64-trompeloeil mingw-w64-x86_64-tslib mingw-w64-x86_64-ttf-cm-unicode mingw-w64-x86_64-ttf-dejavu mingw-w64-x86_64-ttf-paratype mingw-w64-x86_64-ttfautohint mingw-w64-x86_64-twapi mingw-w64-x86_64-tzdata mingw-w64-x86_64-uasm mingw-w64-x86_64-uchardet mingw-w64-x86_64-ucl mingw-w64-x86_64-udunits mingw-w64-x86_64-uhttpmock mingw-w64-x86_64-unbound mingw-w64-x86_64-uncrustify mingw-w64-x86_64-unibilium mingw-w64-x86_64-unicode-character-database mingw-w64-x86_64-unifdef mingw-w64-x86_64-unittest-cpp mingw-w64-x86_64-unixodbc mingw-w64-x86_64-unrar mingw-w64-x86_64-upx mingw-w64-x86_64-uriparser mingw-w64-x86_64-usrsctp mingw-w64-x86_64-utf8cpp mingw-w64-x86_64-uthash mingw-w64-x86_64-uv mingw-w64-x86_64-uvatlas mingw-w64-x86_64-uvicorn mingw-w64-x86_64-vala mingw-w64-x86_64-vid.stab mingw-w64-x86_64-vtk mingw-w64-x86_64-vulkan-headers mingw-w64-x86_64-vulkan-loader mingw-w64-x86_64-vulkan-utility-libraries mingw-w64-x86_64-vulkan-validation-layers mingw-w64-x86_64-w3c-mathml2 mingw-w64-x86_64-wavpack mingw-w64-x86_64-webp-pixbuf-loader mingw-w64-x86_64-wget mingw-w64-x86_64-whisper.cpp mingw-w64-x86_64-win7appid mingw-w64-x86_64-windows-default-manifest mingw-w64-x86_64-wineditline mingw-w64-x86_64-winfile mingw-w64-x86_64-winico mingw-w64-x86_64-winpthreads mingw-w64-x86_64-winpthreads-stub mingw-w64-x86_64-winpty mingw-w64-x86_64-wintab-sdk mingw-w64-x86_64-woff2 mingw-w64-x86_64-wolfssl mingw-w64-x86_64-x264 mingw-w64-x86_64-x265 mingw-w64-x86_64-xapian-core mingw-w64-x86_64-xapian-core-docs mingw-w64-x86_64-xerces-c mingw-w64-x86_64-xmlstarlet mingw-w64-x86_64-xmlto mingw-w64-x86_64-xpm-nox mingw-w64-x86_64-xsimd mingw-w64-x86_64-xvidcore mingw-w64-x86_64-xxhash mingw-w64-x86_64-xz mingw-w64-x86_64-yaml-cpp mingw-w64-x86_64-z3 mingw-w64-x86_64-zeromq mingw-w64-x86_64-zimg mingw-w64-x86_64-zlib mingw-w64-x86_64-zlib-ng mingw-w64-x86_64-zlib-ng-compat mingw-w64-x86_64-zstd mingw-w64-x86_64-zvbi
override FULL_ALIAS_LIST := mingw-w64-x86_64-7zip mingw-w64-x86_64-DevIL mingw-w64-x86_64-SDL mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_gfx mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-SDL2_net mingw-w64-x86_64-SDL2_pango mingw-w64-x86_64-SDL2_sound mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL_gfx mingw-w64-x86_64-SDL_image mingw-w64-x86_64-SDL_mixer mingw-w64-x86_64-SDL_net mingw-w64-x86_64-SDL_ttf mingw-w64-x86_64-Snorenotify mingw-w64-x86_64-abseil-cpp mingw-w64-x86_64-adios2 mingw-w64-x86_64-adobe-source-code-pro-fonts mingw-w64-x86_64-adwaita-icon-theme mingw-w64-x86_64-adwaita-icon-theme-legacy mingw-w64-x86_64-alembic mingw-w64-x86_64-amf-headers mingw-w64-x86_64-angleproject mingw-w64-x86_64-aom mingw-w64-x86_64-appstream-glib mingw-w64-x86_64-argon2 mingw-w64-x86_64-armadillo mingw-w64-x86_64-arpack mingw-w64-x86_64-arpack-ng mingw-w64-x86_64-arpack64 mingw-w64-x86_64-arrow mingw-w64-x86_64-asciidoc mingw-w64-x86_64-asciidoctor mingw-w64-x86_64-asio mingw-w64-x86_64-asiosdk mingw-w64-x86_64-assimp mingw-w64-x86_64-atk mingw-w64-x86_64-autotools mingw-w64-x86_64-aws-c-auth mingw-w64-x86_64-aws-c-cal mingw-w64-x86_64-aws-c-common mingw-w64-x86_64-aws-c-compression mingw-w64-x86_64-aws-c-event-stream mingw-w64-x86_64-aws-c-http mingw-w64-x86_64-aws-c-io mingw-w64-x86_64-aws-c-mqtt mingw-w64-x86_64-aws-c-s3 mingw-w64-x86_64-aws-c-sdkutils mingw-w64-x86_64-aws-checksums mingw-w64-x86_64-aws-crt-cpp mingw-w64-x86_64-aws-sdk-cpp mingw-w64-x86_64-b3sum mingw-w64-x86_64-bin2header mingw-w64-x86_64-binaryen mingw-w64-x86_64-binutils mingw-w64-x86_64-blas mingw-w64-x86_64-blas64 mingw-w64-x86_64-blosc mingw-w64-x86_64-blosc2 mingw-w64-x86_64-boost mingw-w64-x86_64-boost-libs mingw-w64-x86_64-brotli mingw-w64-x86_64-brotli-testdata mingw-w64-x86_64-bullet mingw-w64-x86_64-bullet-dp mingw-w64-x86_64-bzip2 mingw-w64-x86_64-c-ares mingw-w64-x86_64-ca-certificates mingw-w64-x86_64-cairo mingw-w64-x86_64-cantarell-fonts mingw-w64-x86_64-capstone mingw-w64-x86_64-cargo-c mingw-w64-x86_64-cargo-edit mingw-w64-x86_64-cargo-generate mingw-w64-x86_64-cargo-local-registry mingw-w64-x86_64-cas-offinder mingw-w64-x86_64-castxml mingw-w64-x86_64-catch mingw-w64-x86_64-cblas mingw-w64-x86_64-cblas64 mingw-w64-x86_64-cc mingw-w64-x86_64-cc-libs mingw-w64-x86_64-ccache mingw-w64-x86_64-cccl mingw-w64-x86_64-ccmake mingw-w64-x86_64-cddlib mingw-w64-x86_64-cereal mingw-w64-x86_64-ceres-solver mingw-w64-x86_64-cfitsio mingw-w64-x86_64-cgal mingw-w64-x86_64-cglm mingw-w64-x86_64-cglm-docs mingw-w64-x86_64-cgns mingw-w64-x86_64-chafa mingw-w64-x86_64-check mingw-w64-x86_64-chicken mingw-w64-x86_64-cjose mingw-w64-x86_64-cjson mingw-w64-x86_64-clang mingw-w64-x86_64-clang-analyzer mingw-w64-x86_64-clang-libs mingw-w64-x86_64-clang-tools-extra mingw-w64-x86_64-clblast mingw-w64-x86_64-cli11 mingw-w64-x86_64-clucene mingw-w64-x86_64-cmake mingw-w64-x86_64-cmake-bootstrap mingw-w64-x86_64-cmake-cmcldeps mingw-w64-x86_64-cmake-doc-qt mingw-w64-x86_64-cmake-docs mingw-w64-x86_64-cmake-gui mingw-w64-x86_64-cmakelang mingw-w64-x86_64-cmakerc mingw-w64-x86_64-cmocka mingw-w64-x86_64-cninja mingw-w64-x86_64-codec2 mingw-w64-x86_64-collada-dom mingw-w64-x86_64-compiler-rt mingw-w64-x86_64-connect mingw-w64-x86_64-corrade mingw-w64-x86_64-cppcheck mingw-w64-x86_64-cppcoro mingw-w64-x86_64-cppdap mingw-w64-x86_64-cppreference-qt mingw-w64-x86_64-cpptest mingw-w64-x86_64-cppunit mingw-w64-x86_64-cpputest mingw-w64-x86_64-cppwinrt mingw-w64-x86_64-cppzmq mingw-w64-x86_64-crt mingw-w64-x86_64-crt-git mingw-w64-x86_64-crypto++ mingw-w64-x86_64-ctags mingw-w64-x86_64-ctpl mingw-w64-x86_64-ctpl-git mingw-w64-x86_64-cunit mingw-w64-x86_64-curl mingw-w64-x86_64-curl-gnutls mingw-w64-x86_64-curl-winssl mingw-w64-x86_64-curses mingw-w64-x86_64-cusp mingw-w64-x86_64-cxxopts mingw-w64-x86_64-cyrus-sasl mingw-w64-x86_64-cython mingw-w64-x86_64-d-feet mingw-w64-x86_64-dav1d mingw-w64-x86_64-db mingw-w64-x86_64-db-docs mingw-w64-x86_64-dbus mingw-w64-x86_64-dbus-c++ mingw-w64-x86_64-dbus-doc mingw-w64-x86_64-dbus-glib mingw-w64-x86_64-dcadec mingw-w64-x86_64-devil mingw-w64-x86_64-directx-headers mingw-w64-x86_64-directxmath mingw-w64-x86_64-directxmesh mingw-w64-x86_64-directxtex mingw-w64-x86_64-djvulibre mingw-w64-x86_64-dlfcn mingw-w64-x86_64-dnssec-anchors mingw-w64-x86_64-docbook-dsssl mingw-w64-x86_64-docbook-mathml mingw-w64-x86_64-docbook-xml mingw-w64-x86_64-docbook-xsl mingw-w64-x86_64-docbook5-xml mingw-w64-x86_64-doctest mingw-w64-x86_64-docx2txt-rs mingw-w64-x86_64-double-conversion mingw-w64-x86_64-doxygen mingw-w64-x86_64-draco mingw-w64-x86_64-dsdp mingw-w64-x86_64-duktape mingw-w64-x86_64-efxc2 mingw-w64-x86_64-egl-headers mingw-w64-x86_64-eigen3 mingw-w64-x86_64-expat mingw-w64-x86_64-exprtk mingw-w64-x86_64-extra-cmake-modules mingw-w64-x86_64-fast_float mingw-w64-x86_64-fc mingw-w64-x86_64-fc-libs mingw-w64-x86_64-ffmpeg mingw-w64-x86_64-ffms2 mingw-w64-x86_64-ffnvcodec-headers mingw-w64-x86_64-fftw mingw-w64-x86_64-file mingw-w64-x86_64-firebird mingw-w64-x86_64-flac mingw-w64-x86_64-flatbuffers mingw-w64-x86_64-flif mingw-w64-x86_64-fluidsynth mingw-w64-x86_64-fmt mingw-w64-x86_64-fontconfig mingw-w64-x86_64-freeglut mingw-w64-x86_64-freeimage mingw-w64-x86_64-freetype mingw-w64-x86_64-frei0r-plugins mingw-w64-x86_64-fribidi mingw-w64-x86_64-fxc2 mingw-w64-x86_64-fxc2-git mingw-w64-x86_64-gavl mingw-w64-x86_64-gc mingw-w64-x86_64-gcab mingw-w64-x86_64-gcc mingw-w64-x86_64-gcc-ada mingw-w64-x86_64-gcc-fortran mingw-w64-x86_64-gcc-libgfortran mingw-w64-x86_64-gcc-libs mingw-w64-x86_64-gcc-lto-dump mingw-w64-x86_64-gcc-objc mingw-w64-x86_64-gcc-plugin mingw-w64-x86_64-gdal mingw-w64-x86_64-gdb mingw-w64-x86_64-gdb-multiarch mingw-w64-x86_64-gdbm mingw-w64-x86_64-gdk-pixbuf2 mingw-w64-x86_64-gdk-pixbuf2-docs mingw-w64-x86_64-gdl mingw-w64-x86_64-geocode-glib mingw-w64-x86_64-geographiclib mingw-w64-x86_64-geoip mingw-w64-x86_64-geos mingw-w64-x86_64-gettext mingw-w64-x86_64-gettext-libtextstyle mingw-w64-x86_64-gettext-runtime mingw-w64-x86_64-gettext-tools mingw-w64-x86_64-gf2x mingw-w64-x86_64-gflags mingw-w64-x86_64-ggml mingw-w64-x86_64-ghostscript mingw-w64-x86_64-gi-docgen mingw-w64-x86_64-giflib mingw-w64-x86_64-git mingw-w64-x86_64-git-archimport mingw-w64-x86_64-git-credential-wincred mingw-w64-x86_64-git-cvs mingw-w64-x86_64-git-doc-html mingw-w64-x86_64-git-doc-man mingw-w64-x86_64-git-for-windows-addons mingw-w64-x86_64-git-gui mingw-w64-x86_64-git-lfs mingw-w64-x86_64-git-p4 mingw-w64-x86_64-git-perl mingw-w64-x86_64-git-send-email mingw-w64-x86_64-git-subtree mingw-w64-x86_64-git-svn mingw-w64-x86_64-gitk mingw-w64-x86_64-gitweb mingw-w64-x86_64-gl2ps mingw-w64-x86_64-glbinding mingw-w64-x86_64-gles-headers mingw-w64-x86_64-glew mingw-w64-x86_64-glfw mingw-w64-x86_64-glfw-docs mingw-w64-x86_64-glib-networking mingw-w64-x86_64-glib2 mingw-w64-x86_64-glm mingw-w64-x86_64-global mingw-w64-x86_64-globjects mingw-w64-x86_64-glog mingw-w64-x86_64-glpk mingw-w64-x86_64-glsl-optimizer mingw-w64-x86_64-glsl-optimizer-git mingw-w64-x86_64-glslang mingw-w64-x86_64-gmp mingw-w64-x86_64-gn mingw-w64-x86_64-gnome-common mingw-w64-x86_64-gnupg mingw-w64-x86_64-gnustep-back mingw-w64-x86_64-gnustep-base mingw-w64-x86_64-gnustep-gui mingw-w64-x86_64-gnustep-make mingw-w64-x86_64-gnutls mingw-w64-x86_64-go mingw-w64-x86_64-gobject-introspection mingw-w64-x86_64-gobject-introspection-runtime mingw-w64-x86_64-gperf mingw-w64-x86_64-gputils mingw-w64-x86_64-graphene mingw-w64-x86_64-graphite2 mingw-w64-x86_64-graphviz mingw-w64-x86_64-graphviz-docs mingw-w64-x86_64-grpc mingw-w64-x86_64-gsettings-desktop-schemas mingw-w64-x86_64-gsfonts mingw-w64-x86_64-gsl mingw-w64-x86_64-gsm mingw-w64-x86_64-gss mingw-w64-x86_64-gst-plugins-base mingw-w64-x86_64-gstreamer mingw-w64-x86_64-gtest mingw-w64-x86_64-gtk-doc mingw-w64-x86_64-gtk-update-icon-cache mingw-w64-x86_64-gtk3 mingw-w64-x86_64-gts mingw-w64-x86_64-gumbo-parser mingw-w64-x86_64-harfbuzz mingw-w64-x86_64-harfbuzz-cairo mingw-w64-x86_64-harfbuzz-docs mingw-w64-x86_64-harfbuzz-icu mingw-w64-x86_64-harfbuzz-utils mingw-w64-x86_64-hdf4 mingw-w64-x86_64-hdf5 mingw-w64-x86_64-headers mingw-w64-x86_64-headers-git mingw-w64-x86_64-hererocks mingw-w64-x86_64-hicolor-icon-theme mingw-w64-x86_64-highway mingw-w64-x86_64-hiredis mingw-w64-x86_64-hlsl2glsl mingw-w64-x86_64-hlsl2glsl-git mingw-w64-x86_64-htslib mingw-w64-x86_64-http-parser mingw-w64-x86_64-hunspell mingw-w64-x86_64-hunspell-en mingw-w64-x86_64-hwloc mingw-w64-x86_64-hypercorn mingw-w64-x86_64-i2pd mingw-w64-x86_64-iconv mingw-w64-x86_64-icoutils mingw-w64-x86_64-icu mingw-w64-x86_64-iir mingw-w64-x86_64-imagemagick mingw-w64-x86_64-imagemagick-docs mingw-w64-x86_64-imath mingw-w64-x86_64-iml mingw-w64-x86_64-include-what-you-use mingw-w64-x86_64-indent mingw-w64-x86_64-inja mingw-w64-x86_64-isa-l mingw-w64-x86_64-isl mingw-w64-x86_64-iso-codes mingw-w64-x86_64-itstool mingw-w64-x86_64-jack2 mingw-w64-x86_64-jansson mingw-w64-x86_64-jasper mingw-w64-x86_64-jasper-docs mingw-w64-x86_64-jbig2dec mingw-w64-x86_64-jbigkit mingw-w64-x86_64-jemalloc mingw-w64-x86_64-json-c mingw-w64-x86_64-json-glib mingw-w64-x86_64-jsoncpp mingw-w64-x86_64-jsonrpc-glib mingw-w64-x86_64-jsonrpc-glib-docs mingw-w64-x86_64-junitparser mingw-w64-x86_64-jxrlib mingw-w64-x86_64-keystone mingw-w64-x86_64-kiss_fft mingw-w64-x86_64-krpc-cpp mingw-w64-x86_64-kseexpr mingw-w64-x86_64-ktx-software mingw-w64-x86_64-kvazaar mingw-w64-x86_64-l-smash mingw-w64-x86_64-ladspa-sdk mingw-w64-x86_64-lager mingw-w64-x86_64-lame mingw-w64-x86_64-lapack mingw-w64-x86_64-lapack64 mingw-w64-x86_64-lapacke mingw-w64-x86_64-lapacke64 mingw-w64-x86_64-lcms2 mingw-w64-x86_64-lcov mingw-w64-x86_64-ldns mingw-w64-x86_64-ledger mingw-w64-x86_64-lensfun mingw-w64-x86_64-leptonica mingw-w64-x86_64-lerc mingw-w64-x86_64-level-zero-headers mingw-w64-x86_64-level-zero-loader mingw-w64-x86_64-lib2geom mingw-w64-x86_64-lib3mf mingw-w64-x86_64-libaacs mingw-w64-x86_64-libaec mingw-w64-x86_64-libarchive mingw-w64-x86_64-libaribcaption mingw-w64-x86_64-libart_lgpl mingw-w64-x86_64-libass mingw-w64-x86_64-libassuan mingw-w64-x86_64-libatomic mingw-w64-x86_64-libatomic_ops mingw-w64-x86_64-libavif mingw-w64-x86_64-libavro mingw-w64-x86_64-libb2 mingw-w64-x86_64-libb64 mingw-w64-x86_64-libbdplus mingw-w64-x86_64-libblake3 mingw-w64-x86_64-libblocksruntime mingw-w64-x86_64-libblocksruntime-swift mingw-w64-x86_64-libbluray mingw-w64-x86_64-libbotan mingw-w64-x86_64-libbs2b mingw-w64-x86_64-libbsdf mingw-w64-x86_64-libc++ mingw-w64-x86_64-libcaca mingw-w64-x86_64-libcacard mingw-w64-x86_64-libcbor mingw-w64-x86_64-libcddb mingw-w64-x86_64-libcdio mingw-w64-x86_64-libcdio-paranoia mingw-w64-x86_64-libcdr mingw-w64-x86_64-libcello mingw-w64-x86_64-libcello-git mingw-w64-x86_64-libcerf mingw-w64-x86_64-libclc mingw-w64-x86_64-libcloudproviders mingw-w64-x86_64-libconfig mingw-w64-x86_64-libconfini mingw-w64-x86_64-libcue mingw-w64-x86_64-libdatrie mingw-w64-x86_64-libdatrie-docs mingw-w64-x86_64-libdazzle mingw-w64-x86_64-libdca mingw-w64-x86_64-libde265 mingw-w64-x86_64-libdeflate mingw-w64-x86_64-libdex mingw-w64-x86_64-libdicom mingw-w64-x86_64-libdispatch mingw-w64-x86_64-libdmtx mingw-w64-x86_64-libdovi mingw-w64-x86_64-libdsm mingw-w64-x86_64-libdvdcss mingw-w64-x86_64-libdvdread mingw-w64-x86_64-libebml mingw-w64-x86_64-libelf mingw-w64-x86_64-libepoxy mingw-w64-x86_64-libevent mingw-w64-x86_64-libexif mingw-w64-x86_64-libffi mingw-w64-x86_64-libfido2 mingw-w64-x86_64-libfreexl mingw-w64-x86_64-libfyaml mingw-w64-x86_64-libgadu mingw-w64-x86_64-libgcc mingw-w64-x86_64-libgccjit mingw-w64-x86_64-libgcrypt mingw-w64-x86_64-libgd mingw-w64-x86_64-libgeotiff mingw-w64-x86_64-libgfortran mingw-w64-x86_64-libgig mingw-w64-x86_64-libgit2 mingw-w64-x86_64-libgit2-glib mingw-w64-x86_64-libgit2-glib-docs mingw-w64-x86_64-libgit2-winhttp mingw-w64-x86_64-libgme mingw-w64-x86_64-libgomp mingw-w64-x86_64-libgoom2 mingw-w64-x86_64-libgpg-error mingw-w64-x86_64-libgsf mingw-w64-x86_64-libguess mingw-w64-x86_64-libgweather mingw-w64-x86_64-libgxps mingw-w64-x86_64-libharu mingw-w64-x86_64-libheif mingw-w64-x86_64-libical mingw-w64-x86_64-libiconv mingw-w64-x86_64-libicsneo mingw-w64-x86_64-libidl2 mingw-w64-x86_64-libidn mingw-w64-x86_64-libidn2 mingw-w64-x86_64-libiff mingw-w64-x86_64-libilbc mingw-w64-x86_64-libilbm mingw-w64-x86_64-libimagequant mingw-w64-x86_64-libimobiledevice-glue mingw-w64-x86_64-libinih mingw-w64-x86_64-libipt mingw-w64-x86_64-libiptcdata mingw-w64-x86_64-libjpeg mingw-w64-x86_64-libjpeg-turbo mingw-w64-x86_64-libjxl mingw-w64-x86_64-libkml mingw-w64-x86_64-libksba mingw-w64-x86_64-liblas mingw-w64-x86_64-liblc3 mingw-w64-x86_64-liblcf mingw-w64-x86_64-liblo mingw-w64-x86_64-liblqr mingw-w64-x86_64-libltdl mingw-w64-x86_64-libmad mingw-w64-x86_64-libmariadbclient mingw-w64-x86_64-libmfx mingw-w64-x86_64-libmicrodns mingw-w64-x86_64-libmikmod mingw-w64-x86_64-libmimic mingw-w64-x86_64-libmng mingw-w64-x86_64-libmodbus mingw-w64-x86_64-libmodbus-git mingw-w64-x86_64-libmodplug mingw-w64-x86_64-libmongocrypt mingw-w64-x86_64-libmongoose mingw-w64-x86_64-libmongoose-docs mingw-w64-x86_64-libmowgli mingw-w64-x86_64-libmpack mingw-w64-x86_64-libmysofa mingw-w64-x86_64-libnfs mingw-w64-x86_64-libnice mingw-w64-x86_64-libntlm mingw-w64-x86_64-libnumbertext mingw-w64-x86_64-liboauth mingw-w64-x86_64-libobjc mingw-w64-x86_64-libobjc2 mingw-w64-x86_64-libodfgen mingw-w64-x86_64-libogg mingw-w64-x86_64-libomemo-c mingw-w64-x86_64-libopenmpt mingw-w64-x86_64-libopenmpt-modplug mingw-w64-x86_64-libopusenc mingw-w64-x86_64-liboqs mingw-w64-x86_64-libosmpbf mingw-w64-x86_64-libotr mingw-w64-x86_64-libp11 mingw-w64-x86_64-libpaper mingw-w64-x86_64-libpcap mingw-w64-x86_64-libplacebo mingw-w64-x86_64-libplacebo-demos mingw-w64-x86_64-libplist mingw-w64-x86_64-libpng mingw-w64-x86_64-libpqxx mingw-w64-x86_64-libpreferencepanes mingw-w64-x86_64-libprojectm mingw-w64-x86_64-libproxy mingw-w64-x86_64-libpsl mingw-w64-x86_64-libpst mingw-w64-x86_64-libquadmath mingw-w64-x86_64-libraqm mingw-w64-x86_64-libraw mingw-w64-x86_64-librdkafka mingw-w64-x86_64-libreplaygain mingw-w64-x86_64-libresidfp mingw-w64-x86_64-librevenge mingw-w64-x86_64-librime-data mingw-w64-x86_64-librsvg mingw-w64-x86_64-librsvg-docs mingw-w64-x86_64-librttopo mingw-w64-x86_64-libsamplerate mingw-w64-x86_64-libsass mingw-w64-x86_64-libsbml mingw-w64-x86_64-libsbsms mingw-w64-x86_64-libsecret mingw-w64-x86_64-libserdes mingw-w64-x86_64-libsharpyuv mingw-w64-x86_64-libshout mingw-w64-x86_64-libsignal-protocol-c mingw-w64-x86_64-libsmi mingw-w64-x86_64-libsndfile mingw-w64-x86_64-libsodium mingw-w64-x86_64-libsoup3 mingw-w64-x86_64-libsoup3-docs mingw-w64-x86_64-libsoxr mingw-w64-x86_64-libspatialite mingw-w64-x86_64-libspiro mingw-w64-x86_64-libspng mingw-w64-x86_64-libsquish mingw-w64-x86_64-libssh mingw-w64-x86_64-libssh2 mingw-w64-x86_64-libssh2-wincng mingw-w64-x86_64-libstdc++ mingw-w64-x86_64-libstemmer mingw-w64-x86_64-libsystre mingw-w64-x86_64-libtasn1 mingw-w64-x86_64-libtermkey mingw-w64-x86_64-libthai mingw-w64-x86_64-libthai-docs mingw-w64-x86_64-libtheora mingw-w64-x86_64-libtiff mingw-w64-x86_64-libtiff-docs mingw-w64-x86_64-libtimidity mingw-w64-x86_64-libtomcrypt mingw-w64-x86_64-libtommath mingw-w64-x86_64-libtool mingw-w64-x86_64-libtorrent-rasterbar mingw-w64-x86_64-libtre mingw-w64-x86_64-libu2f-emu mingw-w64-x86_64-libultrahdr mingw-w64-x86_64-libunibreak mingw-w64-x86_64-libunifex mingw-w64-x86_64-libuninameslist mingw-w64-x86_64-libunistring mingw-w64-x86_64-libunwind mingw-w64-x86_64-libusb-win32 mingw-w64-x86_64-libusbmuxd mingw-w64-x86_64-libutf8proc mingw-w64-x86_64-libutp mingw-w64-x86_64-libuv mingw-w64-x86_64-libva mingw-w64-x86_64-libva-utils mingw-w64-x86_64-libvisio mingw-w64-x86_64-libvoikko mingw-w64-x86_64-libvorbis mingw-w64-x86_64-libvpl mingw-w64-x86_64-libvpx mingw-w64-x86_64-libwebm mingw-w64-x86_64-libwebp mingw-w64-x86_64-libwebsockets mingw-w64-x86_64-libwinpthread mingw-w64-x86_64-libwinpthread-git mingw-w64-x86_64-libwmf mingw-w64-x86_64-libwpd mingw-w64-x86_64-libwpg mingw-w64-x86_64-libx264 mingw-w64-x86_64-libxisf mingw-w64-x86_64-libxlsxwriter mingw-w64-x86_64-libxml++-5.0 mingw-w64-x86_64-libxml2 mingw-w64-x86_64-libxmlb mingw-w64-x86_64-libxmp mingw-w64-x86_64-libxslt mingw-w64-x86_64-libyaml mingw-w64-x86_64-libyuv mingw-w64-x86_64-libzen mingw-w64-x86_64-libzip mingw-w64-x86_64-lis mingw-w64-x86_64-litehtml mingw-w64-x86_64-llama.cpp mingw-w64-x86_64-lld mingw-w64-x86_64-lldb mingw-w64-x86_64-lldb-mi mingw-w64-x86_64-llvm mingw-w64-x86_64-llvm-libs mingw-w64-x86_64-llvm-tools mingw-w64-x86_64-lsp-dsp-lib mingw-w64-x86_64-lto-dump mingw-w64-x86_64-lua mingw-w64-x86_64-lua51 mingw-w64-x86_64-lua53 mingw-w64-x86_64-lua54 mingw-w64-x86_64-lz4 mingw-w64-x86_64-lzo2 mingw-w64-x86_64-make mingw-w64-x86_64-mathjax2 mingw-w64-x86_64-matio mingw-w64-x86_64-maturin mingw-w64-x86_64-maxflow mingw-w64-x86_64-mbedtls mingw-w64-x86_64-mcfgthread mingw-w64-x86_64-mcfgthread-libs mingw-w64-x86_64-md4c mingw-w64-x86_64-mdbook mingw-w64-x86_64-mdbook-admonish mingw-w64-x86_64-mdbook-mermaid mingw-w64-x86_64-mdbook-pikchr mingw-w64-x86_64-mdloader mingw-w64-x86_64-meanwhile mingw-w64-x86_64-mecab mingw-w64-x86_64-mesa mingw-w64-x86_64-meson mingw-w64-x86_64-meson-python mingw-w64-x86_64-metis mingw-w64-x86_64-minisign mingw-w64-x86_64-miniupnpc mingw-w64-x86_64-minizip mingw-w64-x86_64-minizip-ng mingw-w64-x86_64-mman-win32 mingw-w64-x86_64-moab mingw-w64-x86_64-mongo-c-driver mingw-w64-x86_64-mpc mingw-w64-x86_64-mpdecimal mingw-w64-x86_64-mpfr mingw-w64-x86_64-mpg123 mingw-w64-x86_64-mruby mingw-w64-x86_64-mscgen mingw-w64-x86_64-msgpack-c mingw-w64-x86_64-msitools mingw-w64-x86_64-msmpi mingw-w64-x86_64-mtex2MML mingw-w64-x86_64-mtn mingw-w64-x86_64-mtools mingw-w64-x86_64-muparser mingw-w64-x86_64-mxml mingw-w64-x86_64-mypy mingw-w64-x86_64-nanobind mingw-w64-x86_64-nasm mingw-w64-x86_64-ncurses mingw-w64-x86_64-netcdf mingw-w64-x86_64-netcdf-cxx mingw-w64-x86_64-netcdf-fortran mingw-w64-x86_64-nettle mingw-w64-x86_64-ng-log mingw-w64-x86_64-nghttp2 mingw-w64-x86_64-nghttp3 mingw-w64-x86_64-ngtcp2 mingw-w64-x86_64-nim mingw-w64-x86_64-ninja mingw-w64-x86_64-nlohmann-json mingw-w64-x86_64-nlohmann_json mingw-w64-x86_64-nlopt mingw-w64-x86_64-npth mingw-w64-x86_64-nsis mingw-w64-x86_64-nspr mingw-w64-x86_64-nss mingw-w64-x86_64-ntldd mingw-w64-x86_64-ntldd-git mingw-w64-x86_64-nuspell mingw-w64-x86_64-ogre3d mingw-w64-x86_64-omp mingw-w64-x86_64-onevpl mingw-w64-x86_64-openal mingw-w64-x86_64-openblas mingw-w64-x86_64-openblas64 mingw-w64-x86_64-opencascade mingw-w64-x86_64-opencascade-doc-overview mingw-w64-x86_64-opencascade-doc-refman mingw-w64-x86_64-opencl-headers mingw-w64-x86_64-opencl-icd mingw-w64-x86_64-opencore-amr mingw-w64-x86_64-opencsg mingw-w64-x86_64-opencv mingw-w64-x86_64-openexr mingw-w64-x86_64-openfa mingw-w64-x86_64-opengl-man-pages mingw-w64-x86_64-openh264 mingw-w64-x86_64-openjpeg2 mingw-w64-x86_64-openjph mingw-w64-x86_64-openlibm mingw-w64-x86_64-openslide mingw-w64-x86_64-openssl mingw-w64-x86_64-openturns mingw-w64-x86_64-openvdb mingw-w64-x86_64-openvr mingw-w64-x86_64-openxr-sdk mingw-w64-x86_64-optipng mingw-w64-x86_64-opus mingw-w64-x86_64-opusfile mingw-w64-x86_64-oqsprovider mingw-w64-x86_64-orc mingw-w64-x86_64-osl mingw-w64-x86_64-osslsigncode mingw-w64-x86_64-p11-kit mingw-w64-x86_64-pango mingw-w64-x86_64-pango-docs mingw-w64-x86_64-parpack mingw-w64-x86_64-pcaudiolib mingw-w64-x86_64-pcre mingw-w64-x86_64-pcre2 mingw-w64-x86_64-pdal mingw-w64-x86_64-pdcurses mingw-w64-x86_64-pdf2svg mingw-w64-x86_64-pelican mingw-w64-x86_64-perl mingw-w64-x86_64-perl-_charnames mingw-w64-x86_64-perl-amiga-arexx mingw-w64-x86_64-perl-amiga-exec mingw-w64-x86_64-perl-anydbm_file mingw-w64-x86_64-perl-app-cpan mingw-w64-x86_64-perl-app-prove mingw-w64-x86_64-perl-app-prove-state mingw-w64-x86_64-perl-app-prove-state-result mingw-w64-x86_64-perl-app-prove-state-result-test mingw-w64-x86_64-perl-archive-tar mingw-w64-x86_64-perl-archive-tar-constant mingw-w64-x86_64-perl-archive-tar-file mingw-w64-x86_64-perl-attribute-handlers mingw-w64-x86_64-perl-attributes mingw-w64-x86_64-perl-autodie mingw-w64-x86_64-perl-autodie-exception mingw-w64-x86_64-perl-autodie-exception-system mingw-w64-x86_64-perl-autodie-hints mingw-w64-x86_64-perl-autodie-scope-guard mingw-w64-x86_64-perl-autodie-scope-guardstack mingw-w64-x86_64-perl-autodie-skip mingw-w64-x86_64-perl-autodie-util mingw-w64-x86_64-perl-autoloader mingw-w64-x86_64-perl-autosplit mingw-w64-x86_64-perl-autouse mingw-w64-x86_64-perl-b mingw-w64-x86_64-perl-b-concise mingw-w64-x86_64-perl-b-deparse mingw-w64-x86_64-perl-b-op_private mingw-w64-x86_64-perl-b-showlex mingw-w64-x86_64-perl-b-terse mingw-w64-x86_64-perl-b-xref mingw-w64-x86_64-perl-base mingw-w64-x86_64-perl-benchmark mingw-w64-x86_64-perl-bigfloat mingw-w64-x86_64-perl-bigint mingw-w64-x86_64-perl-bignum mingw-w64-x86_64-perl-bigrat mingw-w64-x86_64-perl-blib mingw-w64-x86_64-perl-builtin mingw-w64-x86_64-perl-bytes mingw-w64-x86_64-perl-carp mingw-w64-x86_64-perl-carp-heavy mingw-w64-x86_64-perl-charnames mingw-w64-x86_64-perl-class-struct mingw-w64-x86_64-perl-compress-raw-bzip2 mingw-w64-x86_64-perl-compress-raw-zlib mingw-w64-x86_64-perl-compress-zlib mingw-w64-x86_64-perl-config mingw-w64-x86_64-perl-config-extensions mingw-w64-x86_64-perl-config-perl-v mingw-w64-x86_64-perl-constant mingw-w64-x86_64-perl-cpan mingw-w64-x86_64-perl-cpan-author mingw-w64-x86_64-perl-cpan-bundle mingw-w64-x86_64-perl-cpan-cachemgr mingw-w64-x86_64-perl-cpan-complete mingw-w64-x86_64-perl-cpan-debug mingw-w64-x86_64-perl-cpan-deferredcode mingw-w64-x86_64-perl-cpan-distribution mingw-w64-x86_64-perl-cpan-distroprefs mingw-w64-x86_64-perl-cpan-distrostatus mingw-w64-x86_64-perl-cpan-exception-blocked_urllist mingw-w64-x86_64-perl-cpan-exception-recursivedependency mingw-w64-x86_64-perl-cpan-exception-yaml_not_installed mingw-w64-x86_64-perl-cpan-exception-yaml_process_error mingw-w64-x86_64-perl-cpan-firsttime mingw-w64-x86_64-perl-cpan-ftp mingw-w64-x86_64-perl-cpan-ftp-netrc mingw-w64-x86_64-perl-cpan-handleconfig mingw-w64-x86_64-perl-cpan-http-client mingw-w64-x86_64-perl-cpan-http-credentials mingw-w64-x86_64-perl-cpan-index mingw-w64-x86_64-perl-cpan-infoobj mingw-w64-x86_64-perl-cpan-kwalify mingw-w64-x86_64-perl-cpan-lwp-useragent mingw-w64-x86_64-perl-cpan-meta mingw-w64-x86_64-perl-cpan-meta-converter mingw-w64-x86_64-perl-cpan-meta-feature mingw-w64-x86_64-perl-cpan-meta-history mingw-w64-x86_64-perl-cpan-meta-merge mingw-w64-x86_64-perl-cpan-meta-prereqs mingw-w64-x86_64-perl-cpan-meta-requirements mingw-w64-x86_64-perl-cpan-meta-requirements-range mingw-w64-x86_64-perl-cpan-meta-spec mingw-w64-x86_64-perl-cpan-meta-validator mingw-w64-x86_64-perl-cpan-meta-yaml mingw-w64-x86_64-perl-cpan-mirrors mingw-w64-x86_64-perl-cpan-module mingw-w64-x86_64-perl-cpan-nox mingw-w64-x86_64-perl-cpan-plugin mingw-w64-x86_64-perl-cpan-plugin-specfile mingw-w64-x86_64-perl-cpan-prompt mingw-w64-x86_64-perl-cpan-queue mingw-w64-x86_64-perl-cpan-shell mingw-w64-x86_64-perl-cpan-tarzip mingw-w64-x86_64-perl-cpan-url mingw-w64-x86_64-perl-cpan-version mingw-w64-x86_64-perl-cwd mingw-w64-x86_64-perl-data-dumper mingw-w64-x86_64-perl-db mingw-w64-x86_64-perl-db_file mingw-w64-x86_64-perl-dbm_filter mingw-w64-x86_64-perl-dbm_filter-compress mingw-w64-x86_64-perl-dbm_filter-encode mingw-w64-x86_64-perl-dbm_filter-int32 mingw-w64-x86_64-perl-dbm_filter-null mingw-w64-x86_64-perl-dbm_filter-utf8 mingw-w64-x86_64-perl-deprecate mingw-w64-x86_64-perl-devel-peek mingw-w64-x86_64-perl-devel-ppport mingw-w64-x86_64-perl-devel-selfstubber mingw-w64-x86_64-perl-diagnostics mingw-w64-x86_64-perl-digest mingw-w64-x86_64-perl-digest-base mingw-w64-x86_64-perl-digest-file mingw-w64-x86_64-perl-digest-md5 mingw-w64-x86_64-perl-digest-sha mingw-w64-x86_64-perl-dirhandle mingw-w64-x86_64-perl-doc mingw-w64-x86_64-perl-dumpvalue mingw-w64-x86_64-perl-dynaloader mingw-w64-x86_64-perl-encode mingw-w64-x86_64-perl-encode-alias mingw-w64-x86_64-perl-encode-byte mingw-w64-x86_64-perl-encode-cjkconstants mingw-w64-x86_64-perl-encode-cn mingw-w64-x86_64-perl-encode-cn-hz mingw-w64-x86_64-perl-encode-compat mingw-w64-x86_64-perl-encode-config mingw-w64-x86_64-perl-encode-ebcdic mingw-w64-x86_64-perl-encode-encoder mingw-w64-x86_64-perl-encode-encoding mingw-w64-x86_64-perl-encode-gsm0338 mingw-w64-x86_64-perl-encode-guess mingw-w64-x86_64-perl-encode-jp mingw-w64-x86_64-perl-encode-jp-h2z mingw-w64-x86_64-perl-encode-jp-jis7 mingw-w64-x86_64-perl-encode-kr mingw-w64-x86_64-perl-encode-kr-2022_kr mingw-w64-x86_64-perl-encode-mime-header mingw-w64-x86_64-perl-encode-mime-header-iso_2022_jp mingw-w64-x86_64-perl-encode-mime-name mingw-w64-x86_64-perl-encode-symbol mingw-w64-x86_64-perl-encode-tw mingw-w64-x86_64-perl-encode-unicode mingw-w64-x86_64-perl-encode-unicode-utf7 mingw-w64-x86_64-perl-encoding mingw-w64-x86_64-perl-encoding-warnings mingw-w64-x86_64-perl-english mingw-w64-x86_64-perl-env mingw-w64-x86_64-perl-errno mingw-w64-x86_64-perl-experimental mingw-w64-x86_64-perl-exporter mingw-w64-x86_64-perl-exporter-heavy mingw-w64-x86_64-perl-extutils-cbuilder mingw-w64-x86_64-perl-extutils-cbuilder-base mingw-w64-x86_64-perl-extutils-cbuilder-platform-aix mingw-w64-x86_64-perl-extutils-cbuilder-platform-android mingw-w64-x86_64-perl-extutils-cbuilder-platform-cygwin mingw-w64-x86_64-perl-extutils-cbuilder-platform-darwin mingw-w64-x86_64-perl-extutils-cbuilder-platform-dec_osf mingw-w64-x86_64-perl-extutils-cbuilder-platform-os2 mingw-w64-x86_64-perl-extutils-cbuilder-platform-unix mingw-w64-x86_64-perl-extutils-cbuilder-platform-vms mingw-w64-x86_64-perl-extutils-cbuilder-platform-windows mingw-w64-x86_64-perl-extutils-cbuilder-platform-windows-bcc mingw-w64-x86_64-perl-extutils-cbuilder-platform-windows-gcc mingw-w64-x86_64-perl-extutils-cbuilder-platform-windows-msvc mingw-w64-x86_64-perl-extutils-command mingw-w64-x86_64-perl-extutils-command-mm mingw-w64-x86_64-perl-extutils-constant mingw-w64-x86_64-perl-extutils-constant-base mingw-w64-x86_64-perl-extutils-constant-proxysubs mingw-w64-x86_64-perl-extutils-constant-utils mingw-w64-x86_64-perl-extutils-constant-xs mingw-w64-x86_64-perl-extutils-embed mingw-w64-x86_64-perl-extutils-install mingw-w64-x86_64-perl-extutils-installed mingw-w64-x86_64-perl-extutils-liblist mingw-w64-x86_64-perl-extutils-liblist-kid mingw-w64-x86_64-perl-extutils-makemaker mingw-w64-x86_64-perl-extutils-makemaker-config mingw-w64-x86_64-perl-extutils-makemaker-locale mingw-w64-x86_64-perl-extutils-makemaker-version mingw-w64-x86_64-perl-extutils-makemaker-version-regex mingw-w64-x86_64-perl-extutils-manifest mingw-w64-x86_64-perl-extutils-miniperl mingw-w64-x86_64-perl-extutils-mkbootstrap mingw-w64-x86_64-perl-extutils-mksymlists mingw-w64-x86_64-perl-extutils-mm mingw-w64-x86_64-perl-extutils-mm_aix mingw-w64-x86_64-perl-extutils-mm_any mingw-w64-x86_64-perl-extutils-mm_beos mingw-w64-x86_64-perl-extutils-mm_cygwin mingw-w64-x86_64-perl-extutils-mm_darwin mingw-w64-x86_64-perl-extutils-mm_dos mingw-w64-x86_64-perl-extutils-mm_macos mingw-w64-x86_64-perl-extutils-mm_nw5 mingw-w64-x86_64-perl-extutils-mm_os2 mingw-w64-x86_64-perl-extutils-mm_os390 mingw-w64-x86_64-perl-extutils-mm_qnx mingw-w64-x86_64-perl-extutils-mm_unix mingw-w64-x86_64-perl-extutils-mm_uwin mingw-w64-x86_64-perl-extutils-mm_vms mingw-w64-x86_64-perl-extutils-mm_vos mingw-w64-x86_64-perl-extutils-mm_win32 mingw-w64-x86_64-perl-extutils-mm_win95 mingw-w64-x86_64-perl-extutils-my mingw-w64-x86_64-perl-extutils-packlist mingw-w64-x86_64-perl-extutils-parsexs mingw-w64-x86_64-perl-extutils-parsexs-constants mingw-w64-x86_64-perl-extutils-parsexs-countlines mingw-w64-x86_64-perl-extutils-parsexs-eval mingw-w64-x86_64-perl-extutils-parsexs-node mingw-w64-x86_64-perl-extutils-parsexs-utilities mingw-w64-x86_64-perl-extutils-pl2bat mingw-w64-x86_64-perl-extutils-testlib mingw-w64-x86_64-perl-extutils-typemaps mingw-w64-x86_64-perl-extutils-typemaps-cmd mingw-w64-x86_64-perl-extutils-typemaps-inputmap mingw-w64-x86_64-perl-extutils-typemaps-outputmap mingw-w64-x86_64-perl-extutils-typemaps-type mingw-w64-x86_64-perl-extutils-xssymset mingw-w64-x86_64-perl-fatal mingw-w64-x86_64-perl-fcntl mingw-w64-x86_64-perl-feature mingw-w64-x86_64-perl-fields mingw-w64-x86_64-perl-file-basename mingw-w64-x86_64-perl-file-compare mingw-w64-x86_64-perl-file-copy mingw-w64-x86_64-perl-file-dosglob mingw-w64-x86_64-perl-file-fetch mingw-w64-x86_64-perl-file-find mingw-w64-x86_64-perl-file-glob mingw-w64-x86_64-perl-file-globmapper mingw-w64-x86_64-perl-file-path mingw-w64-x86_64-perl-file-spec mingw-w64-x86_64-perl-file-spec-amigaos mingw-w64-x86_64-perl-file-spec-cygwin mingw-w64-x86_64-perl-file-spec-epoc mingw-w64-x86_64-perl-file-spec-functions mingw-w64-x86_64-perl-file-spec-mac mingw-w64-x86_64-perl-file-spec-os2 mingw-w64-x86_64-perl-file-spec-unix mingw-w64-x86_64-perl-file-spec-vms mingw-w64-x86_64-perl-file-spec-win32 mingw-w64-x86_64-perl-file-stat mingw-w64-x86_64-perl-file-temp mingw-w64-x86_64-perl-file-which mingw-w64-x86_64-perl-filecache mingw-w64-x86_64-perl-filehandle mingw-w64-x86_64-perl-filetest mingw-w64-x86_64-perl-filter-simple mingw-w64-x86_64-perl-filter-util-call mingw-w64-x86_64-perl-findbin mingw-w64-x86_64-perl-gdbm_file mingw-w64-x86_64-perl-getopt-long mingw-w64-x86_64-perl-getopt-long-parser mingw-w64-x86_64-perl-getopt-std mingw-w64-x86_64-perl-hash-util mingw-w64-x86_64-perl-hash-util-fieldhash mingw-w64-x86_64-perl-html-tagset mingw-w64-x86_64-perl-http-tiny mingw-w64-x86_64-perl-i18n-collate mingw-w64-x86_64-perl-i18n-langinfo mingw-w64-x86_64-perl-i18n-langtags mingw-w64-x86_64-perl-i18n-langtags-detect mingw-w64-x86_64-perl-i18n-langtags-list mingw-w64-x86_64-perl-if mingw-w64-x86_64-perl-integer mingw-w64-x86_64-perl-io mingw-w64-x86_64-perl-io-compress mingw-w64-x86_64-perl-io-compress-adapter-bzip2 mingw-w64-x86_64-perl-io-compress-adapter-deflate mingw-w64-x86_64-perl-io-compress-adapter-identity mingw-w64-x86_64-perl-io-compress-base mingw-w64-x86_64-perl-io-compress-base-common mingw-w64-x86_64-perl-io-compress-bzip2 mingw-w64-x86_64-perl-io-compress-deflate mingw-w64-x86_64-perl-io-compress-gzip mingw-w64-x86_64-perl-io-compress-gzip-constants mingw-w64-x86_64-perl-io-compress-rawdeflate mingw-w64-x86_64-perl-io-compress-zip mingw-w64-x86_64-perl-io-compress-zip-constants mingw-w64-x86_64-perl-io-compress-zlib-constants mingw-w64-x86_64-perl-io-compress-zlib-extra mingw-w64-x86_64-perl-io-dir mingw-w64-x86_64-perl-io-file mingw-w64-x86_64-perl-io-handle mingw-w64-x86_64-perl-io-pipe mingw-w64-x86_64-perl-io-poll mingw-w64-x86_64-perl-io-seekable mingw-w64-x86_64-perl-io-select mingw-w64-x86_64-perl-io-socket mingw-w64-x86_64-perl-io-socket-inet mingw-w64-x86_64-perl-io-socket-ip mingw-w64-x86_64-perl-io-socket-unix mingw-w64-x86_64-perl-io-string mingw-w64-x86_64-perl-io-uncompress-adapter-bunzip2 mingw-w64-x86_64-perl-io-uncompress-adapter-identity mingw-w64-x86_64-perl-io-uncompress-adapter-inflate mingw-w64-x86_64-perl-io-uncompress-anyinflate mingw-w64-x86_64-perl-io-uncompress-anyuncompress mingw-w64-x86_64-perl-io-uncompress-base mingw-w64-x86_64-perl-io-uncompress-bunzip2 mingw-w64-x86_64-perl-io-uncompress-gunzip mingw-w64-x86_64-perl-io-uncompress-inflate mingw-w64-x86_64-perl-io-uncompress-rawinflate mingw-w64-x86_64-perl-io-uncompress-unzip mingw-w64-x86_64-perl-io-zlib mingw-w64-x86_64-perl-ipc-cmd mingw-w64-x86_64-perl-ipc-msg mingw-w64-x86_64-perl-ipc-open2 mingw-w64-x86_64-perl-ipc-open3 mingw-w64-x86_64-perl-ipc-semaphore mingw-w64-x86_64-perl-ipc-sharedmem mingw-w64-x86_64-perl-ipc-sysv mingw-w64-x86_64-perl-json-pp mingw-w64-x86_64-perl-json-pp-boolean mingw-w64-x86_64-perl-less mingw-w64-x86_64-perl-lib mingw-w64-x86_64-perl-list-util mingw-w64-x86_64-perl-list-util-xs mingw-w64-x86_64-perl-locale mingw-w64-x86_64-perl-locale-maketext mingw-w64-x86_64-perl-locale-maketext-guts mingw-w64-x86_64-perl-locale-maketext-gutsloader mingw-w64-x86_64-perl-locale-maketext-simple mingw-w64-x86_64-perl-math-bigfloat mingw-w64-x86_64-perl-math-bigfloat-trace mingw-w64-x86_64-perl-math-bigint mingw-w64-x86_64-perl-math-bigint-calc mingw-w64-x86_64-perl-math-bigint-fastcalc mingw-w64-x86_64-perl-math-bigint-lib mingw-w64-x86_64-perl-math-bigint-trace mingw-w64-x86_64-perl-math-bigrat mingw-w64-x86_64-perl-math-bigrat-trace mingw-w64-x86_64-perl-math-complex mingw-w64-x86_64-perl-math-int64 mingw-w64-x86_64-perl-math-trig mingw-w64-x86_64-perl-memoize mingw-w64-x86_64-perl-memoize-anydbm_file mingw-w64-x86_64-perl-memoize-expire mingw-w64-x86_64-perl-memoize-ndbm_file mingw-w64-x86_64-perl-memoize-sdbm_file mingw-w64-x86_64-perl-memoize-storable mingw-w64-x86_64-perl-meta_notation mingw-w64-x86_64-perl-mime-base64 mingw-w64-x86_64-perl-mime-quotedprint mingw-w64-x86_64-perl-module-corelist mingw-w64-x86_64-perl-module-corelist-utils mingw-w64-x86_64-perl-module-load mingw-w64-x86_64-perl-module-load-conditional mingw-w64-x86_64-perl-module-loaded mingw-w64-x86_64-perl-module-metadata mingw-w64-x86_64-perl-mozilla-ca mingw-w64-x86_64-perl-mro mingw-w64-x86_64-perl-ndbm_file mingw-w64-x86_64-perl-net-cmd mingw-w64-x86_64-perl-net-config mingw-w64-x86_64-perl-net-domain mingw-w64-x86_64-perl-net-ftp mingw-w64-x86_64-perl-net-ftp-a mingw-w64-x86_64-perl-net-ftp-dataconn mingw-w64-x86_64-perl-net-ftp-e mingw-w64-x86_64-perl-net-ftp-i mingw-w64-x86_64-perl-net-ftp-l mingw-w64-x86_64-perl-net-hostent mingw-w64-x86_64-perl-net-netent mingw-w64-x86_64-perl-net-netrc mingw-w64-x86_64-perl-net-nntp mingw-w64-x86_64-perl-net-ping mingw-w64-x86_64-perl-net-pop3 mingw-w64-x86_64-perl-net-protoent mingw-w64-x86_64-perl-net-servent mingw-w64-x86_64-perl-net-smtp mingw-w64-x86_64-perl-net-time mingw-w64-x86_64-perl-next mingw-w64-x86_64-perl-o mingw-w64-x86_64-perl-odbm_file mingw-w64-x86_64-perl-ok mingw-w64-x86_64-perl-opcode mingw-w64-x86_64-perl-open mingw-w64-x86_64-perl-ops mingw-w64-x86_64-perl-os2-dll mingw-w64-x86_64-perl-os2-extattr mingw-w64-x86_64-perl-os2-prfdb mingw-w64-x86_64-perl-os2-process mingw-w64-x86_64-perl-os2-rexx mingw-w64-x86_64-perl-overload mingw-w64-x86_64-perl-overload-numbers mingw-w64-x86_64-perl-overloading mingw-w64-x86_64-perl-params-check mingw-w64-x86_64-perl-parent mingw-w64-x86_64-perl-parse-cpan-meta mingw-w64-x86_64-perl-perl-ostype mingw-w64-x86_64-perl-perlfaq mingw-w64-x86_64-perl-perlio mingw-w64-x86_64-perl-perlio-encoding mingw-w64-x86_64-perl-perlio-mmap mingw-w64-x86_64-perl-perlio-scalar mingw-w64-x86_64-perl-perlio-via mingw-w64-x86_64-perl-perlio-via-quotedprint mingw-w64-x86_64-perl-pod-checker mingw-w64-x86_64-perl-pod-escapes mingw-w64-x86_64-perl-pod-functions mingw-w64-x86_64-perl-pod-html mingw-w64-x86_64-perl-pod-html-util mingw-w64-x86_64-perl-pod-man mingw-w64-x86_64-perl-pod-parselink mingw-w64-x86_64-perl-pod-perldoc mingw-w64-x86_64-perl-pod-perldoc-baseto mingw-w64-x86_64-perl-pod-perldoc-getoptsoo mingw-w64-x86_64-perl-pod-perldoc-toansi mingw-w64-x86_64-perl-pod-perldoc-tochecker mingw-w64-x86_64-perl-pod-perldoc-toman mingw-w64-x86_64-perl-pod-perldoc-tonroff mingw-w64-x86_64-perl-pod-perldoc-topod mingw-w64-x86_64-perl-pod-perldoc-tortf mingw-w64-x86_64-perl-pod-perldoc-toterm mingw-w64-x86_64-perl-pod-perldoc-totext mingw-w64-x86_64-perl-pod-perldoc-totk mingw-w64-x86_64-perl-pod-perldoc-toxml mingw-w64-x86_64-perl-pod-simple mingw-w64-x86_64-perl-pod-simple-blackbox mingw-w64-x86_64-perl-pod-simple-checker mingw-w64-x86_64-perl-pod-simple-debug mingw-w64-x86_64-perl-pod-simple-dumpastext mingw-w64-x86_64-perl-pod-simple-dumpasxml mingw-w64-x86_64-perl-pod-simple-html mingw-w64-x86_64-perl-pod-simple-htmlbatch mingw-w64-x86_64-perl-pod-simple-htmllegacy mingw-w64-x86_64-perl-pod-simple-justpod mingw-w64-x86_64-perl-pod-simple-linksection mingw-w64-x86_64-perl-pod-simple-methody mingw-w64-x86_64-perl-pod-simple-progress mingw-w64-x86_64-perl-pod-simple-pullparser mingw-w64-x86_64-perl-pod-simple-pullparserendtoken mingw-w64-x86_64-perl-pod-simple-pullparserstarttoken mingw-w64-x86_64-perl-pod-simple-pullparsertexttoken mingw-w64-x86_64-perl-pod-simple-pullparsertoken mingw-w64-x86_64-perl-pod-simple-rtf mingw-w64-x86_64-perl-pod-simple-search mingw-w64-x86_64-perl-pod-simple-simpletree mingw-w64-x86_64-perl-pod-simple-text mingw-w64-x86_64-perl-pod-simple-textcontent mingw-w64-x86_64-perl-pod-simple-tiedoutfh mingw-w64-x86_64-perl-pod-simple-transcode mingw-w64-x86_64-perl-pod-simple-transcodedumb mingw-w64-x86_64-perl-pod-simple-transcodesmart mingw-w64-x86_64-perl-pod-simple-xhtml mingw-w64-x86_64-perl-pod-simple-xmloutstream mingw-w64-x86_64-perl-pod-text mingw-w64-x86_64-perl-pod-text-color mingw-w64-x86_64-perl-pod-text-overstrike mingw-w64-x86_64-perl-pod-text-termcap mingw-w64-x86_64-perl-pod-usage mingw-w64-x86_64-perl-posix mingw-w64-x86_64-perl-re mingw-w64-x86_64-perl-safe mingw-w64-x86_64-perl-scalar-list-utils mingw-w64-x86_64-perl-scalar-util mingw-w64-x86_64-perl-sdbm_file mingw-w64-x86_64-perl-search-dict mingw-w64-x86_64-perl-selectsaver mingw-w64-x86_64-perl-selfloader mingw-w64-x86_64-perl-sigtrap mingw-w64-x86_64-perl-socket mingw-w64-x86_64-perl-sort mingw-w64-x86_64-perl-source-encoding mingw-w64-x86_64-perl-stable mingw-w64-x86_64-perl-storable mingw-w64-x86_64-perl-strict mingw-w64-x86_64-perl-sub-util mingw-w64-x86_64-perl-subs mingw-w64-x86_64-perl-symbol mingw-w64-x86_64-perl-sys-hostname mingw-w64-x86_64-perl-sys-syslog mingw-w64-x86_64-perl-sys-syslog-win32 mingw-w64-x86_64-perl-tap-base mingw-w64-x86_64-perl-tap-formatter-base mingw-w64-x86_64-perl-tap-formatter-color mingw-w64-x86_64-perl-tap-formatter-console mingw-w64-x86_64-perl-tap-formatter-console-parallelsession mingw-w64-x86_64-perl-tap-formatter-console-session mingw-w64-x86_64-perl-tap-formatter-file mingw-w64-x86_64-perl-tap-formatter-file-session mingw-w64-x86_64-perl-tap-formatter-session mingw-w64-x86_64-perl-tap-harness mingw-w64-x86_64-perl-tap-harness-env mingw-w64-x86_64-perl-tap-object mingw-w64-x86_64-perl-tap-parser mingw-w64-x86_64-perl-tap-parser-aggregator mingw-w64-x86_64-perl-tap-parser-grammar mingw-w64-x86_64-perl-tap-parser-iterator mingw-w64-x86_64-perl-tap-parser-iterator-array mingw-w64-x86_64-perl-tap-parser-iterator-process mingw-w64-x86_64-perl-tap-parser-iterator-stream mingw-w64-x86_64-perl-tap-parser-iteratorfactory mingw-w64-x86_64-perl-tap-parser-multiplexer mingw-w64-x86_64-perl-tap-parser-result mingw-w64-x86_64-perl-tap-parser-result-bailout mingw-w64-x86_64-perl-tap-parser-result-comment mingw-w64-x86_64-perl-tap-parser-result-plan mingw-w64-x86_64-perl-tap-parser-result-pragma mingw-w64-x86_64-perl-tap-parser-result-test mingw-w64-x86_64-perl-tap-parser-result-unknown mingw-w64-x86_64-perl-tap-parser-result-version mingw-w64-x86_64-perl-tap-parser-result-yaml mingw-w64-x86_64-perl-tap-parser-resultfactory mingw-w64-x86_64-perl-tap-parser-scheduler mingw-w64-x86_64-perl-tap-parser-scheduler-job mingw-w64-x86_64-perl-tap-parser-scheduler-spinner mingw-w64-x86_64-perl-tap-parser-source mingw-w64-x86_64-perl-tap-parser-sourcehandler mingw-w64-x86_64-perl-tap-parser-sourcehandler-executable mingw-w64-x86_64-perl-tap-parser-sourcehandler-file mingw-w64-x86_64-perl-tap-parser-sourcehandler-handle mingw-w64-x86_64-perl-tap-parser-sourcehandler-perl mingw-w64-x86_64-perl-tap-parser-sourcehandler-rawtap mingw-w64-x86_64-perl-tap-parser-yamlish-reader mingw-w64-x86_64-perl-tap-parser-yamlish-writer mingw-w64-x86_64-perl-term-ansicolor mingw-w64-x86_64-perl-term-cap mingw-w64-x86_64-perl-term-complete mingw-w64-x86_64-perl-term-readline mingw-w64-x86_64-perl-term-table mingw-w64-x86_64-perl-term-table-cell mingw-w64-x86_64-perl-term-table-cellstack mingw-w64-x86_64-perl-term-table-hashbase mingw-w64-x86_64-perl-term-table-linebreak mingw-w64-x86_64-perl-term-table-spacer mingw-w64-x86_64-perl-term-table-util mingw-w64-x86_64-perl-test mingw-w64-x86_64-perl-test-builder mingw-w64-x86_64-perl-test-builder-formatter mingw-w64-x86_64-perl-test-builder-module mingw-w64-x86_64-perl-test-builder-tester mingw-w64-x86_64-perl-test-builder-tester-color mingw-w64-x86_64-perl-test-builder-tododiag mingw-w64-x86_64-perl-test-harness mingw-w64-x86_64-perl-test-more mingw-w64-x86_64-perl-test-simple mingw-w64-x86_64-perl-test-tester mingw-w64-x86_64-perl-test-tester-capture mingw-w64-x86_64-perl-test-tester-capturerunner mingw-w64-x86_64-perl-test-tester-delegate mingw-w64-x86_64-perl-test-use-ok mingw-w64-x86_64-perl-test2 mingw-w64-x86_64-perl-test2-api mingw-w64-x86_64-perl-test2-api-breakage mingw-w64-x86_64-perl-test2-api-context mingw-w64-x86_64-perl-test2-api-instance mingw-w64-x86_64-perl-test2-api-interceptresult mingw-w64-x86_64-perl-test2-api-interceptresult-event mingw-w64-x86_64-perl-test2-api-interceptresult-facet mingw-w64-x86_64-perl-test2-api-interceptresult-hub mingw-w64-x86_64-perl-test2-api-interceptresult-squasher mingw-w64-x86_64-perl-test2-api-stack mingw-w64-x86_64-perl-test2-asyncsubtest mingw-w64-x86_64-perl-test2-asyncsubtest-event-attach mingw-w64-x86_64-perl-test2-asyncsubtest-event-detach mingw-w64-x86_64-perl-test2-asyncsubtest-formatter mingw-w64-x86_64-perl-test2-asyncsubtest-hub mingw-w64-x86_64-perl-test2-bundle mingw-w64-x86_64-perl-test2-bundle-extended mingw-w64-x86_64-perl-test2-bundle-more mingw-w64-x86_64-perl-test2-bundle-simple mingw-w64-x86_64-perl-test2-compare mingw-w64-x86_64-perl-test2-compare-array mingw-w64-x86_64-perl-test2-compare-bag mingw-w64-x86_64-perl-test2-compare-base mingw-w64-x86_64-perl-test2-compare-bool mingw-w64-x86_64-perl-test2-compare-custom mingw-w64-x86_64-perl-test2-compare-deepref mingw-w64-x86_64-perl-test2-compare-delta mingw-w64-x86_64-perl-test2-compare-event mingw-w64-x86_64-perl-test2-compare-eventmeta mingw-w64-x86_64-perl-test2-compare-float mingw-w64-x86_64-perl-test2-compare-hash mingw-w64-x86_64-perl-test2-compare-isa mingw-w64-x86_64-perl-test2-compare-meta mingw-w64-x86_64-perl-test2-compare-negatable mingw-w64-x86_64-perl-test2-compare-number mingw-w64-x86_64-perl-test2-compare-object mingw-w64-x86_64-perl-test2-compare-orderedsubset mingw-w64-x86_64-perl-test2-compare-pattern mingw-w64-x86_64-perl-test2-compare-ref mingw-w64-x86_64-perl-test2-compare-regex mingw-w64-x86_64-perl-test2-compare-scalar mingw-w64-x86_64-perl-test2-compare-set mingw-w64-x86_64-perl-test2-compare-string mingw-w64-x86_64-perl-test2-compare-undef mingw-w64-x86_64-perl-test2-compare-wildcard mingw-w64-x86_64-perl-test2-env mingw-w64-x86_64-perl-test2-event mingw-w64-x86_64-perl-test2-event-bail mingw-w64-x86_64-perl-test2-event-diag mingw-w64-x86_64-perl-test2-event-encoding mingw-w64-x86_64-perl-test2-event-exception mingw-w64-x86_64-perl-test2-event-fail mingw-w64-x86_64-perl-test2-event-generic mingw-w64-x86_64-perl-test2-event-note mingw-w64-x86_64-perl-test2-event-ok mingw-w64-x86_64-perl-test2-event-pass mingw-w64-x86_64-perl-test2-event-plan mingw-w64-x86_64-perl-test2-event-skip mingw-w64-x86_64-perl-test2-event-subtest mingw-w64-x86_64-perl-test2-event-tap-version mingw-w64-x86_64-perl-test2-event-v2 mingw-w64-x86_64-perl-test2-event-waiting mingw-w64-x86_64-perl-test2-eventfacet mingw-w64-x86_64-perl-test2-eventfacet-about mingw-w64-x86_64-perl-test2-eventfacet-amnesty mingw-w64-x86_64-perl-test2-eventfacet-assert mingw-w64-x86_64-perl-test2-eventfacet-control mingw-w64-x86_64-perl-test2-eventfacet-error mingw-w64-x86_64-perl-test2-eventfacet-hub mingw-w64-x86_64-perl-test2-eventfacet-info mingw-w64-x86_64-perl-test2-eventfacet-info-table mingw-w64-x86_64-perl-test2-eventfacet-meta mingw-w64-x86_64-perl-test2-eventfacet-parent mingw-w64-x86_64-perl-test2-eventfacet-plan mingw-w64-x86_64-perl-test2-eventfacet-render mingw-w64-x86_64-perl-test2-eventfacet-trace mingw-w64-x86_64-perl-test2-formatter mingw-w64-x86_64-perl-test2-formatter-tap mingw-w64-x86_64-perl-test2-hub mingw-w64-x86_64-perl-test2-hub-interceptor mingw-w64-x86_64-perl-test2-hub-interceptor-terminator mingw-w64-x86_64-perl-test2-hub-subtest mingw-w64-x86_64-perl-test2-ipc mingw-w64-x86_64-perl-test2-ipc-driver mingw-w64-x86_64-perl-test2-ipc-driver-files mingw-w64-x86_64-perl-test2-manual mingw-w64-x86_64-perl-test2-manual-anatomy mingw-w64-x86_64-perl-test2-manual-anatomy-api mingw-w64-x86_64-perl-test2-manual-anatomy-context mingw-w64-x86_64-perl-test2-manual-anatomy-endtoend mingw-w64-x86_64-perl-test2-manual-anatomy-event mingw-w64-x86_64-perl-test2-manual-anatomy-hubs mingw-w64-x86_64-perl-test2-manual-anatomy-ipc mingw-w64-x86_64-perl-test2-manual-anatomy-utilities mingw-w64-x86_64-perl-test2-manual-concurrency mingw-w64-x86_64-perl-test2-manual-contributing mingw-w64-x86_64-perl-test2-manual-testing mingw-w64-x86_64-perl-test2-manual-testing-introduction mingw-w64-x86_64-perl-test2-manual-testing-migrating mingw-w64-x86_64-perl-test2-manual-testing-planning mingw-w64-x86_64-perl-test2-manual-testing-todo mingw-w64-x86_64-perl-test2-manual-tooling mingw-w64-x86_64-perl-test2-manual-tooling-firsttool mingw-w64-x86_64-perl-test2-manual-tooling-formatter mingw-w64-x86_64-perl-test2-manual-tooling-nesting mingw-w64-x86_64-perl-test2-manual-tooling-plugin-testexit mingw-w64-x86_64-perl-test2-manual-tooling-plugin-testingdone mingw-w64-x86_64-perl-test2-manual-tooling-plugin-toolcompletes mingw-w64-x86_64-perl-test2-manual-tooling-plugin-toolstarts mingw-w64-x86_64-perl-test2-manual-tooling-subtest mingw-w64-x86_64-perl-test2-manual-tooling-testbuilder mingw-w64-x86_64-perl-test2-manual-tooling-testing mingw-w64-x86_64-perl-test2-mock mingw-w64-x86_64-perl-test2-plugin mingw-w64-x86_64-perl-test2-plugin-bailonfail mingw-w64-x86_64-perl-test2-plugin-dieonfail mingw-w64-x86_64-perl-test2-plugin-exitsummary mingw-w64-x86_64-perl-test2-plugin-srand mingw-w64-x86_64-perl-test2-plugin-times mingw-w64-x86_64-perl-test2-plugin-utf8 mingw-w64-x86_64-perl-test2-require mingw-w64-x86_64-perl-test2-require-authortesting mingw-w64-x86_64-perl-test2-require-automatedtesting mingw-w64-x86_64-perl-test2-require-envvar mingw-w64-x86_64-perl-test2-require-extendedtesting mingw-w64-x86_64-perl-test2-require-fork mingw-w64-x86_64-perl-test2-require-module mingw-w64-x86_64-perl-test2-require-noninteractivetesting mingw-w64-x86_64-perl-test2-require-perl mingw-w64-x86_64-perl-test2-require-realfork mingw-w64-x86_64-perl-test2-require-releasetesting mingw-w64-x86_64-perl-test2-require-threads mingw-w64-x86_64-perl-test2-suite mingw-w64-x86_64-perl-test2-todo mingw-w64-x86_64-perl-test2-tools mingw-w64-x86_64-perl-test2-tools-asyncsubtest mingw-w64-x86_64-perl-test2-tools-basic mingw-w64-x86_64-perl-test2-tools-class mingw-w64-x86_64-perl-test2-tools-classiccompare mingw-w64-x86_64-perl-test2-tools-compare mingw-w64-x86_64-perl-test2-tools-defer mingw-w64-x86_64-perl-test2-tools-encoding mingw-w64-x86_64-perl-test2-tools-event mingw-w64-x86_64-perl-test2-tools-exception mingw-w64-x86_64-perl-test2-tools-exports mingw-w64-x86_64-perl-test2-tools-gentemp mingw-w64-x86_64-perl-test2-tools-grab mingw-w64-x86_64-perl-test2-tools-mock mingw-w64-x86_64-perl-test2-tools-ref mingw-w64-x86_64-perl-test2-tools-refcount mingw-w64-x86_64-perl-test2-tools-spec mingw-w64-x86_64-perl-test2-tools-subtest mingw-w64-x86_64-perl-test2-tools-target mingw-w64-x86_64-perl-test2-tools-tester mingw-w64-x86_64-perl-test2-tools-tiny mingw-w64-x86_64-perl-test2-tools-warnings mingw-w64-x86_64-perl-test2-util mingw-w64-x86_64-perl-test2-util-externalmeta mingw-w64-x86_64-perl-test2-util-facets2legacy mingw-w64-x86_64-perl-test2-util-grabber mingw-w64-x86_64-perl-test2-util-guard mingw-w64-x86_64-perl-test2-util-hashbase mingw-w64-x86_64-perl-test2-util-importer mingw-w64-x86_64-perl-test2-util-ref mingw-w64-x86_64-perl-test2-util-sig mingw-w64-x86_64-perl-test2-util-stash mingw-w64-x86_64-perl-test2-util-sub mingw-w64-x86_64-perl-test2-util-table mingw-w64-x86_64-perl-test2-util-table-cell mingw-w64-x86_64-perl-test2-util-table-linebreak mingw-w64-x86_64-perl-test2-util-term mingw-w64-x86_64-perl-test2-util-times mingw-w64-x86_64-perl-test2-util-trace mingw-w64-x86_64-perl-test2-v0 mingw-w64-x86_64-perl-test2-workflow mingw-w64-x86_64-perl-test2-workflow-blockbase mingw-w64-x86_64-perl-test2-workflow-build mingw-w64-x86_64-perl-test2-workflow-runner mingw-w64-x86_64-perl-test2-workflow-task mingw-w64-x86_64-perl-test2-workflow-task-action mingw-w64-x86_64-perl-test2-workflow-task-group mingw-w64-x86_64-perl-text-abbrev mingw-w64-x86_64-perl-text-balanced mingw-w64-x86_64-perl-text-parsewords mingw-w64-x86_64-perl-text-tabs mingw-w64-x86_64-perl-text-wrap mingw-w64-x86_64-perl-thread mingw-w64-x86_64-perl-thread-queue mingw-w64-x86_64-perl-thread-semaphore mingw-w64-x86_64-perl-threads mingw-w64-x86_64-perl-threads-shared mingw-w64-x86_64-perl-tie-array mingw-w64-x86_64-perl-tie-file mingw-w64-x86_64-perl-tie-handle mingw-w64-x86_64-perl-tie-hash mingw-w64-x86_64-perl-tie-hash-namedcapture mingw-w64-x86_64-perl-tie-memoize mingw-w64-x86_64-perl-tie-refhash mingw-w64-x86_64-perl-tie-scalar mingw-w64-x86_64-perl-tie-stdhandle mingw-w64-x86_64-perl-tie-substrhash mingw-w64-x86_64-perl-time-gmtime mingw-w64-x86_64-perl-time-hires mingw-w64-x86_64-perl-time-local mingw-w64-x86_64-perl-time-localtime mingw-w64-x86_64-perl-time-piece mingw-w64-x86_64-perl-time-seconds mingw-w64-x86_64-perl-time-tm mingw-w64-x86_64-perl-unicode mingw-w64-x86_64-perl-unicode-collate mingw-w64-x86_64-perl-unicode-collate-cjk-big5 mingw-w64-x86_64-perl-unicode-collate-cjk-gb2312 mingw-w64-x86_64-perl-unicode-collate-cjk-jisx0208 mingw-w64-x86_64-perl-unicode-collate-cjk-korean mingw-w64-x86_64-perl-unicode-collate-cjk-pinyin mingw-w64-x86_64-perl-unicode-collate-cjk-stroke mingw-w64-x86_64-perl-unicode-collate-cjk-zhuyin mingw-w64-x86_64-perl-unicode-collate-locale mingw-w64-x86_64-perl-unicode-normalize mingw-w64-x86_64-perl-unicode-ucd mingw-w64-x86_64-perl-unicore-name mingw-w64-x86_64-perl-universal mingw-w64-x86_64-perl-user-grent mingw-w64-x86_64-perl-user-pwent mingw-w64-x86_64-perl-utf8 mingw-w64-x86_64-perl-vars mingw-w64-x86_64-perl-version mingw-w64-x86_64-perl-version-regex mingw-w64-x86_64-perl-vms-dclsym mingw-w64-x86_64-perl-vms-filespec mingw-w64-x86_64-perl-vms-stdio mingw-w64-x86_64-perl-vmsish mingw-w64-x86_64-perl-warnings mingw-w64-x86_64-perl-warnings-register mingw-w64-x86_64-perl-win32 mingw-w64-x86_64-perl-win32-api mingw-w64-x86_64-perl-win32-console mingw-w64-x86_64-perl-win32-shortcut mingw-w64-x86_64-perl-win32-tieregistry mingw-w64-x86_64-perl-win32-winerror mingw-w64-x86_64-perl-win32api-file mingw-w64-x86_64-perl-win32api-registry mingw-w64-x86_64-perl-win32core mingw-w64-x86_64-perl-xs-apitest mingw-w64-x86_64-perl-xs-typemap mingw-w64-x86_64-perl-xsloader mingw-w64-x86_64-pg_probackup-16 mingw-w64-x86_64-phodav mingw-w64-x86_64-phonon mingw-w64-x86_64-phonon-qt6 mingw-w64-x86_64-picosha2 mingw-w64-x86_64-pikchr mingw-w64-x86_64-pinentry mingw-w64-x86_64-pixman mingw-w64-x86_64-pkcs11-helper mingw-w64-x86_64-pkg-config mingw-w64-x86_64-pkgconf mingw-w64-x86_64-plfit mingw-w64-x86_64-plutovg mingw-w64-x86_64-podofo mingw-w64-x86_64-polipo mingw-w64-x86_64-polly mingw-w64-x86_64-poppler mingw-w64-x86_64-poppler-data mingw-w64-x86_64-poppler-qt5 mingw-w64-x86_64-poppler-qt6 mingw-w64-x86_64-popt mingw-w64-x86_64-port-scanner mingw-w64-x86_64-portablexdr mingw-w64-x86_64-portaudio mingw-w64-x86_64-portaudio-asio mingw-w64-x86_64-postgresql mingw-w64-x86_64-postgresql-16 mingw-w64-x86_64-potrace mingw-w64-x86_64-pqp mingw-w64-x86_64-prjtrellis mingw-w64-x86_64-proj mingw-w64-x86_64-protobuf mingw-w64-x86_64-protobuf-c mingw-w64-x86_64-pugixml mingw-w64-x86_64-pupnp mingw-w64-x86_64-putty mingw-w64-x86_64-putty-ssh mingw-w64-x86_64-pybind11 mingw-w64-x86_64-pybind11-json mingw-w64-x86_64-pyinstaller mingw-w64-x86_64-pyinstaller-hooks-contrib mingw-w64-x86_64-pyqt-builder mingw-w64-x86_64-pyqt5-sip mingw-w64-x86_64-pystring mingw-w64-x86_64-python mingw-w64-x86_64-python-aiofiles mingw-w64-x86_64-python-aiosmtpd mingw-w64-x86_64-python-aiosqlite mingw-w64-x86_64-python-altgraph mingw-w64-x86_64-python-annotated-doc mingw-w64-x86_64-python-annotated-types mingw-w64-x86_64-python-anyio mingw-w64-x86_64-python-apipkg mingw-w64-x86_64-python-appdirs mingw-w64-x86_64-python-argcomplete mingw-w64-x86_64-python-argon2-cffi-bindings mingw-w64-x86_64-python-argon2_cffi mingw-w64-x86_64-python-asgiref mingw-w64-x86_64-python-asn1crypto mingw-w64-x86_64-python-ast-serialize mingw-w64-x86_64-python-astor mingw-w64-x86_64-python-astroid mingw-w64-x86_64-python-asttokens mingw-w64-x86_64-python-astunparse mingw-w64-x86_64-python-async-timeout mingw-w64-x86_64-python-async_generator mingw-w64-x86_64-python-atomicwrites mingw-w64-x86_64-python-atpublic mingw-w64-x86_64-python-attrs mingw-w64-x86_64-python-audioop-lts mingw-w64-x86_64-python-audioread mingw-w64-x86_64-python-automat mingw-w64-x86_64-python-autopage mingw-w64-x86_64-python-autopep8 mingw-w64-x86_64-python-awesomeversion mingw-w64-x86_64-python-babel mingw-w64-x86_64-python-bcrypt mingw-w64-x86_64-python-beautifulsoup4 mingw-w64-x86_64-python-beniget mingw-w64-x86_64-python-blinker mingw-w64-x86_64-python-boolean.py mingw-w64-x86_64-python-bootstrap-flask mingw-w64-x86_64-python-bootstrap-flask-docs mingw-w64-x86_64-python-bottle mingw-w64-x86_64-python-branca mingw-w64-x86_64-python-breathe mingw-w64-x86_64-python-brotli mingw-w64-x86_64-python-bsddb3 mingw-w64-x86_64-python-build mingw-w64-x86_64-python-cabarchive mingw-w64-x86_64-python-cachecontrol mingw-w64-x86_64-python-cached-property mingw-w64-x86_64-python-cachetools mingw-w64-x86_64-python-cachy mingw-w64-x86_64-python-cairo mingw-w64-x86_64-python-calver mingw-w64-x86_64-python-capstone mingw-w64-x86_64-python-cattrs mingw-w64-x86_64-python-certifi mingw-w64-x86_64-python-cffi mingw-w64-x86_64-python-charset-normalizer mingw-w64-x86_64-python-cleo mingw-w64-x86_64-python-click mingw-w64-x86_64-python-click-docs mingw-w64-x86_64-python-cliff mingw-w64-x86_64-python-cloudpickle mingw-w64-x86_64-python-cmd2 mingw-w64-x86_64-python-codespell mingw-w64-x86_64-python-colorama mingw-w64-x86_64-python-colored-traceback mingw-w64-x86_64-python-colorlog mingw-w64-x86_64-python-commonmark mingw-w64-x86_64-python-comtypes mingw-w64-x86_64-python-constantly mingw-w64-x86_64-python-contextlib2 mingw-w64-x86_64-python-contourpy mingw-w64-x86_64-python-coverage mingw-w64-x86_64-python-cppy mingw-w64-x86_64-python-cryptography mingw-w64-x86_64-python-cssselect mingw-w64-x86_64-python-csv23 mingw-w64-x86_64-python-cvxopt mingw-w64-x86_64-python-cx-logging mingw-w64-x86_64-python-cycler mingw-w64-x86_64-python-darkdetect mingw-w64-x86_64-python-dateutil mingw-w64-x86_64-python-decouple mingw-w64-x86_64-python-defusedxml mingw-w64-x86_64-python-deprecation mingw-w64-x86_64-python-diff-match-patch mingw-w64-x86_64-python-dill mingw-w64-x86_64-python-distlib mingw-w64-x86_64-python-distro mingw-w64-x86_64-python-django mingw-w64-x86_64-python-dnspython mingw-w64-x86_64-python-docopt mingw-w64-x86_64-python-docutils mingw-w64-x86_64-python-dulwich mingw-w64-x86_64-python-e3-core mingw-w64-x86_64-python-ecdsa mingw-w64-x86_64-python-editables mingw-w64-x86_64-python-elementpath mingw-w64-x86_64-python-email-validator mingw-w64-x86_64-python-emoji mingw-w64-x86_64-python-et-xmlfile mingw-w64-x86_64-python-execnet mingw-w64-x86_64-python-faker mingw-w64-x86_64-python-fasteners mingw-w64-x86_64-python-fastexcel mingw-w64-x86_64-python-fastjsonschema mingw-w64-x86_64-python-feedgenerator mingw-w64-x86_64-python-filelock mingw-w64-x86_64-python-findpython mingw-w64-x86_64-python-fixtures mingw-w64-x86_64-python-flake8 mingw-w64-x86_64-python-flaky mingw-w64-x86_64-python-flask mingw-w64-x86_64-python-flask-babel mingw-w64-x86_64-python-flask-babel-docs mingw-w64-x86_64-python-flatbuffers mingw-w64-x86_64-python-flexmock mingw-w64-x86_64-python-flit mingw-w64-x86_64-python-flit-core mingw-w64-x86_64-python-fonttools mingw-w64-x86_64-python-ford mingw-w64-x86_64-python-freetype-py mingw-w64-x86_64-python-freezegun mingw-w64-x86_64-python-ftfy mingw-w64-x86_64-python-func-timeout mingw-w64-x86_64-python-future mingw-w64-x86_64-python-gast mingw-w64-x86_64-python-gdal mingw-w64-x86_64-python-geographiclib mingw-w64-x86_64-python-geographiclib-docs mingw-w64-x86_64-python-glad mingw-w64-x86_64-python-gmpy2 mingw-w64-x86_64-python-gobject mingw-w64-x86_64-python-graphviz mingw-w64-x86_64-python-grpcio mingw-w64-x86_64-python-grpcio-tools mingw-w64-x86_64-python-gym mingw-w64-x86_64-python-gym-notices mingw-w64-x86_64-python-h11 mingw-w64-x86_64-python-h2 mingw-w64-x86_64-python-h5py mingw-w64-x86_64-python-halo mingw-w64-x86_64-python-hatch-fancy-pypi-readme mingw-w64-x86_64-python-hatch-vcs mingw-w64-x86_64-python-hatchling mingw-w64-x86_64-python-hererocks mingw-w64-x86_64-python-hpack mingw-w64-x86_64-python-html5lib mingw-w64-x86_64-python-httpcore mingw-w64-x86_64-python-httplib2 mingw-w64-x86_64-python-httpretty mingw-w64-x86_64-python-httpx mingw-w64-x86_64-python-hyperframe mingw-w64-x86_64-python-hyperlink mingw-w64-x86_64-python-hypothesis mingw-w64-x86_64-python-icu mingw-w64-x86_64-python-id mingw-w64-x86_64-python-identify mingw-w64-x86_64-python-idna mingw-w64-x86_64-python-ifstools mingw-w64-x86_64-python-imagesize mingw-w64-x86_64-python-immutables mingw-w64-x86_64-python-importlib-metadata mingw-w64-x86_64-python-importlib_resources mingw-w64-x86_64-python-incremental mingw-w64-x86_64-python-inflection mingw-w64-x86_64-python-iniconfig mingw-w64-x86_64-python-installer mingw-w64-x86_64-python-intervaltree mingw-w64-x86_64-python-iso8601 mingw-w64-x86_64-python-isodate mingw-w64-x86_64-python-isosurfaces mingw-w64-x86_64-python-itsdangerous mingw-w64-x86_64-python-jaraco.classes mingw-w64-x86_64-python-jaraco.context mingw-w64-x86_64-python-jaraco.envs mingw-w64-x86_64-python-jaraco.functools mingw-w64-x86_64-python-jaraco.path mingw-w64-x86_64-python-jinja mingw-w64-x86_64-python-jinja-docs mingw-w64-x86_64-python-json-logger mingw-w64-x86_64-python-json-rpc mingw-w64-x86_64-python-kbinxml mingw-w64-x86_64-python-keyring mingw-w64-x86_64-python-keystone mingw-w64-x86_64-python-kiwisolver mingw-w64-x86_64-python-lark-parser mingw-w64-x86_64-python-lazy-importer mingw-w64-x86_64-python-lazy-object-proxy mingw-w64-x86_64-python-levenshtein mingw-w64-x86_64-python-lhafile mingw-w64-x86_64-python-librt mingw-w64-x86_64-python-libsass mingw-w64-x86_64-python-license-expression mingw-w64-x86_64-python-log_symbols mingw-w64-x86_64-python-lws mingw-w64-x86_64-python-lxml mingw-w64-x86_64-python-lz4 mingw-w64-x86_64-python-lzo mingw-w64-x86_64-python-mako mingw-w64-x86_64-python-mallard-ducktype mingw-w64-x86_64-python-manhole mingw-w64-x86_64-python-markdown mingw-w64-x86_64-python-markdown-include mingw-w64-x86_64-python-markdown-it-py mingw-w64-x86_64-python-markdown-math mingw-w64-x86_64-python-markups mingw-w64-x86_64-python-markupsafe mingw-w64-x86_64-python-matplotlib mingw-w64-x86_64-python-maturin mingw-w64-x86_64-python-mccabe mingw-w64-x86_64-python-mdit_py_plugins mingw-w64-x86_64-python-mdurl mingw-w64-x86_64-python-meshio mingw-w64-x86_64-python-mimeparse mingw-w64-x86_64-python-mingw-ldd mingw-w64-x86_64-python-miniupnpc mingw-w64-x86_64-python-mock mingw-w64-x86_64-python-more-itertools mingw-w64-x86_64-python-mpi4py mingw-w64-x86_64-python-msgpack mingw-w64-x86_64-python-multidict mingw-w64-x86_64-python-multipledispatch mingw-w64-x86_64-python-multiprocess mingw-w64-x86_64-python-mypy_extensions mingw-w64-x86_64-python-myst-parser mingw-w64-x86_64-python-natsort mingw-w64-x86_64-python-nh3 mingw-w64-x86_64-python-node-semver mingw-w64-x86_64-python-nodeenv mingw-w64-x86_64-python-numexpr mingw-w64-x86_64-python-numpy mingw-w64-x86_64-python-olefile mingw-w64-x86_64-python-opencv mingw-w64-x86_64-python-openpyxl mingw-w64-x86_64-python-opt_einsum mingw-w64-x86_64-python-ordered-set mingw-w64-x86_64-python-outcome mingw-w64-x86_64-python-overrides mingw-w64-x86_64-python-owslib mingw-w64-x86_64-python-pacdb mingw-w64-x86_64-python-packageurl-python mingw-w64-x86_64-python-packaging mingw-w64-x86_64-python-pallets-sphinx-themes mingw-w64-x86_64-python-pandas mingw-w64-x86_64-python-pandocfilters mingw-w64-x86_64-python-passlib mingw-w64-x86_64-python-pastel mingw-w64-x86_64-python-patch-ng mingw-w64-x86_64-python-path mingw-w64-x86_64-python-path.py mingw-w64-x86_64-python-pathspec mingw-w64-x86_64-python-pbr mingw-w64-x86_64-python-pbs-installer mingw-w64-x86_64-python-pcpp mingw-w64-x86_64-python-pdfminer mingw-w64-x86_64-python-pdm-backend mingw-w64-x86_64-python-pefile mingw-w64-x86_64-python-pesq mingw-w64-x86_64-python-pexpect mingw-w64-x86_64-python-pgen2 mingw-w64-x86_64-python-pi-heif mingw-w64-x86_64-python-pika mingw-w64-x86_64-python-pillow mingw-w64-x86_64-python-pip mingw-w64-x86_64-python-pipenv mingw-w64-x86_64-python-pkg-resources mingw-w64-x86_64-python-pkgconfig mingw-w64-x86_64-python-pkginfo mingw-w64-x86_64-python-platformdirs mingw-w64-x86_64-python-pluggy mingw-w64-x86_64-python-pluginbase mingw-w64-x86_64-python-ply mingw-w64-x86_64-python-poetry mingw-w64-x86_64-python-poetry-core mingw-w64-x86_64-python-poetry-plugin-export mingw-w64-x86_64-python-poetry-plugin-shell mingw-w64-x86_64-python-pptx mingw-w64-x86_64-python-pretend mingw-w64-x86_64-python-prettytable mingw-w64-x86_64-python-priority mingw-w64-x86_64-python-progress mingw-w64-x86_64-python-prompt_toolkit mingw-w64-x86_64-python-protobuf mingw-w64-x86_64-python-psleak mingw-w64-x86_64-python-psutil mingw-w64-x86_64-python-psycopg2 mingw-w64-x86_64-python-ptyprocess mingw-w64-x86_64-python-purl mingw-w64-x86_64-python-py-cpuinfo mingw-w64-x86_64-python-py-cpuinfo2 mingw-w64-x86_64-python-pyaml mingw-w64-x86_64-python-pyarrow mingw-w64-x86_64-python-pyasn1 mingw-w64-x86_64-python-pyasn1-modules mingw-w64-x86_64-python-pybullet mingw-w64-x86_64-python-pycodestyle mingw-w64-x86_64-python-pycparser mingw-w64-x86_64-python-pydocstyle mingw-w64-x86_64-python-pydub mingw-w64-x86_64-python-pyelftools mingw-w64-x86_64-python-pyfastbti mingw-w64-x86_64-python-pyfastbti-git mingw-w64-x86_64-python-pyfasttextureutils mingw-w64-x86_64-python-pyfasttextureutils-git mingw-w64-x86_64-python-pyfdt mingw-w64-x86_64-python-pyfilesystem2 mingw-w64-x86_64-python-pyflakes mingw-w64-x86_64-python-pygccxml mingw-w64-x86_64-python-pygit2 mingw-w64-x86_64-python-pyglm mingw-w64-x86_64-python-pygments mingw-w64-x86_64-python-pygments-github-lexers mingw-w64-x86_64-python-pyicu mingw-w64-x86_64-python-pykwalify mingw-w64-x86_64-python-pylev mingw-w64-x86_64-python-pymongocrypt mingw-w64-x86_64-python-pynput mingw-w64-x86_64-python-pyodbc mingw-w64-x86_64-python-pyopengl mingw-w64-x86_64-python-pyopengl-accelerate mingw-w64-x86_64-python-pyopenssl mingw-w64-x86_64-python-pyparsing mingw-w64-x86_64-python-pyperclip mingw-w64-x86_64-python-pyproject-api mingw-w64-x86_64-python-pyproject-hooks mingw-w64-x86_64-python-pyproject-metadata mingw-w64-x86_64-python-pyqt5 mingw-w64-x86_64-python-pyqt6-sip mingw-w64-x86_64-python-pyrr mingw-w64-x86_64-python-pysfcgal mingw-w64-x86_64-python-pyshp mingw-w64-x86_64-python-pysocks mingw-w64-x86_64-python-pystemmer mingw-w64-x86_64-python-pytest mingw-w64-x86_64-python-pytest-asyncio mingw-w64-x86_64-python-pytest-benchmark mingw-w64-x86_64-python-pytest-cov mingw-w64-x86_64-python-pytest-fixture-config mingw-w64-x86_64-python-pytest-flake8 mingw-w64-x86_64-python-pytest-localserver mingw-w64-x86_64-python-pytest-mock mingw-w64-x86_64-python-pytest-rerunfailures mingw-w64-x86_64-python-pytest-shutil mingw-w64-x86_64-python-pytest-subprocess mingw-w64-x86_64-python-pytest-timeout mingw-w64-x86_64-python-pytest-xdist mingw-w64-x86_64-python-python-discovery mingw-w64-x86_64-python-python-multipart mingw-w64-x86_64-python-python-sbom mingw-w64-x86_64-python-pythran mingw-w64-x86_64-python-pytoolconfig mingw-w64-x86_64-python-pytz mingw-w64-x86_64-python-pyuca mingw-w64-x86_64-python-pyvda mingw-w64-x86_64-python-pywin32 mingw-w64-x86_64-python-pywin32-ctypes mingw-w64-x86_64-python-pyzopfli mingw-w64-x86_64-python-quart mingw-w64-x86_64-python-rapidfuzz mingw-w64-x86_64-python-re-assert mingw-w64-x86_64-python-regex mingw-w64-x86_64-python-requests mingw-w64-x86_64-python-requests-cache mingw-w64-x86_64-python-requests-mock mingw-w64-x86_64-python-requests-toolbelt mingw-w64-x86_64-python-requests-unixsocket mingw-w64-x86_64-python-resolvelib mingw-w64-x86_64-python-responses mingw-w64-x86_64-python-retrying mingw-w64-x86_64-python-rfc3986 mingw-w64-x86_64-python-rfc3987 mingw-w64-x86_64-python-rich mingw-w64-x86_64-python-rich-argparse mingw-w64-x86_64-python-roman-numerals mingw-w64-x86_64-python-roman-numerals-py mingw-w64-x86_64-python-ruamel-yaml mingw-w64-x86_64-python-ruamel.yaml.clib mingw-w64-x86_64-python-s2geometry mingw-w64-x86_64-python-scikit-build-core mingw-w64-x86_64-python-scipy mingw-w64-x86_64-python-semantic-version mingw-w64-x86_64-python-setuptools mingw-w64-x86_64-python-setuptools-rust mingw-w64-x86_64-python-setuptools-scm mingw-w64-x86_64-python-setuptools-scm-git-archive mingw-w64-x86_64-python-shellingham mingw-w64-x86_64-python-six mingw-w64-x86_64-python-smartypants mingw-w64-x86_64-python-sniffio mingw-w64-x86_64-python-snowballstemmer mingw-w64-x86_64-python-sortedcontainers mingw-w64-x86_64-python-soupsieve mingw-w64-x86_64-python-sphinx mingw-w64-x86_64-python-sphinx-alabaster-theme mingw-w64-x86_64-python-sphinx-copybutton mingw-w64-x86_64-python-sphinx-issues mingw-w64-x86_64-python-sphinx-notfound-page mingw-w64-x86_64-python-sphinx-tabs mingw-w64-x86_64-python-sphinx_rtd_theme mingw-w64-x86_64-python-sphinxcontrib-applehelp mingw-w64-x86_64-python-sphinxcontrib-devhelp mingw-w64-x86_64-python-sphinxcontrib-github-alt mingw-w64-x86_64-python-sphinxcontrib-htmlhelp mingw-w64-x86_64-python-sphinxcontrib-jquery mingw-w64-x86_64-python-sphinxcontrib-jsmath mingw-w64-x86_64-python-sphinxcontrib-log-cabinet mingw-w64-x86_64-python-sphinxcontrib-moderncmakedomain mingw-w64-x86_64-python-sphinxcontrib-qthelp mingw-w64-x86_64-python-sphinxcontrib-serializinghtml mingw-w64-x86_64-python-sphinxcontrib-svg2pdfconverter mingw-w64-x86_64-python-spinners mingw-w64-x86_64-python-sqlparse mingw-w64-x86_64-python-standard-aifc mingw-w64-x86_64-python-standard-chunk mingw-w64-x86_64-python-standard-sunau mingw-w64-x86_64-python-stestr mingw-w64-x86_64-python-stevedore mingw-w64-x86_64-python-subunit mingw-w64-x86_64-python-termcolor mingw-w64-x86_64-python-testpath mingw-w64-x86_64-python-testrepository mingw-w64-x86_64-python-testresources mingw-w64-x86_64-python-testscenarios mingw-w64-x86_64-python-testtools mingw-w64-x86_64-python-toml mingw-w64-x86_64-python-tomli mingw-w64-x86_64-python-tomli-w mingw-w64-x86_64-python-tomlkit mingw-w64-x86_64-python-toposort mingw-w64-x86_64-python-tornado mingw-w64-x86_64-python-tox mingw-w64-x86_64-python-tqdm mingw-w64-x86_64-python-trio mingw-w64-x86_64-python-trove-classifiers mingw-w64-x86_64-python-twisted mingw-w64-x86_64-python-typing_extensions mingw-w64-x86_64-python-typogrify mingw-w64-x86_64-python-tzdata mingw-w64-x86_64-python-ufolib2 mingw-w64-x86_64-python-ukpostcodeparser mingw-w64-x86_64-python-unidecode mingw-w64-x86_64-python-url-normalize mingw-w64-x86_64-python-urllib3 mingw-w64-x86_64-python-uv mingw-w64-x86_64-python-uv-build mingw-w64-x86_64-python-vcs-versioning mingw-w64-x86_64-python-versioneer mingw-w64-x86_64-python-virtualenv mingw-w64-x86_64-python-virtualenv-clone mingw-w64-x86_64-python-voluptuous mingw-w64-x86_64-python-watchfiles mingw-w64-x86_64-python-wcag-contrast-ratio mingw-w64-x86_64-python-wcwidth mingw-w64-x86_64-python-webencodings mingw-w64-x86_64-python-werkzeug mingw-w64-x86_64-python-wheel mingw-w64-x86_64-python-wmi mingw-w64-x86_64-python-wsproto mingw-w64-x86_64-python-wsproto-docs mingw-w64-x86_64-python-wtforms mingw-w64-x86_64-python-wtforms-docs mingw-w64-x86_64-python-xlsxwriter mingw-w64-x86_64-python-xmlschema mingw-w64-x86_64-python-yaml mingw-w64-x86_64-python-zipp mingw-w64-x86_64-python-zipremove mingw-w64-x86_64-python-zope.interface mingw-w64-x86_64-python-zstandard mingw-w64-x86_64-python3 mingw-w64-x86_64-python3.14 mingw-w64-x86_64-qbs mingw-w64-x86_64-qdia mingw-w64-x86_64-qhull mingw-w64-x86_64-qrencode mingw-w64-x86_64-qt5-3d mingw-w64-x86_64-qt5-3d-debug mingw-w64-x86_64-qt5-activeqt mingw-w64-x86_64-qt5-activeqt-debug mingw-w64-x86_64-qt5-base mingw-w64-x86_64-qt5-base-debug mingw-w64-x86_64-qt5-charts mingw-w64-x86_64-qt5-charts-debug mingw-w64-x86_64-qt5-connectivity mingw-w64-x86_64-qt5-connectivity-debug mingw-w64-x86_64-qt5-datavis3d mingw-w64-x86_64-qt5-datavis3d-debug mingw-w64-x86_64-qt5-declarative mingw-w64-x86_64-qt5-declarative-debug mingw-w64-x86_64-qt5-gamepad mingw-w64-x86_64-qt5-gamepad-debug mingw-w64-x86_64-qt5-graphicaleffects mingw-w64-x86_64-qt5-graphicaleffects-debug mingw-w64-x86_64-qt5-imageformats mingw-w64-x86_64-qt5-imageformats-debug mingw-w64-x86_64-qt5-location mingw-w64-x86_64-qt5-location-debug mingw-w64-x86_64-qt5-lottie mingw-w64-x86_64-qt5-lottie-debug mingw-w64-x86_64-qt5-multimedia mingw-w64-x86_64-qt5-multimedia-debug mingw-w64-x86_64-qt5-networkauth mingw-w64-x86_64-qt5-networkauth-debug mingw-w64-x86_64-qt5-purchasing mingw-w64-x86_64-qt5-purchasing-debug mingw-w64-x86_64-qt5-quick3d mingw-w64-x86_64-qt5-quick3d-debug mingw-w64-x86_64-qt5-quickcontrols mingw-w64-x86_64-qt5-quickcontrols-debug mingw-w64-x86_64-qt5-quickcontrols2 mingw-w64-x86_64-qt5-quickcontrols2-debug mingw-w64-x86_64-qt5-quicktimeline mingw-w64-x86_64-qt5-quicktimeline-debug mingw-w64-x86_64-qt5-remoteobjects mingw-w64-x86_64-qt5-remoteobjects-debug mingw-w64-x86_64-qt5-script mingw-w64-x86_64-qt5-script-debug mingw-w64-x86_64-qt5-scxml mingw-w64-x86_64-qt5-scxml-debug mingw-w64-x86_64-qt5-sensors mingw-w64-x86_64-qt5-sensors-debug mingw-w64-x86_64-qt5-serialbus mingw-w64-x86_64-qt5-serialbus-debug mingw-w64-x86_64-qt5-serialport mingw-w64-x86_64-qt5-serialport-debug mingw-w64-x86_64-qt5-speech mingw-w64-x86_64-qt5-speech-debug mingw-w64-x86_64-qt5-static mingw-w64-x86_64-qt5-svg mingw-w64-x86_64-qt5-svg-debug mingw-w64-x86_64-qt5-tools mingw-w64-x86_64-qt5-tools-debug mingw-w64-x86_64-qt5-translations mingw-w64-x86_64-qt5-virtualkeyboard mingw-w64-x86_64-qt5-virtualkeyboard-debug mingw-w64-x86_64-qt5-webchannel mingw-w64-x86_64-qt5-webchannel-debug mingw-w64-x86_64-qt5-webglplugin mingw-w64-x86_64-qt5-webglplugin-debug mingw-w64-x86_64-qt5-websockets mingw-w64-x86_64-qt5-websockets-debug mingw-w64-x86_64-qt5-webview mingw-w64-x86_64-qt5-webview-debug mingw-w64-x86_64-qt5-winextras mingw-w64-x86_64-qt5-winextras-debug mingw-w64-x86_64-qt5-xmlpatterns mingw-w64-x86_64-qt5-xmlpatterns-debug mingw-w64-x86_64-qt6-3d mingw-w64-x86_64-qt6-3d-debug mingw-w64-x86_64-qt6-5compat mingw-w64-x86_64-qt6-5compat-debug mingw-w64-x86_64-qt6-activeqt mingw-w64-x86_64-qt6-activeqt-debug mingw-w64-x86_64-qt6-base mingw-w64-x86_64-qt6-base-debug mingw-w64-x86_64-qt6-charts mingw-w64-x86_64-qt6-charts-debug mingw-w64-x86_64-qt6-connectivity mingw-w64-x86_64-qt6-connectivity-debug mingw-w64-x86_64-qt6-datavis3d mingw-w64-x86_64-qt6-datavis3d-debug mingw-w64-x86_64-qt6-declarative mingw-w64-x86_64-qt6-declarative-debug mingw-w64-x86_64-qt6-grpc mingw-w64-x86_64-qt6-grpc-debug mingw-w64-x86_64-qt6-httpserver mingw-w64-x86_64-qt6-httpserver-debug mingw-w64-x86_64-qt6-imageformats mingw-w64-x86_64-qt6-imageformats-debug mingw-w64-x86_64-qt6-languageserver mingw-w64-x86_64-qt6-location mingw-w64-x86_64-qt6-location-debug mingw-w64-x86_64-qt6-lottie mingw-w64-x86_64-qt6-lottie-debug mingw-w64-x86_64-qt6-multimedia mingw-w64-x86_64-qt6-multimedia-debug mingw-w64-x86_64-qt6-multimedia-ffmpeg mingw-w64-x86_64-qt6-multimedia-wmf mingw-w64-x86_64-qt6-networkauth mingw-w64-x86_64-qt6-networkauth-debug mingw-w64-x86_64-qt6-positioning mingw-w64-x86_64-qt6-positioning-debug mingw-w64-x86_64-qt6-quick3d mingw-w64-x86_64-qt6-quick3d-debug mingw-w64-x86_64-qt6-quick3dphysics mingw-w64-x86_64-qt6-quick3dphysics-debug mingw-w64-x86_64-qt6-quickeffectmaker mingw-w64-x86_64-qt6-quickeffectmaker-debug mingw-w64-x86_64-qt6-quicktimeline mingw-w64-x86_64-qt6-quicktimeline-debug mingw-w64-x86_64-qt6-remoteobjects mingw-w64-x86_64-qt6-remoteobjects-debug mingw-w64-x86_64-qt6-scxml mingw-w64-x86_64-qt6-scxml-debug mingw-w64-x86_64-qt6-sensors mingw-w64-x86_64-qt6-sensors-debug mingw-w64-x86_64-qt6-serialbus mingw-w64-x86_64-qt6-serialbus-debug mingw-w64-x86_64-qt6-serialport mingw-w64-x86_64-qt6-serialport-debug mingw-w64-x86_64-qt6-shadertools mingw-w64-x86_64-qt6-shadertools-debug mingw-w64-x86_64-qt6-speech mingw-w64-x86_64-qt6-speech-debug mingw-w64-x86_64-qt6-svg mingw-w64-x86_64-qt6-svg-debug mingw-w64-x86_64-qt6-tools mingw-w64-x86_64-qt6-tools-debug mingw-w64-x86_64-qt6-translations mingw-w64-x86_64-qt6-virtualkeyboard mingw-w64-x86_64-qt6-virtualkeyboard-debug mingw-w64-x86_64-qt6-webchannel mingw-w64-x86_64-qt6-webchannel-debug mingw-w64-x86_64-qt6-websockets mingw-w64-x86_64-qt6-websockets-debug mingw-w64-x86_64-qt6-webview mingw-w64-x86_64-qt6-webview-debug mingw-w64-x86_64-qtkeychain mingw-w64-x86_64-qtkeychain-qt6 mingw-w64-x86_64-qtwebkit mingw-w64-x86_64-qwt-qt5 mingw-w64-x86_64-qwt-qt6 mingw-w64-x86_64-rabbitmq-c mingw-w64-x86_64-ragel mingw-w64-x86_64-range-v3 mingw-w64-x86_64-rapidfuzz-cpp mingw-w64-x86_64-rapidjson mingw-w64-x86_64-rav1e mingw-w64-x86_64-rcedit mingw-w64-x86_64-re2 mingw-w64-x86_64-readline mingw-w64-x86_64-redis-plus-plus mingw-w64-x86_64-rhash mingw-w64-x86_64-rime-bopomofo mingw-w64-x86_64-rime-cangjie mingw-w64-x86_64-rime-double-pinyin mingw-w64-x86_64-rime-emoji mingw-w64-x86_64-rime-essay mingw-w64-x86_64-rime-luna-pinyin mingw-w64-x86_64-rime-pinyin-simp mingw-w64-x86_64-rime-prelude mingw-w64-x86_64-rime-stroke mingw-w64-x86_64-rime-terra-pinyin mingw-w64-x86_64-rime-wubi mingw-w64-x86_64-robin-hood-hashing mingw-w64-x86_64-robin-map mingw-w64-x86_64-rocksdb mingw-w64-x86_64-rtmpdump mingw-w64-x86_64-rtmpdump-git mingw-w64-x86_64-ruby mingw-w64-x86_64-ruby-abbrev mingw-w64-x86_64-ruby-base64 mingw-w64-x86_64-ruby-benchmark mingw-w64-x86_64-ruby-bigdecimal mingw-w64-x86_64-ruby-bundler mingw-w64-x86_64-ruby-csv mingw-w64-x86_64-ruby-date mingw-w64-x86_64-ruby-dbus mingw-w64-x86_64-ruby-debug mingw-w64-x86_64-ruby-delegate mingw-w64-x86_64-ruby-did_you_mean mingw-w64-x86_64-ruby-digest mingw-w64-x86_64-ruby-drb mingw-w64-x86_64-ruby-english mingw-w64-x86_64-ruby-erb mingw-w64-x86_64-ruby-error_highlight mingw-w64-x86_64-ruby-etc mingw-w64-x86_64-ruby-fcntl mingw-w64-x86_64-ruby-fiddle mingw-w64-x86_64-ruby-fileutils mingw-w64-x86_64-ruby-find mingw-w64-x86_64-ruby-forwardable mingw-w64-x86_64-ruby-getoptlong mingw-w64-x86_64-ruby-io-console mingw-w64-x86_64-ruby-io-nonblock mingw-w64-x86_64-ruby-io-wait mingw-w64-x86_64-ruby-ipaddr mingw-w64-x86_64-ruby-irb mingw-w64-x86_64-ruby-json mingw-w64-x86_64-ruby-logger mingw-w64-x86_64-ruby-matrix mingw-w64-x86_64-ruby-minitest mingw-w64-x86_64-ruby-mustache mingw-w64-x86_64-ruby-mutex_m mingw-w64-x86_64-ruby-native-package-installer mingw-w64-x86_64-ruby-net-ftp mingw-w64-x86_64-ruby-net-http mingw-w64-x86_64-ruby-net-imap mingw-w64-x86_64-ruby-net-pop mingw-w64-x86_64-ruby-net-protocol mingw-w64-x86_64-ruby-net-smtp mingw-w64-x86_64-ruby-nkf mingw-w64-x86_64-ruby-observer mingw-w64-x86_64-ruby-open-uri mingw-w64-x86_64-ruby-open3 mingw-w64-x86_64-ruby-openssl mingw-w64-x86_64-ruby-optparse mingw-w64-x86_64-ruby-ostruct mingw-w64-x86_64-ruby-pkg-config mingw-w64-x86_64-ruby-power_assert mingw-w64-x86_64-ruby-pp mingw-w64-x86_64-ruby-prettyprint mingw-w64-x86_64-ruby-prime mingw-w64-x86_64-ruby-prism mingw-w64-x86_64-ruby-pstore mingw-w64-x86_64-ruby-psych mingw-w64-x86_64-ruby-racc mingw-w64-x86_64-ruby-rake mingw-w64-x86_64-ruby-rbs mingw-w64-x86_64-ruby-rdiscount mingw-w64-x86_64-ruby-rdoc mingw-w64-x86_64-ruby-readline mingw-w64-x86_64-ruby-reline mingw-w64-x86_64-ruby-repl_type_completor mingw-w64-x86_64-ruby-resolv mingw-w64-x86_64-ruby-resolv-replace mingw-w64-x86_64-ruby-rexml mingw-w64-x86_64-ruby-rinda mingw-w64-x86_64-ruby-rss mingw-w64-x86_64-ruby-ruby2_keywords mingw-w64-x86_64-ruby-securerandom mingw-w64-x86_64-ruby-shellwords mingw-w64-x86_64-ruby-singleton mingw-w64-x86_64-ruby-stringio mingw-w64-x86_64-ruby-strscan mingw-w64-x86_64-ruby-syntax_suggest mingw-w64-x86_64-ruby-tempfile mingw-w64-x86_64-ruby-test-unit mingw-w64-x86_64-ruby-time mingw-w64-x86_64-ruby-timeout mingw-w64-x86_64-ruby-tmpdir mingw-w64-x86_64-ruby-tsort mingw-w64-x86_64-ruby-typeprof mingw-w64-x86_64-ruby-un mingw-w64-x86_64-ruby-uri mingw-w64-x86_64-ruby-weakref mingw-w64-x86_64-ruby-win32-registry mingw-w64-x86_64-ruby-win32ole mingw-w64-x86_64-ruby-yaml mingw-w64-x86_64-ruby-zlib mingw-w64-x86_64-ruc-vm mingw-w64-x86_64-rust mingw-w64-x86_64-rust-bindgen mingw-w64-x86_64-rust-src mingw-w64-x86_64-rustup mingw-w64-x86_64-rxspencer mingw-w64-x86_64-s2geometry mingw-w64-x86_64-samtools mingw-w64-x86_64-sassc mingw-w64-x86_64-sbc mingw-w64-x86_64-sbcl mingw-w64-x86_64-sccache mingw-w64-x86_64-scons mingw-w64-x86_64-scour mingw-w64-x86_64-sdcv mingw-w64-x86_64-sdl mingw-w64-x86_64-sdl-gfx mingw-w64-x86_64-sdl-image mingw-w64-x86_64-sdl-mixer mingw-w64-x86_64-sdl-net mingw-w64-x86_64-sdl-ttf mingw-w64-x86_64-sdl12-compat mingw-w64-x86_64-sdl2 mingw-w64-x86_64-sdl2-compat mingw-w64-x86_64-sdl2-gfx mingw-w64-x86_64-sdl2-image mingw-w64-x86_64-sdl2-mixer mingw-w64-x86_64-sdl2-net mingw-w64-x86_64-sdl2-pango mingw-w64-x86_64-sdl2-sound mingw-w64-x86_64-sdl2-ttf mingw-w64-x86_64-sdl3 mingw-w64-x86_64-sdl3-image mingw-w64-x86_64-sdl3-mixer mingw-w64-x86_64-sdl3-sound mingw-w64-x86_64-sdl3-ttf mingw-w64-x86_64-seacas mingw-w64-x86_64-seexpr mingw-w64-x86_64-sfcgal mingw-w64-x86_64-sgml-common mingw-w64-x86_64-shaderc mingw-w64-x86_64-shared-mime-info mingw-w64-x86_64-shim_executable mingw-w64-x86_64-shine mingw-w64-x86_64-shishi mingw-w64-x86_64-silc-toolkit mingw-w64-x86_64-simdjson mingw-w64-x86_64-simple-mail mingw-w64-x86_64-sip mingw-w64-x86_64-skia mingw-w64-x86_64-skyr-url mingw-w64-x86_64-sl mingw-w64-x86_64-smpeg mingw-w64-x86_64-snappy mingw-w64-x86_64-snoregrowl mingw-w64-x86_64-snorenotify mingw-w64-x86_64-snowball mingw-w64-x86_64-soil mingw-w64-x86_64-sol2 mingw-w64-x86_64-solid3 mingw-w64-x86_64-sonivox mingw-w64-x86_64-soundtouch mingw-w64-x86_64-source-highlight mingw-w64-x86_64-spandsp mingw-w64-x86_64-sparsehash mingw-w64-x86_64-spatialindex mingw-w64-x86_64-spdlog mingw-w64-x86_64-spdylay mingw-w64-x86_64-spectra mingw-w64-x86_64-speex mingw-w64-x86_64-speexdsp mingw-w64-x86_64-spirv-cross mingw-w64-x86_64-spirv-headers mingw-w64-x86_64-spirv-llvm-translator mingw-w64-x86_64-spirv-tools mingw-w64-x86_64-spooles mingw-w64-x86_64-sqlite mingw-w64-x86_64-sqlite-docs mingw-w64-x86_64-sqlite-orm mingw-w64-x86_64-sqlite3 mingw-w64-x86_64-sqlite3mc mingw-w64-x86_64-srt mingw-w64-x86_64-stk mingw-w64-x86_64-stoken mingw-w64-x86_64-stxxl mingw-w64-x86_64-stxxl-git mingw-w64-x86_64-styrene mingw-w64-x86_64-suitesparse mingw-w64-x86_64-sundials mingw-w64-x86_64-superlu mingw-w64-x86_64-svt-av1 mingw-w64-x86_64-swi-prolog mingw-w64-x86_64-swi-prolog-archive mingw-w64-x86_64-swi-prolog-bdb mingw-w64-x86_64-swi-prolog-core mingw-w64-x86_64-swi-prolog-doc mingw-w64-x86_64-swi-prolog-edit mingw-w64-x86_64-swi-prolog-examples mingw-w64-x86_64-swi-prolog-full mingw-w64-x86_64-swi-prolog-json mingw-w64-x86_64-swi-prolog-odbc mingw-w64-x86_64-swi-prolog-packages mingw-w64-x86_64-swi-prolog-pcre2 mingw-w64-x86_64-swi-prolog-python mingw-w64-x86_64-swi-prolog-ssl mingw-w64-x86_64-swi-prolog-tests mingw-w64-x86_64-swi-prolog-x mingw-w64-x86_64-swi-prolog-yaml mingw-w64-x86_64-swig mingw-w64-x86_64-systemc mingw-w64-x86_64-t1utils mingw-w64-x86_64-taglib mingw-w64-x86_64-taucs mingw-w64-x86_64-tbb mingw-w64-x86_64-tcl mingw-w64-x86_64-tcllib mingw-w64-x86_64-tclvfs mingw-w64-x86_64-tclvfs-cvs mingw-w64-x86_64-tclx mingw-w64-x86_64-tdlib mingw-w64-x86_64-template-glib mingw-w64-x86_64-termcap mingw-w64-x86_64-tesseract-ocr mingw-w64-x86_64-testu01 mingw-w64-x86_64-tetgen mingw-w64-x86_64-texinfo mingw-w64-x86_64-tgui mingw-w64-x86_64-thorvg mingw-w64-x86_64-thrift mingw-w64-x86_64-tiledb mingw-w64-x86_64-tinyexpr mingw-w64-x86_64-tinyformat mingw-w64-x86_64-tinyxml mingw-w64-x86_64-tinyxml2 mingw-w64-x86_64-tixi mingw-w64-x86_64-tixi3 mingw-w64-x86_64-tk mingw-w64-x86_64-tklib mingw-w64-x86_64-tktable mingw-w64-x86_64-tl-expected mingw-w64-x86_64-tmxlite mingw-w64-x86_64-tmxlite-docs mingw-w64-x86_64-toml11 mingw-w64-x86_64-tomlc17 mingw-w64-x86_64-tomlplusplus mingw-w64-x86_64-tools-xctest mingw-w64-x86_64-treedec mingw-w64-x86_64-trompeloeil mingw-w64-x86_64-tslib mingw-w64-x86_64-ttf-cm-unicode mingw-w64-x86_64-ttf-dejavu mingw-w64-x86_64-ttf-paratype mingw-w64-x86_64-ttfautohint mingw-w64-x86_64-twapi mingw-w64-x86_64-tzdata mingw-w64-x86_64-uasm mingw-w64-x86_64-uchardet mingw-w64-x86_64-ucl mingw-w64-x86_64-udunits mingw-w64-x86_64-uhttpmock mingw-w64-x86_64-unbound mingw-w64-x86_64-uncrustify mingw-w64-x86_64-unibilium mingw-w64-x86_64-unicode-character-database mingw-w64-x86_64-unifdef mingw-w64-x86_64-unittest-cpp mingw-w64-x86_64-universal-ctags mingw-w64-x86_64-universal-ctags-git mingw-w64-x86_64-unixodbc mingw-w64-x86_64-unrar mingw-w64-x86_64-unzip mingw-w64-x86_64-upx mingw-w64-x86_64-uriparser mingw-w64-x86_64-usrsctp mingw-w64-x86_64-utf8cpp mingw-w64-x86_64-uthash mingw-w64-x86_64-uv mingw-w64-x86_64-uvatlas mingw-w64-x86_64-uvicorn mingw-w64-x86_64-vala mingw-w64-x86_64-vid.stab mingw-w64-x86_64-vtk mingw-w64-x86_64-vulkan mingw-w64-x86_64-vulkan-headers mingw-w64-x86_64-vulkan-loader mingw-w64-x86_64-vulkan-utility-libraries mingw-w64-x86_64-vulkan-validation-layers mingw-w64-x86_64-w3c-mathml2 mingw-w64-x86_64-wavpack mingw-w64-x86_64-wcurl mingw-w64-x86_64-webp-pixbuf-loader mingw-w64-x86_64-wget mingw-w64-x86_64-whisper.cpp mingw-w64-x86_64-win7appid mingw-w64-x86_64-windows-default-manifest mingw-w64-x86_64-wineditline mingw-w64-x86_64-winfile mingw-w64-x86_64-winico mingw-w64-x86_64-winpthreads mingw-w64-x86_64-winpthreads-git mingw-w64-x86_64-winpthreads-stub mingw-w64-x86_64-winpty mingw-w64-x86_64-wintab-sdk mingw-w64-x86_64-woff2 mingw-w64-x86_64-wolfssl mingw-w64-x86_64-x264 mingw-w64-x86_64-x264-git mingw-w64-x86_64-x265 mingw-w64-x86_64-xapian-core mingw-w64-x86_64-xapian-core-docs mingw-w64-x86_64-xerces-c mingw-w64-x86_64-xmlstarlet mingw-w64-x86_64-xmlstarlet-git mingw-w64-x86_64-xmlto mingw-w64-x86_64-xpm-nox mingw-w64-x86_64-xsimd mingw-w64-x86_64-xvidcore mingw-w64-x86_64-xxhash mingw-w64-x86_64-xz mingw-w64-x86_64-yaml-cpp mingw-w64-x86_64-z3 mingw-w64-x86_64-zeromq mingw-w64-x86_64-zimg mingw-w64-x86_64-zlib mingw-w64-x86_64-zlib-ng mingw-w64-x86_64-zlib-ng-compat mingw-w64-x86_64-zstd mingw-w64-x86_64-zvbi
