#include <stdio.h>

#define NUM_FRAMES   3
#define PAGE_SIZE    3
#define REF_LEN      12

typedef struct {
    int page_num;                  /* which page currently occupies frame, -1 = empty */
    unsigned char data[PAGE_SIZE]; /* 3-byte payload for the page (demo data) */
    int load_time;                 /* logical "time" the page was loaded, for FIFO */
} Frame;

int reference_string[REF_LEN] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};

void fill_dummy_data(unsigned char *buf, int page_num) {
    buf[0] = (unsigned char)(page_num);
    buf[1] = (unsigned char)(page_num * 2);
    buf[2] = (unsigned char)(page_num * 3);
}

void run_fifo(void) {
    Frame frames[NUM_FRAMES];
    for (int i = 0; i < NUM_FRAMES; i++) frames[i].page_num = -1;
    int next_victim = 0; /* FIFO pointer: index of the oldest-loaded frame */
    int faults = 0, hits = 0;

    printf("--- FIFO Page Replacement (frames=%d, page size=%dB) ---\n",
           NUM_FRAMES, PAGE_SIZE);

    for (int t = 0; t < REF_LEN; t++) {
        int page = reference_string[t];
        int hit = 0;
        for (int i = 0; i < NUM_FRAMES; i++) {
            if (frames[i].page_num == page) { hit = 1; break; }
        }
        if (hit) {
            hits++;
            printf("t=%2d ref=%d : HIT   frames=[", t, page);
        } else {
            faults++;
            frames[next_victim].page_num = page;
            fill_dummy_data(frames[next_victim].data, page);
            frames[next_victim].load_time = t;
            next_victim = (next_victim + 1) % NUM_FRAMES; /* advance FIFO ptr */
            printf("t=%2d ref=%d : FAULT frames=[", t, page);
        }
        for (int i = 0; i < NUM_FRAMES; i++)
            printf("%d ", frames[i].page_num);
        printf("]\n");
    }
    printf("FIFO Result -> Hits=%d Faults=%d Hit Ratio=%.2f%% Miss Ratio=%.2f%%\n",
           hits, faults, 100.0 * hits / REF_LEN, 100.0 * faults / REF_LEN);
}

int main(void) {
    printf("Reference string (array): [ ");
    for (int i = 0; i < REF_LEN; i++) printf("%d ", reference_string[i]);
    printf("]\n\n");

    run_fifo();
    return 0;
}
