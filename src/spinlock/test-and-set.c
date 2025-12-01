// code for test-and-set algorithm

#include <stdio.h>
#include <spinlock.h> // includes the spinlock header file containing the lock and unlock function declarations.
// The implementations inside spinlock.c will be linked during compilation.






void main(argc, char *argv[]) {

    int nb_threads = atoi(argv[1]);
    int n_critical_sections = 32768/nb_threads; // number of critical sections each thread will execute.



}