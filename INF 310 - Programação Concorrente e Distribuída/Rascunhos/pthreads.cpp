#include<pthread.h>
#include<iostream>
using namespace std;


//func testar ordem saida de cada execucao thread
void *f (void*id){
    for(int i=0; i<10; ++i)  
        cout << "thread executada " << i << " vezes - id: " << id << endl;
    return id;
}

struct DadosThread {
    int id;
    float valor;
    bool ativo;
    
};

int main(){
    DadosThread* dados = new DadosThread;
    dados->id = 2;
    dados->valor = 20.0;
    dados->ativo = true;
    pthread_t t1;      //identificador da primeria thread

    pthread_create(&t1, NULL, f, static_cast<void*>(dados)); 


    void *r1;
    pthread_join(t1, &r1);
    cout << (long)r1 << endl;
    
    return 0;
}
