//
//  optimal_page_replacement.c
//  project3
//
//  Created by School on 7/16/16.
//  Copyright (c) 2016 School. All rights reserved.
//

#include "optimal_page_replacement.h"

int mincount(struct Page_table_entry *frames[]){
    int min = frames[0]->reference_bit;
    int min_count = 1;
    
    singleton instance = get_instance();
    
    int j;
    for (j = 1; j < instance->d->frame_count; j++) {
        // for each item in frame increment if I see myself again
        if (frames[j]->reference_bit < min) {
            min = frames[j]->reference_bit;
            min_count = 1;
        } else if (frames[j]->reference_bit == min){
            min_count++;
        }
    }

    return min_count;
}

int sort(const void *x, const void *y) {
    
    int result;
    struct Page_table_entry *orderA = (struct Page_table_entry *)x;
    struct Page_table_entry *orderB = (struct Page_table_entry *)y;
    
    unsigned int a = orderA->next_reference;
    unsigned int b = orderB->next_reference;
    
    if (a == b) {
        result = 0;
    } else if (a>b) {
        result = -1;
    } else {
        result = 1;
    }
    
    return result;
}

void optimal_page_replacement() {
    
    print_debug(("Inside optimal_page_replacement\n"));
    
    singleton instance = get_instance();
    //
    // create page_frames;
    // this bit array failed. to use frames the whole way
//        unsigned int A[134217727];     // This is our bit array (429496730/10 = 4294967300 bits !)
//        int i;
//        for ( i = 0; i < 134217727; i++ ){
//            A[i] = 0;                    // Clear the bit array
//        }
    //
    
    // need to preprocess to include the next occurance of the item in the linked list
    
    struct Page_table_entry *frames[instance->d->frame_count];
    int k;
    for (k = 0; k < instance->d->frame_count; k++) {
        frames[k] = NULL;
    }
    
    // replace the page that will not be used for the longest period of time
    
    // algorithm implementation 1
    // go from back to front
    // for each Page_table_entry
    // go from current entry to back until I see myself again
    // the worst case is that I'm not used again and I go from front of queue to back of queue
    // mark next use
    
    // algorithm implementation 2
    unsigned int add;
    char m;
    
    // go from front to back
    TAILQ_FOREACH(trace_tail_queue_entry, instance->t, entries) {
        
        add = trace_tail_queue_entry->address;
        m = trace_tail_queue_entry->mode;
        
        int j;
        bool in_frame = false;
        for (j = 0; j < instance->d->frame_count; j++) {
            // for each item in frame increment if I see myself again
            
            if (frames[j] == NULL){
                frames[j] = calloc(1, sizeof(struct Page_table_entry));
                frames[j]->address = add;
                frames[j]->modify_bit = -1;
                frames[j]->reference_bit = 1; // I'm in use
                frames[j]->next_reference = trace_tail_queue_entry->next_reference; // I'm in use
                
                // TODO: add to page faults
                instance->d->fault_count++;
                if (m == 'W') {
//                    printf("%c", trace_tail_queue_entry->t->mode);
                    instance->d->write_count++;
                } else {
                    instance->d->read_count++;
                }
                in_frame = true;
                break;
            } else if (frames[j]->address == add) {
                // I'm already in here and there's nothing left to do...
                frames[j]->next_reference = trace_tail_queue_entry->next_reference; // I'm in use
                in_frame = true;
                break;
            }
        }
        
        if (!in_frame) {

//            printf("Add %8.8x Next ref %u\n", trace_tail_queue_entry->address , trace_tail_queue_entry->next_reference);
            
            qsort(*frames, instance->d->frame_count, sizeof(struct Page_table_entry), sort);

//            for (j = 0; j < instance->d->frame_count; j++) {
//                // for each item in frame increment if I see myself again
//                
//                printf("[%d] Add %8.8x Next ref %u\n", j, frames[j]->address , frames[j]->next_reference);
//            }

            // the largest to smallest
            // when I have a single lowest number than that is the one to evict
            frames[0]->address = add;
            frames[0]->modify_bit = -1;
            frames[0]->reference_bit = 1; // I'm in use
            frames[0]->next_reference = trace_tail_queue_entry->next_reference; // I'm in use
            
            // TODO: add to page faults
            instance->d->fault_count++;
            if (m == 'W') {
                //                    printf("%c", trace_tail_queue_entry->t->mode);
                instance->d->write_count++;
            } else {
                instance->d->read_count++;
            }
//            in_frame = true;
        }
        
        instance->d->access_count++;
    }
    
    int j;
    for (j = 0; j < instance->d->frame_count; j++) {
        // for each item in frame increment if I see myself again
        
        if (frames[j] != NULL){
            free(frames[j]);
            frames[j] = NULL;
        }
    }
}