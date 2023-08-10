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
	//const char *sql = "CREATE TEMP TABLE IF NOT EXISTS main.foo /* This is the main table */ (col1 INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, col2 TEXT DEFAULT CURRENT_TIMESTAMP, col3 FLOAT(8.12), col4 BLOB COLLATE BINARY /* Use this column for storing pictures */, CONSTRAINT tbl1 UNIQUE (col1 COLLATE c1 ASC, col2 COLLATE c2 DESC)) WITHOUT ROWID; -- this is a line comment";
	//const char *sql = "CREATE TABLE foo (col1, col2, col3, col4, CONSTRAINT const1 PRIMARY KEY(col1, col2), UNIQUE(col3, col4), FOREIGN KEY (col1, col4) REFERENCES foo2 (c45,c46) ON DELETE SET NULL DEFERRABLE INITIALLY IMMEDIATE);";
	
    //const char *sql = "CREATE TABLE \"BalancesTbl2\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,  \"checkingBal\" REAL DEFAULT 0,  \"cashBal\" REAL DEFAULT .0,  \"defitCardBal\" REAL DEFAULT 1.0,  \"creditCardBal\" REAL DEFAULT +1.5,  testValue TEXT DEFAULT 'Hello World',   testValue2 TEXT DEFAULT 'Hello''s World', testValue3 TEXT DEFAULT \"Hello''s World\", testValue4 TEXT DEFAULT \"Hello\"\" World\") WITHOUT ROWID, STRICT;";
    
    //const char *sql = "CREATE TABLE User\
    -- A table comment\n\
    (\
    uid INTEGER,    -- A field comment\n\
    flags INTEGER,  -- Another field comment\n\
    test TEXT /* Another C style comment */\
    );";
    
    const char *sql = "CREATE TABLE User\
    -- A table comment\n\
(\
    uid INTEGER,    -- A field comment\n\
    flags /*This is another column comment*/ INTEGER   -- Another field comment\n\
, test -- test 123\n\
INTEGER, UNIQUE (flags /* Hello World*/, test) -- This is another table comment\n\
);";
    
    sql3error_code err;
	sql3table *table = sql3parse_table(sql, 0, &err);
	if (!table) printf("An error occurred while parsing table (%d).\n", err);
	table_dump(table);
	sql3table_free(table);
	
    return 0;
}
