//
//  view.h
//  project3
//
//  Created by School on 7/16/16.
//  Copyright (c) 2016 School. All rights reserved.
//

#ifndef __project3__view__
#define __project3__view__

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <ctype.h>
#include "controller.h"
#include "model.h"
#include <unistd.h>

void display_results();

void init_view(struct Args * args);

void destruct_view();

void *draw_thread(void *ptr);

#endif /* defined(__project3__view__) */
