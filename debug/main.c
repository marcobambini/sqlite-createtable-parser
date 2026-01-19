//
//  main.c
//  CreateTableParser
//
//  Created by Marco Bambini on 14/02/16.
//

#include "sql3parse_table.h"
#include "sql3parse_debug.h"

static int ntest = 0;
static int nsuccess = 0;

static void test_case (const char *sql) {
    printf("\n=== SQL: %s\n", sql);
    
    ++ntest;
    sql3error_code err = 0;
    sql3table *t = sql3parse_table(sql, strlen(sql), &err);
    printf("Error = %d\n", (int)err);
    if (err == 0) {table_dump(t); ++nsuccess;}
    
    sql3table_free(t);
}

int main (void) {
    // https://www.sqlite.org/lang_createtable.html
    
    test_case("CREATE TABLE t(a INT, b TEXT)");
    test_case("CREATE TABLE t1(\n  id INTEGER PRIMARY KEY ASC,\n  name TEXT DEFAULT (upper('x')),\n  c TEXT CHECK((a+(b))) -- col comment\n) -- table comment");
    test_case("CREATE TEMP TABLE IF NOT EXISTS [w\"eird]]t] (\"q\"\"q\" INT)");
    test_case("ALTER TABLE t RENAME COLUMN a TO b;");
    test_case("ALTER TABLE main.t ADD COLUMN z INTEGER DEFAULT (1+(2*(3)))");
    test_case("/* cstyle */ CREATE TABLE x(y INT); -- tail");
    test_case("CREATE TABLE ct (d INT DEFAULT ( (1+2) ), e TEXT DEFAULT '))')");
    
    test_case("CREATE TABLE foo (col1 INTEGER PRIMARY KEY AUTOINCREMENT, col2 TEXT, col3 TEXT);");
    test_case("CREATE TABLE tcpkai (col INTEGER, PRIMARY KEY (col AUTOINCREMENT));");
    test_case("CREATE TABLE t1(x INTEGER PRIMARY KEY, y);");
    test_case("create table employee(first varchar(15),last varchar(20),age number(3),address varchar(30),city varchar(20),state varchar(20));");
    test_case("CREATE TEMP TABLE IF NOT EXISTS main.foo /* This is the main table */ (col1 INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, col2 TEXT DEFAULT CURRENT_TIMESTAMP, col3 FLOAT(8.12), col4 BLOB COLLATE BINARY /* Use this column for storing pictures */, CONSTRAINT tbl1 UNIQUE (col1 COLLATE c1 ASC, col2 COLLATE c2 DESC)) WITHOUT ROWID; -- this is a line comment");
    test_case("CREATE TABLE \"BalancesTbl2\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,  \"checkingBal\" REAL DEFAULT 0,  \"cashBal\" REAL DEFAULT .0,  \"defitCardBal\" REAL DEFAULT 1.0,  \"creditCardBal\" REAL DEFAULT +1.5,  testValue TEXT DEFAULT 'Hello World',   testValue2 TEXT DEFAULT 'Hello''s World', testValue3 TEXT DEFAULT \"Hello''s World\", testValue4 TEXT DEFAULT \"Hello\"\" World\") WITHOUT ROWID, STRICT;");
    test_case("CREATE TABLE User\
            -- A table comment\n\
            (\
            uid INTEGER,    -- A field comment\n\
            flags INTEGER,  -- Another field comment\n\
            test TEXT /* Another C style comment */\
            );");
    test_case("CREATE TABLE User\
            -- A table comment\n\
        (\
            uid INTEGER,    -- A field comment\n\
            flags /*This is another column comment*/ INTEGER   -- Another field comment\n\
        , test -- test 123\n\
        INTEGER, UNIQUE (flags /* Hello World*/, test) -- This is another table comment\n\
        );");
    test_case("CREATE TABLE Sales(Price INT, Qty INT, Total INT GENERATED ALWAYS AS (Price*Qty) VIRTUAL, Item TEXT);");
    test_case("CREATE TABLE Constraints(\
        PK  INTEGER CONSTRAINT 'PrimaryKey' PRIMARY KEY  CONSTRAINT 'NotNull' NOT NULL  CONSTRAINT 'Unique' UNIQUE\
                    CONSTRAINT 'Check'      CHECK (PK>0) CONSTRAINT 'Default' DEFAULT 2 CONSTRAINT 'Collate' COLLATE NOCASE,\
        FK  INTEGER CONSTRAINT 'ForeignKey' REFERENCES ForeignTable (Id),\
        GEN INTEGER CONSTRAINT 'Generated' AS (abs(PK)));");
    test_case("CREATE TABLE ColumnChecks(Num INT CONSTRAINT 'GT' CHECK (Num>0) CONSTRAINT 'LT' CHECK (Num<10) CONSTRAINT 'NE' CHECK(Num<>5));");
    
    // https://www.sqlite.org/lang_altertable.html
    test_case("ALTER TABLE foo RENAME TO bar");
    test_case("ALTER TABLE temp.foo RENAME TO bar");
    test_case("ALTER TABLE foo RENAME COLUMN col1 TO col2");
    test_case("ALTER TABLE foo RENAME col1 TO col2");
    test_case("ALTER TABLE foo DROP COLUMN col1");
    test_case("ALTER TABLE foo ADD COLUMN col1 TEXT DEFAULT 'Hello' COLLATE NOCASE");
	
    printf("\n=========== STATS ===========\n");
    printf("Number of tests:     %d\n", ntest);
    printf("Number of successes: %d\n", nsuccess);
    printf("Succesfull rate:     %.2f%%\n", (float)(((float)nsuccess*100.0f)/(float)ntest));
    printf("==============================\n");
    
    return 0;
}
