#ifndef TRABALHOPRATICO_CIDADE_H
#define TRABALHOPRATICO_CIDADE_H
#include <iostream>
#include <vector>
#include "Caravana.h"

using namespace std;

class Cidade{

     char simbolo;
     int preco_venda, preco_compra, preco_caravana;
    bool CaravanaComercioDisponivel;
    bool CaravanaMilitarDisponivel;
    bool CaravanaSecretaDisponivel;//todas inicializadas a true quando a cidade é criada
    vector<Caravana*> caravanasNaCidade;

    int x,y;



public:
    Cidade(const int &x, const int &y, const char &simbolo, int preco_compra=0, int preco_venda=0, int preco_caravana=0);

    char getSimbolo()const;

    void setSimbolo(char sim);
    void setPrecoCompra(int preco);
    void setPrecoVenda(int preco);
    void setPrecoCaravana(int preco);

    int getX()const;
    int getY()const;


    void setX(int x);
    void setY(int y);

    void lista_precos()const;

    void compra_c(const string &nome_cidade, const char &tipo_caravana);

    void adicionarCaravana(Caravana *caravana);
    int removerCaravana(Caravana* caravana);
    int PodeComprarCaravana(char tipo_caravana, int saldo);
    int Caravana_Esta_Aqui(char simbolo);
    int VendeMercadoria(char simbolo);
    int CompraMercadoria(char simbolo, int toneladas, int saldo);
    int CompraTripulantes(char simbolo, int quantidade, int saldo);
    void listarCaravanas() const;

    ~Cidade();

};

#endif //TRABALHOPRATICO_CIDADE_H
