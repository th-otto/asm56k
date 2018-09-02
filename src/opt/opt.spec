summary: Options and Parameter parsing Tool
name: opt
version: 3.19
release: 1
source: %{name}-%{version}.tar.gz
vendor: James Theiler
packager: James Theiler
copyright: US Government and GPL
group: Development/Libraries
%define mybuildroot /var/tmp/%{name}-build
BuildRoot: %{mybuildroot}

%description
  Opt is a library of subroutines for communicating options and parameter values to a C program via the command line, parameter files, environment variables, or a rudimentary builtin interactive menu.  It is similar in aims to the standard getopt() utility, but it has a different (I would say better) interface to the programmer, and a few more bells and whistles for the end-users of programs that employ opt.

%prep
%setup
%build
./configure --prefix=/usr --disable-test --with-readline ; make

%install
make install prefix=%{mybuildroot}/usr

%post
%postun

%files
## %doc opt.texi 
/usr/lib/libopt.a
/usr/include/opt.h

