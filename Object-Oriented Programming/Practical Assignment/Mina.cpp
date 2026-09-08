
using namespace std;
#include <iostream>
#include "Mina.h"
#include <sstream>

int Mina::contador = 0;

Mina::Mina(const int &x, const int &y):Itens(x,y,"m"+to_string(++contador)) {
    cout << "Mina criada" << endl;
}

void Mina::setAcao(Simulador *s) {
    bool cima,baixo,direita,esquerda;
    for(Caravana *c: s->getCaravanas()){
        if(c->getTipoCaravana()!="Comercio")
            return;//so as de comercio comem os itens
        cima = c->getX() == getX()-1 && c->getY() == getY();
        baixo = c->getX() == getX()+1 && c->getY() == getY();
        direita = c->getX() == getX() && c->getY() == getY()+1;
        esquerda = c->getX() == getX() && c->getY() == getY()-1;
        if(cima || baixo || direita || esquerda){
            s->removerCaravana(c);
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
            s->removerBarbara(b);
            s->removeItem(this);
            return;
        }
    }
}

Mina::~Mina() {
    cout << "Mina destruida" << endl;
}