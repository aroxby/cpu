FROM ubuntu:focal-20220113

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get -y install build-essential curl git python3-pip
RUN pip install 'pyyaml<6' cpp-coveralls

COPY . /app
WORKDIR app
CMD make test
