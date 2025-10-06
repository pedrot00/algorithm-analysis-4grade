#include<iostream>
#include<mutex>
#include<semaphore.h>
using namespace std;

sem_t jovem, mtx, repor_cerveja, repor_refri, pronto_refri, pronto_cerveja;
int cont_refri = 0, cont_cerveja = 0;

void toma_refri(){
    cout << "o refri foi tomado"<<endl;
    sem_wait(&mtx);
        cont_refri--;
    sem_post(&mtx);
}

void toma_cerveja(){
    cout << "a cerveja foi tomada"<<endl;
    sem_wait(&mtx);
        cont_cerveja--;
    sem_post(&mtx);
}

//conside que os unicos id's disponiveis sao
// 0 - jovem que bee so refri; 1 - jovem que bebe cerveja senao refri
void beber (int id){
    while(true){
        sem_wait(&mtx);

        if(id == 0){
            if(cont_refri > 0){
                sem_post(&mtx);
                toma_refri();
            }
            else {
                sem_post(&mtx);
                sem_post(&repor_refri);
                sem_wait(&pronto_refri);
                toma_refri();
            }
        } else {
            if(cont_cerveja == 0){
                if(cont_refri >0){
                    sem_post(&mtx);
                    toma_refri();
                }
                else{
                    sem_post(&mtx);
                    sem_post(&repor_cerveja);
                    sem_wait(&pronto_cerveja);
                    toma_cerveja();
                }
            } else{
                sem_post(&mtx);
                toma_cerveja();
            }
        }
    }
}

void atendente_cerveja(){
   while(true){
        sem_wait(&repor_cerveja);
        sem_wait(&mtx);
        cont_cerveja = 6;
        sem_post(&mtx);
        sem_post(&pronto_cerveja);
   }
}

void atendente_refri(){
   while(true){
        sem_wait(&repor_refri);
        sem_wait(&mtx);
        cont_refri = 10;
        sem_post(&mtx);
        sem_post(&pronto_refri);
   }
}