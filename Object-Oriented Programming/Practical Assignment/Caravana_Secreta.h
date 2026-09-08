//
// Created by rodri on 04/12/2024.
//

#ifndef TRABALHOPRATICO_CARAVANA_SECRETA_H
#define TRABALHOPRATICO_CARAVANA_SECRETA_H
#include "Caravana.h"

class Secreta : public Caravana {
public:
    Secreta(const int &x, const int &y, const char &simbolo);
    Secreta* clone(const int &x, const int &y, const char simbolo) const override;
    string getTipoCaravana() const;
    Secreta(const Secreta &s);
    void lista_caravana() const override;
    bool TempestadeAreia() override;
    void DeslocamentoAutonomo(Simulador* s) override;
    void DeslocamentoSemTripulantes(Simulador* s) override;
    virtual void DecrementaCoisasPorInstante() override;
    virtual void TrataMovimentoAleatorio(Simulador* s) override;
    // COLOCAR AQUI FUNCOES QUE SEJAM DIFERENTES A CADA TIPO DE CARAVANA, MOVER AUTONOMO VAI SER DIFERENTE POR EXEMPLO
};


#endif //TRABALHOPRATICO_CARAVANA_SECRETA_H
