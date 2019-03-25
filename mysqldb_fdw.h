/*-------------------------------------------------------------------------
 *
 * mysqldb_fdw.h
 * 		Foreign-data wrapper for remote MySQL servers
 *
 * Portions Copyright (c) 2012-2014, PostgreSQL Global Development Group
 *
 * Portions Copyright (c) 2004-2014, EnterpriseDB Corporation.
 *
 * IDENTIFICATION
 * 		mysqldb_fdw.h
 *
 *-------------------------------------------------------------------------
 */

#ifndef MYSQL_FDW_H
#define MYSQL_FDW_H

#define list_length mysqldb_list_length
#define list_delete mysqldb_list_delete
#define list_free mysqldb_list_free

#include <mysql.h>
#undef list_length
#undef list_delete
#undef list_free

#include "access/tupdesc.h"
#include "foreign/foreign.h"
#include "lib/stringinfo.h"
#include "nodes/relation.h"
#include "utils/rel.h"

#define MYSQL_PREFETCH_ROWS	100
#define MYSQL_BLKSIZ		(1024 * 4)
#define MYSQL_PORT			3306
#define MAXDATALEN			1024 * 64

#define WAIT_TIMEOUT		0
#define INTERACTIVE_TIMEOUT 0


#define CR_NO_ERROR 0
/*
 * Options structure to store the MySQL
 * server information
 */
typedef struct mysqldb_opt
{
	int           svr_port;               /* MySQL port number */
	char          *svr_address;           /* MySQL server ip address */
	char          *svr_username;          /* MySQL user name */
	char          *svr_password;          /* MySQL password */
	char          *svr_database;          /* MySQL database name */
	char          *svr_table;             /* MySQL table name */
	bool          svr_sa;                 /* MySQL secure authentication */
	char          *svr_init_command;      /* MySQL SQL statement to execute when connecting to the MySQL server. */
	unsigned long max_blob_size;          /* Max blob size to read without truncation */
	bool          use_remote_estimate;    /* use remote estimate for rows */
	
	// SSL parameters; unused options may be given as NULL
	char          *ssl_key;               /* MySQL SSL: path to the key file */
	char          *ssl_cert;              /* MySQL SSL: path to the certificate file */
	char          *ssl_ca;                /* MySQL SSL: path to the certificate authority file */
	char          *ssl_capath;            /* MySQL SSL: path to a directory that contains trusted SSL CA certificates in PEM format */
	char          *ssl_cipher;            /* MySQL SSL: list of permissible ciphers to use for SSL encryption */
} mysqldb_opt;

typedef struct mysqldb_column
{
	Datum         value;
	unsigned long length;
	bool          is_null;
	bool          error;
	MYSQL_BIND    *_mysqldb_bind;
} mysqldb_column;

typedef struct mysqldb_table
{
	MYSQL_RES *_mysqldb_res;
	MYSQL_FIELD *_mysqldb_fields;

	mysqldb_column *column;
	MYSQL_BIND *_mysqldb_bind;
} mysqldb_table;

/*
 * FDW-specific information for ForeignScanState 
 * fdw_state.
 */
typedef struct MySQLFdwExecState
{
	MYSQL           *conn;              /* MySQL connection handle */
	MYSQL_STMT      *stmt;              /* MySQL prepared stament handle */
		mysqldb_table *table;
	char            *query;             /* Query string */
	Relation        rel;                /* relcache entry for the foreign table */
	List            *retrieved_attrs;   /* list of target attribute numbers */

	bool		cursor_exists;	    /* have we created the cursor? */
	int		numParams;	    /* number of parameters passed to query */
	FmgrInfo	*param_flinfo;	    /* output conversion functions for them */
	List		*param_exprs;	    /* executable expressions for param values */
	const char	**param_values;	    /* textual values of query parameters */
	Oid		*param_types;	    /* type of query parameters */

	int             p_nums;             /* number of parameters to transmit */
	FmgrInfo        *p_flinfo;          /* output conversion functions for them */

	mysqldb_opt       *mysqlFdwOptions;   /* MySQL FDW options */

	List            *attr_list;         /* query attribute list */
	List            *column_list;       /* Column list of MySQL Column structures */

	/* working memory context */
	MemoryContext   temp_cxt;           /* context for per-tuple temporary data */
} MySQLFdwExecState;


/* MySQL Column List */
typedef struct MySQLColumn
{
	int   attnum;          /* Attribute number */
	char  *attname;        /* Attribute name */
	int   atttype;         /* Attribute type */
} MySQLColumn;

extern bool is_foreign_expr(PlannerInfo *root,
                                RelOptInfo *baserel,
                                Expr *expr);


