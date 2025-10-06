#include <iostream>
#include <mutex>
#include <condition_variable>
using namespace std;

class Festa{
private:
    mutex mtx;
    bool copoCheio = false;
    condition_variable  cv_taVazio, cv_taCheio;
    int tipo_bebida;

public:
    void serve (int bebida){
        unique_lock<mutex> lck(mtx);

        cv_taVazio.wait(lck, [this]()->bool {return !copoCheio;});
        copoCheio = true;
        tipo_bebida = bebida;
        cv_taCheio.notify_all();
    }

    void bebe(int convidado){
        unique_lock<mutex> lck(mtx);

        cv_taCheio.wait(lck, [this]()->bool {return copoCheio;});

        if(convidado == 1 && tipo_bebida == 1)
            cout << "O convidado 1 bebeu a bebida" <<endl;
        else if(convidado == 2 && (tipo_bebida == 1 || tipo_bebida == 2))
            cout << "O convidado 2 bebeu a bebida" <<endl;
        else
            cout << "O convidado 3 bebeu a bebida" <<endl;

        copoCheio = false;
        cv_taVazio.notify_one();
    }
};