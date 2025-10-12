#include<iostream>
#include<mutex>
#include<semaphore.h>
#include<semaphore.h>
using namespace std;

void decolando();
void pousando();

sem_t espera_avioes; //inicia com 2, sera a lista de espera dos avieos
sem_t mutex_pista; //incia com 1
sem_t mtx;  //mutex para verificacoes

sem_t podeDecolar; //inicia com 0;
sem_t podePousar; //inicia com 0;
bool temEsperando_pousar = false, taDecolando = false;


void aviao_pousando(){
    while(true){
         sem_wait(&mtx);
        temEsperando_pousar = true;
        sem_wait(&podePousar);

        sem_wait(&podePousar);          //note que n faria sentido um aviao pousar se em algum momento nenhum voao decolou
        sem_wait(&mutex_pista);
        pousando();     //aqui realizad a funcao de pousar
        sem_post(&espera_avioes);
        sem_post(&mutex_pista);
    }
}


void aviao_decolando(){
    while(true){
        sem_wait(&mtx);
        if(temEsperando_pousar || taDecolando){        // no caso se tiver um aviao pousando ou qurendo pousar eh bloqueado
            sem_post(&mtx);
            sem_wait(&podeDecolar);
        }
        sem_post(&mtx);

        sem_wait(&mutex_pista);
        decolando();                //aqui o aviao ta fazendo sua funcao
        
        if(temEsperando_pousar) 
        sem_post(&podePousar);
        
        sem_post(&mutex_pista);     //aqui ele libera a pista
    }
}
