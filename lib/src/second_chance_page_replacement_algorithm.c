//
//  second_chance_page_replacement_algorithm.c
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

#include "second_chance_page_replacement_algorithm.h"

static page *frames = NULL;

void shift_frame_bits() {
    singleton instance = get_instance();
    int j;
    for (j = 0; j < instance->d->frame_count; j++) {
        if (frames == NULL || frames[j] == 0 || frames[j]->address == EMPTY_ADDRESS) {
            break;
        } else {
//            printf("[%d] Add %8.8x ref bit %4.4x\n", j, frames[j]->address , frames[j]->reference_bit);
            frames[j]->reference_bit = frames[j]->reference_bit >> 0b01;
            frames[j]->reference_bit &= 0b11111111;
//            printf("[%d] Add %8.8x ref bit %4.4x\n", j, frames[j]->address , frames[j]->reference_bit);
        }
    }
}

void *second_chance_refresh_rate_algorithm(void *ptr) {

    singleton instance = get_instance();

    while (!instance->completed) {

        sleep_ms(instance->d->refresh_interval_ms);
        shift_frame_bits();
    }

    pthread_exit((void *) 0);
}

// we want to remove the top item and we store things from small to large
int sort_reference_bit(const void *x, const void *y) {
    int result;
    struct Page_table_entry **pointer_pointer_A = (struct Page_table_entry **) x;
    struct Page_table_entry **pointer_pointer_B = (struct Page_table_entry **) y;

    struct Page_table_entry *pointer_A = *pointer_pointer_A;
    struct Page_table_entry *pointer_B = *pointer_pointer_B;

    int a = pointer_A->reference_bit;
    int b = pointer_B->reference_bit;

    if (a == b) {
        result = 0;
    } else if (a < b) {
        result = -1;
    } else {
        result = 1;
    }

    return result;

}

void second_chance_page_replacement_algorithm() {
    print_debug(("Inside second_chance_page_replacement_algorithm\n"));

    singleton instance = get_instance();

    frames = malloc(sizeof(page) * instance->d->frame_count);

    int k;
    for (k = 0; k < instance->d->frame_count; k++) {
        frames[k] = malloc(sizeof(struct Page_table_entry));
        frames[k]->address = (unsigned int) EMPTY_ADDRESS;
        frames[k]->modify_bit = -1;
        frames[k]->next_reference = (unsigned int) -1;
        frames[k]->reference_bit = -1;
    }

    // replace the page that will not be used for the longest period of time

    // go from front to back
    TAILQ_FOREACH(trace_tail_queue_entry, instance->t, entries) {

        int j;
        bool in_frame = false;
        for (j = 0; j < instance->d->frame_count; j++) {
            // for each item in frame increment if I see myself again

            if (frames[j]->address == EMPTY_ADDRESS) {
                frames[j] = calloc(1, sizeof(struct Page_table_entry));
                frames[j]->address = trace_tail_queue_entry->address;
                frames[j]->reference_bit = 0b10000000; // I'm in use

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
                frames[j]->reference_bit |= 0b10000000; // I'm in use

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

//            printf("Add %8.8x Next ref %u\n", trace_tail_queue_entry->address , trace_tail_queue_entry->next_reference);

            // the largest to smallest
            qsort(frames, (size_t) instance->d->frame_count, sizeof(struct Page_table_entry *), sort_reference_bit);

//            int i;
//            for (i = 0; i < instance->d->frame_count; i++) {
//                // for each item in frame increment if I see myself again
//
//                printf("[%d] pos: %u %4.4u\n", i, frames[i]->address , frames[i]->reference_bit);
//            }

            // when I have a the largest number than that is the one to evict
            frames[0]->address = trace_tail_queue_entry->address;
            frames[0]->reference_bit = 0b10000000; // I'm in use

            instance->d->fault_count++;
            if (trace_tail_queue_entry->mode == 'W') {
                instance->d->write_count++;
            } else {
                instance->d->read_count++;
            }
        }

        instance->d->access_count++;
    }

    int j;

    for (j = 0; j < instance->d->frame_count; j++) {
        if (frames[j] != NULL) {
            free(frames[j]);
            frames[j] = NULL;
        }
    }
}