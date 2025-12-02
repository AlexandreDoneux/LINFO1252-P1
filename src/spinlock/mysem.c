#include "mysem.h"

void mysem_init(mysem_t *s, int value)
{
    s->value = value;
    s->lock = 0;
}

void mysem_wait(mysem_t *s)
{
    while (1) {
        lock(&s->lock);
        if (s->value > 0) {
            s->value--;
            unlock(&s->lock);
            return;
        }
        unlock(&s->lock);
    }
}

void mysem_post(mysem_t *s)
{
    lock(&s->lock);
    s->value++;
    unlock(&s->lock);
}
