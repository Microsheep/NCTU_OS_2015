#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;

void* hello(void*) {
    pthread_mutex_lock(&mutex);
    printf("%ld says hello\n", pthread_self());
    pthread_mutex_unlock(&mutex); 
}

int main() {
    pthread_t thread[3];
    int i;
    for (i=0;i<3;i++){
        pthread_create(&thread[i], NULL, hello, NULL);
    }
    for (i=0;i<3;i++){
        pthread_join(thread[i], NULL);
    }
    return 0;
}

