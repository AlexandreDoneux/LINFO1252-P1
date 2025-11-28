#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>

#define WRITER_TOTAL 6400
#define READER_TOTAL 25400

int wcount = 0;
int rcount = 0;
pthread_mutex_t m_rcount;
pthread_mutex_t m_wcount;
pthread_mutex_t z;

sem_t wsem_db;
sem_t rsem_db;

int limit_writer;
int limit_reader;

void *writer(void *arg) {

    int id = *(int*)arg;
    int local = 0;

    while (local < limit_writer) {

        printf("Writer %d wants to write\n", id);

        pthread_mutex_lock(&m_wcount);
        wcount++;
        if (wcount == 1){
            printf("Writer %d blocks readers\n", id);
            sem_wait(&rsem_db);
        }
        pthread_mutex_unlock(&m_wcount);

        sem_wait(&wsem_db);
        printf("Writer %d WRITING\n", id);

        for (int i=0; i<10000; i++); 

        printf("Writer %d finished writing\n", id);
        sem_post(&wsem_db);

        pthread_mutex_lock(&m_wcount);
        wcount--;
        if (wcount == 0){
            printf("Writer %d unblocks readers\n", id);
            sem_post(&rsem_db);
        }
        pthread_mutex_unlock(&m_wcount);

        local++;
    }

    printf("Writer %d DONE (wrote %d times)\n", id, limit_writer);
    return NULL;
}


void *reader(void *arg){

    int id = *(int*)arg;
    int local = 0;

    while (local < limit_reader){

        printf("Reader %d wants to read\n", id);

        pthread_mutex_lock(&z);
        sem_wait(&rsem_db);

        pthread_mutex_lock(&m_rcount);
        rcount++;
        if (rcount == 1) {
            printf("Reader %d blocks writers (first reader)\n", id);
            sem_wait(&wsem_db);
        }
        pthread_mutex_unlock(&m_rcount);

        sem_post(&rsem_db);
        pthread_mutex_unlock(&z);

        printf("Reader %d READING\n", id);

        for (int i=0; i<10000; i++); 

        printf("Reader %d finished reading\n", id);

        pthread_mutex_lock(&m_rcount);
        rcount--;
        if (rcount == 0){
            printf("Reader %d unblocks writers (last reader)\n", id);
            sem_post(&wsem_db);
        }
        pthread_mutex_unlock(&m_rcount);

        local++;
    }

    printf("Reader %d DONE (read %d times)\n", id, limit_reader);
    return NULL;
}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <nb_writers> <nb_readers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int nb_writers = atoi(argv[1]);
    int nb_readers = atoi(argv[2]);

    if (nb_writers <= 0 || nb_readers <= 0) {
        fprintf(stderr, "nb_writers and nb_readers must be > 0\n");
        return EXIT_FAILURE;
    }

    limit_writer = WRITER_TOTAL / nb_writers;
    limit_reader = READER_TOTAL / nb_readers;

    pthread_mutex_init(&m_rcount, NULL);
    pthread_mutex_init(&m_wcount, NULL);
    pthread_mutex_init(&z, NULL);

    sem_init(&wsem_db, 0, 1);
    sem_init(&rsem_db, 0, 1);

    pthread_t writers[nb_writers];
    pthread_t readers[nb_readers];

    int writer_id[nb_writers];
    int reader_id[nb_readers];

    for (int i = 0; i < nb_writers; i++) {
        writer_id[i] = i;
        pthread_create(&writers[i], NULL, writer, &writer_id[i]);
    }

    for (int i = 0; i < nb_readers; i++) {
        reader_id[i] = i;
        pthread_create(&readers[i], NULL, reader, &reader_id[i]);
    }

    for (int i = 0; i < nb_writers; i++)
        pthread_join(writers[i], NULL);

    for (int i = 0; i < nb_readers; i++)
        pthread_join(readers[i], NULL);


    return EXIT_SUCCESS;
}
