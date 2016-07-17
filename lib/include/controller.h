//
//  controller.h
//  project3
//
//  Created by School on 7/16/16.
//  Copyright (c) 2016 School. All rights reserved.
//

#include <stdio.h>
#include <sys/errno.h>
#include <stdbool.h>
#include <string.h>
#include "radix-tree.h"
#include "model.h"
#include "view.h"

#ifndef __project3__controller__
#define __project3__controller__

#ifdef __cplusplus
extern "C" {
#endif


void init_controller(int i, char **pString);

void destruct_controller();

struct trace_tail_queue *read_trace_file(const char *file_name);

//typedef int algorithm_option;

//typedef void (*page_replacement_algorithm)();
//
//page_replacement_algorithm select_page_replacement_algorithm(algorithm_option o);

#ifdef __cplusplus
}
#endif

#endif /* defined(__project3__controller__) */
