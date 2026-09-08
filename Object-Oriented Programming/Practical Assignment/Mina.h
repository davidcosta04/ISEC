#ifndef TRABALHOPRATICO_MINA_H
#define TRABALHOPRATICO_MINA_H

#include "Itens.h"
#include "Simulador.h"

class Mina:public Itens{
    static int contador;
public:
    Mina(const int &x, const int &y);


    void setAcao(Simulador *s)override;
    ~Mina();
};

#endif //TRABALHOPRATICO_MINA_H
