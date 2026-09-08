//
// Created by david on 16/12/2024.
//

#ifndef TRABALHOPRATICO_SURPRESA_H
#define TRABALHOPRATICO_SURPRESA_H

#include "Itens.h"
#include "Simulador.h"
class Surpresa:public Itens{
    static int contador;

public:
    Surpresa(const int &x, const int &y);

    void setAcao(Simulador *s)override;
    ~Surpresa();
};

#endif //TRABALHOPRATICO_SURPRESA_H
