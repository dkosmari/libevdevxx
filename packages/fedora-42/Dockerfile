FROM fedora:42

# install build dependencies
RUN dnf -qy group install rpm-development-tools
RUN dnf -qy group install c-development
RUN dnf -qy install fedora-rpm-macros
RUN dnf -qy install doxygen
RUN dnf -qy install libevdev-devel
RUN dnf -qy install gtkmm3.0-devel

RUN rpmdev-setuptree
WORKDIR /root/rpmbuild
COPY . .

CMD ["/bin/bash"]
