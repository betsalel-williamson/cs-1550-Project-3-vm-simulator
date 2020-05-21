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

#include "controller.h"


// not thread safe
pthread_mutex_t controller_mutex = PTHREAD_MUTEX_INITIALIZER;

void *controller_thread(void *ptr) {
    pthread_mutex_lock(&controller_mutex);

    struct Args *args = (struct Args *) ptr;

    init_controller(args->argc, args->argv);

    singleton instance = get_instance();

    instance->completed = true;

    pthread_mutex_unlock(&controller_mutex);

    pthread_exit((void *) 0);
}

RADIX_TREE(my_Tree);

void perform_algorithm(algorithm_option o) {
    page_replacement_algorithm f;
    f = select_page_replacement_algorithm(o);
    f();
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

void init_controller(int argc, char **argv) {

    singleton instance = get_instance();

    // Display each command-line argument.
//    print_debug(("\nCommand-line arguments:\n"));
    int count;
    for (count = 0; count < argc; count++) {
//        print_debug(("  argv[%d]   %s\n", count, argv[count]));

        if (strcmp(argv[count], "-n") == 0) {
            count++;
            instance->d->frame_count = atoi(argv[count]);
        } else if (strcmp(argv[count], "-a") == 0) {
            count++;
            if (strcmp(argv[count], OPT_ARG_STRING) == 0) {
                instance->o = OPT;
            } else if (strcmp(argv[count], CLOCK_ARG_STRING) == 0) {
                instance->o = CLOCK;
            } else if (strcmp(argv[count], AGING_ARG_STRING) == 0) {
                instance->o = AGING;
            } else if (strcmp(argv[count], LRU_ARG_STRING) == 0) {
                instance->o = LRU;
            }
        } else if (strcmp(argv[count], "-r") == 0) {
            count++;
            instance->d->refresh_interval_ms = atoi(argv[count]);

            // spawn refresh rate thread here

            pthread_t refresh_rate_thread;

            int iret1;

            iret1 = pthread_create(&refresh_rate_thread, NULL, select_refresh_rate_algorithm(instance->o), NULL);

            if (iret1) {
                fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
                exit(EXIT_FAILURE);
            }

        } else if (count + 1 == argc) {

            // error if file extenstion is wrong
            const char *filename_ext = get_filename_ext(argv[count]);
            if (strcmp(filename_ext, "trace") != 0) {
                print_debug(("This is not a trace file: '%s'\n", argv[count]));
                exit(EXIT_FAILURE);
            }

            instance->t = read_trace_file(argv[count]);
        }
    }

    perform_algorithm(instance->o);
}

void destruct_controller() {
    destruct_model();
}

void insert_into_trace_tail_queue(struct trace_tail_queue *head, unsigned int address, char mode,
                                  unsigned int position) {
    /*	Insert at the tail. */
    struct Trace_tail_queue_entry *t;

    t = malloc(sizeof(struct Trace_tail_queue_entry));
    //    trace_tail_queue_entry->t = malloc(sizeof(struct Trace));
    t->address = address;
    t->mode = mode;
    t->next_reference = 0xffffffff;
    t->position = position;

    singleton instance = get_instance();

    if (instance->o == OPT) {

        struct trace_tail_queue *radix_tail_queue_head_p = radix_tree_lookup(&my_Tree, t->address);

        if (radix_tail_queue_head_p == NULL) {
            radix_tail_queue_head_p = malloc(sizeof(trace_tail_queue_head));
            TAILQ_INIT(radix_tail_queue_head_p);
            struct Trace_tail_queue_entry *p1;
            p1 = malloc(sizeof(struct Trace_tail_queue_entry));
            p1->position = t->position;
            p1->address = t->address;
            p1->next_reference = t->next_reference;
            p1->t = t;

            TAILQ_INSERT_HEAD(radix_tail_queue_head_p, p1, entries);
            radix_tree_insert(&my_Tree, t->address, radix_tail_queue_head_p);
        } else {
            // TODO: find way to improve temp variable replacement when not overworked.
            struct Trace_tail_queue_entry *p1, *p2;
            p1 = TAILQ_FIRST(radix_tail_queue_head_p);
            p2 = malloc(sizeof(struct Trace_tail_queue_entry));

            p2->position = t->position;
            p2->address = t->address;
            p2->next_reference = t->next_reference;
            p2->t = t;

            p1->next_reference = p2->position - p1->position;
            p1->t->next_reference = p2->position - p1->position;

            TAILQ_INSERT_HEAD(radix_tail_queue_head_p, p2, entries);

            TAILQ_REMOVE(radix_tail_queue_head_p, p1, entries);
            free(p1);
            p1 = NULL;
        }
    }

    TAILQ_INSERT_TAIL(head, t, entries);
}

struct trace_tail_queue *read_trace_file(const char *file_name) {
    print_debug(("Hello from inside read trace file\n"));

    TAILQ_INIT(&trace_tail_queue_head);

    unsigned int address;
    char mode;

    FILE *fp;
    if ((fp = fopen(file_name, "r")) == NULL) {
        print_debug(("No such file '%s'\n", file_name));
        exit(EXIT_FAILURE);
    }

    singleton instance = get_instance();
    instance->lines_read = 0;
    while (true) {
        int ret = fscanf(fp, "%x %c", &address, &mode);
        if (ret == 2) {
            unsigned int position = instance->lines_read + 1;
            insert_into_trace_tail_queue(&trace_tail_queue_head, address, mode, position);
            instance->lines_read++;
        }
        else if (errno != 0) {
            perror("scanf:");
            break;
        } else if (ret == EOF) {
            break;
        } else {
            print_debug(("No match.\n"));
        }
    }

    return &trace_tail_queue_head;
}
