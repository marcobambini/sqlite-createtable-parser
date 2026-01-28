## SQLite CREATE and ALTER TABLE Parser
A parser for SQLite [create table](https://www.sqlite.org/lang_createtable.html) and [alter table](https://www.sqlite.org/lang_altertable.html) sql statements.

* Extremely fast parser with no memory copy overhead
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
- SQL statement must be **successfully compiled** by SQLite.

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
sql3string          *sql3table_comment (sql3table *table);
bool                sql3table_is_temporary (sql3table *table);
bool                sql3table_is_ifnotexists (sql3table *table);
bool                sql3table_is_withoutrowid (sql3table *table);
size_t              sql3table_num_columns (sql3table *table);
sql3column          *sql3table_get_column (sql3table *table, size_t index);
size_t              sql3table_num_constraints (sql3table *table);
sql3tableconstraint *sql3table_get_constraint (sql3table *table, size_t index);
void                sql3table_free (sql3table *table);
sql3statement_type  sql3table_type (sql3table *table);
sql3string          *sql3table_current_name (sql3table *table);
sql3string          *sql3table_new_name (sql3table *table);

	
// Table constraints
sql3string          *sql3table_constraint_name (sql3tableconstraint *tconstraint);
sql3constraint_type sql3table_constraint_type (sql3tableconstraint *tconstraint);
bool                sql3table_constraint_is_autoincrement (sql3tableconstraint* tconstraint);
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
sql3string          *sql3column_comment (sql3column *column);
bool                sql3column_is_primarykey (sql3column *column);
bool                sql3column_is_autoincrement (sql3column *column);
bool                sql3column_is_notnull (sql3column *column);
bool                sql3column_is_unique (sql3column *column);
sql3string          *sql3column_pk_constraint_name (sql3column *column);
sql3order_clause    sql3column_pk_order (sql3column *column);
sql3conflict_clause sql3column_pk_conflictclause (sql3column *column);
sql3string          *sql3column_notnull_constraint_name (sql3column *column);
sql3conflict_clause sql3column_notnull_conflictclause (sql3column *column);
sql3string          *sql3column_unique_constraint_name (sql3column *column);
sql3conflict_clause sql3column_unique_conflictclause (sql3column *column);
size_t              sql3column_num_check_constraints (sql3column *column);
sql3string          *sql3column_check_constraint_name (sql3column *column, size_t index);
sql3string          *sql3column_check_expr (sql3column *column, size_t index);
sql3string          *sql3column_default_constraint_name (sql3column *column);
sql3string          *sql3column_default_expr (sql3column *column);
sql3string          *sql3column_collate_constraint_name (sql3column *column);
sql3string          *sql3column_collate_name (sql3column *column);
sql3string          *sql3column_foreignkey_constraint_name (sql3column *column);
sql3foreignkey      *sql3column_foreignkey_clause (sql3column *column);
sql3string          *sql3column_generated_constraint_name (sql3column *column);
sql3string          *sql3column_generated_expr (sql3column *column);
sql3gen_type        sql3column_generated_type (sql3column *column);
	
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
char                *sql3string_alloc_cstring (sql3string *s);
void                sql3string_free_cstring (char *s);

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

    // table comment
    ptr = sql3table_comment(table);
    if (ptr) sql3string_dump(ptr, "Table Comment");
    
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
    printf("\nNum Table Constraint: %zu\n", num_constraint);
    for (size_t i=0; i<num_constraint; ++i) {
        sql3tableconstraint *constraint = sql3table_get_constraint(table, i);
        printf("\n== TABLE CONSTRAINT %zu ==\n", i);
        sql3tableconstraint_dump(constraint);
    }
    printf("\n");
}
```

Here’s a cleaner, more precise, and more professional rewrite of your markdown, with improved flow, correctness, and clarity while preserving your intent and technical tone.

---

## Implementing a Complete `ALTER TABLE` in SQLite

SQLite supports only a limited subset of the SQL standard’s [`ALTER TABLE`](https://www.sqlite.org/lang_altertable.html) functionality. Specifically, SQLite can:

* Rename a table
* Rename a column
* Add a column
* Drop a column

Unlike most SQL database engines, SQLite does **not** store a parsed representation of the schema in internal system tables. Instead, the schema is stored verbatim as SQL text in the `sqlite_schema` table. As a consequence, altering a table often requires rewriting the original `CREATE TABLE` statement. This design keeps SQLite lightweight and flexible, but makes non-trivial schema changes more complex—especially for creatively structured schemas.

While SQLite directly supports only the four ALTER operations listed above, **any arbitrary table transformation** can still be achieved by following a well-defined sequence of operations. This approach is the official recommendation from SQLite, and it is precisely why this project provides a parser: to reconstruct a new `CREATE TABLE` statement from an existing table definition while preserving all relevant constraints, indexes, and metadata.

### General ALTER TABLE Emulation Algorithm

```sql
PRAGMA writable_schema = ON;
PRAGMA foreign_keys = OFF;

BEGIN TRANSACTION;

/* Recreate the new table by parsing the old one and extracting all schema details */
CREATE TABLE new_table (
    column_definition,
    ...
);

INSERT INTO new_table (column_list)
    SELECT column_list
    FROM old_table;

DROP TABLE old_table;

ALTER TABLE new_table RENAME TO old_table;

COMMIT;

PRAGMA foreign_keys = ON;
PRAGMA writable_schema = OFF;
```

More details about this procedure can be found in the official SQLite documentation:
[https://www.sqlite.org/lang_altertable.html](https://www.sqlite.org/lang_altertable.html)

The critical step is reconstructing the `CREATE TABLE` statement for `new_table`. This repository provides a fast and memory-efficient SQL parser specifically designed to extract complete structural information from existing SQLite table definitions.

---

## Speed and Memory Considerations

The parser is designed for high performance. It performs very few heap allocations and avoids copying between the input SQL string and internal `sql3string` structures. As a result, parsing is extremely fast even for complex schemas.

Memory usage grows linearly with the number of table columns and constraints. On a 64-bit system, memory consumption can be estimated as:

```
N1 = number of columns without a FOREIGN KEY constraint
N2 = number of columns with a FOREIGN KEY constraint
N3 = number of indexed columns in table-level constraints
K  = 0 if no table-level FOREIGN KEY constraint is used, otherwise 64

Memory usage (bytes):
144 + (N1 × 144) + (N2 × 208) + (N3 × 40) + K
```

This predictable footprint makes the parser suitable for embedded and edge environments where both speed and memory efficiency are critical.

---

## Other information
This code is actually used by [Creo](https://creolabs.com).

If you are interested in my others GitHub projects then take a look at the [Gravity](https://github.com/marcobambini/gravity) programming language.
