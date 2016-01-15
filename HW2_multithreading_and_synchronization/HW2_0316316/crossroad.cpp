#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <iomanip>
#define thread_num 4
using namespace std;

char direction[4]={'N','E','S','W'};

class Counter{
private:
    int total;
public:
    Counter(){
        total=0;
    }
    void Increment(){
        total++;
    }
    int globaltime(){
        return total/4;
    }
    void Print(){
        cout << total << endl;
    }
};

struct road{
    int point;
    int first;
    int second;
    int cars;
};

Counter x;
pthread_mutex_t info;
pthread_mutex_t timer;
pthread_mutex_t testdead;
pthread_mutex_t mutex[thread_num];
int finish[4],pos[4];

bool dead(){
    int a;
    for(a=0;a<thread_num;a++){
        if(pos[a]!=1){
            return false;
        }
    }
    return true;
}

void* ThreadRunner(void* pass){
    int one=((road*)pass)->first;
    int two=((road*)pass)->second;
    int num=((road*)pass)->cars;
    int way=((road*)pass)->point;
    int mytime=0,k=0;
    while(num>0){
        if(x.globaltime()==mytime){
            mytime++;
            if(pos[way]==0){
                if(pthread_mutex_trylock(&mutex[one])==0){
                    pos[way]++;
                    /*
                    pthread_mutex_lock(&info);
                    cout << "Time: " << mytime << " " << direction[way] << k << " get lock one" << endl;
                    pthread_mutex_unlock(&info);
                    */
                }
            }
            else if(pos[way]==1){
                pthread_mutex_lock(&testdead);
                if(pthread_mutex_trylock(&mutex[two])==0){
                    pos[way]++;
                    /*
                    pthread_mutex_lock(&info);
                    cout << "Time: " << mytime << " " << direction[way] << k << " get lock two" << endl;
                    pthread_mutex_unlock(&info);
                    */
                }
                else{
                    if(dead()){
                        pthread_mutex_lock(&info);
                        cout << "A DEADLOCK HAPPENS at " << mytime << endl;
                        pthread_mutex_unlock(&info);
                        pos[way]--;
                        pthread_mutex_unlock(&mutex[one]);
                        /*
                        pthread_mutex_lock(&info);
                        cout << "Let other Pass: " << direction[way] << endl;
                        pthread_mutex_unlock(&info);
                        */
                    }
                }
                pthread_mutex_unlock(&testdead);
            }
            else if(pos[way]==2){
                pos[way]++;
                pthread_mutex_lock(&info);
                cout << direction[way] << " " << k << " leaves at " << mytime << endl;
                pthread_mutex_unlock(&info);
            }
            else{
                num--;
                pos[way]=0;
                pthread_mutex_unlock(&mutex[one]);
                pthread_mutex_unlock(&mutex[two]);
                /*
                pthread_mutex_lock(&info);
                cout << direction[way] << " " << k << " unlock at " << mytime << endl;
                pthread_mutex_unlock(&info);
                */
                k++;
            }
            pthread_mutex_lock(&timer);
            x.Increment();
            pthread_mutex_unlock(&timer);
        }
        else{
            continue;
        }
    }
    finish[way]=1;
    while(finish[0]==0||finish[1]==0||finish[2]==0||finish[3]==0){
        if(x.globaltime()==mytime){
            mytime++;
            pthread_mutex_lock(&timer);
            x.Increment();
            pthread_mutex_unlock(&timer);
        }
        else{
            continue;
        }
    }
}

int main(int argc, char* argv[]){
    pthread_t tid[thread_num];
    int i;
    road data[thread_num];
    data[0].point=0;data[0].first=0;data[0].second=2;
    data[1].point=1;data[1].first=1;data[1].second=0;
    data[2].point=2;data[2].first=3;data[2].second=1;
    data[3].point=3;data[3].first=2;data[3].second=3;
    for (i=0;i<thread_num;i++){
        if (argv[i+1]){
            data[i].cars=stoi(argv[i+1]);
        }
        else{
            cout << "Not enough arguments! " << i << endl;
            exit(0);
        }
    } 
    for(i=0;i<thread_num;i++){
        pthread_create(&tid[i], NULL, ThreadRunner, (void*) &data[i]);
    }
    for(i=0;i<thread_num;i++){
        pthread_join(tid[i], NULL);
    }
    return 0;
}

