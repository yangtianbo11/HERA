#if ! defined PROCESS_TIME_H
#define PROCESS_TIME_H 1

// this just gives a simpler interface to the system function

#include <time.h>   // for clock_t

extern long   process_time_ticks_per_second_l;
extern double process_time_ticks_per_second_d;

clock_t process_time();
// return the CPU time so far for this process (excluding "system" time)
// units are "ticks", convert to seconds by dividing by one of
// process_time_clocks_per_sec_*

double process_time_in_seconds();

#endif
