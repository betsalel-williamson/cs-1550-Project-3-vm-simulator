//
// Created by School on 7/2/16.
//

#ifndef MAIN_TRAFFIC_RULES_H
#define MAIN_TRAFFIC_RULES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "view.h"
#include "model.h"

struct trace_tail_queue *read_trace_file(const char *file_name);

usage_status get_usage_status(page p);

void init_controller();

void destruct_controller();

program_results optimal_page_replacement(struct trace_tail_queue *ttqp);
program_results least_recently_used_algorithm(struct trace_tail_queue *ttqp);
program_results enhanced_second_chance_algorithm(struct trace_tail_queue *ttqp);
program_results second_chance_page_replacement_algorithm(struct trace_tail_queue *ttqp);

#ifdef __cplusplus
}
#endif

#endif //MAIN_TRAFFIC_RULES_H
