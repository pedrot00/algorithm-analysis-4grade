#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <numeric>

using namespace std;

// Funções base
int lancarMoeda(mt19937 &gen) {
    uniform_int_distribution<int> dist(0, 1);
    return dist(gen);
}

double media(const vector<int>& v) {
    double soma = accumulate(v.begin(), v.end(), 0.0);
    return soma / v.size();
}

double desvioPadrao(const vector<int>& v) {
    double m = media(v);
    double soma = 0;
    for (int x : v) 
        soma += (x - m) * (x - m);
    return sqrt(soma / v.size());
}

void caso1_analise(mt19937 &gen) {
    cout << "\n=== CASO 1: Convergência para 50% ===" << endl;
    
    ofstream arquivo("caso1_convergencia.txt");
    arquivo << "NumLancamentos\tCaras\tPorcentagem" << endl;

    // Testar diferentes quantidades de lançamentos
    for (int n : {10, 100, 1000, 10000, 100000, 1000000}) {
        int caras = 0;
        for (int i = 0; i < n; i++) {
            caras += lancarMoeda(gen);
        }
        double porcentagem = 100.0 * caras / n;

        // Mostrar no console
        cout << n << " lancamentos -> "
             << caras << " caras (" << porcentagem << "%)" << endl;

        // Salvar no arquivo
        arquivo << n << "\t" << caras << "\t" << porcentagem << endl;
    }

    arquivo.close();
    cout << "Resultados salvos em caso1_convergencia.txt" << endl;
}

void caso2_analise(mt19937 &gen) {
    vector<int> num_simulacoes = {100, 1000, 10000};
    
    cout << "\n=== CASO 2: 100 lançamentos repetidos ===" << endl;
    
    for (int sim : num_simulacoes) {
        vector<int> resultados;
        
        for (int i = 0; i < sim; i++) {
            int caras = 0;
            for (int j = 0; j < 100; j++) {
                caras += lancarMoeda(gen);
            }
            resultados.push_back(caras);
        }
        
        cout << "\n" << sim << " simulações:" << endl;
        cout << "Media: " << media(resultados) << endl;
        cout << "Desvio Padrão: " << desvioPadrao(resultados) << endl;
        
        // Salvar dados para histograma
        ofstream arquivo("caso2_" + to_string(sim) + "_simulacoes.txt");
        for (int resultado : resultados) {
            arquivo << resultado << endl;
        }
        arquivo.close();
        cout << "Dados salvos em: caso2_" << sim << "_simulacoes.txt" << endl;
    }
}

void caso3_analise(mt19937 &gen) {
    vector<int> num_simulacoes = {1000, 10000, 50000};
    
    cout << "\n=== CASO 3: Lançamentos até obter cara ===" << endl;
    
    for (int sim : num_simulacoes) {
        vector<int> resultados;
        
        for (int i = 0; i < sim; i++) {
            int tentativas = 0;
            while (true) {
                tentativas++;
                if (lancarMoeda(gen) == 1) break;
            }
            resultados.push_back(tentativas);
        }
        
        cout << "\n" << sim << " simulações:" << endl;
        cout << "Media: " << media(resultados) << endl;
        cout << "Desvio Padrão: " << desvioPadrao(resultados) << endl;
        
        // Contar frequências para histograma
        vector<int> frequencia(21, 0); // até 20 lançamentos
        for (int resultado : resultados) {
            if (resultado <= 20) {
                frequencia[resultado-1]++;
            }
        }
        
        // Salvar histograma
        ofstream arquivo("caso3_" + to_string(sim) + "_histograma.txt");
        arquivo << "Lancamentos\tFrequencia\tPorcentagem" << endl;
        for (int i = 0; i < 20; i++) {
            double porcentagem = 100.0 * frequencia[i] / sim;
            arquivo << (i+1) << "\t\t" << frequencia[i] << "\t\t" 
                   << porcentagem << "%" << endl;
        }
        arquivo.close();
        cout << "Histograma salvo em: caso3_" << sim << "_histograma.txt" << endl;
    }
}

int main() {
    random_device rd;
    mt19937 gen(rd());
    
    cout << "=== ANÁLISE COMPLETA - LANÇAMENTO DE MOEDAS ===" << endl;
    
    caso1_analise(gen);
    caso2_analise(gen);
    caso3_analise(gen);
    
    cout << "\n=== CONCLUSÕES ESPERADAS ===" << endl;
    cout << "Caso 1: Deve convergir para ~50% conforme aumenta o número" << endl;
    cout << "Caso 2: Média deve ser ~50, distribuição normal (sino)" << endl;
    cout << "Caso 3: Média ~2, mas moda = 1 (mais provável 1 lançamento)" << endl;
    
    return 0;
}