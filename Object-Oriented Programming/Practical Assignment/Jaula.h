//
// Created by david on 16/12/2024.
//

#ifndef TRABALHOPRATICO_JAULA_H
#define TRABALHOPRATICO_JAULA_H

#include "Itens.h"
#include "Simulador.h"

class Jaula:public Itens{
    static int contador;

public:

    Jaula(const int &x, const int &y);

    void setAcao(Simulador *s)override;
    ~Jaula();
};

#endif //TRABALHOPRATICO_JAULA_H
