#ifndef _RAND_H
#define _RAND_H

/* Thanks C standard for da free code! */

static unsigned long int next = 1;

#define RAND_MAX 32768

int rand() {
    next = next * 1103515245 + 12345;
    return (unsigned int) (next / 65536) % RAND_MAX;
}

void srand(unsigned int seed) {
    next = seed;
}

int rand_uniform(int bound) {
  return rand() % bound;
}

#endif
