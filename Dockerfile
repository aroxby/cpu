FROM ubuntu

RUN apt-get update && apt-get -y install build-essential git

COPY . /src
WORKDIR src
CMD make test