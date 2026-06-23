FROM ubuntu:noble

ARG DEBIAN_FRONTEND=noninteractive
ARG uid
ARG gid
ARG user
ARG group

# Ubuntu 24.04 enforces PEP 668 (externally-managed Python).
# This env var allows pip to install into the system Python without a venv.
ENV PIP_BREAK_SYSTEM_PACKAGES=1

RUN apt-get update
RUN apt-get install -y tzdata
RUN apt-get upgrade -y

RUN echo 'alias ll="ls -l --color -a"' >> /root/.bashrc

# Python 3.12 is the default on ubuntu:noble
RUN apt-get install -y python3 python3-pip python3-dev python3-venv

# C++ build tools
RUN apt-get install -y build-essential cmake git pkg-config
RUN apt-get install -y pybind11-dev
RUN apt-get install -y xz-utils
RUN apt-get install -y g++

# CGAL arithmetic dependencies (CGAL headers are downloaded by CMake FetchContent)
RUN apt-get install -y libboost-all-dev
RUN apt-get install -y libgmp-dev
RUN apt-get install -y libmpfr-dev
# System CGAL satisfies any residual cmake detection; FetchContent supplies v6.0.1 headers
RUN apt-get install -y libcgal-dev

# Geospatial system libraries
RUN apt-get install -y libspatialindex-dev libgeos-dev libproj-dev

# Display / Qt libraries (libgl1 replaces libgl1-mesa-glx on ubuntu:noble)
RUN apt-get install -y libgl1 libglib2.0-0 libsm6 libxrender1 libxext6 libfontconfig1
RUN apt-get install -y python3-pyqt5 python3-pyqt5.qtopengl

RUN rm -rf /var/lib/apt/lists/*

RUN mkdir -p /workspace

# ubuntu:noble ships a default 'ubuntu' user at UID 1000 — remove it before
# creating our own user so useradd does not fail on a duplicate UID.
RUN userdel -r ubuntu 2>/dev/null || true
RUN groupadd -g ${gid} ${group} || true
RUN useradd -m -u ${uid} -g ${gid} -s /bin/bash ${user} || true

RUN echo 'alias ll="ls -l --color -a"' >> /home/${user}/.bashrc || true

RUN chown -R ${uid}:${gid} /workspace

USER ${user}
WORKDIR /workspace

COPY --chown=${user}:${group} . /workspace/

RUN pip3 install -r requirements.txt

RUN mkdir -p build
WORKDIR /workspace/build
RUN cmake .. && make -j$(nproc)

WORKDIR /workspace
