#ifndef TRABALHOPRATICO_ARCATESOURO_H
#define TRABALHOPRATICO_ARCATESOURO_H

#include "Itens.h"
#include "Caravana.h"
#include "Simulador.h"
class ArcaTesouro:public Itens{
    static int contador;

public:
    ArcaTesouro(const int &x, const int &y);

    void setAcao(Simulador *s)override;
    ~ArcaTesouro();
};

#endif //TRABALHOPRATICO_ARCATESOURO_H
