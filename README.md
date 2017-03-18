## SQLite CREATE TABLE parser
A parser for sqlite create table sql statements.

SQLite is a very powerful software but it lacks an easy way to extract complete information about table and columns constraints. The built-in sql pragma:  
```c
PRAGMA schema.table_info(table-name);  
PRAGMA foreign_key_list(table-name);
```  
provide incomplete information and a manual parsing is required in order to extract more useful information.

CREATE TABLE syntax diagrams can be found on the official [sqlite website](http://www.sqlite.org/lang_createtable.html).

## Usage
Just include sql3parse_table.h and sql3parse_table.c in your project and invoke:
```c
// sql is the CREATE TABLE sql statement
// length is maximum length of sql in bytes or if 0 is passed then strlen is used to determine it
// error is the returned error code (can be NULL)

sql3table *sql3parse_table (const char *sql, size_t length, sql3error_code *error);
```
**sql3table** is an opaque struct that you can introspect using the sql3table* public functions.  
The file sql3parse_debug.c shows you how to use all the API.


## Speed and memory considerations
Parser should be pretty fast because very few memory allocations are performed (for each string for example it does not allocate memory and the reason why a sql3string is used). Memory requirement is linearly proportional to the number of columns in the table.
```
You can estimate memory usage (on a 64bit system) usign the following formula:
N1: number of columns WITHOUT a foreign key constraint
N2: number of columns WITH a foreign key constraint
N3: number of indexed columns in table constraint
K: 0 if no foreign key yable constraint is used or 64
Memory usage (in bytes): 144 + (N1 * 144) + (N2 * 208) + (N3 * 40) + K
```

## Current Limitations
- CREATE TABLE AS select-stmt syntax is not supported (SQL3ERROR_UNSUPPORTEDSQL is returned).
- EXPRESSIONS in column constraints (CHECK and DEFAULT constraint) and table constraint (CHECK constraint) are not supported (SQL3ERROR_UNSUPPORTEDSQL is returned).

## Pre-requisites
- A C99 compiler.
- SQL statement must be successfully compiled by sqlite.
