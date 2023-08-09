## SQLite CREATE TABLE Parser
A parser for SQLite create table sql statements.

* Extremely fast parser with no string copy overhead
* MIT licensed with no dependencies (just drop the C file into your project)
* Never recurses or allocates more memory than it needs
* Very simple API 

## Motivation
[SQLite](https://www.sqlite.org/) is a very powerful software but it lacks an easy way to extract complete information about tables and columns constraints. This drawback in addition to the lack of full ALTER TABLE support makes alterring a table a very hard task. The built-in sqlite pragmas provide incomplete information and a manual parsing is required in order to extract all the metadata from a table.
```c
PRAGMA table_info(table-name);  
PRAGMA foreign_key_list(table-name);
```
CREATE TABLE syntax diagrams can be found on the official [sqlite website](https://www.sqlite.org/lang_createtable.html).

## Pre-requisites
- A C99 compiler.
- SQL statement must be successfully compiled by SQLite.

## Usage
In order to extract the original CREATE TABLE sql statement you need to query the sqlite_master table from within an SQLite database:
```sql
SELECT sql FROM sqlite_master WHERE name = 'myTable';
```

then just include sql3parse_table.h and sql3parse_table.c in your project and invoke:
```c
// sql is the CREATE TABLE sql statement
// length is the length of sql (if 0 then strlen will be used)
// error is the returned error code (can be NULL)
// return value: NULL in case of error or an opaque pointer in case of success

sql3table *sql3parse_table (const char *sql, size_t length, sql3error_code *error);
```
**sql3table** is an opaque struct that you can introspect using the sql3table* public functions.

## API
```c
// Main Entrypoint
sql3table *sql3parse_table (const char *sql, size_t length, sql3error_code *error);

// Table information
sql3string          *sql3table_schema (sql3table *table);
sql3string          *sql3table_name (sql3table *table);
bool                sql3table_is_temporary (sql3table *table);
bool                sql3table_is_ifnotexists (sql3table *table);
bool                sql3table_is_withoutrowid (sql3table *table);
size_t              sql3table_num_columns (sql3table *table);
sql3column          *sql3table_get_column (sql3table *table, size_t index);
size_t              sql3table_num_constraints (sql3table *table);
sql3tableconstraint *sql3table_get_constraint (sql3table *table, size_t index);
void                sql3table_free (sql3table *table);
	
// Table constraints
sql3string          *sql3table_constraint_name (sql3tableconstraint *tconstraint);
sql3constraint_type sql3table_constraint_type (sql3tableconstraint *tconstraint);
size_t              sql3table_constraint_num_idxcolumns (sql3tableconstraint *tconstraint);
sql3idxcolumn       *sql3table_constraint_get_idxcolumn (sql3tableconstraint *tconstraint, size_t index);
sql3conflict_clause sql3table_constraint_conflict_clause (sql3tableconstraint *tconstraint);
sql3string          *sql3table_constraint_check_expr (sql3tableconstraint *tconstraint);
size_t              sql3table_constraint_num_fkcolumns (sql3tableconstraint *tconstraint);
sql3string          *sql3table_constraint_get_fkcolumn (sql3tableconstraint *tconstraint, size_t index);
sql3foreignkey      *sql3table_constraint_foreignkey_clause (sql3tableconstraint *tconstraint);

// Column constraints
sql3string          *sql3column_name (sql3column *column);
sql3string          *sql3column_type (sql3column *column);
sql3string          *sql3column_length (sql3column *column);
sql3string          *sql3column_constraint_name (sql3column *column);
bool                sql3column_is_primarykey (sql3column *column);
bool                sql3column_is_autoincrement (sql3column *column);
bool                sql3column_is_notnull (sql3column *column);
bool                sql3column_is_unique (sql3column *column);
sql3order_clause    sql3column_pk_order (sql3column *column);
sql3conflict_clause sql3column_pk_conflictclause (sql3column *column);
sql3conflict_clause sql3column_notnull_conflictclause (sql3column *column);
sql3conflict_clause sql3column_unique_conflictclause (sql3column *column);
sql3string          *sql3column_check_expr (sql3column *column);
sql3string          *sql3column_default_expr (sql3column *column);
sql3string          *sql3column_collate_name (sql3column *column);
sql3foreignkey      *sql3column_foreignkey_clause (sql3column *column);
	
// Foreign key
sql3string          *sql3foreignkey_table (sql3foreignkey *fk);
size_t              sql3foreignkey_num_columns (sql3foreignkey *fk);
sql3string          *sql3foreignkey_get_column (sql3foreignkey *fk, size_t index);
sql3fk_action       sql3foreignkey_ondelete_action (sql3foreignkey *fk);
sql3fk_action       sql3foreignkey_onupdate_action (sql3foreignkey *fk);
sql3string          *sql3foreignkey_match (sql3foreignkey *fk);
sql3fk_deftype      sql3foreignkey_deferrable (sql3foreignkey *fk);

// Indexed column
sql3string          *sql3idxcolumn_name (sql3idxcolumn *idxcolumn);
sql3string          *sql3idxcolumn_collate (sql3idxcolumn *idxcolumn);
sql3order_clause    sql3idxcolumn_order (sql3idxcolumn *idxcolumn);
	
// String Utils
const char          *sql3string_ptr (sql3string *s, size_t *length);
const char          *sql3string_cstring (sql3string *s);
```

## Example
Dump to stdout complete table information:
```c
// all the necessary code is in sql3parse_debug.h/.c
void table_dump (sql3table *table) {
    if (!table) return;
    
    // schema name
    sql3string *ptr = sql3table_schema(table);
    sql3string_dump(ptr, "Schema Name");
    
    // table name
    ptr = sql3table_name(table);
    sql3string_dump(ptr, "Table Name");
    
    // table flags
    printf("Temporary: %d\n", sql3table_is_temporary(table));
    printf("If Not Exists: %d\n", sql3table_is_ifnotexists(table));
    printf("Without RowID: %d\n", sql3table_is_withoutrowid(table));
    
    // loop to print complete columns info
    size_t num_columns = sql3table_num_columns(table);
    printf("Num Columns: %zu\n", num_columns);
    for (size_t i=0; i<num_columns; ++i) {
        sql3column *column = sql3table_get_column(table, i);
        printf("\n== COLUMN %zu ==\n", i);
        sql3column_dump(column);
    }
    
    // loop to print complete table constraints
    size_t num_constraint = sql3table_num_constraints(table);
    printf("Num Table Constraint: %zu\n", num_constraint);
    for (size_t i=0; i<num_constraint; ++i) {
        sql3tableconstraint *constraint = sql3table_get_constraint(table, i);
        printf("\n== TABLE CONSTRAINT %zu ==\n", i);
        sql3tableconstraint_dump(constraint);
    }
    printf("\n");
}
```

## IMPLEMENT a COMPLETE ALTER TABLE in SQLite
SQLite supports only a limite subset of the [ALTER TABLE](https://www.sqlite.org/lang_altertable.html) sql command. In particular only TABLE RENAME and COLUMN ADD can be natively used. What if we need to change a column definition or dropping a column? Then a series of manual steps are required.


The strategy consists of creating a new table with the up-to-date schema and then copy data from the old table to the new one. To create the new table starting from the old one you need a way to extract complete information from a SQLite table and that's the main reason why I created this parser.


ALTER TABLE algorithm looks like:
```sql
PRAGMA foreign_keys=off;
 
BEGIN TRANSACTION;
 
ALTER TABLE old_table RENAME TO temp_table;

/* new_table can be recreated by parsing the old_table and extracting all relevant information using this repo */
CREATE TABLE new_table(
   column_definition,
   ...
);
 
INSERT INTO new_table (column_list)
  SELECT column_list
  FROM temp_table;
 
DROP TABLE temp_table;
 
COMMIT;
 
PRAGMA foreign_keys=on;
```


## Speed and memory considerations
The parser is blazing fast, mainly because very few memory allocations are performed and no copy operations are used between the sql string and the internal sql3string structs. Memory requirement is linearly proportional to the number of columns in the table.
```
You can estimate memory usage (on a 64bit system) usign the following formula:
N1: number of columns WITHOUT a foreign key constraint
N2: number of columns WITH a foreign key constraint
N3: number of indexed columns in table constraint
K: 0 if no foreign key yable constraint is used or 64
Memory usage (in bytes): 144 + (N1 * 144) + (N2 * 208) + (N3 * 40) + K
```

## Other information
This code is actually used by [Creo](https://creolabs.com).

If you are interested in my others GitHub projects then take a look at the [Gravity](https://github.com/marcobambini/gravity) programming language.
