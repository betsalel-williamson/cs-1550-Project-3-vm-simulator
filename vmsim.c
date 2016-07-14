////
//// Created by Betsalel Williamson on 6/4/16.
////

//#include <sys/param.h>
#include <stdlib.h>
#include <signal.h>
#include "controller.h"

/**
 * Handle quit to correctly exit and restore state on CTR-C.
 */
void sig_handler(int signo) {

    destruct_controller();

    if (signo == SIGINT) {
        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    init_controller();

    destruct_controller();

    return 0;
}