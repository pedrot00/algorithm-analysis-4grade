#include <iostream>
#include <random>
#include <vector>
#include <cmath>    

using namespace std;

// o rand nao tem uma aleatoriadade real muito boa, utilizaremos uma solucao mais moderna pra simular isso

// ----------- ESCOPO BASE DOS CALCULOS
int lancarMoeda(mt19937 &gen) { // func simula lançamento moeda; 0 - coroa; 1 - cara
    uniform_int_distribution<int> dist(0, 1);
    return dist(gen);
}

double media(const vector<int>& v) { // calcula media com acumulaet
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

// ----------- PARTE 1
void caso1(mt19937 &gen, int n) { //Um lançamento
    int caras = 0;
    for (int i = 0; i < n; i++) {
        if (lancarMoeda(gen) == 1) 
            caras++; //se for cara guarda valor
    }
    cout << "Caso 1: " << n << " lancamentos: " 
         << "\n|Porcentagem: " << (100.0 * caras / n) << "% de caras" << endl;
}

void caso2(mt19937 &gen) { //100 lançamentos contar quantas caras
    vector<int> resultados;
    int caras = 0;

    for (int j = 0; j < 100; j++) 
        caras += lancarMoeda(gen);
    resultados.push_back(caras);
    
    cout << "Caso 2: 100 lancamentos" 
         << "\n|Numero de caras: " << caras << endl;
}

void caso3(mt19937 &gen, int simulacoes) { //quantidade lancamento ate cara
    vector<int> resultados;         //temos que usar vetor pois precisaremos da media e do desvio depois

    for (int i = 0; i < simulacoes; i++) {
        int tentativas = 0;
        while (true) {
            tentativas++;
            if (lancarMoeda(gen) == 1) break; // saiu cara
        }
        resultados.push_back(tentativas);
    }

    cout << "Caso 3: " << simulacoes << " repeticoes\n"
         << "|Media: " << media(resultados) 
         << "\n|Desvio Padrao: " << desvioPadrao(resultados) << endl;
}

int main() {
    random_device rd; // gerador num rand inicializado com hora atual
    mt19937 gen(rd());  //parametro das funcoes

    cout << "Testando Simulacoes Lancamento" << endl;

     cout << "---------------------------------" << endl;
    //testar para 10, 100, 1000 e 10000 vezes;
    caso1(gen, 100000);   // 1 lancamento realizado n(10 mil) vezes
    cout << "---------------------------------" << endl;

    caso2(gen);    // quantidade caras em 100 lancamentos
    cout << "---------------------------------" << endl;

    //testar para 10, 100, 1000 e 10000 vezes;
    caso3(gen, 10000);    // lancamentos realizados ate sair cara com n (10 mil) simulações
    cout << "\n---------------------------------" << endl;
    return 0;
}
