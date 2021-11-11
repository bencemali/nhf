#include "../headers/split.h"

typedef struct Word_list {
    int num;
    char **pointer;
} Word_list;

void split(char * string, Word_list * result) {
    int cnt = 1;
    for(int i = 0; string[i] != '\0'; ++i) {
        if(str[i] == ' ') {
            ++cnt;
        }
    }

    char ** array = (char**) malloc(cnt * sizeof(char*));

    int from = 0;
    int how_many = 0;

    for(int i = 0; string[from] != '\0'; ++i) {
        if(from != 0) {
            ++from;
        }
        mennyit = 0;
        while(string[from + how_many] != ' ' && string[from + how_many] != '\0') {
            ++how_many;
        }
        array[i] = (char*) malloc((how_many + 1) * sizeof(char));
        strncpy(array[i], string + from, how_many);
        array[i][how_many] = '\0';
        from += how_many;
    }

    result->num = cnt;
    result-pointer = array;
}