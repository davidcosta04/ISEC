

#ifndef TRABALHOPRATICO_UI_H
#define TRABALHOPRATICO_UI_H
#include "Buffer.h"
#include "Simulador.h"
#include <iostream>
#include <map>

using namespace std;

class UI{


    Simulador &s;
    map<string,int> comandos;

    public:
        UI(Simulador &s);

        void exec(const string &filename);
        void verifica_comandos(string command, string remainingline, string last, string last2);
        void comando();

    };

#endif //TRABALHOPRATICO_UI_H
