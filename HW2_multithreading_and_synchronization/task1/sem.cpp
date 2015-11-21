#include <iostream>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

class Counter{
private:
    int value;
public:
    Counter(){
        value=0;
    }
    void Increment(){
        value++;
    }
    void Print(){
        cout << value << endl;
    }
};

Counter x;
sem_t sem;

void* ThreadRunner(void*){
    int k;
    for (k=0;k<100000000;k++){
        sem_wait(&sem);
        x.Increment();
        sem_post(&sem);
    }
}

int main(){
    pthread_t tid[3];
    int i;
    sem_init(&sem, 0, 1);
    for(i=0;i<3;i++){
        pthread_create(&tid[i], NULL, ThreadRunner, 0);
    }
    for(i=0;i<3;i++){
        pthread_join(tid[i], NULL);
    }
    x.Print();
    return 0;
}

