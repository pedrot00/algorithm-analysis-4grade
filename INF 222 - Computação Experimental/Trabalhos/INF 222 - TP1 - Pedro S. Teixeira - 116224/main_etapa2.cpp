#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <numeric>

using namespace std;

// Funções base
int lancarDado(mt19937 &gen) {
    uniform_int_distribution<int> dist(1, 6);
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

// Caso 1: Lançar o dado e verificar frequência do valor 6
void caso1_dado(mt19937 &gen) {
    cout << "\n=== CASO 1 (DADO): Convergência para 1/6 ===" << endl;

    ofstream arquivo("dado_caso1.txt");
    arquivo << "NumLancamentos\tSeis\tPorcentagem" << endl;

    for (int n : {10, 100, 1000, 10000, 100000, 1000000}) {
        int seis = 0;
        for (int i = 0; i < n; i++) {
            if (lancarDado(gen) == 6) seis++;
        }
        double porcentagem = 100.0 * seis / n;

        cout << n << " lançamentos -> "
             << seis << " vezes saiu 6 (" << porcentagem << "%)" << endl;

        arquivo << n << "\t" << seis << "\t" << porcentagem << endl;
    }

    arquivo.close();
    cout << "Resultados salvos em dado_caso1.txt" << endl;
}

// Caso 2: 120 lançamentos, contar quantas vezes saiu 6
void caso2_dado(mt19937 &gen) {
    vector<int> num_simulacoes = {100, 1000, 10000};

    cout << "\n=== CASO 2 (DADO): 120 lançamentos repetidos ===" << endl;

    for (int sim : num_simulacoes) {
        vector<int> resultados;

        for (int i = 0; i < sim; i++) {
            int seis = 0;
            for (int j = 0; j < 120; j++) {
                if (lancarDado(gen) == 6) seis++;
            }
            resultados.push_back(seis);
        }

        cout << "\n" << sim << " simulações:" << endl;
        cout << "Média: " << media(resultados) << endl;
        cout << "Desvio Padrão: " << desvioPadrao(resultados) << endl;

        ofstream arquivo("dado_caso2_" + to_string(sim) + ".txt");
        for (int resultado : resultados) {
            arquivo << resultado << endl;
        }
        arquivo.close();
        cout << "Dados salvos em: dado_caso2_" << sim << ".txt" << endl;
    }
}

// Caso 3: Lançar até sair 6
void caso3_dado(mt19937 &gen) {
    vector<int> num_simulacoes = {1000, 10000, 50000};

    cout << "\n=== CASO 3 (DADO): Lançamentos até obter 6 ===" << endl;

    for (int sim : num_simulacoes) {
        vector<int> resultados;

        for (int i = 0; i < sim; i++) {
            int tentativas = 0;
            while (true) {
                tentativas++;
                if (lancarDado(gen) == 6) break;
            }
            resultados.push_back(tentativas);
        }

        cout << "\n" << sim << " simulações:" << endl;
        cout << "Média: " << media(resultados) << endl;
        cout << "Desvio Padrão: " << desvioPadrao(resultados) << endl;

        // Frequência até 20 lançamentos
        vector<int> frequencia(21, 0);
        for (int resultado : resultados) {
            if (resultado <= 20) frequencia[resultado - 1]++;
        }

        ofstream arquivo("dado_caso3_" + to_string(sim) + "_histograma.txt");
        arquivo << "Lancamentos\tFrequencia\tPorcentagem" << endl;
        for (int i = 0; i < 20; i++) {
            double porcentagem = 100.0 * frequencia[i] / sim;
            arquivo << (i + 1) << "\t\t" << frequencia[i] << "\t\t"
                    << porcentagem << "%" << endl;
        }
        arquivo.close();
        cout << "Histograma salvo em: dado_caso3_" << sim << "_histograma.txt" << endl;
    }
}

int main() {
    random_device rd;
    mt19937 gen(rd());

    cout << "=== ANÁLISE COMPLETA - LANÇAMENTO DE DADOS ===" << endl;

    caso1_dado(gen);
    caso2_dado(gen);
    caso3_dado(gen);

    cout << "\n=== CONCLUSÕES ESPERADAS ===" << endl;
    cout << "Caso 1: Frequência converge para ~16.6% (1/6)" << endl;
    cout << "Caso 2: Média deve ser ~20, distribuição normal (sino)" << endl;
    cout << "Caso 3: Média ~6, mas moda = 1 (mais provável em 1 lançamento)" << endl;

    return 0;
}
