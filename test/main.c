//
//  main.c
//  CreateTableParser
//
//  Created by Marco Bambini on 14/02/16.
//

#include "sql3parse_table.h"
#include "sql3parse_debug.h"

int main (void) {
	//const char *sql = "CREATE TABLE foo (col1 INTEGER PRIMARY KEY AUTOINCREMENT, col2 TEXT, col3 TEXT);";
	//const char *sql = "CREATE TABLE t1(x INTEGER PRIMARY KEY, y);";
	//const char *sql = "create table employee(first varchar(15),last varchar(20),age number(3),address varchar(30),city varchar(20),state varchar(20));";
	const char *sql = "CREATE TEMP TABLE IF NOT EXISTS main.foo /* This is the main table */ (col1 INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, col2 TEXT DEFAULT CURRENT_TIMESTAMP, col3 FLOAT(8.12), col4 BLOB COLLATE BINARY /* Use this column for storing pictures */, CONSTRAINT tbl1 UNIQUE (col1 COLLATE c1 ASC, col2 COLLATE c2 DESC)) WITHOUT ROWID; -- this is a line comment";
	//const char *sql = "CREATE TABLE foo (col1, col2, col3, col4, CONSTRAINT const1 PRIMARY KEY(col1, col2), UNIQUE(col3, col4), FOREIGN KEY (col1, col4) REFERENCES foo2 (c45,c46) ON DELETE SET NULL DEFERRABLE INITIALLY IMMEDIATE);";
	
	sql3table *table = sql3parse_table(sql, 0, NULL);
	if (!table) printf("An error occurred while parsing table.\n");
	table_dump(table);
	sql3table_free(table);
	
    return 0;
}
