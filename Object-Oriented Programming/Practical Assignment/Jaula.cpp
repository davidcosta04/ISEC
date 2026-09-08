#include "Jaula.h"
#include <iostream>
#include <sstream>
using namespace std;

int Jaula::contador = 0;

Jaula::Jaula(const int &x, const int &y):Itens(x,y, "j"+to_string(++contador)){
    cout << "Jaula criada" << endl;
}
void Jaula::setAcao(Simulador *s) {
    bool cima,baixo,direita,esquerda;
    for(Caravana *c: s->getCaravanas()){
        if(c->getTipoCaravana()!="Comercio")
            return;//so as de comercio comem os itens
        cima = c->getX() == getX()-1 && c->getY() == getY();
        baixo = c->getX() == getX()+1 && c->getY() == getY();
        direita = c->getX() == getX() && c->getY() == getY()+1;
        esquerda = c->getX() == getX() && c->getY() == getY()-1;
        if(cima || baixo || direita || esquerda){
            c->setTripulantes(c->getTripulantes()+10);
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
            b.setBarbaros(b.getBarbaros()+10);
            s->removeItem(this);
            return;
        }
    }
}
Jaula::~Jaula(){
    cout << "Jaula destruida" << endl;
}
