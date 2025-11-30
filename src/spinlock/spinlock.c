// code for a simple spinlock implementation in C + semaaphore using this spinlock

// can this be implemented in a separate file and then include in desired files?

#include <stdio.h>

typedef volatile int spinlock_t; // defining a type that is acttually a volatile int. We could just use volatile int directly but this is more readable.

void lock(spinlock_t *lock) {
    asm ( // add volatile ? -> will that have an effect ?
        "movl    $1, %eax      ; %eax=1"
        "xchgl   %eax, (lock)  ; instruction atomique, échange (lock) et %eax après exécution, %eax contient la donnée qui était dans lock, et lock contient la valeur 1"
        "testl   %eax, %eax    ; met le flag ZF à vrai si %eax contient 0"
        "jnz     enter         ; retour à enter: si ZF n'est pas vrai"
        "ret"
    ); // course syllabi
}


void unlock(spinlock_t *lock) {
    asm (
        "movl    $0, %eax    ; libère le lock en mettant sa valeur à 0"
        "xchgl   %eax, (lock)  ; instruction atomique"
        "ret"
    ); // course syllabi

}

// Usage example
/*
int main() {
    spinlock_t lock_var = 0;

    lock(&lock_var);
    // Critical section
    unlock(&lock_var);

    return 0;
}
*/


// Semaphore using spinlock ?

// later