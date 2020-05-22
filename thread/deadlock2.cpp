#include <stdio.h>                                                                                                                              
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int i = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* start_routine(void *arg)
{
    int j;
    char *str = (char*)arg;
    for (j=0; j<100; ++j)
    {
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex); //死锁产生的位置
        printf("i=%d, from %s\n",i, arg);   
        //pthread_mutex_lock(&mutex);
        i++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void)
{
    pthread_t pid1,pid2;
    char *str1 = "A";
    char *str2 = "B";
    pthread_create(&pid1,NULL,start_routine,(void*)str1);
    pthread_join(pid1,NULL);
    return 0;
}  
