%define libname    libevdevxx
%define devname    libevdevxx-devel
%define staticname libevdevxx-static-devel
%define toolsname  evdevxx-tools

Name:           libevdevxx
Version:        0.5.3
Release:        1%{?dist}
Summary:        A C++20 wrapper for libevdev.
Group:          System/Libraries
License:        MIT
URL:            https://github.com/dkosmari/libevdevxx
Source0:        https://github.com/dkosmari/libevdevxx/releases/download/v%{version}/%{name}-%{version}.tar.gz

%description
This is the library needed to run programs linked with %{name}.

BuildRequires:  automake
BuildRequires:  doxygen
BuildRequires:  gcc-c++
BuildRequires:  libtool
BuildRequires:  pkgconfig(libevdev)


#############
## library ##
#############


###########
## devel ##
###########

%package -n     %{devname}
Summary:        Development package for %{libname}.
Group:          Development/C++
Provides:       %{devname} = %{version}
Requires:       %{libname} = %{version}-%{release}
Requires:       pkgconfig(libevdev)

%description -n %{devname}
This package provides libraries and headers files for %{name}
development.


##################
## static-devel ##
##################

%package -n     %{staticname}
Summary:        Development package for %{libname}, with static linking.
Group:          Development/C++
Provides:       %{staticname} = %{version}
Requires:       pkgconfig(libevdev)

%description -n %{staticname}
This package provides static libraries for %{name} development.


###########
## tools ##
###########

%package -n     %{toolsname}
Summary:        Tools from the %{name} package.
Requires:       %{libname} = %{version}-%{release}

%description -n %{toolsname}
This package contains tools from %{name}:
- evdevxx-query
- evdevxx-read


###########
## build ##
###########

%prep
%autosetup

%build
%configure --disable-examples --enable-docs --enable-tools
%make_build

%install
%make_install


#####################
## package library ##
#####################

%files -n %{libname}
%license COPYING
%doc AUTHORS
%{_libdir}/*.so.*


###################
## package devel ##
###################

%files -n %{devname}
%doc README.md
%{_pkgdocdir}/html
%{_includedir}/libevdevxx/*
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc


##########################
## package static-devel ##
##########################

%files -n %{staticname}
%doc README.md
%{_pkgdocdir}/html
%{_includedir}/libevdevxx/*
%{_libdir}/*.a
%{_libdir}/pkgconfig/*.pc


###################
## package tools ##
###################

%files -n %{toolsname}
%{_bindir}/evdevxx-*
