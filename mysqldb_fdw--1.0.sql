/*-------------------------------------------------------------------------
 *
 * mysqldb_fdw--1.0.sql
 * 			Foreign-data wrapper for remote MySQL servers
 *
 * Portions Copyright (c) 2012-2014, PostgreSQL Global Development Group
 *
 * Portions Copyright (c) 2004-2014, EnterpriseDB Corporation.
 * 
 * Portions Copyright (c) 2018-2019, Postgres Connect.
 *
 * IDENTIFICATION
 * 			mysqldb_fdw--1.0.sql
 *
 *-------------------------------------------------------------------------
 */


CREATE FUNCTION mysqldb_fdw_handler()
RETURNS fdw_handler
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION mysqldb_fdw_validator(text[], oid)
RETURNS void
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FOREIGN DATA WRAPPER mysqldb_fdw
  HANDLER mysqldb_fdw_handler
  VALIDATOR mysqldb_fdw_validator;
