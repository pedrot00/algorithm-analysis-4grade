#include <iostream>
#include <thread>
#include <barrier>
#include <mutex>
#include <vector>
#include <atomic>

using namespace std;

#define VOLTAS 1
#define ETAPAS 5
#define NUM_JOGADORES 4

// Variáveis globais
atomic<int> colocacao_atual{0};
mutex mux;
barrier sync_point(NUM_JOGADORES);

// Estrutura do jogador
struct Jogador {
    int etapas_sum[ETAPAS] = {0};   //guarda pontuacao por etapa de cada jogador
    int total_sum = 0;              //guarda soma da pontuacao de cad   a jogador
};

void competition(Jogador &jogador) {
    for (int etapa = 0; etapa < ETAPAS; etapa++) {
        for (int volta = 0; volta < VOLTAS; volta++) {
            this_thread::yield();  
            
            int minha_colocacao = 0;    //var local de cada thread
            {
                lock_guard<mutex> lock(mux);
                minha_colocacao = colocacao_atual++;    //copia var global depois incrementa
            }
            jogador.etapas_sum[etapa] += minha_colocacao; //alimenta pontuacao da etapa com var local
            
            sync_point.arrive_and_wait();
            
            if (colocacao_atual == NUM_JOGADORES) { //so uma thread atualiza colocacao atual pos sincronizacao
                colocacao_atual = 0;
            }
            
            sync_point.arrive_and_wait(); //barreria sincroniza para prox etapa
        }
        jogador.total_sum += jogador.etapas_sum[etapa];  //final de etapa setta pontuacao total (add valor da etapa)
        sync_point.arrive_and_wait();
    }
}

void imprimeJogadores(const vector<Jogador> &jogadores) {
    cout << "\n=== RESULTADO FINAL ===\n";
    for (int i = 0; i < NUM_JOGADORES; i++) {
        cout << "Jogador " << i << ":\n";
        for (int j = 0; j < ETAPAS; j++) {
            cout << "  ETAPA " << j << " | " << jogadores[i].etapas_sum[j] << " pontos\n";
        }
        cout << "  TOTAL | " << jogadores[i].total_sum << " pontos\n";
        cout << string(40, '-') << endl;
    }
}

int main() {
    vector<Jogador> jogadores(NUM_JOGADORES);
    vector<thread> threads;

    cout << "Competicao iniciada!\n";
    cout << NUM_JOGADORES << " jogadores, " << ETAPAS << " etapas, " << VOLTAS << " voltas por etapa\n\n";

    for (int i = 0; i < NUM_JOGADORES; i++) {
        threads.emplace_back(competition, ref(jogadores[i]));
    }
    
    for (auto &t : threads) {
        t.join();
    }
    
    imprimeJogadores(jogadores);
    return 0;
}