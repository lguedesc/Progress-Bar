/*  
--------------------------------------------------------------------------------------------------------------
This library offers functions to implement progress bars in both single-threaded and multi-threaded command-
line applications, providing visual feedback during long-running tasks.

Author: Luã G Costa [https://github.com/lguedesc]
Created: 07 Apr 2024
Last Update: 28 Jan 2025 
--------------------------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "progress_monitor.h"

/* Simple progress bar for one thread applications */

static void print_progress_bar(double perc, int bar_length) {
    /*
    ------------------------------------------------------------------------
    This static (internal) function displays a single iteration of a 
    progress bar of a running single-threaded command-line application.
    ------------------------------------------------------------------------
    Description of function arguments:
    - perc:       Current percentage of process completed (0 - 100%)
    - bar_length: The length of the progress bar (number of characters)
    ------------------------------------------------------------------------
    */
    // Filled Part of the progress bar
    int fill = (perc * bar_length) / 100;  
    printf("\rProgress: [");
    for(int i = 0; i < fill; i++) {
        if (perc < 33) {
            printf(T_LRED);
        }
        else if ((perc >= 33) && (perc < 66)) {
            printf(T_LYELLOW);
        }
        else if (perc >= 66 && (perc < 100)) {
            printf(T_LGREEN);
        }
        else if (perc == 100) {
            printf(T_LBLUE);
        }
        else {
            printf(RESET_STYLE);
        }
        printf("#");
        printf(RESET_STYLE);
    }
    // Unfilled part of the progress bar
    for (int i = 0; i < bar_length - fill; i++) {
        printf(" ");
    }
    if (perc > 100) {
        perc = 100;
    }
    printf("] %5.1lf %%", perc);
    fflush(stdout);
}

void update_bar(int current_step, int n_steps, int bar_length) {
    /*
    ------------------------------------------------------------------------
    This function updates the progress bar of a running single-threaded 
    command-line application.
    ------------------------------------------------------------------------
    Description of function arguments:
    - current_step: Current step of the process
    - n_steps:      Total number of steps of the process
    - bar_length: The length of the progress bar (number of characters)
    ------------------------------------------------------------------------
    */
    // Compute percentage
    double perc = ((double)current_step)/((double)n_steps)*100;
    // Print bar
    print_progress_bar(perc, bar_length);
}

/* Progress bar to monitor the progress of each thread separately */

static void print_progress_bar_thread(thread_info *t_info, int bar_length) {
    /*
    ------------------------------------------------------------------------
    This static (internal) function displays a single iteration of a 
    progress bar of a running multi-threaded command-line application.
    ------------------------------------------------------------------------
    Description of function arguments:
    - t_info:     Structure containing the progress of the process, the ID
                  of the current utilized thread, and a flag variable to
                  inform if the process is finished or not
    - bar_length: The length of the progress bar (number of characters)
    ------------------------------------------------------------------------
    */
    // Filled Part of the progress bar
    int fill = (t_info->progress * bar_length) / 100;  
    printf("Thread %3d: [", t_info->ID);
    for(int i = 0; i < fill; i++) {
        if (t_info->progress < 33) {
            printf(T_LRED);
        }
        else if ((t_info->progress >= 33) && (t_info->progress < 66)) {
            printf(T_LYELLOW);
        }
        else if (t_info->progress >= 66 && (t_info->progress < 100)) {
            printf(T_LGREEN);
        }
        else if (t_info->progress == 100) {
            printf(T_LBLUE);
        }
        else {
            printf(RESET_STYLE);
        }
        printf("#");
        printf(RESET_STYLE);
    }
    // Unfilled part of the progress bar
    for (int i = 0; i < bar_length - fill; i++) {
        printf(" ");
    }
    if (t_info->progress > 100) {
        t_info->progress = 100;
    }
    printf("] %5.1lf %%\n", t_info->progress);
}

int get_num_threads(void) {
    /*
    ------------------------------------------------------------------------
    This function get the number of threads being used in the application
    ------------------------------------------------------------------------
    Returns:
    The number of threads being used in the application
    ------------------------------------------------------------------------
    */
    int n_threads = 0;
    // Check if OMP_NUM_THREADS was defines in terminal before
    char *n_threads_str = getenv("OMP_NUM_THREADS");
    // If null, get the maximum number of threads
    if (n_threads_str == NULL) {
        n_threads = omp_get_max_threads();    
    }
    // If not null, get the defined number of threads
    else {
        n_threads = atoi(n_threads_str);         
    }

    return n_threads;
}

void update_thread_bars(thread_info *t_info, int num_threads, int bar_length, bool *all_done) {
    /*
    ------------------------------------------------------------------------
    This function updates the progress bar of a running multi-threaded 
    command-line application.
    ------------------------------------------------------------------------
    Description of function arguments:
    - t_info:      Structure containing the progress of the process, the ID
                   of the current utilized thread, and a flag variable to
                   inform if the process is finished or not
    - num_threads: Number of threads used in the process
    - all done:    Flag to indicate if all processes are finished
    ------------------------------------------------------------------------
    */
    if ((*all_done) != true) {
        (*all_done) = true;
        for (int i = 0; i < num_threads; i++) {
            print_progress_bar_thread(&t_info[i], bar_length);
            if (t_info[i].progress >= 100) {
                t_info[i].done = true;
            }
            else {
                t_info[i].done = false;
            }
        }
        for (int i = 0; i < num_threads; i++) {
            if (t_info[i].done == false) {
                (*all_done) = false;
            }
        }
    }
    if ((*all_done) == false) {
        printf(ESC CSI "%d" PREVIOUSLINE, num_threads);
    }
}

void initialize_thread_info_struct(int n_threads, thread_info (*t_info)[n_threads]) {
    /*
    ------------------------------------------------------------------------
    This function initializes the thread_info data structure to monitor the
    progress of the process
    ------------------------------------------------------------------------
    Description of function arguments:
    - n_threads:   number of threads being used
    - t_info:      Structure containing the progress of the process, the ID
                   of the current utilized thread, and a flag variable to
                   inform if the process is finished or not
    ------------------------------------------------------------------------
    */
    for (int i = 0; i < n_threads; i++) {
        (*t_info)[i].ID = -1;
        (*t_info)[i].progress = 0.0;
    }
}