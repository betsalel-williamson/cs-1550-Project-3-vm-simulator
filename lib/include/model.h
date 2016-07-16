//
// Created by School on 7/3/16.
//

#ifndef MAIN_MODEL_H
#define MAIN_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

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

typedef int algorithm_id;
#define OPT 0
#define CLOCK 1
#define AGING 2
#define LRU 3

typedef struct Program_results{
    algorithm_id a;
    long elapsed_time;
    // disk
} *program_results;

// disk
//long frame_count;
//int refresh_interval;
//int access_count;
//int hit_count;
//int fault_count;
//int disk_write_count;

typedef struct Trace {
    unsigned int address;
    char mode;
} *trace;

typedef struct Page_table_entry {
    unsigned int address;
    long time_of_use;
    int valid_bit;
    int reference_bit;
    int modify_bit;
} *page;

struct Trace_tail_queue_entry {
    trace t;
    TAILQ_ENTRY(Trace_tail_queue_entry) entries;
} *trace_tail_queue_entry;

TAILQ_HEAD(trace_tail_queue, Trace_tail_queue_entry) trace_tail_queue_head;

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

typedef int usage_status;
// 11 recently used and modified, probably will be used again soon and the page will need to be written out to disk before it can be replaced
// 10 recently used, but clean, probably will be used again soon
// 01 not recently use dbut modified, not quite as good because the page will need to be written out before replacement
// 00 neither recently used nor modified and is the best page to replace
#define RECENTLY_USED_AND_MODIFIED 3
#define RECENTLY_USED_AND_NOT_MODIFIED 2
#define NOT_RECENTLY_USED_AND_MODIFIED 1
#define NOT_RECENTLY_USED_AND_NOT_MODIFIED 0

void init_model();

void destruct_model();

typedef int option;

typedef program_results (*page_replacement_algorithm)(struct trace_tail_queue *ttqp);

page_replacement_algorithm select_page_replacement_algorithm(option o);


#ifdef __cplusplus
}
#endif

#endif //MAIN_MODEL_H
