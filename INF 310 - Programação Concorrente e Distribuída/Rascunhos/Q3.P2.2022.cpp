#include <iostream>
#include <mutex>
#include <condition_variable>
using namespace std;

class MonitorFumante{
    //0 - tabaco
    //1 - papel
    //2 - fosforo
private:   
    mutex mtx;
    condition_variable  cv_fumante1, cv_fumante2, cv_fumante3, cv_podeServir;
    bool podeServir = false, temTabaco = false, temPapel = false,temFosforo = false;
    bool mesaVazia = true;
    void pode_fumar() {cout << "Fumante está fumando..." << endl;}

public:

    void servir(int ingrediente1, int ingrediente2){
        // considere que os ingredientes 1 e 2 sempre serao diferentes;
        unique_lock<mutex> lck(mtx);
        cv_podeServir.wait(lck, [&]{ return mesaVazia; });
        mesaVazia = false;

        if((ingrediente1 ==  0 && ingrediente2 == 1) || (ingrediente1 ==  1 && ingrediente2 == 0) ){
            temTabaco = true; temPapel = true; cv_fumante1.notify_one();
        }
        else if((ingrediente1 ==  0 && ingrediente2 == 2) || (ingrediente1 ==  2 && ingrediente2 == 0)){
            temTabaco = true; temFosforo = true; cv_fumante2.notify_one();
        }
        else{
             temPapel = true; temFosforo = true; cv_fumante3.notify_one();
        }
        
    }   
    void fumar(int ingrediente){
        unique_lock<mutex> lck(mtx); 
        
        if(ingrediente == 0)
            cv_fumante3.wait(lck,[this]()->bool {return temPapel && temFosforo;});
        
        else if(ingrediente == 1)
            cv_fumante2.wait(lck,[this]()->bool {return temTabaco && temFosforo;});
        
        else{
            cv_fumante1.wait(lck,[this]()->bool {return temTabaco && temPapel;});
        }
        pode_fumar();
        mesaVazia = true;

        temFosforo = temTabaco = temPapel = false;
    }
};