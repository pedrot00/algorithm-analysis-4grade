#include <iostream>
#include <thread>
#include <barrier>
#include <mutex>
#include <vector>

using namespace std;

#define VOLTAS 10
#define ETAPAS 10
#define JOGADORES 4

// ESCOPO VAR GLOBAIS
int pontuacao_por_volta = 0;  
mutex mux;
barrier sync_point(JOGADORES, [](){  pontuacao_por_volta = 0;}); // setta barreira e reset da pontuacao


// INFOMARCOES DO JOGADOR
struct Jogador{
   int arr_pontos[ETAPAS][VOLTAS] = {{0}};  //guarda potuacao do jogador em todas voltas em todas etapas

    int etapas_sum = 0;             //guarda soma por etapa
    int total_sum = 0;              //guarda soma total
};

// ESCOPO FUNCAO THREADS
void competition(Jogador &jogador){
    
    for(int i =0; i<ETAPAS; i++){
        for(int j=0; j<VOLTAS; j++){
            { // regiao critica lockada para cada iteracao de voltas
                lock_guard<mutex> lock(mux);
                jogador.arr_pontos[i][j] = pontuacao_por_volta;
                pontuacao_por_volta++;
           }
        }
        this_thread::yield();
        sync_point.arrive_and_wait(); //construtor barreira zera pontuacao voltas antes prox etapa 
    }
}


int main(){
    cout<<"teste";


    return 0;
}