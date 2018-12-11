# To build and publish image run following commands:
# > docker build -t cahirwpz/demoscene-fs-uae-buildenv:latest .
# > docker login
# > docker push cahirwpz/demoscene-fs-uae-buildenv:latest

FROM debian:stretch

WORKDIR /root

RUN apt-get -q update && apt-get upgrade -y
RUN apt-get install -y --no-install-recommends \
            ca-certificates zip \
            git make patch gcc g++ automake libtool gettext pkg-config \
            libc6-dev libglib2.0-dev libpng-dev libsdl2-dev libopenal-dev
