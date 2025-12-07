/*

// semaphore using ttas spinlock -> plus lent que ttas directement dans mysem
// Avec cette version on un problème : on fait quand même des xchgl souvent. Car on utilise en effet un spinlock ttas mais une fois dans la section critique le sémaphore peut ne pas être disponible.
// On fait donc un unlock et on retente un lock tout de suite après. Ce cas de verrou disponible et sémaphore indisponible est relativement fréquent et provoque beaucoup de xchgl inutiles.

#include "mysem.h"
#include "spinlock.h" // needed ?

void mysem_init(mysem_t *s, int value)
{
    s->value = value;
    s->lock = 0;
}

void mysem_wait(mysem_t *s)
{
    while (1) {
        lock_ttas(&s->lock);
        if (s->value > 0) {
            s->value--;
            unlock_ttas(&s->lock);
            return;
        }
        unlock_ttas(&s->lock);
    }
}

void mysem_post(mysem_t *s)
{
    lock_ttas(&s->lock);
    s->value++;
    unlock_ttas(&s->lock);
}

*/

//ttas directly in mysem and using tas spinlock lock/unlock

#include "mysem.h"
#include "spinlock.h"

void mysem_init(mysem_t *s, int value)
{
    s->value = value;
    s->lock = 0; // initialize spinlock as unlocked
}

void mysem_wait(mysem_t *s)
{
    while (1) {
        // First, spin on the semaphore value without locking (test part)
        while (s->value <= 0) {
            // busy wait (do nothing) — cheap read
        }

        // Then acquire the spinlock
        lock(&s->lock);

        // Check the value again in case another thread grabbed it
        if (s->value > 0) {
            s->value--;
            unlock(&s->lock);
            return;
        }

        unlock(&s->lock);
        // optional: yield CPU to reduce contention
        // sched_yield(); // include <sched.h> if you want
    }
}

void mysem_post(mysem_t *s)
{
    lock(&s->lock);
    s->value++;
    unlock(&s->lock);
}

