// Philosopher simulation using spinlocks

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

//#include "mysem.h"
#include "spinlock.h"

//#define CYCLES 100
#define CYCLES 1000000

struct args {
    int id;
    int philosophes;
    spinlock_t *baguette;   // au lieu de thread_mutex_t *baguette;
};

void mange(int id)
{
	// philosophe mange
	//printf("Philosophe [%d] mange\n", id);
	//printf("Philosophe [%d] ne mange plus\n", id);
}

void *philosophe(void *args)
{
    int id = ((struct args*)args)->id;
    int left = id;
    int philosophes = ((struct args*)args)->philosophes;
    spinlock_t *baguette = ((struct args*)args)->baguette;
    int right = (left + 1) % philosophes;
    int count = 0;

    while (count < CYCLES)
    {
        // philosophe pense
        //printf("Philosophe [%d] thinking\n", id);

        if (left < right)
        {
            lock_ttas(&baguette[left]);
            //printf("baguette left taken Philosophe [%d]\n", id);
            lock_ttas(&baguette[right]);
            //printf("baguette right taken Philosophe [%d]\n", id);
        }
        else
        {
            lock_ttas(&baguette[right]);
            //printf("baguette right taken Philosophe [%d]\n", id);
            lock_ttas(&baguette[left]);
            //printf("baguette left taken Philosophe [%d]\n", id);
        }

        // philosophe mange
        //printf("Philosophe [%d] eating\n", id);
        mange(id);

        unlock_ttas(&baguette[left]);
        //printf("baguette left free Philosophe [%d]\n", id);
        unlock_ttas(&baguette[right]);
        //printf("baguette right free Philosophe [%d]\n", id);

        count++;
    }
    return NULL;
}

int main(int argc, char **argv)
{
    int philosophes;

    if(argc != 2){
        printf("Usage : %s <number_of_philosophers>\n", argv[0]);
        return 1;
    }

    philosophes = atoi(argv[1]);
    printf("Number of philosophers : %d\n", philosophes);

    
    pthread_t phil[philosophes];
    spinlock_t baguette[philosophes];

    struct args PhilArgs[philosophes];
    for (int i = 0; i < philosophes; i++) {
        //mysem_init(&baguette[i], 1);
        baguette[i] = 0; // initialize spinlock as unlocked
    }

    for (int i = 0; i < philosophes; i++) {
        PhilArgs[i].philosophes = philosophes;
        PhilArgs[i].baguette = baguette;
        PhilArgs[i].id = i;
        //printf("Creating philosopher %d\n", i);
        pthread_create(&phil[i], NULL, philosophe, &PhilArgs[i]);
    }

    for (int i = 0; i < philosophes; i++) {
        pthread_join(phil[i], NULL);
    }

    return 0;
}
