#ifndef MYSEM_H
#define MYSEM_H

#include "spinlock.h" 

typedef struct {
    spinlock_t lock;
    int value;
} mysem_t;

void mysem_init(mysem_t *s, int value);
void mysem_wait(mysem_t *s);
void mysem_post(mysem_t *s);

#endif