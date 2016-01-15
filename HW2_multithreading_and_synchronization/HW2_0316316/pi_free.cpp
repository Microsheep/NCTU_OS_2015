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
    void operator +=(Counter in){
        (*this).hit += in.hit;
        (*this).total += in.total;
    }
    void Print(){
        //cout << hit << " / " << total << endl;
        cout << "Pi is " << fixed << setprecision(6) << double(hit)*4/total << endl;
    }
};

Counter x[thread_num];

struct arg{
    int time;
    int num;
} t[thread_num];

void* ThreadRunner(void* in){
    int k;
    unsigned int seed=rand();
    for (k=0;k<((arg*)in)->time;k++){
        x[((arg*)in)->num].Increment(double(rand_r(&seed))/RAND_MAX,double(rand_r(&seed))/RAND_MAX);
    }
}

int main(int argc, char* argv[]){
    pthread_t tid[thread_num];
    int i;
    int all=10000000;
    if (argv[1]){
        all=stoi(argv[1]);
    }
    srand(time(NULL));
    for(i=0;i<thread_num;i++){
        t[i].num=i;
        if(i==0){
            t[i].time=all-((all/thread_num)*(thread_num-1));
        }
        else{
            t[i].time=(all/thread_num);
        }
    }
    for(i=0;i<thread_num;i++){
        pthread_create(&tid[i], NULL, ThreadRunner, (void*) &t[i]);
    }
    for(i=0;i<thread_num;i++){
        pthread_join(tid[i], NULL);
    }
    Counter total;
    for(i=0;i<thread_num;i++){
        total += x[i];
        //x[i].Print();
    }
    total.Print();
    return 0;
}

