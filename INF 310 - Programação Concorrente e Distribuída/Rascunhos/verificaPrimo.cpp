#include<iostream>
#include<pthread.h>
using namespace std;

bool isPrime(long int val){
    if(val <=2) return false;
    if(val%2 == 0 && val!=2) return false;
    
    for (long int i = 3; i * i <= val; i += 2) {
        if (val % i == 0) return false;
    }
    return true;
}

void *f(void* arg){
    long int num = (long int) arg;
    bool res = isPrime(num);
    cout<< "Numero - " << num << "| eh primo: " << res << endl;
    return NULL;
}

int main() {
    long int val1 = 1379455703, val2 =1988595883, val3= 1482377683, val4 = 24562922401;
    
    pthread_t t1,t2,t3,t4;
    void* r1;
    void* r2;
    void* r3;
    void* r4;
    
    pthread_create(&t1, NULL, f, (void*)val1);
    pthread_create(&t2, NULL, f, (void*)val2);
    pthread_create(&t3, NULL, f, (void*)val3);
    pthread_create(&t4, NULL, f, (void*)val4);
    
    pthread_join(t1, &r1);
    pthread_join(t2, &r2);
    pthread_join(t3, &r3);
    pthread_join(t4, &r4);
    
    return 0;
}