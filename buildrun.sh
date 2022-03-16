#!/bin/bash

clear && gcc -Wall -I ./include/ -c src/*.c && gcc -Wall -I ./include/ -o emulator *.o && rm *.o && ./emulator
#make emulator && ./emulator
