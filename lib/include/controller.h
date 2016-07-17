//
//  controller.h
//  project3
//
//  Created by School on 7/16/16.
//  Copyright (c) 2016 School. All rights reserved.
//

#ifndef __project3__controller__
#define __project3__controller__

#include <stdio.h>
#include <sys/errno.h>
#include <stdbool.h>
#include <string.h>

#include "model.h"
#include "view.h"

void init_controller(int i, char **pString);

void destruct_controller();

struct trace_tail_queue *read_trace_file(const char *file_name);

typedef int algorithm_option;

//typedef void (*page_replacement_algorithm)();

//page_replacement_algorithm select_page_replacement_algorithm(algorithm_option o);

typedef int usage_status;

usage_status get_usage_status(struct Page_table_entry * p);

#endif /* defined(__project3__controller__) */
