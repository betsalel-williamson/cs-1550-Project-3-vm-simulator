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

struct stailhead *read_trace_file(const char *file_name);

void init_controller();

void destruct_controller();

#ifdef __cplusplus
}
#endif

#endif //MAIN_TRAFFIC_RULES_H
