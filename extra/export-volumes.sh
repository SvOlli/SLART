#!/bin/sh

echo "BEGIN TRANSACTION;"
echo "select 'UPDATE slart_tracks SET Volume=' || Volume || ', LastScanned=' || LastScanned || ', Flags=' || Flags || ' WHERE Directory = ' || QUOTE(Directory) || ' AND FileName = ' || QUOTE(FileName) || ';' from slart_tracks WHERE Volume > 0;" |
sqlite3 ${HOME}/.slartdb
echo "COMMIT;"

