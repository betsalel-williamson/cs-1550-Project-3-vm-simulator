/*
 * MIT License
 *
 * Copyright (c) 2016 Betsalel "Saul" Williamson
 *
 * contact: saul.williamson@pitt.edu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "model.h"

const char *algorithmStrings[] = {
        OPT_STRING,
        CLOCK_STRING,
        AGING_STRING,
        LRU_STRING
};

pthread_mutex_t instance_mutex = PTHREAD_MUTEX_INITIALIZER;;


singleton get_instance() {
    //    print_debug(("In get instance\n"));
    static singleton instance = NULL;

    pthread_mutex_lock (&instance_mutex);

    if (instance == NULL) {

        // get map for struct
        instance = (singleton) calloc(1, sizeof(struct Singleton));

        //        print_debug(("Calloc instance\n"));

        // get map for disk
        instance->d = (disk) calloc(1, sizeof(struct Disk));

        instance->d->access_count = 0;
        instance->d->fault_count = 0;
        instance->d->frame_count = 0;
        instance->d->hit_count = 0;
        instance->d->read_count = 0;
        instance->d->refresh_interval_ms = 100; // default value
        instance->d->write_count = 0;

        instance->p = (program_results) calloc(1, sizeof(struct Program_results));
        instance->p->d = instance->d;
        instance->p->a = &instance->o;

        instance->lines_read = 0;

        instance->completed = false;
    } else {
        //        print_debug(("Trying to access instance\n"));
    }
    pthread_mutex_unlock (&instance_mutex);

    return instance;
}


void destruct_instance() {
    // TODO find way to ensure that I'm not calling destruct when my instance hasn't been created yet? Not sure that is a big concern
    singleton instance = get_instance();

    // unmap memory
    // need to destruct disk
    free(instance->p);
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

void destruct_trace_tail_queue() {
    struct Trace_tail_queue_entry *n1 = TAILQ_FIRST(&trace_tail_queue_head), *n2;
    while (n1 != NULL) {
        n2 = TAILQ_NEXT(n1, entries);
        //        free(n1->t);
        free(n1);
        n1 = n2;
    }
}







