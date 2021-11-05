#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

typedef enum Type {
    program,
    file_in,
    file_out,
} Type;

typedef struct {
    int size;
    char ** pointer;
} Word_list;

typedef struct {
    int count;
    int * locations;
    Type * types;
} Command_list;




void line_in(char * line) {
    int size = 20, pozicio = 0;
    char karakter;
    line = (char*) malloc(size * sizeof(char));
    while(scanf("%c", &karakter) == 1) {
        if(karakter == '\n' && pozicio < size) {
            line[pozicio] = '\0';
            break;
        } else if(karakter == '\n') {
            size += 1;
            line = (char*) realloc(line, size * sizeof(char));
            line[pozicio] = '\0';
        } else if(pozicio < size) {
            line[pozicio] = karakter;
            ++pozicio;
        } else {
            size += 10;
            line = (char*) realloc(line, size * sizeof(char));
            line[pozicio] = karakter;
            ++pozicio;
        }
    }
}




void line_to_word(char const * line, Word_list * words) {
    words->size = 0;
    words->pointer = (char**) malloc(words->size * sizeof(char*));

    int letter_count = 1;
    char * word = (char*) malloc(letter_count * sizeof(char));
    word[0] = '\0';

    int i = 0;
    while(line[i] != '\0') {
        if(!isspace(line[i])) {
            letter_count += 1;
            word = (char*) realloc(word, letter_count * sizeof(char));
            word[letter_count - 2] = line[i];
            word[letter_count - 1] = '\0';
            ++i;
        } else {
            if(word[0] != '\0') {
                words->size += 1;
                words->pointer = (char**) realloc(words->pointer, words->size * sizeof(char*));
                words->pointer[words->size - 1] = word;
                word = (char*) malloc(sizeof(char));
                word[0] = '\0';
                letter_count = 1;
            }
            ++i;
        }
    }

    if(word[0] != '\0') {
        words->size += 1;
        words->pointer = (char**) realloc(words->pointer, words->size * sizeof(char*));
        words->pointer[words->size - 1] = word;
    }
}




void proc_list(Word_list * word_list, Command_list * result) {
    int command_count = 1;
    result->types = (Type*) malloc(command_count * sizeof(Type*));
    result->locations = (int*) malloc(command_count * sizeof(int));
    result->locations[0] = 1;
    result->types[0] = program;
    for(int i = 0; i < word_list->size; ++i) {
        if(word_list->pointer[i] == "|" && i != 0) {
            word_list->pointer[i] = NULL;
            command_count += 1;
            result->types = (Type*) realloc(result->types, command_count * sizeof(Type));
            result->types[command_count - 1] = program;
            result->locations = (int*) realloc(result->locations, command_count * sizeof(int));
            result->locations[command_count - 1] = i;
        } else if(word_list->pointer[i] == ">" && i != 0) {
            word_list->pointer[i] = NULL;
            command_count += 1;
            result->types = (Type*) realloc(result->types, command_count * sizeof(Type));
            result->types[command_count - 1] = file_out;
            result->locations = (int*) realloc(result->locations, command_count * sizeof(int));
            result->locations[command_count - 1] = i;
        } else if(word_list->pointer[i] == "<" && i != 0) {
            word_list->pointer[i] = NULL;
            command_count += 1;
            result->types = (Type*) realloc(result->types, command_count * sizeof(Type));
            result->types[command_count - 1] = file_in;
            result->locations = (int*) realloc(result->locations, command_count * sizeof(int));
            result->locations[command_count - 1] = i;
        }
    }
    result->count = command_count;
}




int exec_input(Word_list words, Command_list command_list) {
    int * pipefds;
    pipefds = (int*) malloc(command_list.count * 2 * sizeof(int));
    for(int z = 0; z < command_list.count * 2; z += 2) { //This 'for' pipes every second pipefd
        pipe(pipefds + z);
    }
    int rank;
    rank = 0;
    pid_t parentid = getpid();
    for(int i = 1; i < (command_list.count + 1); ++i) { //This 'for' creates the forks, assigns them a rank
        if(getpid() == parentid) {
            fork();
            if(getpid() != parentid) {
                rank = i;
            }
        }
    }

    if(rank != 0) {
        if(command_list.types[rank - 1] == program) {
            if(rank == 1) { //Things for the rank 1 program
                for(int i = 1; i < command_list.count; ++i) { //This 'for' finds which later program or file_out is waiting for input
                    if(command_list.count != 1) {
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
            } else if(command_list.types[rank - 1] == file_in) { // <
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




void prompt(void) {
    char hostname[_SC_HOST_NAME_MAX];
    gethostname(hostname, _SC_HOST_NAME_MAX);
    printf("[%s@%s]:%s$ ", getenv("USER"), hostname, getenv("PWD"));
    char * line;
    line_in(line);
    Word_list words;
    if(strcmp(words.pointer[0], "quit") == 0) {
        exit(0);
    }
    line_to_word(line, &words);
    Command_list commands;
    proc_list(&words, &commands);
    exec_input(words, commands);
    
    prompt();
}


int main(void) {
    prompt(); //main just calls the prompt function

    return 0;
}
