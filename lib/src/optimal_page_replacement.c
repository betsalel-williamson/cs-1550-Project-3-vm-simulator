//
//  optimal_page_replacement.c
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

#include "optimal_page_replacement.h"

int sort_next_reference(const void *x, const void *y) {

    int result;
    struct Page_table_entry **orderA = (struct Page_table_entry **)x;
    struct Page_table_entry **orderB = (struct Page_table_entry **)y;

    struct Page_table_entry * oa = *orderA;
    struct Page_table_entry * ob = *orderB;

    unsigned int a = oa->next_reference;
    unsigned int b = ob->next_reference;

    if (a == b) {
        result = 0;
    } else if (a > b) {
        result = -1;
    } else {
        result = 1;
    }

    return result;
}

static page *frames;

void optimal_page_replacement() {

    print_debug(("Inside optimal_page_replacement\n"));

    singleton instance = get_instance();

    frames = malloc(sizeof(page)*instance->d->frame_count);

    int k;
    for (k = 0; k < instance->d->frame_count; k++) {
        frames[k] = malloc(sizeof(struct Page_table_entry));
        frames[k]->address = (unsigned int) EMPTY_ADDRESS;
        frames[k]->modify_bit = -1;
        frames[k]->next_reference = (unsigned int) -1;
        frames[k]->reference_bit = -1;
    }

    // replace the page that will not be used for the longest period of time

    // algorithm implementation 1
    // go from back to front
    // for each Page_table_entry
    // go from current entry to back until I see myself again
    // the worst case is that I'm not used again and I go from front of queue to back of queue
    // mark next use

    // algorithm implementation 2
    // go from front to back
    TAILQ_FOREACH(trace_tail_queue_entry, instance->t, entries) {

//        printf("Add %8.8x Next ref %u\n", trace_tail_queue_entry->address , trace_tail_queue_entry->next_reference);

        int j;
        bool in_frame = false;
        for (j = 0; j < instance->d->frame_count; j++) {
            // for each item in frame increment if I see myself again

            if (frames[j]->address == EMPTY_ADDRESS){
                frames[j]->address = trace_tail_queue_entry->address;
                frames[j]->next_reference = trace_tail_queue_entry->next_reference; // I'm in use
                frames[j]->reference_bit = trace_tail_queue_entry->position;

                instance->d->fault_count++;
                if (trace_tail_queue_entry->mode == 'W') {
                    instance->d->write_count++;
                } else {
                    instance->d->read_count++;
                }
                in_frame = true;
                break;
            } else if (frames[j]->address == trace_tail_queue_entry->address) {
                // I'm already in here and there's nothing left to do...

                frames[j]->next_reference = trace_tail_queue_entry->next_reference;
                if (trace_tail_queue_entry->mode == 'W') {
                    instance->d->write_count++;
                } else {
                    instance->d->read_count++;
                }
                instance->d->hit_count++;
                in_frame = true;
                break;
            }
        }

        if (!in_frame) {

            qsort(frames, (size_t) instance->d->frame_count, sizeof(struct Page_table_entry *), sort_next_reference);
//#ifdef _DEBUG
//            int i;
//            for (i = 0; i < instance->d->frame_count; i++) {
//                // for each item in frame increment if I see myself again
//
//                printf("[%d] pos: %u Add %8.8x Next ref %u\n", frames[i]->reference_bit, i, frames[i]->address , frames[i]->next_reference);
//            }
//#endif

            // the largest to smallest
            // when I have a single highest number that is the one to evict
            frames[0]->address = trace_tail_queue_entry->address;
            frames[0]->next_reference = trace_tail_queue_entry->next_reference; // I'm in use
            frames[0]->reference_bit = trace_tail_queue_entry->position;

            instance->d->fault_count++;
            if (trace_tail_queue_entry->mode == 'W') {
                instance->d->write_count++;
            } else {
                instance->d->read_count++;
            }
        }

        // each next reference needs to decrease after the next step
        int l;
        for (l = 0; l < instance->d->frame_count; l++) {
//        next frame will never have a distance of zero to cause underflow
//            if (frames[l]->next_reference == 0){
//        printf("Add %8.8x Next ref %u\n", trace_tail_queue_entry->address , trace_tail_queue_entry->next_reference);
//            }
            assert(frames[l] != 0);

            frames[l]->next_reference--;
        }

        instance->d->access_count++;
    }

    int j;
    for (j = 0; j < instance->d->frame_count; j++) {
        if (frames[j] != NULL){
            free(frames[j]);
            frames[j] = NULL;
        }
    }
}