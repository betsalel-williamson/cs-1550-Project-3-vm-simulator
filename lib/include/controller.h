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

#ifndef MAIN_TRAFFIC_RULES_H
#define MAIN_TRAFFIC_RULES_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include <sys/errno.h>
#include <stdbool.h>
#include <string.h>
#include "model.h"

void init_controller(int i, char **pString);

void destruct_controller();

program_results optimal_page_replacement(struct trace_tail_queue *ttqp);
program_results least_recently_used_algorithm(struct trace_tail_queue *ttqp);
program_results enhanced_second_chance_algorithm(struct trace_tail_queue *ttqp);
program_results second_chance_page_replacement_algorithm(struct trace_tail_queue *ttqp);

struct trace_tail_queue *read_trace_file(const char *file_name);

usage_status get_usage_status(page p);

#ifdef __cplusplus
}
#endif

#endif //MAIN_TRAFFIC_RULES_H
