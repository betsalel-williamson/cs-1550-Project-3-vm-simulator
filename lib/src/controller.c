//
// Created by School on 7/2/16.
//
#include <sys/errno.h>
#include <stdbool.h>
#include "controller.h"


void init_controller() {
    init_model();
    init_view();
}

void destruct_controller() {
    destruct_model();
    destruct_view();
}

struct stailhead *read_trace_file(const char *file_name) {
    STAILQ_HEAD(stailhead, trace_tail_queue) head	=
            STAILQ_HEAD_INITIALIZER(head);
    struct stailhead *headp = &head;		     /*	Singly-linked tail queue head. */
    STAILQ_INIT(headp);		     /*	Initialize the queue. */

    printf("Hello from inside read trace file\n");

    unsigned int address;
    char mode;

    FILE *fp;

    if ((fp = fopen(file_name, "r")) == NULL) {
        printf("No such file '%s'\n", file_name);
        exit(1);
    }

    while (true) {
        int ret = fscanf(fp, "%x %c", &address, &mode);
        if(ret == 2){
            n1	= malloc(sizeof(struct trace_tail_queue));	     /*	Insert at the tail. */
            n1->t = malloc(sizeof(struct Trace));
            n1->t->address = address;
            n1->t->mode = mode;
            STAILQ_INSERT_TAIL(headp, n1, entries);
        }
        else if(errno != 0) {
            perror("scanf:");
            break;
        } else if(ret == EOF) {
            break;
        } else {
            printf("No match.\n");
        }
    }

    return NULL;
}




