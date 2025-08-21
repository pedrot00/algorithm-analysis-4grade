#include<iostream>
#include<pthread.h>
using namespace std;

//CODIGO INCORRETO - PARA CONSERTAR

void *f (void* arg){ //soma
    int soma = *(int *) arg;
    pthread_exit((void*)soma);

}

void *g (void* arg){ // produto
     int produto = *(int *) arg;
    pthread_exit((void*)produto);

}

int main(){
    long int numeros[10] = {45, 12, 2, 4, 56, 10, 65, 77, 4, 11};
    pthread_t threads[10];
    void * res[10];

    for(int i= 0; i<10; i++){
        if(i <5)
            pthread_create(&threads[i], NULL, f, &numeros[i]);
        else
            pthread_create(&threads[i], NULL, g, &numeros[i]);
    }
    
    int result = 0;

    for(int i=0; i<10; i++){
        pthread_join(threads[i], &res[i]);
        result += *(int*)res[i];
    }
    
    return 0;
}