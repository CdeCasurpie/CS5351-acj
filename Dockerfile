FROM ubuntu:jammy

ARG DEBIAN_FRONTEND=noninteractive
ARG uid
ARG gid
ARG user
ARG group

RUN apt-get update
RUN apt-get install -y tzdata
RUN apt-get upgrade -y

RUN echo 'alias ll="ls -l --color -a"' >> /root/.bashrc

RUN apt-get install -y python3 python3-pip python3-dev
RUN apt-get install -y build-essential cmake git pkg-config
RUN apt-get install -y pybind11-dev

RUN apt-get install -y xz-utils
RUN apt-get install -y g++
RUN apt-get install -y libboost-all-dev
RUN apt-get install -y libgmp-dev
RUN apt-get install -y libmpfr-dev

RUN apt-get install -y libcgal-dev

RUN mkdir -p /tmp/cgal
COPY CGAL-6.0.1.tar.xz /tmp/cgal/
RUN cd /tmp/cgal && tar -xf CGAL-6.0.1.tar.xz
RUN cd /tmp/cgal/CGAL-6.0.1 && mkdir -p build
RUN cd /tmp/cgal/CGAL-6.0.1/build && cmake ..
RUN cd /tmp/cgal/CGAL-6.0.1/build && make install
RUN rm -rf /tmp/cgal

RUN apt-get install -y libspatialindex-dev libgeos-dev libproj-dev

RUN apt-get install -y libgl1-mesa-glx libglib2.0-0 libsm6 libxrender1 libxext6 libfontconfig1
RUN apt-get install -y python3-pyqt5 python3-pyqt5.qtopengl

RUN rm -rf /var/lib/apt/lists/*

RUN mkdir -p /workspace

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
