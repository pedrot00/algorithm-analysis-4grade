#include<iostream>
#include<condition_variable>
#include<mutex>
using namespace std;

class MonitorPonte{
private:
    int N = 5;          // lim carros podem passar quando outro lado tiver carro p atravessar

    int sentidoAtual = -1;                    // -1: ponte livre; 0: N->S; 1: S-> N
    int carrosPonte = 0;                      // cont de carros SIMULTANEOS na ponte
    int carrosConsecutivos = 0;               // cont de carros SEQUENCIAL na ponte (utilizar p mudar fluxo de direcao se limite = N)
    int esperandoSentido[2] = {0, 0};         // controla se tem esperadno N->S e S->N

    mutex mtx;
    condition_variable podeEntrar;

public:
    MonitorPonte(int limite){ N = limite; }  // destrutor eh default
    
    void entra(int sentido){
        unique_lock<mutex> lck(mtx);
        esperandoSentido[sentido]++;
        
        podeEntrar.wait(lck, [&]()->bool {
             return (sentidoAtual == -1) ||         // sentido atual == -1 n tem ngm na pista
                    (sentido == sentidoAtual &&      
                        (carrosPonte < N || esperandoSentido[1 - sentido] == 0));
        // sentido == sentidoAtual e se n fere os limites ou se nao tem ninguem do outra pista esperando
        });

        carrosPonte++;              // entrou na ponte
        carrosConsecutivos++;       // identifica que p uma direcao ao menos 1 carro sequencial entrou
        esperandoSentido[sentido]--;    // menos um esperando, pois entrou

        if(sentidoAtual == -1) sentidoAtual = sentido;  // altera o sentido fluxo para uma das direcoes

        // aqui carro trafega e depois pode sair
    }

    void sai(int sentido){
        unique_lock<mutex> lck(mtx);
        carrosPonte--;

        if(carrosPonte == 0){       // logica para decidir sentido se nao tiver ngm na ponte
            if(esperandoSentido[sentido] == 0  && esperandoSentido[1-sentido] == 0){
                 sentidoAtual = -1;
                 carrosConsecutivos = 0;
            }  
            else if (esperandoSentido[sentido] == 0 && esperandoSentido[1-sentido] > 0){
                sentidoAtual = 1-sentido;
                carrosConsecutivos = 0;
            }
            else if(carrosConsecutivos >= N && esperandoSentido[1-sentido] > 0){
                sentidoAtual = 1-sentido;
                carrosConsecutivos = 0;
            }
            podeEntrar.notify_all();
            // libera carros p usar a pista ja que um saiu, deve ser all pois tem chance de um carro de outra direcao acordar e simplesmene voltar a dormir (bad smell)        
        }
        // caso ainda tenha carros na ponte n faz mais nada alem de sair
    }
};