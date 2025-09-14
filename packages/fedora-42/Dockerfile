FROM fedora:42

# install packaging tools
RUN dnf -qy group install rpm-development-tools fedora-rpm-macros

# install build dependencies
RUN dnf -qy group install c-development
RUN dnf -qy install doxygen graphviz
RUN dnf -qy install libevdev-devel

RUN rpmdev-setuptree

WORKDIR /root/rpmbuild

COPY . .

CMD ["/bin/bash"]
