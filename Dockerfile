FROM ubuntu

RUN apt-get update && apt-get -y install build-essential git python3-pip
RUN pip install pyyaml cpp-coveralls

COPY . /src
WORKDIR src
CMD make test
