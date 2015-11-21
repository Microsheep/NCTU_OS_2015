#include <iostream>
#include <pthread.h>
#include <stdlib.h>
using namespace std;

class Counter{
private:
    int hit;
    int total;
public:
    Counter(){
        hit=0;
        total=0;
    }
    void Increment(double x, double y){
        if (x*x+y*y<=1){
            hit++;
        }
        total++;
    }
    void Print(){
        cout << hit << " / " << total << endl;
        cout << "Pi is " << double(hit)*4/total << endl;
    }
};

Counter x;

void* ThreadRunner(void* t){
    int k;
    unsigned int seed=0;
    for (k=0;k< *(int*)t ;k++){
        x.Increment(double(rand_r(&seed))/RAND_MAX,double(rand_r(&seed))/RAND_MAX);
    }
}

int main(){
    pthread_t tid[3];
    int i;
    int t=100000000;
    for(i=0;i<3;i++){
        pthread_create(&tid[i], NULL, ThreadRunner, (void*) &t);
    }
    for(i=0;i<3;i++){
        pthread_join(tid[i], NULL);
    }
    x.Print();
    return 0;
}

