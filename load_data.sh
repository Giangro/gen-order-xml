#!/bin/bash

SQLITE_PATH=/usr/bin
SQL_CREATE_PATH=.
DATABASE_NAME=nelist.db

file=ri.csv
table_name=NELIST

echo loading data $file
$SQLITE_PATH/sqlite3 $DATABASE_NAME <<!
.mode csv
.separator ","
.import $file $table_name
!
echo data loaded
