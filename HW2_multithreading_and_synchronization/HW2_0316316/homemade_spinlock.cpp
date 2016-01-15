#include <iostream>
#include <pthread.h>
#define thread_num 3
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
int home_spin=0;

void homemade_spin_lock(int* spinlock_addr){
    asm(
        "spin_lock: \n\t"
        "xorl %%ecx, %%ecx \n\t"
        "incl %%ecx \n\t"
        "spin_lock_retry: \n\t"
        "xorl %%eax, %%eax \n\t"
        "lock; cmpxchgl %%ecx, (%0) \n\t"
        "jnz spin_lock_retry \n\t"
        ::"r" (spinlock_addr):"ecx","eax"
    );
}

void homemade_spin_unlock(int* spinlock_addr){
    asm(
        "spin_unlock: \n\t"
        "movl $0, (%0) \n\t"
        ::"r" (spinlock_addr):
    );
}

void* ThreadRunner(void*){
    int k;
    for (k=0;k<100000000;k++){
        homemade_spin_lock(&home_spin);
        x.Increment();
        homemade_spin_unlock(&home_spin);
    }
}

int main(){
    pthread_t tid[thread_num];
    int i;
    home_spin=0;
    for(i=0;i<thread_num;i++){
        pthread_create(&tid[i], NULL, ThreadRunner, 0);
    }
    for(i=0;i<thread_num;i++){
        pthread_join(tid[i], NULL);
    }
    x.Print();
    return 0;
}

