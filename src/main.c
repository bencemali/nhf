//The whole program is one file for now for simplicity

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "../headers/split.h"
#include "../headers/input.h"
#include "../headers/proc.h"
#include "../headers/exec.h"


int exec_input(Word_list * words, Command_list * command_list) {
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


int main(void) {

    prompt(); //main just calls the prompt function

    return 0;
}
