//
//  enhanced_second_chance_algorithm.h
//  project3
//
//  Created by School on 7/16/16.
//  Copyright (c) 2016 School. All rights reserved.
//

#ifndef __project3__enhanced_second_chance_algorithm__
#define __project3__enhanced_second_chance_algorithm__

#include <stdio.h>
#include "model.h"
#include "view.h"
#include "controller.h"

#ifdef __cplusplus
extern "C" {
#endif

void enhanced_second_chance_algorithm();

typedef int usage_status;

usage_status get_usage_status(struct Page_circle_queue_entry *p);

#ifdef __cplusplus
}
#endif

#endif /* defined(__project3__enhanced_second_chance_algorithm__) */
