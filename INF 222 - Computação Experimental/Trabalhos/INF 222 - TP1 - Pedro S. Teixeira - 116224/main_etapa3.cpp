#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <numeric>
#include <cmath>

using namespace std;

// Lançar moeda: 0 = coroa, 1 = cara
int lancarMoeda(mt19937 &gen) {
    uniform_int_distribution<int> dist(0, 1);
    return dist(gen);
}

// Funções auxiliares
double media(const vector<int>& v) {
    double soma = accumulate(v.begin(), v.end(), 0.0);
    return soma / v.size();
}

double desvioPadrao(const vector<int>& v) {
    double m = media(v);
    double soma = 0;
    for (int x : v) soma += (x - m) * (x - m);
    return sqrt(soma / v.size());
}

// Simula UMA partida do jogo
pair<char,int> simularJogo(mt19937 &gen, int saldoA, int saldoB) {
    int rodadas = 0;
    while (saldoA > 0 && saldoB > 0) {
        int moeda = lancarMoeda(gen);
        if (moeda == 1) { // cara: A ganha
            saldoA++;
            saldoB--;
        } else { // coroa: B ganha
            saldoA--;
            saldoB++;
        }
        rodadas++;
    }
    char vencedor = (saldoA > 0 ? 'A' : 'B');
    return {vencedor, rodadas};
}

// Estatísticas para N simulações
void analisarJogo(mt19937 &gen, int nSim, int saldoA, int saldoB, string nomeArquivo) {
    int vitoriasA = 0, vitoriasB = 0;
    vector<int> duracoes;

    for (int i = 0; i < nSim; i++) {
        auto [vencedor, rodadas] = simularJogo(gen, saldoA, saldoB);
        if (vencedor == 'A') vitoriasA++;
        else vitoriasB++;
        duracoes.push_back(rodadas);
    }

    double probA = 100.0 * vitoriasA / nSim;
    double probB = 100.0 * vitoriasB / nSim;
    double m = media(duracoes);
    double dp = desvioPadrao(duracoes);

    cout << "\n=== Simulação Jogo (" << saldoA << " x " << saldoB << ") ===" << endl;
    cout << "Vitórias A: " << vitoriasA << " (" << probA << "%)" << endl;
    cout << "Vitórias B: " << vitoriasB << " (" << probB << "%)" << endl;
    cout << "Média de rodadas: " << m << endl;
    cout << "Desvio padrão: " << dp << endl;

    // Salvar resultados detalhados
    ofstream arquivo(nomeArquivo);
    arquivo << "Simulacoes: " << nSim << endl;
    arquivo << "Saldo Inicial A: " << saldoA << ", B: " << saldoB << endl;
    arquivo << "Vitorias A: " << vitoriasA << " (" << probA << "%)" << endl;
    arquivo << "Vitorias B: " << vitoriasB << " (" << probB << "%)" << endl;
    arquivo << "Media rodadas: " << m << endl;
    arquivo << "Desvio padrao: " << dp << endl;
    arquivo.close();

    cout << "Resultados salvos em: " << nomeArquivo << endl;
}

int main() {
    random_device rd;
    mt19937 gen(rd());

    int nSim = 10000;

    // Etapa 3.1 - Cenários
    analisarJogo(gen, nSim, 2, 8, "jogo_2x8.txt");    // caso base
    analisarJogo(gen, nSim, 20, 80, "jogo_20x80.txt");
    analisarJogo(gen, nSim, 3, 7, "jogo_3x7.txt");
    analisarJogo(gen, nSim, 1, 100, "jogo_1x100.txt");

    cout << "\n=== Conclusões Esperadas ===" << endl;
    cout << "- Jogador com mais saldo inicial tem mais chance de vencer." << endl;
    cout << "- Duração média do jogo cresce com saldos maiores." << endl;
    cout << "- Jogos muito desequilibrados (ex: 1 x 100) acabam rápido." << endl;

    return 0;
}
