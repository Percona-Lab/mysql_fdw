%global sname percona-postgresql-mysql_fdw
%global pginstdir /usr/pgsql-11/
%global mysqlfdwmajver 2
%global mysqlfdwmidver 5
%global mysqlfdwminver 1

Summary:        PostgreSQL Foreign Data Wrapper (FDW) for the MySQL
Name:           %{sname}_11
Version:        %{mysqlfdwmajver}.%{mysqlfdwmidver}.%{mysqlfdwminver}
Release:        1%{?dist}
License:        BSD
Source0:        %{sname}-%{mysqlfdwmajver}.%{mysqlfdwmidver}.%{mysqlfdwminver}.tar.gz
URL:            https://github.com/Percona-Lab/mysqldb_fdw
BuildRequires:  postgresql11-devel
Requires:       postgresql11-server
BuildRequires:  mariadb-devel
Requires:       mariadb-devel


%description
This PostgreSQL extension implements a Foreign Data Wrapper (FDW) for
the MySQL.

%prep
%setup -q -n %{sname}-%{mysqlfdwmajver}.%{mysqlfdwmidver}.%{mysqlfdwminver}


%build
sed -i 's:PG_CONFIG = pg_config:PG_CONFIG = /usr/pgsql-11/bin/pg_config:' Makefile
export LDFLAGS="-L%{_libdir}/mysql"
%{__make} USE_PGXS=1 %{?_smp_mflags}


%install
%{__rm} -rf %{buildroot}
%{__make} USE_PGXS=1 %{?_smp_mflags} install DESTDIR=%{buildroot}
%{__install} -d %{buildroot}%{pginstdir}/share/extension
%{__install} -m 755 README.md %{buildroot}%{pginstdir}/share/extension/README-mysql_fdw
%{__rm} -f %{buildroot}%{_docdir}/pgsql/extension/README.md

%clean
%{__rm} -rf %{buildroot}

%post -p /sbin/ldconfig


%postun -p /sbin/ldconfig


%files
%defattr(755,root,root,755)
%doc %{pginstdir}/share/extension/README-mysql_fdw
%{pginstdir}/lib/mysqldb_fdw.so
%{pginstdir}/share/extension/mysqldb_fdw--*.sql
%{pginstdir}/share/extension/mysqldb_fdw.control
%{pginstdir}/lib/bitcode/mysqldb_fdw*.bc
%{pginstdir}/lib/bitcode/mysqldb_fdw/*.bc


%changelog
* Thu Oct  3 2019 Evgeniy Patlan <evgeniy.patlan@percona.com> - 2.5.3-1
- Initial build