int ((*_mysqldb_options)(MYSQL *mysql,enum mysql_option option, const void *arg));
int ((*_mysqldb_stmt_prepare)(MYSQL_STMT *stmt, const char *query, unsigned long length));
int ((*_mysqldb_stmt_execute)(MYSQL_STMT *stmt));
int ((*_mysqldb_stmt_fetch)(MYSQL_STMT *stmt));
int ((*_mysqldb_query)(MYSQL *mysql, const char *q));
bool ((*_mysqldb_stmt_attr_set)(MYSQL_STMT *stmt, enum enum_stmt_attr_type attr_type, const void *attr));
bool ((*_mysqldb_stmt_close)(MYSQL_STMT * stmt));
bool ((*_mysqldb_stmt_reset)(MYSQL_STMT * stmt));
bool ((*_mysqldb_free_result)(MYSQL_RES *result));
bool ((*_mysqldb_stmt_bind_param)(MYSQL_STMT *stmt, MYSQL_BIND * bnd));
bool ((*_mysqldb_stmt_bind_result)(MYSQL_STMT *stmt, MYSQL_BIND * bnd));

MYSQL_STMT	*((*_mysqldb_stmt_init)(MYSQL *mysql));
MYSQL_RES	*((*_mysqldb_stmt_result_metadata)(MYSQL_STMT *stmt));
int ((*_mysqldb_stmt_store_result)(MYSQL *mysql));
MYSQL_ROW	((*_mysqldb_fetch_row)(MYSQL_RES *result));
MYSQL_FIELD	*((*_mysqldb_fetch_field)(MYSQL_RES *result));
MYSQL_FIELD	*((*_mysqldb_fetch_fields)(MYSQL_RES *result));
const char	*((*_mysqldb_error)(MYSQL *mysql));
void	((*_mysqldb_close)(MYSQL *sock));
MYSQL_RES* ((*_mysqldb_store_result)(MYSQL *mysql));

MYSQL	*((*_mysqldb_init)(MYSQL *mysql));
bool ((*_mysqldb_ssl_set)(MYSQL *mysql, const char *key, const char *cert, const char *ca, const char *capath, const char *cipher));
MYSQL	*((*_mysqldb_real_connect)(MYSQL *mysql,
								const char *host,
								const char *user,
								const char *passwd,
								const char *db,
								unsigned int port,
								const char *unix_socket,
								unsigned long clientflag));

const char *((*_mysqldb_get_host_info)(MYSQL *mysql));
const char *((*_mysqldb_get_server_info)(MYSQL *mysql));
int ((*_mysqldb_get_proto_info)(MYSQL *mysql));

unsigned int ((*_mysqldb_stmt_errno)(MYSQL_STMT *stmt));
unsigned int ((*_mysqldb_errno)(MYSQL *mysql));
unsigned int ((*_mysqldb_num_fields)(MYSQL_RES *result));
unsigned int ((*_mysqldb_num_rows)(MYSQL_RES *result));


/* option.c headers */
extern bool mysqldb_is_valid_option(const char *option, Oid context);
extern mysqldb_opt *mysqldb_get_options(Oid foreigntableid);

/* depare.c headers */
extern void mysqldb_deparse_select(StringInfo buf, PlannerInfo *root, RelOptInfo *baserel,
							 Bitmapset *attrs_used, char *svr_table, List **retrieved_attrs);
extern void mysqldb_deparse_insert(StringInfo buf, PlannerInfo *root, Index rtindex, Relation rel, List *targetAttrs);
extern void mysqldb_deparse_update(StringInfo buf, PlannerInfo *root, Index rtindex, Relation rel, List *targetAttrs, char *attname);
extern void mysqldb_deparse_delete(StringInfo buf, PlannerInfo *root, Index rtindex, Relation rel, char *name);
extern void mysqldb_append_where_clause(StringInfo buf, PlannerInfo *root, RelOptInfo *baserel, List *exprs,
							 bool is_first,List **params);
extern void mysqldb_deparse_analyze(StringInfo buf, char *dbname, char *relname);


/* connection.c headers */
MYSQL *mysqldb_get_connection(ForeignServer *server, UserMapping *user, mysqldb_opt *opt);
MYSQL *mysqldb_connect(char *svr_address, char *svr_username, char *svr_password, char *svr_database,
							 int svr_port, bool svr_sa, char *svr_init_command,
							 char *ssl_key, char *ssl_cert, char *ssl_ca, char *ssl_capath,
							 char *ssl_cipher);
void  mysqldb_cleanup_connection(void);
void mysqldb_rel_connection(MYSQL *conn);

#if PG_VERSION_NUM < 110000 /* TupleDescAttr is defined from PG version 11 */ 
	#define TupleDescAttr(tupdesc, i) ((tupdesc)->attrs[(i)])
#endif

#endif /* MYSQL_FDW_H */
