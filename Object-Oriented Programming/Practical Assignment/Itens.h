#ifndef TRABALHOPRATICO_ITENS_H
#define TRABALHOPRATICO_ITENS_H

using namespace std;
#include <iostream>

#include "Caravana.h"
class Simulador;
class Itens{

char simbolo;
string id;
int x,y;
int duracao;

public:
    Itens(const int &x, const int &y, const string &id);

    Itens(const Itens &i);

    Itens &operator=(const Itens &i);

    int getDuracao();

    void decrementaDuracao();

    char getSimbolo()const;

    int getX()const;

    int getY()const;

    void setX(const int &x);

    void setY(const int &y);


   virtual void setAcao(Simulador *s)= 0; // definir para cada item a sua acao


    virtual ~Itens();

    string getID()const;

    void setID(const string &id);

    string listaItens();
};

ostream &operator<<(ostream &os, Itens &i);

#endif //TRABALHOPRATICO_ITENS_H
