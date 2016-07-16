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

struct trace_tail_queue *read_trace_file(const char *file_name) {
    /*	Initialize the queue. */
    TAILQ_INIT(&trace_tail_queue_head);

    print_debug(("Hello from inside read trace file\n"));

    unsigned int address;
    char mode;

    FILE *fp;

    if ((fp = fopen(file_name, "r")) == NULL) {
        printf("No such file '%s'\n", file_name);
        exit(1);
    }

    while (true) {
        int ret = fscanf(fp, "%x %c", &address, &mode);
        if (ret == 2) {
            /*	Insert at the tail. */
            trace_tail_queue_entry = malloc(sizeof(struct Trace_tail_queue_entry));
            trace_tail_queue_entry->t = malloc(sizeof(struct Trace));
            trace_tail_queue_entry->t->address = address;
            trace_tail_queue_entry->t->mode = mode;
            TAILQ_INSERT_TAIL(&trace_tail_queue_head, trace_tail_queue_entry, entries);
        }
        else if (errno != 0) {
            perror("scanf:");
            break;
        } else if (ret == EOF) {
            break;
        } else {
            printf("No match.\n");
        }
    }

    return &trace_tail_queue_head;
}

page_replacement_algorithm select_page_replacement_algorithm(option o) {
    page_replacement_algorithm f;

    switch (o){
        case OPT:
            f = optimal_page_replacement;
            break;
        case CLOCK:
            f = enhanced_second_chance_algorithm;
            break;
        case AGING:
            f = second_chance_page_replacement_algorithm;
            break;
        case LRU:
            f = least_recently_used_algorithm;
            break;
        default:
            print_debug(("Not using proper option."));
            exit(EXIT_FAILURE);
    }

    return f;
}

// ask user for information

program_results perform_algorithm(option o, struct trace_tail_queue *t ){
    page_replacement_algorithm f;
    f = select_page_replacement_algorithm(o);
    program_results result = f(t);
    return result;
}

usage_status get_usage_status(page p) {
    return p->reference_bit << 1 | p->modify_bit;
}

program_results optimal_page_replacement(struct trace_tail_queue *ttqp) {

    // replace the page that will not be used for the longest period of time

    // algorithm implementation 1
    // go from back to front
    // for each Page_table_entry
    // go from current entry to back until I see myself again
    // the worst case is that I'm not used again and I go from front of queue to back of queue
    // mark next use

    // algorithm implementation 2
    // go from front to back
    // if room in frames add myself
    // otherwise
    // go through trace list
    // for each item in frame increment if I see myself again
    //

    return 0;
}

program_results least_recently_used_algorithm(struct trace_tail_queue *ttqp) {
    return 0;
}

program_results enhanced_second_chance_algorithm(struct trace_tail_queue *ttqp) {
    return 0;
}

program_results second_chance_page_replacement_algorithm(struct trace_tail_queue *ttqp) {
    return 0;
}
