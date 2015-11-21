#include <iostream>
#include <pthread.h>
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
int lock = pthread_spin_init(&spin, 0);

void* ThreadRunner(void*){
    int k;
    for (k=0;k<100000000;k++){
        lock = pthread_spin_lock(&spin);
        x.Increment();
        lock = pthread_spin_unlock(&spin);
    }
}

int main(){
    pthread_t tid[3];
    int i;
    for(i=0;i<3;i++){
        pthread_create(&tid[i], NULL, ThreadRunner, 0);
    }
    for(i=0;i<3;i++){
        pthread_join(tid[i], NULL);
    }
    x.Print();
    return 0;
}

