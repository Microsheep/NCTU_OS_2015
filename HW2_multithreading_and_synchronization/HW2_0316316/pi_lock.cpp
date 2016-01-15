#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <iomanip>
#include <time.h>
#define thread_num 3
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
        //cout << hit << " / " << total << endl;
        cout << "Pi is " << fixed << setprecision(6) << double(hit)*4/total << endl;
    }
};

Counter x;
pthread_mutex_t mutex;

void* ThreadRunner(void* t){
    int k;
    unsigned int seed=rand();
    for (k=0;k< *(int*)t ;k++){
        pthread_mutex_lock(&mutex);
        x.Increment(double(rand_r(&seed))/RAND_MAX,double(rand_r(&seed))/RAND_MAX);
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[]){
    pthread_t tid[3];
    int i;
    int all=10000000;
    if (argv[1]){
        all=stoi(argv[1]);
    }
    srand(time(NULL));
    int t[thread_num];
    for(i=0;i<thread_num;i++){
        if(i==0){
            t[i]=all-((all/thread_num)*(thread_num-1));
        }
        else{
            t[i]=(all/thread_num);
        }
    }
    for(i=0;i<3;i++){
        pthread_create(&tid[i], NULL, ThreadRunner, (void*) &t[i]);
    }
    for(i=0;i<3;i++){
        pthread_join(tid[i], NULL);
    }
    x.Print();
    return 0;
}

