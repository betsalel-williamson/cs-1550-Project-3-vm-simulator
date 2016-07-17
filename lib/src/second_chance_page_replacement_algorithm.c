//
//  second_chance_page_replacement_algorithm.c
//  project3
//
//  Created by School on 7/16/16.
//  Copyright (c) 2016 School. All rights reserved.
//

#include "second_chance_page_replacement_algorithm.h"

// we want to remove the top item and we store things from small to large
int sort_reference_bit(const void *x, const void *y) {
    int result;
    struct Page_table_entry **pointer_pointer_A = (struct Page_table_entry **)x;
    struct Page_table_entry **pointer_pointer_B = (struct Page_table_entry **)y;
    
    struct Page_table_entry * pointer_A = *pointer_pointer_A;
    struct Page_table_entry * pointer_B = *pointer_pointer_B;
    
    unsigned int a = pointer_A->reference_bit;
    unsigned int b = pointer_B->reference_bit;
    
    if (a == b) {
        result = 0;
    } else if (a > b) {
        result = -1;
    } else {
        result = 1;
    }
    
    return result;
    
    //    int result;
    //    struct Page_table_entry *pointer_pointer_A = (struct Page_table_entry *)x;
    //    struct Page_table_entry *pointer_pointer_B = (struct Page_table_entry *)y;
    //
    //    unsigned int a = pointer_pointer_A->reference_bit;
    //    unsigned int b = pointer_pointer_B->reference_bit;
    //
    //    if (a == b) {
    //        result = 0;
    //    } else if (a > b) {
    //        result = 1;
    //    } else {
    //        result = -1;
    //    }
    //
    //    return result;
}

void shift_frame_bits(struct Page_table_entry *array[]){
    singleton instance = get_instance();
    int j;
    for (j = 0; j < instance->d->frame_count; j++) {
        if (array[j]) { // it is not null
            //            printf("[%d] Add %8.8x ref bit %4.4x\n", j, frames[j]->address , frames[j]->reference_bit);
            array[j]->reference_bit = array[j]->reference_bit >> 0b01;
            array[j]->reference_bit &= 0b11111111;
            //            printf("[%d] Add %8.8x ref bit %4.4x\n", j, frames[j]->address , frames[j]->reference_bit);
        } else {
            //            break;
        }
    }
}

static page *frames;

void second_chance_page_replacement_algorithm() {
    print_debug(("Inside second_chance_page_replacement_algorithm\n"));
    
    singleton instance = get_instance();
    
    frames = malloc(sizeof(page)*instance->d->frame_count);
    
    int k;
    for (k = 0; k < instance->d->frame_count; k++) {
        frames[k] = malloc(sizeof(struct Page_table_entry));
        frames[k]->address = -1;
        frames[k]->modify_bit = -1;
        frames[k]->next_reference = -1;
        frames[k]->reference_bit = -1;
    }
    
    // replace the page that will not be used for the longest period of time
    
    // go from front to back
    TAILQ_FOREACH(trace_tail_queue_entry, instance->t, entries) {
        
        
        int j;
        bool in_frame = false;
        for (j = 0; j < instance->d->frame_count; j++) {
            // for each item in frame increment if I see myself again
            
            if (frames[j]->address == EMPTY){
                frames[j] = calloc(1, sizeof(struct Page_table_entry));
                frames[j]->address = trace_tail_queue_entry->address;
                frames[j]->reference_bit = 0b10000000; // I'm in use
                
                // TODO: add to page faults
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
            
            //                        printf("Add %8.8x Next ref %u\n", trace_tail_queue_entry->address , trace_tail_queue_entry->next_reference);
            
            // the largest to smallest
            qsort(frames, instance->d->frame_count, sizeof(struct Page_table_entry *), sort_reference_bit);
            
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

        // this needs to be done in a loop with the refresh rate
        shift_frame_bits(frames);
        
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