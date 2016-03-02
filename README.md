# SQLite CREATE TABLE parser
A parser for sqlite create table sql statements.

SQLite is a very powerful software but it lacks an easy way to extract complete information about table and columns constraints. The built-in sql pragma:
```PRAGMA schema.table_info(table-name);```
and
```PRAGMA foreign_key_list(table-name);```
provide incomplete information and a manual parsing is required in order to extract some useful information.

CREATE TABLE syntax diagrams can be found on the official [sqlite website](http://www.sqlite.org/lang_createtable.html).

**Current Limitations**
- CREATE TABLE AS select-stmt syntax is not supported.
- EXPRESSIONS in column constraints (CHECK and DEFAULT constraint) and table constraint (CHECK constraint) are not supported.

**Pre-requisites**
- A C99 compiler.
- SQL statement must be successfully compiled by sqlite.
