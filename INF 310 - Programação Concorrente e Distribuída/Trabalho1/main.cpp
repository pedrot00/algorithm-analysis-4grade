//utilizar para compilacaog++ -std=c++20 main.cpp -pthread -o main

#include <iostream>
#include <vector>
#include <thread>
#include <barrier>
#include <mutex>
#include <atomic>

using namespace std;

//futuras atualizacoes -> sistema de excessoes para num_jogadores, etapas e voltas (nao previsto na descricao do trab)

//var globais iniciadas pos leitura main
int NUM_JOGADORES;
int NUM_ETAPAS;
int NUM_VOLTAS;
atomic<int> colocacao_atual{0};
mutex mux;

//infomarcoes corredor
struct Jogador {
    vector<int> etapas_sum;   // vector para flexibilizar
    int total_sum = 0;
    
    Jogador() : etapas_sum(NUM_ETAPAS, 0) {}  //construtor inicializa com zeros
};

void competition(Jogador &jogador, barrier<> &sync_point) {     
    for (int etapa = 0; etapa < NUM_ETAPAS; etapa++) {
        for (int volta = 0; volta < NUM_VOLTAS; volta++) {
            this_thread::yield();   //garantir bom sistema de troca
            
            int minha_colocacao = 0; //var local de cada thread
            {
                lock_guard<mutex> lock(mux);
                minha_colocacao = colocacao_atual++;   //copia var global depois incrementa
            }
            jogador.etapas_sum[etapa] += minha_colocacao; //alimenta pontuacao da etapa com var local
            
            sync_point.arrive_and_wait();
            
            if (colocacao_atual == NUM_JOGADORES) { //so uma thread atualiza colocacao atual pos sincronizacao
                colocacao_atual = 0;
            }
            sync_point.arrive_and_wait();  //barreria sincroniza para prox etapa
        }
        jogador.total_sum += jogador.etapas_sum[etapa];
        sync_point.arrive_and_wait();   // sincroniza saida conjunta para prox iteracao
    }
}

void imprimeJogadores(const vector<Jogador> &jogadores) {       //funcao para impressao
    cout << "\n=== RESULTADO FINAL ===\n";
    for (int i = 0; i < NUM_JOGADORES; i++) {
        cout << "Jogador " << i << ":\n";
        for (int j = 0; j < NUM_ETAPAS; j++) {
            cout << "  ETAPA " << j << " | " << jogadores[i].etapas_sum[j] << " pontos\n";
        }
        cout << "  TOTAL | " << jogadores[i].total_sum << " pontos\n";
        cout << string(40, '-') << endl;
    }
}

void anunciarVencedor(const vector<Jogador> &jogadores) {
    int menor_pontuacao = INT_MAX;
    int vencedor_id = -1;       //caso 0 jogadores

    for (int i = 0; i < NUM_JOGADORES; i++) {
        if (jogadores[i].total_sum < menor_pontuacao) {
            menor_pontuacao = jogadores[i].total_sum;
            vencedor_id = i;
        }
    }
    cout << "VENCEDOR \n";
    cout << "Jogador " << vencedor_id << " com " << menor_pontuacao << " pontos!\n";
    cout << string(40, '-') << endl;
}

int main() {

    cout << "=== SISTEMA DE CORRIDA ===\n";  //leitura dos valores
    cout << "Insira num de jogadores: ";  cin >> NUM_JOGADORES;
    cout << "Insira num de etapas: ";  cin >> NUM_ETAPAS;
    cout << "Insira num de voltas: ";  cin >> NUM_VOLTAS;
    cout << "\n";
    
    barrier sync_point(NUM_JOGADORES);      //inicializa barreira com número correto de jogadores
    
    vector<Jogador> jogadores(NUM_JOGADORES); //cria vetor de jogadores e threads;
    vector<thread> threads;
    
    cout << "Competicao iniciada\n";
    cout << NUM_JOGADORES << " jogadores, " << NUM_ETAPAS << " etapas, " << NUM_VOLTAS << " voltas por etapa\n\n";
    
    //cria threads
    for (int i = 0; i < NUM_JOGADORES; i++) 
        threads.emplace_back(competition, ref(jogadores[i]), ref(sync_point));
    
    //aguarda threads
    for (auto &t : threads) 
        t.join();
        
    imprimeJogadores(jogadores);
    anunciarVencedor(jogadores);
    
    return 0;
}
