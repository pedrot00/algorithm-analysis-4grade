#include<iostream>
#include<mutex>
#include<semaphore.h>
#include<semaphore.h>
#include <condition_variable>
using namespace std;


class MonitorProdCons {
private:
    int bSize,pin,pout,cont;
    int *buffer;
    mutex mux;
    condition_variable naoCheio,naoVazio;
public:

    MonitorProdCons(int bufferSize){
        bSize=bufferSize;
        buffer = new int[bSize];
        pin=pout=cont=0;
    }
    ~MonitorProdCons(){
        delete [] buffer;
    }

    void produzir(int msg) {
        unique_lock<mutex> lck(mux);
        if(cont == bSize){
            naoCheio.wait(lck);
        }

        buffer[pin]=msg;
        pin=(pin+1)%bSize;
        ++cont;
        naoVazio.notify_one();
    }
    int consumir() {
        unique_lock<mutex> lck(mux);

        if(cont ==0){
            naoVazio.wait(lck);
        }
        int msg=buffer[pout];
        pout=(pout+1)%bSize;
        --cont;
        naoCheio.notify_one();
        return msg;
    }
};

