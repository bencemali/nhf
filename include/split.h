#ifndef SPLIT_INCLUDED
#define SPLIT_INCLUDED

typedef struct Word_list {
    int num;
    char **pointer;
} Word_list;

int count_spaces(char * string);

char ** split(char * string, int cnt);

#endif