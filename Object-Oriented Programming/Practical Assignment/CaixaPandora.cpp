using namespace std;
#include <iostream>
#include "CaixaPandora.h"
#include <sstream>
int CaixaPandora::contador = 0;

CaixaPandora::CaixaPandora(const int &x, const int &y):Itens(x,y,"cp"+to_string(++contador)){
     //cout<<"Caixa Pandora criada"<<endl;
}

void CaixaPandora::setAcao(Simulador *s){

    bool cima,baixo,direita,esquerda;
    for(Caravana *c: s->getCaravanas()){
        if(c->getTipoCaravana()!="Comercio")
            return;//so as de comercio comem os itens
        cima = c->getX() == getX()-1 && c->getY() == getY();
        baixo = c->getX() == getX()+1 && c->getY() == getY();
        direita = c->getX() == getX() && c->getY() == getY()+1;
        esquerda = c->getX() == getX() && c->getY() == getY()-1;
        if(cima || baixo || direita || esquerda){
            c->setTripulantes(static_cast <int>(c->getTripulantes()*0.8));
            s->removeItem(this);
            return;
        }
    }

}

CaixaPandora::~CaixaPandora(){
    cout<<"Caixa Pandora destruida"<<endl;
}
