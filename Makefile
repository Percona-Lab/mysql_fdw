######################################################################-------------------------------------------------------------------------
#
# mysqldb_fdw.c
# 		Foreign-data wrapper for remote MySQL servers
#
# Portions Copyright (c) 2012-2014, PostgreSQL Global Development Group
#
# Portions Copyright (c) 2004-2014, EnterpriseDB Corporation.
#
# IDENTIFICATION
# 		mysqldb_fdw.c
#
##########################################################################

MODULE_big = mysqldb_fdw
OBJS = mysqldb_connection.o mysqldb_option.o mysqldb_deparse.o mysqldb_query.o mysqldb_fdw.o

EXTENSION = mysqldb_fdw
DATA = mysqldb_fdw--1.0.sql

REGRESS = mysqldb_fdw

MYSQL_CONFIG = mysql_config
PG_CPPFLAGS := $(shell $(MYSQL_CONFIG) --include)
LIB := $(shell $(MYSQL_CONFIG) --libs)

# In Debian based distros, libmariadbclient-dev provides mariadbclient (rather than mysqlclient)
ifneq ($(findstring mariadbclient,$(LIB)),)
MYSQL_LIB = mariadbclient
else
MYSQL_LIB = mysqlclient
endif

UNAME = uname
OS := $(shell $(UNAME))
ifeq ($(OS), Darwin)
DLSUFFIX = .dylib
else
DLSUFFIX = .so
endif

PG_CPPFLAGS += -D _MYSQL_LIBNAME=\"lib$(MYSQL_LIB)$(DLSUFFIX)\"

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
ifndef MAJORVERSION
MAJORVERSION := $(basename $(VERSION))
endif
ifeq (,$(findstring $(MAJORVERSION), 9.3 9.4 9.5 9.6 10 11))
$(error PostgreSQL 9.3, 9.4, 9.5, 9.6 10 or 11 is required to compile this extension)
endif
