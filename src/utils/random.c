#include <time.h>
#include <stdlib.h>
#include <unistd.h>

void init_random() {
    srand(time(NULL) + getpid());
}

float random_float(float min, float max) {
    return min + (float)(rand()) / (float)(RAND_MAX / (max - min));
}

int random_int(int min, int max) {
    return min + (rand() % (max - min + 1));
}