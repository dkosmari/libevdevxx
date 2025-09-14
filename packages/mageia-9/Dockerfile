FROM mageia:9

RUN urpmi -q --auto-update --auto

# install packaging tools
RUN urpmi -q --auto rpmdevtools bm 

# install build dependencies
RUN urpmi -q --auto task-c++-devel
RUN urpmi -q --auto doxygen graphviz
RUN urpmi -q --auto lib64evdev-devel

WORKDIR /buildroot

COPY . .

CMD ["/bin/bash"]
