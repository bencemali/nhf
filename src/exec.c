#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "../include/exec.h"
#include "../include/split.h"
#include "../include/input.h"
#include "../include/proc.h"
#include "../include/debugmalloc.h"


//MACROS
#define EXIT_WITH_QUIT 22
#define REPROMPT_RETURN_VALUE 0
#define NO_COMMAND_WITH_TYPE 0
#define SOMETHING_WRONG 999
//MACROS


//Closes ALL pipe ends
void close_pipes(int size, int * pipefds) {
    for(int i = 0; i < size; ++i) {
        close(pipefds[i]);
    }
    free(pipefds);
}

//Finds read or write end of pipe
//Accepts current process' rank
//Gives back pipefd index for other process by difference
int pipe_end_by_diff(char * read_or_write_end, int own_rank, int diff) {
    if(strcmp("read", read_or_write_end) == 0) {
        return ((own_rank + diff) - 1) * 2;
    } else {
        return (((own_rank + diff) * 2) - 1);
    }
    return SOMETHING_WRONG;
}

//Finds difference between next 'type' type command rank and current process rank
int next_type_diff(Type sought_type, Type * types, int size, int own_rank) {
    for(int i = own_rank; i < size; ++i) {
        if(types[i] == sought_type) {
            return (i - own_rank + 1);
        }
    }
    return NO_COMMAND_WITH_TYPE;
}

//Connects stdout to other command's pipe's write end
//Other process identified by diff
void connect_stdout(int * pipefds, int rank, int diff) {
    int pipe_index = pipe_end_by_diff("write", rank, diff);
    dup2(pipefds[pipe_index], fileno(stdout));
}

//Same as stdout
void connect_stdin(int * pipefds, int rank, int diff) {
    int pipe_index = pipe_end_by_diff("read", rank, diff);
    dup2(pipefds[pipe_index], fileno(stdin));
}



int exec(Word_list * word_list, Command_list * command_list) {
    if(command_list->num == 1 && strcmp(word_list->pointer[0], "quit") == 0) {
        
        exit(EXIT_WITH_QUIT);
    }
	int * pipefds; pipefds = (int*) malloc(command_list->num * 2 * sizeof(int)); for(int z = 0; z < (command_list->num * 2); z += 2) {
		pipe(pipefds + z);
	}
	int rank;
	rank = 0;
	//int *children = (int*)malloc(command_list->num * sizeof(char));
	pid_t parentid = getpid();
	for(int i = 1; i < (command_list->num + 1); ++i) {
		if(getpid() == parentid) {
			//int pid = fork();
            fork();
			if(getpid() != parentid) {
				rank = i;
			} else {
				//children[i - 1] = pid;
			}
		}
	}

    //If not parent process determine type and get things done
    if(rank != 0) {

        //If type is program
        if(command_list->types[rank - 1] == program) {

            //Get stdin from local pipe, only if...
            if(rank != 1) {
                connect_stdin(pipefds, rank, 0);
                //dup2(pipefds[(rank - 1) * 2], fileno(stdin));
            } else {
                if(command_list->types[1] == file_in) {
                    connect_stdin(pipefds, rank, 0);
                    //dup2(pipefds[0], fileno(stdin));
                }
            }

            //Find out if stdout has to be routed to subsequent command

            //If program is somewhere in the middle of action
            if(rank <= (command_list->num - 2)) {
                if(command_list->types[rank] != file_in) {
                    // Connect stdout to next command's stdin
                    connect_stdout(pipefds, rank, 1);
                    //dup2(pipefds[(rank * 2) + 1], fileno(stdout));
                } else {
                    // Connect stdout to next program's input
                    int next_program_diff = next_type_diff(program, command_list->types, command_list->num, rank);
                    if(next_program_diff != NO_COMMAND_WITH_TYPE) {
                        connect_stdout(pipefds, rank, next_program_diff);
                    }
                    //dup2(pipefds[((rank + 1) * 2) + 1], fileno(stdout));
                }
            //If program is second to last
            } else if(rank == (command_list->num - 1)) {
                if(command_list->types[rank] != file_in) {
                    // Connect stdout to next command's stdin
                    connect_stdout(pipefds, rank, 1);
                    //dup2(pipefds[(rank * 2) + 1], fileno(stdout));
                }
            }
            //If program is last it must write it's output to stdout
            //so no need to dup it


            //Now close all pipe ends
            close_pipes(command_list->num * 2, pipefds);

            //Execute program
            execvp(word_list->pointer[command_list->locations[rank - 1]], word_list->pointer + command_list->locations[rank - 1]);

        //If command is file_in
        //It has to read a file's content into the previous command's write end
        } else if(command_list->types[rank - 1] == file_in) {
            //If type is file_in

            /*
            //Open file
            FILE *in;
            FILE *out;
            in = fopen(word_list->pointer[command_list->locations[rank - 1]], "rt");
            dup2(pipefds[(rank * 2) - 3], fileno(stdout));
            out = fopen("something.txt", "wt");

            //Write file content into previous command's write pipe end
            fwrite(in, sizeof(char), 1, stdout);

            close_pipes(command_list->num * 2, pipefds);

            fclose(in);
            */
        } else if(command_list->types[rank - 1] == file_out) {
            //If type is file_out

            //Open file

            //Overwrite file content with content read from local pipe's read end
            //Content always comes from previous command
        }

    //Parent process waits for all children, then returns 0, initiating another prompt
    } else {
        close_pipes(command_list->num * 2, pipefds);
        //free(children);
		for(int i = 0; i < command_list->num; ++i) {
			wait(NULL);
		}
        return REPROMPT_RETURN_VALUE;
	}

    return 2;
}
