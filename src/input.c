#include "../headers/input.h"

void input(char * dinstring) {
    int size = 20, pozicio = 0;
    char karakter;
    dinstring = (char*) malloc(size * sizeof(char));
    while(scanf("%c", &karakter) == 1) {
        if(karakter == '\n' && pozicio < size) {
            dinstring[pozicio] = '\0';
            break;
        } else if(karakter == '\n') {
            size += 1;
            dinstring = (char*) realloc(dinstring, size * sizeof(char));
            dinstring[pozicio] = '\0';
        } else if(pozicio < size) {
            dinstring[pozicio] = karakter;
            ++pozicio;
        } else {
            size += 10;
            dinstring = (char*) realloc(dinstring, size * sizeof(char));
            dinstring[pozicio] = karakter;
            ++pozicio;
        }
    }
}