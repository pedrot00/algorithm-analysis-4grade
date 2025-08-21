#include<iostream>
#include<pthread.h>
using namespace std;

struct ThreadData{
   int id;
   long int val;

};

bool isPrime(long int val){
    if(val <=2) return false;
    if(val%2 == 0 && val!=2) return false;
    
    for (long int i = 3; i * i <= val; i += 2) {
        if (val % i == 0) return false;
    }
    return true;
}

void *f(void* arg){
  ThreadData* data = (ThreadData*) arg;
  
  bool res = isPrime(data->val);
  cout <<"ID - "<< data->id <<"| NUM - " << data->val << "|PRIMO: " << res;
  return NULL;
 
}



int main() {
    long int numeros[4] = {1379455703, 1988595883, 1482377683, 24562922401};
    pthread_t threads[4];
    ThreadData data[4];
    void *res[4];
    
    for(int i=0; i<4; i++){
        data[i].id = i;
        data[i].val = numeros[i];
        pthread_create(&threads[i], NULL, f, &data[i]);
    }
    
    for(int i=0; i<4; i++){
        pthread_join(threads[i], &res[i]);
    }
    
    return 0;
}