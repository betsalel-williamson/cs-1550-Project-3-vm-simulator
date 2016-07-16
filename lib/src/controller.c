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

typedef int milliseconds;

void init_controller(int i, char **pString) {
    // get from input:
    int count;
    int frame_count = 0;
    algorithm_option option = 0;
    milliseconds refresh_rate = -1;
    const char *tracefile_uri = NULL;

    // Display each command-line argument.
    print_debug(("\nCommand-line arguments:\n"));
    for (count = 0; count < i; count++) {
        print_debug(("  argv[%d]   %s\n", count, pString[count]));

        if (strcmp(pString[count], "-n") == 0) {
            count++;
            frame_count = (int) pString[count];
        } else if (strcmp(pString[count], "-a") == 0) {
            count++;
            if (strcmp(pString[count], OPT_STRING) == 0) {
                option = OPT;
            } else if (strcmp(pString[count], CLOCK_STRING) == 0) {
                option = CLOCK;
            } else if (strcmp(pString[count], AGING_STRING) == 0) {
                option = AGING;
            } else if (strcmp(pString[count], LRU_STRING) == 0) {
                option = LRU;
            }
        } else if (strcmp(pString[count], "-r") == 0) {
            count++;
            refresh_rate = (int) pString[count];
        } else if (count + 1 == i) {
            tracefile_uri = pString[count];
        }
    }

    struct singleton *instance = get_instance();

    instance->t = read_trace_file(tracefile_uri);
    instance->d->refresh_interval_ns = refresh_rate * 1000;
    instance->d->frame_count = frame_count;
    instance->o = option;
}

void destruct_controller() {
    destruct_model();
}

void insert_into_trace_tail_queue(unsigned int address, char mode);

struct trace_tail_queue *read_trace_file(const char *file_name) {
    /*	Initialize the queue. */
    TAILQ_INIT(&trace_tail_queue_head);

    print_debug(("Hello from inside read trace file\n"));

    unsigned int address;
    char mode;

    FILE *fp;

    if ((fp = fopen(file_name, "r")) == NULL) {
        print_debug(("No such file '%s'\n", file_name));
        exit(EXIT_FAILURE);
    }

    while (true) {
        int ret = fscanf(fp, "%x %c", &address, &mode);
        if (ret == 2) {
            insert_into_trace_tail_queue(address, mode);
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

page_replacement_algorithm select_page_replacement_algorithm(algorithm_option o) {
    page_replacement_algorithm f;

    switch (o) {
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
            print_debug(("Not using proper algorithm_option."));
            exit(EXIT_FAILURE);
    }

    return f;
}

// TODO: create function to  ask user for information

program_results perform_algorithm(algorithm_option o, struct trace_tail_queue *t) {
    page_replacement_algorithm f;
    f = select_page_replacement_algorithm(o);
    program_results result = f(t);
    return result;
}

usage_status get_usage_status(page p) {
    return p->reference_bit << 1 | p->modify_bit;
}

program_results optimal_page_replacement(struct trace_tail_queue *ttqp) {

    struct singleton *instance = get_instance();
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
    // while there is a tie for lowest number continue
    // when I have a single lowest number than that is the one to evict

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
