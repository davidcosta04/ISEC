#include <iostream>
#include "ArcaTesouro.h"
using namespace std;
#include <sstream>

int ArcaTesouro::contador = 0;

ArcaTesouro::ArcaTesouro(const int &x, const int &y):Itens(x,y, "a"+to_string(++contador)) {
    cout << "Arca do Tesouro criada" << endl;
}

void ArcaTesouro::setAcao(Simulador *s){
    bool cima,baixo,direita,esquerda;
    for(Caravana *c: s->getCaravanas()) {
        if (c->getTipoCaravana() != "Comercio")
            return;//so as de comercio comem os itens
        cima = c->getX() == getX() - 1 && c->getY() == getY();
        baixo = c->getX() == getX() + 1 && c->getY() == getY();
        direita = c->getX() == getX() && c->getY() == getY() + 1;
        esquerda = c->getX() == getX() && c->getY() == getY() - 1;
        if (cima || baixo || direita || esquerda) {
            s->setMoedas(static_cast<int>(s->getMoedas() * 1.10));
            s->removeItem(this);
        }
    }
    for(Barbara &b: s->getBarbaras()){
        cima = b.getX() == getX()-1 && b.getY() == getY();
        baixo = b.getX() == getX()+1 && b.getY() == getY();
        direita = b.getX() == getX() && b.getY() == getY()+1;
        esquerda = b.getX() == getX() && b.getY() == getY()-1;
        if (cima || baixo || direita || esquerda) {
            s->setMoedas(static_cast<int>(s->getMoedas() * 1.10));
            s->removeItem(this);
        }
    }
}

ArcaTesouro::~ArcaTesouro() {
    cout << "Arca do Tesouro destruida" << endl;
}