FROM debian:trixie

# Install packaging tools
RUN apt -q -y update
RUN apt -q -y upgrade
RUN apt -q -y install devscripts
RUN apt -q -y install wget

# Install dependencies
RUN apt -q -y install build-essential autoconf-archive pkg-config
RUN apt -q -y install doxygen graphviz
RUN apt -q -y install libevdev-dev

WORKDIR /buildroot

COPY . .

CMD ["/bin/bash"]
