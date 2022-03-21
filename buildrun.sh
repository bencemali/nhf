#!/bin/bash

gcc -Wall -g -I./ -c src/*.c && gcc -Wall -g -I./ -o emulator *.o && rm *.o && ./emulator
