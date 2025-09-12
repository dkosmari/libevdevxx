%define libname    %mklibname evdevxx
%define devname    %mklibname evdevxx -d
%define staticname %mklibname evdevxx -d -s
%define toolsname  evdevxx-tools

Name:           libevdevxx
Version:        0.5.2
Release:        %mkrel 1
Summary:        A C++ wrapper for libevdev.
Group:          System/Libraries
License:        MIT
URL:            https://github.com/dkosmari/libevdevxx
Source0:        https://github.com/dkosmari/libevdevxx/releases/download/v%{version}/%{name}-%{version}.tar.gz

%description
This is the library needed to run programs linked with %{name}.

BuildRequires:  automake
BuildRequires:  libtool
BuildRequires:  pkgconfig(libevdev)
BuildRequires:  gcc-c++
BuildRequires:  doxygen


#############
## library ##
#############

%package -n     %{libname}
Summary:        A C++ wrapper for libevdev.
Group:          System/Libraries

%description -n %{libname}
This is the library needed to run programs linked with %{libname}.


###########
## devel ##
###########

%package -n     %{devname}
Summary:        Development package for %{libname}.
Group:          Development/C++
Provides:       %{devname} = %{version}
Requires:       %{libname} = %{version}-%{release}

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
%{_libdir}/pkgconfig/libevdevxx.pc


##########################
## package static-devel ##
##########################

%files -n %{staticname}
%doc README.md
%{_pkgdocdir}/html
%{_includedir}/libevdevxx/*
%{_libdir}/*.a
%{_libdir}/pkgconfig/libevdevxx.pc


###################
## package tools ##
###################

%files -n evdevxx-tools
%{_bindir}/evdevxx-*
