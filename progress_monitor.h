#ifndef PROGRESS_MONITOR_H
#define PROGRESS_MONITOR_H

#include <stdbool.h>

#define TERMINAL_LEN 79

#define RESET_STYLE  "\x1b[0m"
#define T_LRED       "\x1b[91m"
#define T_LGREEN     "\x1b[92m"
#define T_LYELLOW    "\x1b[93m"
#define T_LBLUE      "\x1b[94m"
#define T_LMAGENTA   "\x1b[95m"

#define ESC          "\033"
#define CSI          "["
#define PREVIOUSLINE "F"
#define BACKSPACE    "D"

typedef struct {
    double progress;
    int ID;
    bool done;
} thread_info;

void update_bar(int actual_step, int n_steps, int bar_length);
int get_num_threads(void);
void update_thread_bars(thread_info *t_info, int num_threads, int bar_length, bool *all_done);
void initialize_thread_info_struct(int n_threads, thread_info (*t_info)[n_threads]);

#endif