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

#ifndef MAIN_MODEL_H
#define MAIN_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#ifdef DEBUG
#define print_debug(s) printf s
#else
#define print_debug(s) do {} while (0)
#endif

#include <sys/file.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/queue.h>

#define MIN_ADDRESS 0
#define MAX_ADDRESS 0xFFFFFFFF
// note max address in unsigned 32-bit integer

typedef int algorithm_option;
#define OPT 0
#define CLOCK 1
#define AGING 2
#define LRU 3

#define OPT_STRING "opt"
#define CLOCK_STRING "clock"
#define AGING_STRING "aging"
#define LRU_STRING "lru"
// a frame is the same thing as a page table entry, it is just an array or list of page table entries
//typedef struct Frame {
//
//};


typedef struct Trace {
    unsigned int address;
    char mode;
} *trace;

struct Trace_tail_queue_entry {
    trace t;
    TAILQ_ENTRY(Trace_tail_queue_entry) entries;
} *trace_tail_queue_entry;

TAILQ_HEAD(trace_tail_queue, Trace_tail_queue_entry) trace_tail_queue_head;

void insert_into_trace_tail_queue(unsigned int address, char mode);

void destruct_trace_tail_queue();

typedef struct Page_table_entry {
    unsigned int address;
//    long time_of_use; // could be used to store exact time-stamp information
//    int valid_bit; // not used in this program
    int reference_bit;
    // note that this is a single bit in enhanced_second_chance_algorithm
    // in second_chance_page_replacement_algorithm this is variable, to be 8-bit in this project
    // optimal_page_replacement and least_recently_used_algorithm don't use reference_bit information
    int modify_bit;
} *page;

struct Page_tail_queue_entry {
    page p;
    TAILQ_ENTRY(Page_tail_queue_entry) entries;
} *page_tail_queue_entry;

TAILQ_HEAD(page_tail_queue_entry, Page_tail_queue_entry) page_tail_queue_head;


struct Page_circle_queue_entry {
    page p;
    TAILQ_ENTRY(Page_circle_queue_entry) entries;
} *page_circle_queue_entry;

CIRCLEQ_HEAD(page_circle_queue_entry, Page_circle_queue_entry) page_circle_queue_head;

typedef struct Disk {
    int frame_count;
//    page *frames; // need to malloc(sizeof(Page_table_entry)*frame_count) based on user input
// the frame structure will be specific to each implementation of the algorithm
    long write_count;
    long read_count;
    long access_count; // == reads+writes
    long hit_count;
    int fault_count;
    long refresh_interval_ns; // refresh interval in nano seconds
} *disk;

typedef struct Program_results {
    algorithm_option a;
    long elapsed_time;
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

struct singleton {
    disk d;
    struct trace_tail_queue *t;
    algorithm_option o;
};

void init_model();

void destruct_model();

struct singleton *get_instance();

void destruct_instance();

typedef program_results (*page_replacement_algorithm)(struct trace_tail_queue *ttqp);

page_replacement_algorithm select_page_replacement_algorithm(algorithm_option o);

#ifdef __cplusplus
}
#endif

#endif //MAIN_MODEL_H
