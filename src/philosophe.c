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
    printf("Philosophe [%d] mange\n", id);
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
    pthread_mutex_t *baguette = ((struct args*)args)->baguette;
    int right = (left + 1) % philosophes;
    int count = 0;

    while (count<CYCLES)
    {
        // philosophe pense
        printf("pensanding Philosophe [%d]\n", id);
        if (left < right)
        {
            pthread_mutex_lock(&baguette[left]);
            printf("baguette left taken Philosophe [%d]\n", id);
            pthread_mutex_lock(&baguette[right]);
            printf("baguette right taken Philosophe [%d]\n", id);
        }
        else
        {
            pthread_mutex_lock(&baguette[right]);
            printf("baguette right taken Philosophe [%d]\n", id);
            pthread_mutex_lock(&baguette[left]);
            printf("baguette left taken Philosophe [%d]\n", id);
        }
        mange(id);
        pthread_mutex_unlock(&baguette[left]);
        printf("baguette left free Philosophe [%d]\n", id);
        pthread_mutex_unlock(&baguette[right]);
        printf("baguette right free Philosophe [%d]\n", id);
        count++;
    }
    return (NULL);
}

int main(){
    int philosophes;
    printf("number of philosophers: \n");
    scanf("%d", &philosophes);
    
    pthread_t phil[philosophes];
    pthread_mutex_t baguette[philosophes];

    struct args *PhilArgs = malloc(philosophes * sizeof(struct args));    
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