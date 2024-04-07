#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>
#include "progress_monitor.h"

void test_single_bar(void) {
    unsigned long int N = 50007;
    int bar_len = TERMINAL_LEN - 20;
    for (int i = 0; i <= N; i++) {
        update_bar(i, N, bar_len);
    }
    printf("\n");
}

void test_thread_bars(void) {
    // Determine the number of steps in the for loop
    unsigned long int N = 50007;
    // Get number of threads 
    int n_threads = get_num_threads();
    // Determine bar length
    int bar_len = TERMINAL_LEN - 22;
    // Declare structure that hold information about each thread and initialize it
    thread_info thread[n_threads];
    initialize_thread_info_struct(n_threads, &thread);
    // Declare shared variable that holds approximately the size of each chunk of steps
    int chunk_size = N/n_threads;
    // Declare shared variable that flags if the calculations are done
    bool all_done = false;
    // Parallel loop
    # pragma omp parallel default(shared) 
    {   
        // Get and assign thread ID to structure
        int thread_ID = omp_get_thread_num();
        #pragma omp critical
        {
            thread[thread_ID].ID = thread_ID;
        }
        // Define a counter for each thread to monitor the progress
        int j = 0;
        // Divide the steps evenly (or almost evenly) into n_threads for loops
        #pragma omp for schedule(static)
        for (int i = 0; i < N; i++) {
            // Increase progress
            j++;
            thread[thread_ID].progress = ((double) j)/((double) chunk_size) * 100;
            // Update bar (each thread at a time)
            #pragma omp critical
            {
                update_thread_bars(thread, n_threads, bar_len, &all_done);
            }
        }
    }
}

int main(void) {
    printf("SINGLE PROGRESS MONITOR TEST:\n");
    test_single_bar();
    printf("PROGRESS MONITOR FOR EACH THREAD:\n");
    test_thread_bars();
}