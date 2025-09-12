FROM mageia:9

# install build dependencies
RUN urpmi --auto-update --auto
RUN urpmi --auto --quiet rpmdevtools bm task-c++-devel doxygen lib64evdev-devel

WORKDIR /buildroot
COPY . .

CMD ["/bin/bash"]
