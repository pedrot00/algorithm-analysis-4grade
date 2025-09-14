#include <iostream>
#include <thread>
#include <barrier>
#include <mutex>
#include <vector>

using namespace std;

#define VOLTAS 10
#define ETAPAS 5
#define NUM_JOGADORES 4

// ESCOPO VAR GLOBAIS
int pontuacao_por_volta = 0;  
mutex mux;
barrier sync_point(NUM_JOGADORES, [](){  pontuacao_por_volta = 0;}); // setta barreira e reset da pontuacao


// INFOMARCOES DO JOGADOR
struct Jogador{
   int arr_pontos[ETAPAS][VOLTAS] = {{0}};  //guarda potuacao do jogador em todas voltas em todas etapas

    int etapas_sum[ETAPAS] = {0};    //guarda soma por etapa
    int total_sum = 0;              //guarda soma total
};

// ESCOPO FUNCAO THREADS
void competition(Jogador &jogador){
    
    for(int i =0; i<ETAPAS; i++){
        for(int j=0; j<VOLTAS; j++){
            { // regiao critica lockada para cada iteracao de voltas
                lock_guard<mutex> lock(mux);
                jogador.arr_pontos[i][j] = pontuacao_por_volta;
                jogador.etapas_sum[i] += pontuacao_por_volta;
                pontuacao_por_volta++;
           }
        }
        jogador.total_sum += jogador.etapas_sum[i];
        this_thread::yield();           //ajuda no revezamento
        sync_point.arrive_and_wait(); //construtor barreira zera pontuacao voltas antes prox etapa 
    }
}

void imprimeJogadores(const vector<Jogador> &jogadores) {
    for (int i = 0; i < NUM_JOGADORES; i++) {
        cout << "\nJogador " << i << ":\n";
        for (int j = 0; j < ETAPAS; j++) {
            cout << "  ETAPA " << j << "| " << jogadores[i].etapas_sum[j] << " pontos" << endl;
        }
        cout << "  TOTAL| " << jogadores[i].total_sum << " pontos" <<endl;
        cout<< string(40, '-');
    }
}

// MAIN
int main() {
    vector<Jogador> jogadores(NUM_JOGADORES);
    vector<thread> threads;

    for (int i = 0; i < NUM_JOGADORES; i++)   // cria threads
        threads.emplace_back(competition, ref(jogadores[i]));
    
    for (auto &t : threads)  //sincroniza threads
        t.join();
    
    imprimeJogadores(jogadores);
    return 0;
}