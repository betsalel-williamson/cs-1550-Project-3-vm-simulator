//
//  enhanced_second_chance_algorithm.c
//  project3
//
//  Created by School on 7/16/16.
//  Copyright (c) 2016 School. All rights reserved.
//

#include "enhanced_second_chance_algorithm.h"

usage_status get_usage_status(struct Page_circle_queue_entry *p) {
    return p->reference_bit << 1 | p->modify_bit;
}

void enhanced_second_chance_algorithm() {
    print_debug(("Inside enhanced_second_chance_algorithm\n"));
    
    singleton instance = get_instance();
    
    // keep doubly linked stack for frames
    int number_of_items_in_frames = 0;
    CIRCLEQ_INIT(&page_circle_queue_head);
    
    TAILQ_FOREACH(trace_tail_queue_entry, instance->t, entries) {
        
        // look for item in queue
        struct Page_circle_queue_entry *p, *removed;
        
        if (number_of_items_in_frames == instance->d->frame_count){
            
            bool item_inserted = false;
            CIRCLEQ_FOREACH(p, &page_circle_queue_head, entries) {
                
                // if I hit
                if (p->address == trace_tail_queue_entry->address){
                    p->reference_bit = 1;
                    if (trace_tail_queue_entry->mode == 'W') {
                        //                    printf("%c", trace_tail_queue_entry->t->mode);
                        instance->d->write_count++;
                        p->modify_bit = 1;
                    } else {
                        instance->d->read_count++;
                        p->modify_bit = 0;
                    }
                    instance->d->hit_count++;
                    break;
                }
                
                // if I don't hit evaluate whether to remove item
                usage_status status = get_usage_status(p);
                
                // this needs to be done in a loop with the refresh rate
                switch (status) {
                    case NOT_RECENTLY_USED_AND_NOT_MODIFIED:
                        // evict
                        removed = p;
                        CIRCLEQ_REMOVE(&page_circle_queue_head, p, entries);
                        free(removed);
                        
                        p = malloc(sizeof(struct Page_circle_queue_entry));
                        p->address = trace_tail_queue_entry->address;
                        p->reference_bit = 1;
                        if (trace_tail_queue_entry->mode == 'W') {
                            //                    printf("%c", trace_tail_queue_entry->t->mode);
                            instance->d->write_count++;
                            p->modify_bit = 1;
                        } else {
                            instance->d->read_count++;
                            p->modify_bit = 0;
                        }
                        
                        instance->d->fault_count++;
                        CIRCLEQ_INSERT_HEAD(&page_circle_queue_head, p, entries);
                        
                        item_inserted = true;
                        break;
                    case NOT_RECENTLY_USED_AND_MODIFIED:
                        // make this not used
                        p->reference_bit = 0;
                        break;
                    case RECENTLY_USED_AND_NOT_MODIFIED:
                        // evict
                        p->reference_bit = 0;
                        break;
                    case RECENTLY_USED_AND_MODIFIED:
                        // wait until the
                        p->reference_bit = 0;
                        break;
                    default:
                        print_debug(("Modify and reference bits not set correctly inside enhanced algo"));
                        exit(EXIT_FAILURE);
                        break;
                }
                
                if (item_inserted) {
                    break;
                }
            }
        } else {
            number_of_items_in_frames++;
            
            p = malloc(sizeof(struct Page_circle_queue_entry));
            
            p->address = trace_tail_queue_entry->address;
            p->reference_bit = 1;
            if (trace_tail_queue_entry->mode == 'W') {
                //                    printf("%c", trace_tail_queue_entry->t->mode);
                instance->d->write_count++;
                p->modify_bit = 1;
            } else {
                instance->d->read_count++;
                p->modify_bit = 0;
            }
            
            instance->d->fault_count++;
            CIRCLEQ_INSERT_HEAD(&page_circle_queue_head, p, entries);
        }
        
        instance->d->access_count++;
    }
}
