//
// Created by Centauria V. CHEN on 2021/5/8.
//

#ifndef PHOENIXKILLER_NV_TIME_H
#define PHOENIXKILLER_NV_TIME_H

#include <time.h>

#ifdef CLOCK_MONOTONIC_RAW
#define NVTOP_CLOCK CLOCK_MONOTONIC_RAW
#else
#define NVTOP_CLOCK CLOCK_MONOTONIC
#endif

typedef struct timespec nvtop_time;

static inline void nvtop_get_current_time(nvtop_time *time) {
    clock_gettime(NVTOP_CLOCK, time);
}

static inline double nvtop_difftime(nvtop_time t0, nvtop_time t1) {
    double secdiff = difftime(t1.tv_sec, t0.tv_sec);
    if (t1.tv_nsec < t0.tv_nsec) {
        long val = 1000000000l - t0.tv_nsec + t1.tv_nsec;
        secdiff += (double) val / 1e9 - 1.;
    } else {
        long val = t1.tv_nsec - t0.tv_nsec;
        secdiff += (double) val / 1e9;
    }
    return secdiff;
}

static inline nvtop_time nvtop_hmns_to_time(unsigned hour, unsigned minutes,
                              unsigned long nanosec) {
    nvtop_time t = {(__time_t) (hour * 60 * 60 + 60 * minutes + nanosec / 1000000),
                    (__syscall_slong_t) (nanosec % 1000000)};
    return t;
}

static inline nvtop_time nvtop_substract_time(nvtop_time t0, nvtop_time t1) {
    nvtop_time t =
            t0.tv_nsec - t1.tv_nsec < 0
            ? (nvtop_time) {t0.tv_sec - t1.tv_sec - 1,
                            t0.tv_nsec - t1.tv_nsec + 1000000}
            : (nvtop_time) {t0.tv_sec - t1.tv_sec, t0.tv_nsec - t1.tv_nsec};
    return t;
}

static inline nvtop_time nvtop_add_time(nvtop_time t0, nvtop_time t1) {
    nvtop_time t =
            t0.tv_nsec + t1.tv_nsec > 1000000
            ? (nvtop_time) {t0.tv_sec + t1.tv_sec + 1,
                            t0.tv_nsec + t1.tv_nsec - 1000000}
            : (nvtop_time) {t0.tv_sec + t1.tv_sec, t0.tv_nsec + t1.tv_nsec};
    return t;
}

#endif //PHOENIXKILLER_NV_TIME_H
