#include <iostream>
#include <pthread.h>
#define thread_num 16
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
pthread_spinlock_t spin;

void* ThreadRunner(void*){
    int k;
    for (k=0;k<100000000;k++){
        pthread_spin_lock(&spin);
        x.Increment();
        pthread_spin_unlock(&spin);
    }
}

int main(){
    pthread_t tid[thread_num];
    int i;
    pthread_spin_init(&spin, 0);
    for(i=0;i<thread_num;i++){
        pthread_create(&tid[i], NULL, ThreadRunner, 0);
    }
    for(i=0;i<thread_num;i++){
        pthread_join(tid[i], NULL);
    }
    x.Print();
    return 0;
}

