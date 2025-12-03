// reader-writer problem implementation using spinlocks in C

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "spinlock.h"
#include "mysem.h"

#define TOTAL_WRITES 6400
#define TOTAL_READS  25400

int wcount = 0; 
int rcount = 0; 

spinlock_t m_rcount = 0;
spinlock_t m_wcount = 0;
spinlock_t z = 0;

mysem_t wsem_db;
mysem_t rsem_db;

int limit_writes;
int limit_reads;

void *writer(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < limit_writes; i++) {

        printf("[Writer %d] wants to write\n", id);

        lock(&m_wcount);
        wcount++;
        if (wcount == 1) {
            mysem_wait(&rsem_db);
            printf("[Writer %d] blocks readers\n", id);
        }
        unlock(&m_wcount);

        mysem_wait(&wsem_db);
        printf("[Writer %d] WRITING...\n", id);

        for (int k = 0; k < 10000; k++);

        mysem_post(&wsem_db);
        printf("[Writer %d] finished writing\n", id);

        lock(&m_wcount);
        wcount--;
        if (wcount == 0) {
            mysem_post(&rsem_db);
            printf("[Writer %d] unblocks readers (no writer waiting)\n", id);
        }
        unlock(&m_wcount);
    }

    printf("[Writer %d] DONE (wrote %d times)\n", id, limit_writes);
    return NULL;
}

void *reader(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < limit_reads; i++) {

        printf("[Reader %d] wants to read\n", id);

        lock(&z);
        mysem_wait(&rsem_db);

        lock(&m_rcount);
        rcount++;
        if (rcount == 1) {
            mysem_wait(&wsem_db);
            printf("[Reader %d] blocks writers (first reader)\n", id);
        }
        unlock(&m_rcount);

        mysem_post(&rsem_db);
        unlock(&z);

        printf("[Reader %d] READING...\n", id);
        for (int k = 0; k < 10000; k++);

        lock(&m_rcount);
        rcount--;
        if (rcount == 0) {
            mysem_post(&wsem_db);
            printf("[Reader %d] unblocks writers (last reader)\n", id);
        }
        unlock(&m_rcount);
    }

    printf("[Reader %d] DONE (read %d times)\n", id, limit_reads);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nb_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int nb_writers = atoi(argv[1])/2;
    int nb_readers = atoi(argv[1])/2;

    if (nb_readers <= 0 || nb_writers <= 0) {
        fprintf(stderr, "nb_readers and nb_writers must be > 0\n");
        return EXIT_FAILURE;
    }

    limit_writes = TOTAL_WRITES / nb_writers;
    limit_reads  = TOTAL_READS  / nb_readers;

    printf("Readers: %d (each reads %d times)\n", nb_readers, limit_reads);
    printf("Writers: %d (each writes %d times)\n\n", nb_writers, limit_writes);

    mysem_init(&wsem_db, 1); 
    mysem_init(&rsem_db, 1);

    pthread_t readers[nb_readers];
    pthread_t writers[nb_writers];
    int r_id[nb_readers];
    int w_id[nb_writers];

    for (int i = 0; i < nb_readers; i++) {
        r_id[i] = i;
        pthread_create(&readers[i], NULL, reader, &r_id[i]);
    }

    for (int i = 0; i < nb_writers; i++) {
        w_id[i] = i;
        pthread_create(&writers[i], NULL, writer, &w_id[i]);
    }

    for (int i = 0; i < nb_readers; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < nb_writers; i++) {
        pthread_join(writers[i], NULL);
    }

    printf("\nDONE\n");

    return EXIT_SUCCESS;
}
