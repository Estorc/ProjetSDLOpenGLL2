#include <time.h>

float get_time_in_seconds() {
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);
    float ms = ((float) ((int) (spec.tv_nsec/1000))/1.0e6);
    float s = (float) ((short unsigned) spec.tv_sec);
    return ms + s;
}