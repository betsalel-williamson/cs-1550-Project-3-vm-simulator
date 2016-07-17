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

typedef struct Args {
    int argc; char **argv;
} *args;

void init_view(args arguments);

void destruct_view();

void *draw_thread(void *ptr);

#endif /* defined(__project3__view__) */
