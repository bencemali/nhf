#ifndef EXEC_INCLUDED
#define EXEC_INCLUDED
#include "proc.h"
#include "split.h"

void close_pipes(int size, int * pipefds);

int pipe_end_by_diff(char * read_or_write_end, int own_rank, int diff);

int next_type_diff(Type sought_type, Type * types, int size, int own_rank);

void connect_stdout(int * pipefds, int rank, int diff);

void connect_stdin(int * pipefds, int rank, int diff);

int exec(Word_list * word_list, Command_list * command_list);

#endif
