//
//  least_recently_used_algorithm.c
//  project3
//
//  Created by School on 7/16/16.
//  Copyright (c) 2016 School. All rights reserved.
//

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
            if (p->address == trace_tail_queue_entry->address){
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
        
        if (!item_found){
            // if not
            // when full remove from tail of stack
            // remove head
            if (number_of_items_in_frames == instance->d->frame_count){
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
            // TODO: add to page faults
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