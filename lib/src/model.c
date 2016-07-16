//
// Created by School on 7/3/16.
//
#include "model.h"

struct singleton *get_instance() {
    static struct singleton *instance = NULL;

    if (instance == NULL) {

        // get map for struct
        instance = (struct singleton *) malloc(sizeof(struct singleton));

        // get map for disk
        instance->d = (disk) malloc(sizeof(struct Disk));
    }

    return instance;
}


void destruct_instance() {
    // TODO find way to ensure that I'm not calling destruct when my instance hasn't been created yet? Not sure that is a big concern
    struct singleton *instance = get_instance();

    // unmap memory
    // need to destruct disk
    free(instance->d);
    free(instance);
}

// the model is. it requires clean-up before exiting, but the get instance is the init model
void init_model() {
    get_instance();
}

void destruct_model() {
    destruct_instance();
    destruct_trace_tail_queue();
}


void insert_into_trace_tail_queue(unsigned int address, char mode) {
    /*	Insert at the tail. */
    trace_tail_queue_entry = malloc(sizeof(struct Trace_tail_queue_entry));
    trace_tail_queue_entry->t = malloc(sizeof(struct Trace));
    trace_tail_queue_entry->t->address = address;
    trace_tail_queue_entry->t->mode = mode;
    TAILQ_INSERT_TAIL(&trace_tail_queue_head, trace_tail_queue_entry, entries);
}

void destruct_trace_tail_queue() {
    struct Trace_tail_queue_entry *n1 = TAILQ_FIRST(&trace_tail_queue_head), *n2;
    while (n1 != NULL) {
        n2 = TAILQ_NEXT(n1, entries);
        free(n1->t);
        free(n1);
        n1 = n2;
    }
}







