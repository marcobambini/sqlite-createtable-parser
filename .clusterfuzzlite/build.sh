#!/bin/bash
$CC $CFLAGS -c sql3parse_table.c
llvm-ar rcs libfuzz.a *.o


$CC $CFLAGS $LIB_FUZZING_ENGINE $SRC/fuzzer.c -Wl,--whole-archive $SRC/sqlite-createtable-parser/libfuzz.a -Wl,--allow-multiple-definition -I$SRC/sqlite-createtable-parser/ -I$SRC/sqlite-createtable-parser/debug  -o $OUT/fuzzer
