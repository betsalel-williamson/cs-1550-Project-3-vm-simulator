//
//  least_recently_used_algorithm.c
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

#include "least_recently_used_algorithm.h"

void least_recently_used_algorithm() {
    print_debug(("Inside least_recently_used_algorithm\n"));

    singleton instance = get_instance();

    // keep doubly linked stack for frames
    int number_of_items_in_frames = 0;
    TAILQ_INIT(&page_tail_queue_head);

    TAILQ_FOREACH(trace_tail_queue_entry, instance->t, entries) {

        // look for item in queue
        bool item_found = false;
        struct Page_tail_queue_entry *p, *removed;
        TAILQ_FOREACH(p, &page_tail_queue_head, entries) {
            // when accessed move to the tail of stack
            if (p->address == trace_tail_queue_entry->address) {
                // if found
                //    TAILQ_SWAP
                item_found = true;
                TAILQ_REMOVE(&page_tail_queue_head, p, entries);
                TAILQ_INSERT_TAIL(&page_tail_queue_head, p, entries);
                if (trace_tail_queue_entry->mode == 'W') {
                    //                    printf("%c", trace_tail_queue_entry->t->mode);
                    instance->d->write_count++;
                } else {
                    instance->d->read_count++;
                }
                instance->d->hit_count++;
                break;
            }
        }

        if (!item_found) {
            // if not
            // when full remove from tail of stack
            // remove head
            if (number_of_items_in_frames == instance->d->frame_count) {
                removed = TAILQ_FIRST(&page_tail_queue_head);
                TAILQ_REMOVE(&page_tail_queue_head, TAILQ_FIRST(&page_tail_queue_head), entries);
                free(removed);
            } else {
                number_of_items_in_frames++;
            }

            // insert into tail
            p = malloc(sizeof(struct Page_tail_queue_entry));
            p->address = trace_tail_queue_entry->address;
            TAILQ_INSERT_TAIL(&page_tail_queue_head, p, entries);

            instance->d->fault_count++;
            if (trace_tail_queue_entry->mode == 'W') {
                //                    printf("%c", trace_tail_queue_entry->t->mode);
                instance->d->write_count++;
            } else {
                instance->d->read_count++;
            }
        }

        instance->d->access_count++;
    }
}