%define _name		libezV24
%define _version	0.1.2
%define _rel		1

%define _prefix	/usr
%define _pkgdoc	%{_docdir}/%{_name}

%define _suse		%(if [ -f /etc/SuSE-release ]; then echo 1; else echo 0; fi)

%if %_suse
%define _suse_version	%(grep VERSION /etc/SuSE-release|cut -f3 -d" ")
%define _suse_vernum	%(echo "%_suse_version"|tr -d '.')
%define _release	%{_rel}suse%{_suse_vernum}
%define _has_distro	1
%define _distribution	SuSE Linux %_suse_version
%define _group		Development/Libraries/C and C++
%else
%define _release	%_rel
%define _has_distro	0
%define _group		Development/Libraries
%endif

Summary:		Easy C programming interface to Linux serial ports
Name:			%{_name}
Version:		%{_version}
Release:		%{_release}
Source:			%{_name}-%{_version}.tar.gz
Patch:			%{_name}.make.diff
URL:			http://ezV24.sourceforge.net/
Group:			%{_group}
Packager:		Pascal Bleser <guru@unixtech.be>
Copyright:		LGPL
BuildRoot:		%{_tmppath}/build-%{_name}-%{_version}
Prefix:			%{_prefix}
%if %_has_distro
Distribution:  %_distribution
%endif

%description
The goal of this libary is to provide an easy to use programming
interface to the serial ports of the linux system.

%package devel
Summary:		Development Environment for %{_name}
Group:			%{_group}
Requires:		%{_name} = %{_version}

%description devel
Development Environment for %{_name}
For documentation and API reference see %{_pkgdoc}/api-html/

%changelog
* Thu Mar 27 2003 Pascal Bleser <guru@unixtech.be> 0.1.1-%{_rel}suse%{_suse_vernum}
- first RPM

%prep
%setup -q
%patch0

%build
%{__make} \
	PREFIX="%{_prefix}" \
	COPT="${RPM_OPT_FLAGS}" \
	shared static

%install
%{__rm} -rf "${RPM_BUILD_ROOT}"
%{__mkdir_p} "${RPM_BUILD_ROOT}%{_libdir}"
%{__make} \
	DESTDIR="${RPM_BUILD_ROOT}" \
	PREFIX="%{_prefix}" \
	install

%{__mkdir_p} "${RPM_BUILD_ROOT}%{_pkgdoc}"
echo -n > _rpm_doc_files
for f in AUTHORS BUGS COPYING COPYRIGHT ChangeLog HISTORY README; do
	%{__cp} "$f" "${RPM_BUILD_ROOT}%{_pkgdoc}/"
	echo "%doc %{_pkgdoc}/${f}" >> _rpm_doc_files
done
%{__cp} -R api-html "${RPM_BUILD_ROOT}%{_pkgdoc}/"

%clean
%{__rm} -rf "${RPM_BUILD_ROOT}"

%post
echo Updating dynamic linker cache...
/sbin/ldconfig

%postun
echo Updating dynamic linker cache...
/sbin/ldconfig

%files -f _rpm_doc_files
%defattr(-,root,root)
%{_libdir}/lib*.so.*

%files devel
%defattr(-,root,root)
%doc %{_pkgdoc}/api-html
%{_includedir}/*
%{_libdir}/lib*.so
%{_libdir}/lib*.a

# Local Variables:
# mode: rpm-spec
# tab-width: 3
# End:

