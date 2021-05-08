//
// Created by Centauria V. CHEN on 2021/5/8.
//

#ifndef PHOENIXKILLER_GET_PROCESS_INFO_H
#define PHOENIXKILLER_GET_PROCESS_INFO_H

#include "nv_time.h"

#include <stdlib.h>
#include <stdbool.h>

struct process_cpu_usage {
    double total_user_time;   // Seconds
    double total_kernel_time; // Seconds
    size_t virtual_memory;    // Bytes
    size_t resident_memory;   // Bytes
    nvtop_time time;
};

void get_username_from_pid(pid_t pid, char **buffer);

void get_command_from_pid(pid_t pid, char **buffer);

bool get_process_info(pid_t pid, struct process_cpu_usage *usage);

#endif //PHOENIXKILLER_GET_PROCESS_INFO_H
