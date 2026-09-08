#include "Surpresa.h"
#include <iostream>
#include <sstream>
using namespace std;

int Surpresa::contador = 0;

Surpresa::Surpresa(const int &x, const int &y):Itens(x,y,"s" + to_string(++contador)) {
    cout << "Surpresa criada" << endl;
}

void Surpresa::setAcao(Simulador *s) {
    bool cima,baixo,direita,esquerda;
    for(Caravana *c: s->getCaravanas()){
        if(c->getTipoCaravana()!="Comercio")
            return;//so as de comercio comem os itens
        cima = c->getX() == getX()-1 && c->getY() == getY();
        baixo = c->getX() == getX()+1 && c->getY() == getY();
        direita = c->getX() == getX() && c->getY() == getY()+1;
        esquerda = c->getX() == getX() && c->getY() == getY()-1;
        if(cima || baixo || direita || esquerda){
            c->setQuantidadeAgua(static_cast <int>(c->getQuantidadeAgua()*1.5));
            s->removeItem(this);
            return;
        }
    }
    for(Barbara &b: s->getBarbaras()){
        cima = b.getX() == getX()-1 && b.getY() == getY();
        baixo = b.getX() == getX()+1 && b.getY() == getY();
        direita = b.getX() == getX() && b.getY() == getY()+1;
        esquerda = b.getX() == getX() && b.getY() == getY()-1;
        if(cima || baixo || direita || esquerda){
            b.setAgua(static_cast <int>(b.getAgua()*1.5));
            s->removeItem(this);
            return;
        }
    }
}

Surpresa::~Surpresa() {
    cout << "Surpresa destruida" << endl;
}
