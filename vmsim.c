////
//// Created by Betsalel Williamson on 6/4/16.
////

//#include <sys/param.h>
#include <stdlib.h>
#include <signal.h>
#include "view.h"

/**
 * Handle quit to correctly exit and restore state on CTR-C.
 */
void sig_handler(int signo) {

    destruct_view();

    if (signo == SIGINT) {
        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
}

//./vmsim –n <numframes> -a <opt|clock|aging|lru> [-r <refresh>] <tracefile>

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    init_view(argc, argv);

    destruct_view();

    return 0;
}