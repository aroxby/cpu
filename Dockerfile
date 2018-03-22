FROM ubuntu:trusty

# apt complains about our terminal settings without this
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get -y install software-properties-common
RUN add-apt-repository ppa:ubuntu-toolchain-r/test

RUN apt-get update && apt-get -y install build-essential git g++-6
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-6 60

COPY . /src
WORKDIR src
CMD make test