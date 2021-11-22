#include <errno.h>
#include "prompt.h"
//#include "debugmalloc.h"

void main(void) {
    //system("clear");
    int prompt_return_value = 0;

    while(prompt_return_value == 0) {
    prompt_return_value = prompt(); //main just calls the prompt function
    }

}
