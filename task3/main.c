#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int page;
    unsigned char counter;
    int referenced;
} PageFrame;

void simulate_aging(int frames, int *page_refs, int ref_count) {
    PageFrame *memory = (PageFrame *)malloc(frames * sizeof(PageFrame));
    if (memory == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    int page_faults = 0;
    int i, j;
    
    for (i = 0; i < frames; i++) {
        memory[i].page = -1;
        memory[i].counter = 0;
        memory[i].referenced = 0;
    }
    
    for (i = 0; i < ref_count; i++) {
        int page = page_refs[i];
        int found = 0, empty = -1, replace_idx = -1;
        
        for (j = 0; j < frames; j++) {
            if (memory[j].page == page) {
                found = 1;
                memory[j].referenced = 1;
                break;
            }
            if (memory[j].page == -1 && empty == -1) {
                empty = j;
            }
        }
        
        if (!found) {
            page_faults++;
            if (empty != -1) {
                memory[empty].page = page;
                memory[empty].referenced = 1;
                memory[empty].counter = 0x80;
            } else {
                for (j = 0; j < frames; j++) {
                    if (replace_idx == -1 || memory[j].counter < memory[replace_idx].counter) {
                        replace_idx = j;
                    }
                }
                memory[replace_idx].page = page;
                memory[replace_idx].referenced = 1;
                memory[replace_idx].counter = 0x80;
            }
        }
        
        for (j = 0; j < frames; j++) {
            memory[j].counter = (memory[j].counter >> 1) | (memory[j].referenced ? 0x80 : 0);
            memory[j].referenced = 0;
        }
    }
    
    printf("Page Faults: %d, Page Faults per: 1000 refs: %.2f\n",
            page_faults, (page_faults / (ref_count / 1000.0)));
    
    free(memory);
}

int main(int argc, char *argv[]) {
    int frames = atoi(argv[2]);
    
    int *page_refs = (int *)malloc(100 * sizeof(int));
    if (page_refs == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    int ref_count = 0;
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        free(page_refs);
        return 1;
    }
    
    while (fscanf(file, "%d", &page_refs[ref_count]) == 1 && ref_count < 100) {
        ref_count++;
    }
    fclose(file);
    
    simulate_aging(frames, page_refs, ref_count);
    
    free(page_refs);
    return 0;
}

