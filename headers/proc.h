#ifndef PROC_INCLUDED
#define

typedef enum Type {
    program,
    file_in,
    file_out,
} Type;

typedef struct Command_list {
    int num;
    int * locations;
    Type * types;
}

void proc(Word_list * word_list, Command_list * result);

#endif