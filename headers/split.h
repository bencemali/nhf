#ifndef LINETOWORD_H
#define LINETOWORD_H

typedef struct Word_list {
    int num;
    char **pointer;
} Word_list;

void split(char * string, Word_list * result);

#endif