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

#include "view.h"


void *controller_thread(void *ptr){
    
    struct Args * args = (struct Args *) ptr;
    
    // not thread safe
    init_controller(args->argc, args->argv);
    
    return NULL;
}

void sleep_ms(long ms) {
    struct timespec request = {0};
    
    time_t sec = (int) (ms / 1000);
    ms = ms - (sec * 1000);
    
    request.tv_sec = sec;
    request.tv_nsec = ms * 1000000L;
    
    while (nanosleep(&request, &request) == -1 && errno == EINTR)
        continue;
}

int my_clock = 0;
void *increment_clock_thread(void *ptr) {
    
    while (true) {
        my_clock += 1;
        sleep_ms(1000);
    }
    
    //    return &my_clock;
}

void init_view(struct Args * args ) {
    
    pthread_t draw_pthread, controller_pthread, clock_thread;
    
    int iret1, iret2, iret6;
    
    iret1 = pthread_create(&draw_pthread, NULL, draw_thread, NULL);
    
    if (iret1) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
        exit(EXIT_FAILURE);
    }
    
    iret2 = pthread_create(&controller_pthread, NULL, controller_thread, (void *) args);
    
    if (iret2) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
        exit(EXIT_FAILURE);
    }
    
    iret6 = pthread_create(&clock_thread, NULL, increment_clock_thread, NULL);
    if (iret6) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }
    
    char mystring[100];
    bool quit = false;
    while (quit == false) {
        
        fgets(mystring, 100, stdin);
        
        
        if (toupper(mystring[0]) != 'Q') {
            quit = false;
        } else {
            quit = true;
        }
    }
    
}

void destruct_view() {
    destruct_controller();
}

void clear_screen() {
    const char msg[] = "\033[2J";
    syscall(4, STDOUT_FILENO, msg, sizeof(msg) - 1);
}



void draw() {
    clear_screen();
    
    display_results();
}

#define REFRESH_RATE 100

void *draw_thread(void *ptr) {
    while (true) {
        draw();
        sleep_ms(REFRESH_RATE);
    }
}



void display_results() {
    singleton instance =     get_instance();
    
    //    Algorithm: Clock
    //    Number of frames:       8
    //    Total memory accesses:  1000000
    //    Total page faults:      181856
    //    Total writes to disk:   29401
    
    printf("\n%-32s\n\n", algorithmStrings[instance->o]);
    printf("%-22s%10u\n", "Lines read:", instance->files_read);
    printf("%-22s%10d\n", "Number of frames:", instance->d->frame_count);
    printf("%-22s%10li\n", "Total memory accesses:", instance->d->access_count);
    printf("%-22s%10li\n", "Total page faults:", instance->d->fault_count);
    printf("%-22s%10li\n", "Total writes to disk:", instance->d->write_count);
    printf("\n%-27s%2.2d:%2.2d\n", "Time elapsed is: ", my_clock / 60, my_clock % 60);
    
}