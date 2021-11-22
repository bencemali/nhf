#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "input.h"
//#include "debugmalloc.h"

char * input(void) {
    int size = 1;
    int position = 0;
    char * line = (char*) malloc(size * sizeof(char));
    line[0] = '\0';
    char newchar;
    while(scanf("%c", &newchar) == 1 && newchar != '\n' && newchar != EOF) {
        if(position + 1 >= size) {
            size *= 2;
            line = (char*) realloc(line, size * sizeof(char));
        }
        line[position] = newchar;
        line[position+1] = '\0';
        ++position;
    }
    return line;
}
