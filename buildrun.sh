#!/bin/bash

clear && gcc -Wall  -c src/*.c && gcc -Wall  -o emulator *.o && rm *.o && ./emulator
