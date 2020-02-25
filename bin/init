#!/bin/bash
BASEDIR=$(dirname "$0")
cd $BASEDIR

gcc  db_init.c -o db_init $(mariadb_config --libs) $(pkg-config --libs --cflags libcurl)

./db_init
./compile_linux