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

void perform_algorithm(algorithm_option o) {
    page_replacement_algorithm f;
    f = select_page_replacement_algorithm(o);
    f();
}

void init_controller(int argc, char **argv) {
    // get from input:
    int count;
    for (count = 0; count < argc; count++) {
        print_debug(("  argv[%d]   %s\n", count, argv[count]));
    }
    
    int my_frame_count = 0;
    algorithm_option option = 0;
    milliseconds refresh_rate = -1;
    char *tracefile_uri = NULL;
    
    // Display each command-line argument.
    print_debug(("\nCommand-line arguments:\n"));
    for (count = 0; count < argc; count++) {
        print_debug(("  argv[%d]   %s\n", count, argv[count]));
        
        if (strcmp(argv[count], "-n") == 0) {
            count++;
            my_frame_count = atoi (argv[count]);
        } else if (strcmp(argv[count], "-a") == 0) {
            count++;
            if (strcmp(argv[count], OPT_ARG_STRING) == 0) {
                option = OPT;
            } else if (strcmp(argv[count], CLOCK_ARG_STRING) == 0) {
                option = CLOCK;
            } else if (strcmp(argv[count], AGING_ARG_STRING) == 0) {
                option = AGING;
            } else if (strcmp(argv[count], LRU_ARG_STRING) == 0) {
                option = LRU;
            }
        } else if (strcmp(argv[count], "-r") == 0) {
            count++;
            refresh_rate = atoi (argv[count]);
        } else if (count + 1 == argc) {
            
            tracefile_uri = argv[count];
        }
    }
    
    singleton instance = get_instance();
    
    instance->t = read_trace_file(tracefile_uri);
    instance->d->refresh_interval_ns = refresh_rate * 1000;
    instance->d->frame_count = my_frame_count;
    instance->o = option;
    
    perform_algorithm(instance->o);
    
    display_results();
}

void destruct_controller() {
    destruct_model();
}

//void insert_into_trace_tail_queue(struct trace_tail_queue *head, unsigned int address, char mode);

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

// need to optimize this code to store information about the distance between instructions.
struct trace_tail_queue *read_trace_file(const char *file_name) {
    print_debug(("Hello from inside read trace file\n"));
    
    const char *filename_ext = get_filename_ext(file_name);
    if(strcmp(filename_ext,"trace")!=0){
        print_debug(("This is not a trace file: '%s'\n", file_name));
        exit(EXIT_FAILURE);
    }
    
    /*	Initialize the queue. */
    TAILQ_INIT(&trace_tail_queue_head);
    
    unsigned int address;
    char mode;
    
    FILE *fp;
    if ((fp = fopen(file_name, "r")) == NULL) {
        print_debug(("No such file '%s'\n", file_name));
        exit(EXIT_FAILURE);
    }
    
    singleton instance =  get_instance();
    instance->files_read = 0;
    while (true) {
        int ret = fscanf(fp, "%x %c", &address, &mode);
        if (ret == 2) {
            insert_into_trace_tail_queue(&trace_tail_queue_head, address, mode, instance->files_read);
        }
        else if (errno != 0) {
            perror("scanf:");
            break;
        } else if (ret == EOF) {
            break;
        } else {
            print_debug(("No match.\n"));
        }
        
        instance->files_read++;
    }
    
    return &trace_tail_queue_head;
}

//usage_status get_usage_status(page p) {
//    return p->reference_bit << 1 | p->modify_bit;
//}
