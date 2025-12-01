// code for a simple spinlock implementation in C + semaaphore using this spinlock

// can this be implemented in a separate file and then include in desired files?

#include <stdio.h>

typedef volatile int spinlock_t; // defining a type that is acttually a volatile int. We could just use volatile int directly but this is more readable.

void lock(spinlock_t *lock) {
    asm volatile ( // add volatile to prevent compiler optimizations that could reorder instructions
        "enter:\n"                // étiquette pour revenir au début de la fonction si le lock est déjà pris
        "    movl    $1, %%eax\n"      // %eax=1
        "    xchgl   %%eax, %0\n"  // instruction atomique, échange (lock) et %eax après exécution, %eax contient la donnée qui était dans lock, et lock contient la valeur 1
        "    testl   %%eax, %%eax\n"    // met le flag ZF à vrai si %eax contient 0
        "    jnz     enter\n"         // retour à enter: si ZF n'est pas vrai
        : /* no output */
        : "m" (*lock)        // input : la mémoire pointée par lock
        : "%eax"             // registre "clobbered" -> pour indiquer que %eax est modifié par l'asm (dont les données sont imprévisibles ?)
    ); // course syllabi

    // besoin de mettre tout le test dans cette fonction ou uniquement xchg ?
    // si on met tout le test, on a une boucle d'attente active (spin) dans cette fonction
    // si on met uniquement xchg, on doit faire la boucle d'attente active dans la fonction appelante
    // ici on fait la boucle d'attente active dans cette fonction -> on boucle pour revenir au début de la fonction si le lock est déjà pris

    return;
}


void unlock(spinlock_t *lock) {
    asm volatile (
        "    movl    $0, %%eax\n"      // libère le lock en mettant sa valeur à 0"
        "    xchgl   %%eax, %0\n"  // instruction atomique"
        : /* no output */
        : "m" (*lock)        // input : la mémoire pointée par lock
        : "%eax"             // registre "clobbered"
    ); // course syllabi

    return;
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