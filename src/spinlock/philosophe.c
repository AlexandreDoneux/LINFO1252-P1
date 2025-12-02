// Philosopher simulation using spinlocks

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "mysem.h"

#define CYCLES 1000000

struct args {
    int id;
    int philosophes;
    mysem_t *baguette;   // au lieu de thread_mutex_t *baguette;
};

void mange(int id)
{
    for (int i = 0; i < rand(); i++)
    {
        // philosophe mange
    }
}

void *philosophe(void *args)
{
    int id = ((struct args*)args)->id;
    int left = id;
    int philosophes = ((struct args*)args)->philosophes;
    mysem_t *baguette = ((struct args*)args)->baguette;
    int right = (left + 1) % philosophes;
    int count = 0;

    while (count < CYCLES)
    {
        // philosophe pense
        //printf("Philosophe [%d] thinking\n", id);

        if (left < right)
        {
            mysem_wait(&baguette[left]);
            //printf("baguette left taken Philosophe [%d]\n", id);
            mysem_wait(&baguette[right]);
            //printf("baguette right taken Philosophe [%d]\n", id);
        }
        else
        {
            mysem_wait(&baguette[right]);
            //printf("baguette right taken Philosophe [%d]\n", id);
            mysem_wait(&baguette[left]);
            //printf("baguette left taken Philosophe [%d]\n", id);
        }

        // philosophe mange
        //printf("Philosophe [%d] eating\n", id);
        mange(id);

        mysem_post(&baguette[left]);
        //printf("baguette left free Philosophe [%d]\n", id);
        mysem_post(&baguette[right]);
        //printf("baguette right free Philosophe [%d]\n", id);

        count++;
    }
    return NULL;
}

int main(int argc, char **argv)
{
    int philosophes;

    if(argc != 2){
        printf("Usage: %s <number_of_philosophers>\n", argv[0]);
        return 1;
    }


    philosophes = atoi(argv[1]);
    printf("Number of philosophers : %d\n", philosophes);

    pthread_t *phil = malloc(philosophes * sizeof(pthread_t));
    mysem_t *baguette = malloc(philosophes * sizeof(mysem_t));
    struct args *PhilArgs = malloc(philosophes * sizeof(struct args));

    if (!phil || !baguette || !PhilArgs) {
        fprintf(stderr, "malloc error\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < philosophes; i++) {
        mysem_init(&baguette[i], 1);
    }

    for (int i = 0; i < philosophes; i++) {
        PhilArgs[i].philosophes = philosophes;
        PhilArgs[i].baguette = baguette;
        PhilArgs[i].id = i;
        pthread_create(&phil[i], NULL, philosophe, &PhilArgs[i]);
    }

    for (int i = 0; i < philosophes; i++) {
        pthread_join(phil[i], NULL);
    }

    return 0;
}
