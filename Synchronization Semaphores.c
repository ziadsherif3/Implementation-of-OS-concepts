#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/**Arguments for counter threads and seeds for random number generators*/
typedef struct mcounter_args{
    int id;
}mcounter_args;
unsigned int* seeds;
/**Functions executed by threads*/
void* mcounter(void* args);
void* mmonitor(void* args);
void* mcollector(void* args);
/**Arguments passed to program through CLI*/
int max_no_msgs;
int N_cthreads;
int sbuffer;
/**Semaphores and shared variables*/
int msgcount;
int* buffer;
int empty_places;
sem_t msgcount_lock;
sem_t buffer_lock;

int main(int argc, char* argv[])
{
    int i;
    N_cthreads = atoi(argv[1]);
    sbuffer = atoi(argv[2]);
    max_no_msgs = atoi(argv[3]);

    buffer = malloc(sizeof(int) * sbuffer);
    empty_places = sbuffer;

    seeds = malloc(sizeof(unsigned int) * (N_cthreads + 2));

    pthread_t tmcounter[N_cthreads];
    pthread_t tmmonitor;
    pthread_t tmcollector;
    mcounter_args** mca = malloc(sizeof(mcounter_args*) * N_cthreads);

    msgcount = 0;

    sem_init(&msgcount_lock, 0, 1);
    sem_init(&buffer_lock, 0, 1);

    for(i = 0; i < N_cthreads; i++){
        mca[i] = (mcounter_args*)malloc(sizeof(mcounter_args));
        mca[i]->id = i;
        pthread_create(&tmcounter[i], NULL, mcounter, (void*)(mca[i]));
    }
    pthread_create(&tmmonitor, NULL, mmonitor, NULL);
    pthread_create(&tmcollector, NULL, mcollector, NULL);

    for(i = 0; i < N_cthreads; i++){
        pthread_join(tmcounter[i], NULL);
    }
    pthread_join(tmmonitor, NULL);
    pthread_join(tmcollector, NULL);

    free(buffer);
    free(seeds);
    for(i = 0; i < N_cthreads; i++){
        free(mca[i]);
    }
    free(mca);
    sem_destroy(&msgcount_lock);
    sem_destroy(&buffer_lock);
    return 0;
}

void* mcounter(void* args){
    int msgs_to_prod = max_no_msgs;
    seeds[((mcounter_args*)args)->id] = time(NULL) ^ getpid() ^ pthread_self();
    while((msgs_to_prod--) > 0){
        sleep(rand_r(&seeds[((mcounter_args*)args)->id]) % 15 + 1);
        printf("Counter thread %d: Received a message.\n", ((mcounter_args*)args)->id);
        printf("Counter thread %d: Waiting to write.\n", ((mcounter_args*)args)->id);
        sem_wait(&msgcount_lock);
        printf("Counter thread %d: Adding to counter, counter value = %d.\n", ((mcounter_args*)args)->id, ++msgcount);
        sem_post(&msgcount_lock);
    }
}

void* mmonitor(void* args){
    int checks = N_cthreads * max_no_msgs;
    int bpointer = 0;
    int writebuffer;
    int oldmsgcount;
    seeds[N_cthreads] = time(NULL) ^ getpid() ^ pthread_self();
    while((checks--) > 0){
        sleep(rand_r(&seeds[N_cthreads]) % 15 + 1);
        printf("Monitor thread: Waiting to read counter.\n");
        sem_wait(&msgcount_lock);
        printf("Monitor thread: Reading a counter of value %d.\n", msgcount);
        oldmsgcount = msgcount;
        writebuffer = 0;
        msgcount = 0;
        sem_post(&msgcount_lock);
        while(!writebuffer){
            sleep(rand_r(&seeds[N_cthreads]) % 15 + 1);
            sem_wait(&buffer_lock);
            if(empty_places == 0){
                printf("Monitor thread: Buffer is full.\n");
                sem_post(&buffer_lock);
            }
            else{
              if(bpointer == sbuffer){
                bpointer = 0;
              }
              printf("Monitor thread: Writing to buffer at position %d value %d.\n", bpointer, oldmsgcount);
              buffer[bpointer++] = oldmsgcount;
              empty_places--;
              writebuffer = 1;
              sem_post(&buffer_lock);
            }
        }
    }
}

void* mcollector(void* args){
    int checks = N_cthreads * max_no_msgs;
    int bpointer = 0;
    int collectbuffer;
    seeds[N_cthreads + 1] = time(NULL) ^ getpid() ^ pthread_self();
    while((checks--) > 0){
        sleep(rand_r(&seeds[N_cthreads + 1]) % 15 + 1);
        collectbuffer = 0;
        while(!collectbuffer){
            sleep(rand_r(&seeds[N_cthreads + 1]) % 15 + 1);
            sem_wait(&buffer_lock);
            if(empty_places == sbuffer){
                printf("Collector thread: Nothing is in the buffer.\n");
                sem_post(&buffer_lock);
            }
            else{
               if(bpointer == sbuffer){
                bpointer = 0;
              }
              printf("Collector thread: Reading from buffer at position %d value %d.\n", bpointer, buffer[bpointer]);
              bpointer++;
              empty_places++;
              collectbuffer = 1;
              sem_post(&buffer_lock);
            }
        }
    }
}
