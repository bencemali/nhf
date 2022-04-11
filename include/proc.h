#ifndef PROC_INCLUDED
#define PROC_INCLUDED
#include "split.h"

//enum for command types
typedef enum Type {
    program, //callable program
    file_in, //file to write stdout to 
    file_out, //file to read stdin from 
    file_append //file to append stdout to
} Type;

//struct to store number of commands, their location in the word array, and their types
typedef struct Command_list {
    int num;
    int * locations;
    Type * types;
} Command_list;

void proc(Word_list * word_list, Command_list * result);

#endif

