#include <iostream>
#include <unistd.h>
#include <string.h>
using namespace std;

int main(){
    int x=10;
    for(int a=1;a<=x;a++){
        sleep(3);
        cout << a << " Hello ~~~" << endl;
    }
}
