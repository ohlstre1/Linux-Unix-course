# This is a sample spec file for 

%define name			service
%define release		1
%define version 	0.1
%define buildroot %{_topdir}/%{name}-%{version}-root

BuildRoot:	%{buildroot}
Summary: 		Signal service
License: 		GPL
Name: 			%{name}
Version: 		%{version}
Release: 		%{release}
Source: 		%{name}-%{version}.tar.gz
Prefix: 		/usr
Group: 			System/Monitor

%description
Signal service packaging demonstrtor.

%prep
%setup -q

%build
make

%install
make
install -D service $RPM_BUILD_ROOT/usr/local/bin/service
install -D service.1 $RPM_BUILD_ROOT/usr/local/share/man/man1/service.1

%files
%defattr(-,root,root)
/usr/local/bin/service

%doc %attr(0444,root,root) /usr/local/share/man/man1/service.1
