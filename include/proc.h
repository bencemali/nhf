#ifndef PROC_INCLUDED
#define PROC_INCLUDED
#include "split.h"

typedef enum Type {
    program,
    file_in,
    file_out,
} Type;

typedef struct Command_list {
    int num;
    int * locations;
    Type * types;
} Command_list;

void proc(Word_list * word_list, Command_list * result);

#endif