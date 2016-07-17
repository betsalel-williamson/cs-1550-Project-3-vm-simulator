//
//  view.h
//  project3
//
//  Created by School on 7/16/16.
//  Copyright (c) 2016 School. All rights reserved.
//



#ifndef MAIN_DRAW_H
#define MAIN_DRAW_H

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>

#include "controller.h"
#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif


void display_results();

typedef struct Args {
    int argc;
    char **argv;
} *args;

void init_view(args arguments);

void destruct_view();

void *draw_thread(void *ptr);


#ifdef __cplusplus
}
#endif


#endif /* defined(__project3__view__) */
