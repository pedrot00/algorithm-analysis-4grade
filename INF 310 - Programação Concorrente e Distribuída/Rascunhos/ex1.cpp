#include<pthread.h>
#include<iostream>
using namespace std;

void *f(void* arg){
    int val = *(int*) arg;

    cout << "Numero: " << val <<"|Quadrado: " << val*val << endl;
    return NULL;

}

int main(){
    int n;
    cout<<"Insira o numero de threads: ";
    cin >> n;

    int vec[n];
    pthread_t threads[n];

    for(int i =0; i<n; i++){
        cout << "Insira o elemento numero " << i+1 << ": ";
        cin >> vec[i];
        pthread_create(&threads[i], NULL, f, &vec[i]);
    }
    for(int i=0; i<n; i++)
        pthread_join(threads[i], NULL);

    return 0;
}