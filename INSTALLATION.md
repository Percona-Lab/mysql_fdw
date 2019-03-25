Installation
---------------

To compile the foreign data wrapper, MySQL's C client library is needed. This library can be downloaded from the official MySQL Website.

1. To build on POSIX-compliant systems you need to ensure the `pg_config` executable is in your path when you run `make`. This executable is typically in your PostgreSQL installation's `bin` directory. For example:

    ```
    $ export PATH=/usr/local/pgsql/bin/:$PATH
    ```

2. The `mysql_config` must also be in the path, it resides in the MySQL `bin` directory.

    ```
    $ export PATH=/usr/local/mysql/bin/:$PATH
    ```

3. Compile the code using make.

    ```
    $ make USE_PGXS=1
    ```

4.  Finally install the foreign data wrapper.

    ```
    $ make USE_PGXS=1 install
    ```

Not that we have tested the `mysql_fdw` extension only on MacOS and Ubuntu systems, but other \*NIX's should also work.

