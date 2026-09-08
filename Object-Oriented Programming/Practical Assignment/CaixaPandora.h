#ifndef TRABALHOPRATICO_CAIXAPANDORA_H
#define TRABALHOPRATICO_CAIXAPANDORA_H

using namespace std;
#include <iostream>
#include "Itens.h"
#include "Simulador.h"

class CaixaPandora: public Itens{

    static int contador;

public:
    CaixaPandora(const int &x, const int &y);
    void setAcao(Simulador *s)override;

    ~CaixaPandora();
};

#endif //TRABALHOPRATICO_CAIXAPANDORA_H
