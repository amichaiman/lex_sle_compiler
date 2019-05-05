#!/bin/bash
if [ "$#" -ne 2 ]; then
    echo "usage ./run.sh <lex.l file> <input file for compiler>"
    echo "example: ./run.sh lex.l foo.sle"
    exit 1
fi
lex $1
gcc *.c -o $1.runme
./$1.runme $2
echo "--------------------------------------------------"
echo "printing the generated file $2.tok"
echo "--------------------------------------------------"
cat $2.tok
echo "--------------------------------------------------"
echo "printing the generated file $2.lst"
echo "--------------------------------------------------"
cat $2.lst
