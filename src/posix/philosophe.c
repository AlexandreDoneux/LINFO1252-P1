#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define CYCLES 1000000

struct args {
    int id;
    int philosophes;
    pthread_mutex_t *baguette;
};

void mange(int id)
{
 //mange
}

void *philosophe(void *args)
{
    int id = ((struct args*)args)->id;
    int left = id;
    int philosophes = ((struct args*)args)->philosophes;
    pthread_mutex_t *baguette = ((struct args*)args)->baguette;
    int right = (left + 1) % philosophes;
    int count = 0;

    while (count<CYCLES)
    {
        // philosophe pense
        //printf("Philosophe [%d] thinking\n", id);
        if (left < right)
        {
            pthread_mutex_lock(&baguette[left]);
            //printf("baguette left taken Philosophe [%d]\n", id);
            pthread_mutex_lock(&baguette[right]);
            //printf("baguette right taken Philosophe [%d]\n", id);
        }
        else
        {
            pthread_mutex_lock(&baguette[right]);
            //printf("baguette right taken Philosophe [%d]\n", id);
            pthread_mutex_lock(&baguette[left]);
            //printf("baguette left taken Philosophe [%d]\n", id);
        }
        mange(id);
        pthread_mutex_unlock(&baguette[left]);
        //printf("baguette left free Philosophe [%d]\n", id);
        pthread_mutex_unlock(&baguette[right]);
        //printf("baguette right free Philosophe [%d]\n", id);
        count++;
    }
    return (NULL);
}

int main(int argc, char **argv){
    int philosophes;

    if(argc != 2){
        printf("Usage: %s <number_of_philosophers>\n", argv[0]);
        return 1;
    }

    philosophes = atoi(argv[1]);
    printf("Number of philosophers : %d\n", philosophes);

    
    pthread_t phil[philosophes];
    pthread_mutex_t baguette[philosophes];

    struct args PhilArgs[philosophes];
    for (int i = 0; i < philosophes; i++) {
        pthread_mutex_init(&baguette[i], NULL);
    }

    for (int i=0; i<philosophes; i++){
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