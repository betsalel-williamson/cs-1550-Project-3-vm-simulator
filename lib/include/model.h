//
//  model.h
//  project3
//
//  Created by Betsalel "Saul" Williamson on 7/16/16.
//  Copyright (c) 2016 Betsalel "Saul" Williamson. All rights reserved.
//

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

#ifndef __project3__model__
#define __project3__model__

#include <stdio.h>
#include <sys/mman.h>
#include <sys/file.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/queue.h>
#include "optimal_page_replacement.h"
#include "least_recently_used_algorithm.h"
#include "second_chance_page_replacement_algorithm.h"
#include "enhanced_second_chance_algorithm.h"

//#ifdef __cplusplus
//extern "C" {
//#endif

#ifndef _DEBUG
#define _DEBUG
#endif

#ifdef _DEBUG
#define print_debug(s) printf s
#else
#define print_debug(s) do {} while (0)
#endif

#define MIN_ADDRESS 0
#define MAX_ADDRESS 0xFFFFFFFF
// note max address in unsigned 32-bit integer

typedef int algorithm_option;
#define OPT 0
#define CLOCK 1
#define AGING 2
#define LRU 3

#define OPT_ARG_STRING "opt"
#define CLOCK_ARG_STRING "clock"
#define AGING_ARG_STRING "aging"
#define LRU_ARG_STRING "lru"

#define OPT_STRING "Opt: Optimal page replacement"
#define CLOCK_STRING "Clock: Enhanced second chance algorithm"
#define AGING_STRING "Aging: Second chance page replacement algorithm"
#define LRU_STRING "LRU: Least recently used algorithm"

extern const char *algorithmStrings[];

// a frame is the same thing as a page table entry, it is just an array or list of page table entries
//typedef struct Frame {
//
//};

typedef int milliseconds;

//typedef struct Trace {
//    unsigned int address;
//    char mode;
//} *my_trace;

struct Trace_tail_queue_entry {
    struct Trace_tail_queue_entry * t;
    unsigned int address;
    char mode;
    unsigned int next_reference;
    unsigned int position;
    TAILQ_ENTRY(Trace_tail_queue_entry) entries;
} *trace_tail_queue_entry;

TAILQ_HEAD(trace_tail_queue, Trace_tail_queue_entry) trace_tail_queue_head, *radix_tail_queue_head;

#define	TAILQ_FOREACH_FROM(var, start, field)					\
for ((var) = (start);				\
(var);							\
(var) = TAILQ_NEXT((var), field))

void insert_into_trace_tail_queue(struct trace_tail_queue *head, unsigned int address, char mode, unsigned int position);

void destruct_trace_tail_queue();

// I'm currently trying to get around the fact of having a contiguous memory block for my disk of size int

typedef struct Page_table_entry {
    unsigned int address;
    unsigned int next_reference;

    //    long time_of_use; // could be used to store exact time-stamp information
    //    int valid_bit; // not used in this program
    int reference_bit;
    // note that this is a single bit in enhanced_second_chance_algorithm
    // in second_chance_page_replacement_algorithm this is variable, to be 8-bit in this project
    // optimal_page_replacement and least_recently_used_algorithm don't use reference_bit information
    int modify_bit;
    // note that this is a single bit in enhanced_second_chance_algorithm
    // the other algorithms will not use this
} *page;


struct Page_tail_queue_entry {
//    page p;
    unsigned int address;

    //    long time_of_use; // could be used to store exact time-stamp information
    //    int valid_bit; // not used in this program
    int reference_bit;
    // note that this is a single bit in enhanced_second_chance_algorithm
    // in second_chance_page_replacement_algorithm this is variable, to be 8-bit in this project
    // optimal_page_replacement and least_recently_used_algorithm don't use reference_bit information
    int modify_bit;
    // note that this is a single bit in enhanced_second_chance_algorithm
    // the other algorithms will not use this
//    SQ

    TAILQ_ENTRY(Page_tail_queue_entry) entries;
} *page_tail_queue_entry;

TAILQ_HEAD(page_tail_queue_entry, Page_tail_queue_entry) page_tail_queue_head;


struct Page_circle_queue_entry {
    unsigned int address;
    //    long time_of_use; // could be used to store exact time-stamp information
    //    int valid_bit; // not used in this program
    int reference_bit;
    // note that this is a single bit in enhanced_second_chance_algorithm
    // in second_chance_page_replacement_algorithm this is variable, to be 8-bit in this project
    // optimal_page_replacement and least_recently_used_algorithm don't use reference_bit information
    int modify_bit;
    // note that this is a single bit in enhanced_second_chance_algorithm
    // the other algorithms will not use this
    //    SQ
    CIRCLEQ_ENTRY(Page_circle_queue_entry) entries;
} *page_circle_queue_entry;

CIRCLEQ_HEAD(page_circle_queue_entry, Page_circle_queue_entry) page_circle_queue_head;

typedef struct Disk {
    int frame_count;
    //    page *frames; // need to malloc(sizeof(Page_table_entry)*frame_count) based on user input
    // the frame structure will be specific to each implementation of the algorithm
    long write_count;
    long read_count;
    long access_count; // == reads+writes
//    long page_fault_no_eviction; couldn't understand from prompt how to use these
//    long page_fault_evict_clean;
//    long page_fault_evict_dirty;
    long hit_count;
    long fault_count;
    long refresh_interval_ms; // refresh interval in milli seconds
} *disk;

typedef struct Program_results {
    algorithm_option * a;
    disk d;
} *program_results;

typedef int usage_status;
// 11 recently used and modified, probably will be used again soon and the page will need to be written out to disk before it can be replaced
// 10 recently used, but clean, probably will be used again soon
// 01 not recently use dbut modified, not quite as good because the page will need to be written out before replacement
// 00 neither recently used nor modified and is the best page to replace
#define RECENTLY_USED_AND_MODIFIED 3
#define RECENTLY_USED_AND_NOT_MODIFIED 2
#define NOT_RECENTLY_USED_AND_MODIFIED 1
#define NOT_RECENTLY_USED_AND_NOT_MODIFIED 0

extern pthread_mutex_t mutexsum;

void init_model();

void destruct_model();

void destruct_instance();

typedef struct Singleton {
    disk d;
    struct trace_tail_queue *t;
    algorithm_option o;
    program_results p;
    unsigned int lines_read;
    bool completed;
} *singleton;

singleton get_instance();

#define EMPTY_ADDRESS -1

//#ifdef __cplusplus
//}
//#endif

#endif /* defined(__project3__model__) */
