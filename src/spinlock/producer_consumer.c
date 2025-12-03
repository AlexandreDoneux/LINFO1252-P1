// producer-consumer problem using spinlocks

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "mysem.h"

#define N 8         
#define TOTAL_ITEMS 131072  

int buffer[N];
int in = 0; 
int out = 0;

mysem_t mutex;  // remplace pthread_mutex_t
mysem_t empty;     // remplace sem_t empty
mysem_t full;      // remplace sem_t full

int limit_prod;
int limit_cons;

void *producer(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < limit_prod; i++) {
        int item = id; 

        for (int k = 0; k < 10000; k++);

        mysem_wait(&empty);

        mysem_wait(&mutex); 

        buffer[in] = item;
        //printf("[Producer %d] produced %d at index %d\n", id, item, in);
        in = (in + 1) % N;

        mysem_post(&mutex); 
        mysem_post(&full);      
    }

    //printf("[Producer %d] DONE (produced %d items)\n", id, limit_prod);
    return NULL;
}

void *consumer(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < limit_cons; i++) {
        int item;

        mysem_wait(&full);

        mysem_wait(&mutex);

        item = buffer[out];
        //printf("[Consumer %d] consumed %d from index %d\n", id, item, out);
        out = (out + 1) % N;

        mysem_post(&mutex);
        mysem_post(&empty);

        for (int k = 0; k < 10000; k++);
    }

    //printf("[Consumer %d] DONE (consumed %d items)\n", id, limit_cons);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nb_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int nb_producers = atoi(argv[1])/2;
    int nb_consumers = atoi(argv[1])/2;

    if (nb_producers <= 0 || nb_consumers <= 0) {
        fprintf(stderr, "nb_producers and nb_consumers must be > 0\n");
        return EXIT_FAILURE;
    }

    limit_prod = TOTAL_ITEMS / nb_producers;
    limit_cons = TOTAL_ITEMS / nb_consumers;

    printf("Producers: %d (each produces %d items)\n", nb_producers, limit_prod);
    printf("Consumers: %d (each consumes %d items)\n\n", nb_consumers, limit_cons);

    mysem_init(&mutex, 1); 
    mysem_init(&empty, N);
    mysem_init(&full, 0); 

    pthread_t producers[nb_producers];
    pthread_t consumers[nb_consumers];
    int prod_id[nb_producers];
    int cons_id[nb_consumers];

    for (int i = 0; i < nb_producers; i++) {
        prod_id[i] = i;
        pthread_create(&producers[i], NULL, producer, &prod_id[i]);
    }

    for (int i = 0; i < nb_consumers; i++) {
        cons_id[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &cons_id[i]);
    }

    for (int i = 0; i < nb_producers; i++)
        pthread_join(producers[i], NULL);

    for (int i = 0; i < nb_consumers; i++)
        pthread_join(consumers[i], NULL);

    printf("\nDONE\n");

    return EXIT_SUCCESS;
}
