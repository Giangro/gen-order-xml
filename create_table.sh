#!/bin/bash

SQLITE_PATH=/usr/bin
SQL_CREATE_PATH=.
DATABASE_NAME=nelist.db

echo -n creating table ...
$SQLITE_PATH/sqlite3 $DATABASE_NAME <$SQL_CREATE_PATH/create_table.sql
echo created.
