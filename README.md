[![Build Status](https://travis-ci.com/Postgres-Connect/mysqldb_fdw.svg?branch=master)][STATUS]
[![Coverage](http://img.shields.io/coveralls/Postgres-Connect/mysqldb_fdw.svg)][COVERAGE]

MySQL Foreign Data Wrapper for PostgreSQL
=========================================

The mysqldb_fdw is the PostgreSQL's extension implements a Foreign Data Wrapper (FDW) for [MySQL][MYSQL].

### Write-able FDW
The previous version was only read-only, the latest version provides the write capability. The user can now issue insert/update and delete statements for the foreign tables using the mysql FDW. It uses the PG type casting mechanism to provide opposite type casting between mysql and PG data types.

### Connection Pooling
The latest version comes with a connection pooler that utilises the same mysql database connection for all the queries in the same session. The previous version would open a new mysql database connection for every query. This is a performance enhancement.

### Where clause push-down
The latest version will push-down the foreign table where clause to the foreign server. The where condition on the foreign table will be executed on the foreign server hence there will be fewer rows to to bring across to PostgreSQL. This is a performance feature.

### Column push-down
The previous version was fetching all the columns from the target foreign table. The latest version does the column push-down and only brings back the columns that are part of the select target list. This is a performance feature.

### Prepared Statement
(Refactoring for `select` queries to use prepared statement)

The `select` queries are now using prepared statements instead of simple query protocol.

Installation
------------
For installation guide, read [installation guidlines][INSTALLATION].

Usage
-----
For usage guide, read [usage guidlines][USAGE].

Contributing
------------
If you experince any bug and have a fix for that, or have a new idea, create a ticket on github page Before creating
a pull request please read the [contributing guidlines][CONTRIBUTING].

Support
-------
This project will be modified to maintain compatibility with new PostgreSQL releases.

Release Notes
-------------
For release notes, read [release_notes][RELEASE].

License
-------
Copyright (c) 2018 Postgres-Connect.

Copyright (c) 2011 - 2016, EnterpriseDB Corporation

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose, without fee, and without a written agreement is
hereby granted, provided that the above copyright notice and this paragraph and
the following two paragraphs appear in all copies.

See the [`LICENSE`][LICENCE] file for full details.

[MYSQL]: http://www.mysql.com
[CONTRIBUTING]: CONTRIBUTING.md
[LICENCE]: LICENSE
[RELEASE]: RELEASE.md
[INSTALLATION]: INSTALLATION.md
[USAGE]: USAGE.md
[STATUS]: https://travis-ci.org/Postgres-Connect/mysqldb_fdw
[COVERAGE]: https://coveralls.io/r/Postgres-Connect/mysqldb_fdw
