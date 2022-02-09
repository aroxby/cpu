FROM ubuntu:focal-20220113

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get -y install build-essential curl git

COPY . /app
WORKDIR app
CMD make test
