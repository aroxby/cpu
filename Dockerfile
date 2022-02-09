FROM ubuntu:focal-20220113

RUN apt-get update && apt-get -y install build-essential git python3-pip
RUN pip install 'pyyaml<6' cpp-coveralls

COPY . /app
WORKDIR app
CMD make test
