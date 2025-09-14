FROM debian:trixie

RUN apt -y update
RUN apt -y upgrade
RUN apt -y install devscripts
RUN apt -y install wget
# Install dependencies
RUN apt -y install build-essential autoconf-archive pkg-config doxygen graphviz libevdev-dev

WORKDIR /buildroot
COPY . .

CMD ["/bin/bash"]
