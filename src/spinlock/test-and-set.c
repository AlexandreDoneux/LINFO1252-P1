// code for test-and-set algorithm

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "spinlock.h" // includes the spinlock header file containing the lock and unlock function declarations.
// The implementations inside spinlock.c will be linked during compilation.

spinlock_t lock_var = 0; // initialize the spinlock variable. // put volatile ?

void *run_func(void *arg) {
    //int id = *(int *)arg[0];
    //int n_critical_sections = *(int *)arg[1];
    int *args = (int *)arg;
    int id = args[0];
    int n_critical_sections = args[1];
    printf("Thread %d starting, will execute %d critical sections.\n", id, n_critical_sections);

    for (int i = 0; i < n_critical_sections; i++) {
        lock(&lock_var); // accède bien à la variable lock_var ?
        // critical section
        //printf("Thread %d in critical section %d\n", id, i);
        for (int k = 0; k < 10000; k++); // simulating work
        //printf("Thread %d leaving critical section %d\n", id, i);
        unlock(&lock_var);
    }
    return NULL;
}



int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("Usage: %s <number of threads>\n", argv[0]);
        return 1;
    }

    int nb_threads = atoi(argv[1]);
    int n_critical_sections = 32768/nb_threads; // number of critical sections each thread will execute.

    printf("Starting %d threads, each executing %d critical sections.\n", nb_threads, n_critical_sections);

    pthread_t threads[nb_threads];


    for (int i = 0; i < nb_threads; i++) {
        int thread_args[2];
        thread_args[0] = i; // is overwritten before thread uses it?
        thread_args[1] = n_critical_sections;

        pthread_create(&threads[i], NULL, run_func, &thread_args);
    }

    for (int i = 0; i < nb_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed.\n");

    return 0;
}