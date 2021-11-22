#! /bin/bash

rm *.o

gcc -c main.c
gcc -c prompt.c
gcc -c input.c
gcc -c split.c
gcc -c proc.c
gcc -o terminal *.o

./terminal
