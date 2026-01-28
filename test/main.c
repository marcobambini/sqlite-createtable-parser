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
    
    // GENERATED ALWAYS AS ... STORED
    test_case("CREATE TABLE Inventory(Price REAL, Qty INT, Total REAL GENERATED ALWAYS AS (Price*Qty) STORED);");

    // Table-level CHECK constraint
    test_case("CREATE TABLE RangeCheck(lo INT, hi INT, CHECK (lo < hi));");

    // Table-level CHECK constraint with name
    test_case("CREATE TABLE NamedCheck(val INT, CONSTRAINT 'ValidRange' CHECK (val >= 0 AND val <= 100));");

    // Composite PRIMARY KEY (table-level)
    test_case("CREATE TABLE CompositePK(a INT, b TEXT, c REAL, PRIMARY KEY (a, b));");

    // Composite PRIMARY KEY with ordering and collation
    test_case("CREATE TABLE CompositePKOrder(x INT, y TEXT, PRIMARY KEY (x DESC, y COLLATE NOCASE ASC));");

    // Table-level FOREIGN KEY with ON DELETE CASCADE and ON UPDATE SET NULL
    test_case("CREATE TABLE Orders(id INTEGER PRIMARY KEY, customer_id INT, FOREIGN KEY (customer_id) REFERENCES Customers(id) ON DELETE CASCADE ON UPDATE SET NULL);");

    // Table-level FOREIGN KEY with ON DELETE SET DEFAULT and ON UPDATE RESTRICT
    test_case("CREATE TABLE LineItems(id INTEGER PRIMARY KEY, order_id INT, FOREIGN KEY (order_id) REFERENCES Orders(id) ON DELETE SET DEFAULT ON UPDATE RESTRICT);");

    // Table-level FOREIGN KEY with ON DELETE NO ACTION and DEFERRABLE INITIALLY DEFERRED
    test_case("CREATE TABLE Payments(id INTEGER PRIMARY KEY, order_id INT, FOREIGN KEY (order_id) REFERENCES Orders(id) ON DELETE NO ACTION DEFERRABLE INITIALLY DEFERRED);");

    // Table-level FOREIGN KEY with NOT DEFERRABLE INITIALLY IMMEDIATE
    test_case("CREATE TABLE Shipments(id INTEGER PRIMARY KEY, order_id INT, FOREIGN KEY (order_id) REFERENCES Orders(id) ON DELETE RESTRICT NOT DEFERRABLE INITIALLY IMMEDIATE);");

    // Column-level REFERENCES with ON DELETE CASCADE ON UPDATE SET NULL
    test_case("CREATE TABLE Detail(id INTEGER PRIMARY KEY, parent_id INT REFERENCES Parent(id) ON DELETE CASCADE ON UPDATE SET NULL);");

    // Multiple table constraints (PK + UNIQUE + CHECK + FK)
    test_case("CREATE TABLE Multi(a INT, b INT, c INT, d INT,\
        PRIMARY KEY (a, b),\
        UNIQUE (c),\
        CHECK (d > 0),\
        FOREIGN KEY (d) REFERENCES Other(id) ON DELETE CASCADE);");

    // ON CONFLICT clauses on column constraints
    test_case("CREATE TABLE ConflictTest(\
        a INT PRIMARY KEY ON CONFLICT ROLLBACK,\
        b INT NOT NULL ON CONFLICT ABORT,\
        c INT UNIQUE ON CONFLICT REPLACE);");

    // ON CONFLICT clause on table-level PRIMARY KEY
    test_case("CREATE TABLE ConflictPK(a INT, b INT, PRIMARY KEY (a, b) ON CONFLICT IGNORE);");

    // ON CONFLICT clause on table-level UNIQUE
    test_case("CREATE TABLE ConflictUniq(a INT, CONSTRAINT 'uniq1' UNIQUE (a) ON CONFLICT FAIL);");

    // STRICT only (without WITHOUT ROWID)
    test_case("CREATE TABLE StrictOnly(id INTEGER PRIMARY KEY, val TEXT) STRICT;");

    // Backtick-quoted identifiers
    test_case("CREATE TABLE `my table`(`col 1` INT, `col 2` TEXT);");

    // DEFAULT with negative number
    test_case("CREATE TABLE Defaults(a INT DEFAULT -42, b REAL DEFAULT -3.14, c INT DEFAULT +0);");

    // DEFAULT with various keywords
    test_case("CREATE TABLE DefaultKeywords(a TEXT DEFAULT CURRENT_DATE, b TEXT DEFAULT CURRENT_TIME, c TEXT DEFAULT CURRENT_TIMESTAMP);");

    // Single column table
    test_case("CREATE TABLE Single(only_col INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL);");

    // Column with no type
    test_case("CREATE TABLE NoTypes(a, b, c);");

    // PRIMARY KEY DESC with AUTOINCREMENT
    test_case("CREATE TABLE PKDesc(id INTEGER PRIMARY KEY DESC);");

    // Column-level foreign key with multiple columns in referenced table
    test_case("CREATE TABLE FKMultiCol(id INTEGER PRIMARY KEY, a INT, b INT,\
        FOREIGN KEY (a, b) REFERENCES Other(x, y) ON DELETE SET NULL ON UPDATE CASCADE);");

    // DEFERRABLE without INITIALLY clause
    test_case("CREATE TABLE DeferSimple(id INTEGER PRIMARY KEY, ref INT,\
        FOREIGN KEY (ref) REFERENCES Parent(id) DEFERRABLE);");

    // NOT DEFERRABLE INITIALLY DEFERRED
    test_case("CREATE TABLE DeferNotDef(id INTEGER PRIMARY KEY, ref INT,\
        FOREIGN KEY (ref) REFERENCES Parent(id) NOT DEFERRABLE INITIALLY DEFERRED);");

    // Named table-level constraints
    test_case("CREATE TABLE NamedConstraints(a INT, b INT, c INT,\
        CONSTRAINT pk_named PRIMARY KEY (a),\
        CONSTRAINT uq_named UNIQUE (b),\
        CONSTRAINT ck_named CHECK (c != 0));");

    // All constraint types combined on a single column
    test_case("CREATE TABLE FullCol(x INTEGER PRIMARY KEY ON CONFLICT ABORT NOT NULL ON CONFLICT FAIL UNIQUE ON CONFLICT IGNORE\
        CHECK (x > 0) DEFAULT 1 COLLATE BINARY REFERENCES Other(id));");

    // Generated column shorthand (AS without GENERATED ALWAYS)
    test_case("CREATE TABLE GenShort(a INT, b INT, c INT AS (a + b) STORED, d TEXT AS (a || b) VIRTUAL);");

    // Mixed quoting styles
    test_case("CREATE TABLE \"Mixed\"([col1] INT, `col2` TEXT, col3 BLOB);");

    // Table with many columns and types
    test_case("CREATE TABLE AllTypes(a INTEGER, b REAL, c TEXT, d BLOB, e NUMERIC, f BOOLEAN, g DATE, h DATETIME, i DECIMAL(10,2), j VARCHAR(255));");

    // Empty string and NULL defaults
    test_case("CREATE TABLE EmptyDefaults(a TEXT DEFAULT '', b TEXT DEFAULT NULL, c INT DEFAULT 0);");

    // Case insensitivity
    test_case("create temporary table if not exists Foo(Bar integer primary key autoincrement, Baz text not null unique);");

    // ALTER TABLE with schema-qualified name and ADD COLUMN with all constraints
    test_case("ALTER TABLE main.foo ADD COLUMN new_col INTEGER NOT NULL DEFAULT 0 REFERENCES Other(id)");

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
