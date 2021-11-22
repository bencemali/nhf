#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "exec.h"
#include "split.h"
#include "input.h"
#include "proc.h"

int exec(Word_list * word_list, Command_list * command_list) {
    if(command_list->num == 1 && strcmp(word_list->pointer[0], "quit") == 0) {
        exit(1);
    }
	int * pipefds;
	pipefds = (int*) malloc(command_list->num * 2 * sizeof(int));
	for(int z = 0; z < (command_list->num * 2); z += 2) {
		pipe(pipefds + z);
	}
	int rank;
	rank = 0;
	int *children = (int*)malloc(command_list->num * sizeof(char));
	pid_t parentid = getpid();
	for(int i = 1; i < (command_list->num + 1); ++i) {
		if(getpid() == parentid) {
			int pid = fork();
			if(getpid() != parentid) {
				rank = i;
			} else {
				children[i - 1] = pid;
			}
		}
	}

    //Determine type and get things done
    if(rank != 0) {

        //If type is program
        if(command_list->types[rank - 1] == program) {

            //Get stdin from local pipe, only if...
            if(rank != 1) {
                dup2(pipefds[(rank - 1) * 2], fileno(stdin));
                printf("Rank no. %d, stdin to pipefds[%d]", rank, (rank - 1) * 2);
            } else {
                if(command_list->types[1] == file_in) {
                    dup2(pipefds[0], fileno(stdin));
                }
            }

            //Find out if stdout has to be routed to subsequent command
            if(rank <= (command_list->num - 2)) {
                if(command_list->types[rank] != file_in) {
                    // Connect stdout to next command's stdin
                } else {
                    // Connect stdout to second next command's stdin
                }
            } else if(rank == (command_list->num - 1)) {
                if(command_list->types[rank] != file_in) {
                    // Connect stdout to next command's stdin
                }
            }

            //Now close all pipe ends
            for(int i = 0; i < (command_list->num * 2); ++i) {
                close(pipefds[i]);
            }

            //Execute program
            execvp(word_list->pointer[command_list->locations[rank - 1]], word_list->pointer + command_list->locations[rank - 1]);
        } else if(command_list->types[rank - 1] == file_in) {
            //If type is file_in

            //Open file
            FILE *fp;
            fp = fopen(word_list->pointer[command_list->locations[rank - 1]], "r");
            dup2(pipefds[(rank * 2) - 3], fileno(stdout));

            //Write file content into previous command's write pipe end
            while(fwrite(fp, sizeof(char), 1, stdout) == 1);

            for(int i = 0; i < (command_list->num * 2); ++i) {
                close(pipefds[i]);
            }
        } else if(command_list->types[rank - 1] == file_out) {
            //If type is file_out

            //Open file

            //Overwrite file content with content read from local pipe's read end
            //Content always comes from previous command
        }
    } else {
		for(int i = 0; i < command_list->num; ++i) {
			wait(NULL);
		}
    return 0;
	}

}

/* BACKUP
int exec(Word_list * words, Command_list * command_list) {
    int * pipefds;
    pipefds = (int*) malloc(command_list->count * 2 * sizeof(int));
    for(int z = 0; z < command_list->count * 2; z += 2) { //This 'for' pipes every second pipefd
        pipe(pipefds + z);
    }
    int rank;
    rank = 0;
    pid_t parentid = getpid();
    for(int i = 1; i < (command_list->count + 1); ++i) { //This 'for' creates the forks, assigns them a rank
        if(getpid() == parentid) {
            fork();
            if(getpid() != parentid) {
                rank = i;
            }
        }
    }

    if(rank != 0) {
        if(command_list->types[rank - 1] == program) {
            if(rank == 1) { //Things for the rank 1 program
                for(int i = 1; i < command_list->count; ++i) { //This 'for' finds which later program or file_out is waiting for input
                    if(command_list->count != 1) {
                        if(command_list.types[i] == program || command_list.types[i] == file_out) {
                            for(int j = 1; j < (command_list.count * 2); ++j) { //This 'for' closes every pipe other than the one waiting for input
                                if(j != (2 * i - 1)) { //input pipe handled by starting 'for' at 1
                                    close(pipefds[j]);
                                }
                            }
                        }
                        if(command_list[i + 1] == file_in) {
                            dup2(pipefds[0], fileno(stdin));
                        }
                        close(pipefds[0]);
                        dup2(pipefds[2 * i - 1], fileno(stdout)); //redirect stdout to receiving program
                        close(pipefds[2 * i - 1]);
                        execvp(words.pointer[0], words.pointer[0]); //Execute the first program
                    } else { //If there is only one program to be executed close all pipes
                        close[pipefds[0]];
                        close[pipefds + 1];
                        execvp(words.pointer[0], words.pointer[0]); //Execute the only program there is
                    }
                }
            } else {
                if(command_list.types[(rank - 1) + 1] == program) { //If next command is program, and is expecting piped input
                    for(int i = 0; i < (command_list.count * 2); ++i) { //This 'for' closes all pipes that are not this pipe's input or the next pipe's input
                        if(i != ((rank -1) * 2) && i != ((rank * 2) + 1)) {
                            close(pipefds[i]);
                        }
                    }
                }
            }
        } else {
            if(command_list.types[rank - 1] == file_out) { // >
                    for(int j = 0; j < (command_list.count * 2); ++j) { //This 'for' closes all pipes except the one where it gets its input from
                        if(j != (rank - 1) * 2) {
                            close(pipefds[j]);
                        }
                    }
                    FILE * fp;
                    int location = command_list.locations[rank - 1];
                    fp = fopen(words.pointer[location], "wt");
                    dup2(pipefds[(rank - 1) * 2], fp);

                    //IT HAS TO READ INTO FILE

                    fclose(pipefds[(rank - 1) * 2]);
            } else if(command_list->types[rank - 1] == file_in) { // <
                for(int i = 0; i < (command_list.count * 2); ++i) {
                    if(i != (((rank - 1) * 2) - 1)) {
                        close(pipefds[i]);
                    }
                }
                FILE * fp;
                int location = command_list.locations[rank - 1];
                fp = fopen(words.pointer[location], "rt");

                // IT HAS TO READ THE WHOLE FILE INTO pipefds[((rank - 1) * 2) - 1]

                close(pipefds[(rank)]);
                fclose(fp);
            }
        }
    } else {

    }

    return 0;
}
*/