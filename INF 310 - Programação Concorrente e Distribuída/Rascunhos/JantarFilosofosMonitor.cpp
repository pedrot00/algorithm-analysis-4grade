#include<iostream>
#include<mutex>

#include <condition_variable>
using namespace std;

class Jantar{
private:
    enum Estado { PENSANDO, FAMINTO, COMENDO };
    Estado *estado;
    condition_variable * garfo;
    mutex mtx;
    int n;

    void verificar(int id){
        int esq = (id+1)% n;
        int dir = id;
        if(estado[id] == FAMINTO && estado[dir] != COMENDO &&  estado[esq] != COMENDO){
            estado[id] = COMENDO;
            garfo[id].notify_one();
        }
    }

public:

    Jantar(int val){ estado = new Estado[val];n = val; garfo = new condition_variable[val]; }
    ~Jantar(){ delete[] garfo; }

    void pegar_garfo(int id){
        unique_lock<mutex> lck(mtx);
        estado[id] = FAMINTO;

        garfo[id].wait(lck,[this, &id]()->bool { return estado[id] == 1; });
        
        cout<<"o filoso" << id << "esta usando os garfos esquerdo " << (id+1)%n << " e direito " << id;
    }

    void largar_garfo(int id){
        unique_lock<mutex> lck(mtx);
        estado[id] = PENSANDO;
        cout <<"filosfo " << id << " largou os garfos";
        verificar(id);
        verificar((id+1)%n);
    }

};