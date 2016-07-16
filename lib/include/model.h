//
// Created by School on 7/3/16.
//

#ifndef MAIN_MODEL_H
#define MAIN_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/file.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/queue.h>

typedef struct Trace {
    unsigned int address;
    char mode;
} *trace;

struct trace_tail_queue {
    trace t;
    STAILQ_ENTRY(trace_tail_queue) entries;    /* Tail queue. */
} *n1, *n2, *n3, *np;

//STAILQ_HEAD(stailhead, trace_tail_queue) head =
//        STAILQ_HEAD_INITIALIZER(head);

void init_model();

void destruct_model();

#ifdef __cplusplus
}
#endif

#endif //MAIN_MODEL_H
