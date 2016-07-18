//
//  controller.h
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

#ifndef __project3__controller__
#define __project3__controller__

#include <stdio.h>
#include <sys/errno.h>
#include <stdbool.h>
#include <string.h>
#include "radix-tree.h"
#include "model.h"
#include "view.h"

void init_controller(int i, char **pString);

void *controller_thread(void *ptr);

void destruct_controller();

struct trace_tail_queue *read_trace_file(const char *file_name);

typedef int algorithm_option;
#define OPT 0
#define CLOCK 1
#define AGING 2
#define LRU 3

typedef void (*page_replacement_algorithm)();

page_replacement_algorithm select_page_replacement_algorithm(algorithm_option o);

typedef void *(*refresh_rate_algorithm)(void * ptr);

refresh_rate_algorithm select_refresh_rate_algorithm(algorithm_option o);

#endif /* defined(__project3__controller__) */
