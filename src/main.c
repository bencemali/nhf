#include <errno.h>
#include "../include/prompt.h"
#include "../include/debugmalloc.h"

int main(void) {
    //system("clear");
    int prompt_return_value = 0;

    while(prompt_return_value == 0) {
    prompt_return_value = prompt(); //main just calls the prompt function
    }

    return 0;
}
