// p rodar o codigo execute g++ main.cpp MonitorPonte.cpp -o programa -pthread
// em sequencia ./programa

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "MonitorPonte.cpp"

using namespace std;

// utilizando TAD para mostrar de forma adequada como monitor funciona (essa implementacao nao conta com .h)
// problemas a serem considerados em prox manutecoes -> garantir melhor fairness
void carro(int id, int sentido, MonitorPonte &ponte) {
    ponte.entra(sentido);

    string direcao = (sentido == 0 ? "N->S" : "S->N");
    cout << "Carro " << id << " entrando na ponte (" << direcao << ")" << endl;
    
    this_thread::sleep_for(chrono::milliseconds(500)); // cruza ponte

    ponte.sai(sentido);
    cout << "Carro " << id << " saindo da ponte (" << direcao << ")" << endl; 
}

int main() {
    int N;
    cout << "Digite o limite N de veiculos consecutivos: ";
    cin >> N;

    if (N <= 0) {
        cout << "Valor de N invalido. Encerrando programa.\n";
        return 0;
    }

    int totalCarros;
    cout << "Digite o total de carros: ";
    cin >> totalCarros;

    if (totalCarros <= 0) {
        cout << "Quantidade de carros invalida. Encerrando programa.\n";
        return 0;
    }

    MonitorPonte ponte(N);  // cria monitor com limite N

    vector<thread> threads;

    // gera carros sentidos alternados
    for (int i = 0; i < totalCarros; i++) {
        int sentido = (i % 2 == 0) ? 0 : 1; // alterna N->S e S->N
        threads.emplace_back(carro, i + 1, sentido, ref(ponte));
        this_thread::sleep_for(chrono::milliseconds(100)); // utilizaremos p espacar chegadas
    }

    for (auto &t : threads)
        t.join();

    cout << "\nTodos os carros cruzaram a ponte.\n";
    return 0;
}