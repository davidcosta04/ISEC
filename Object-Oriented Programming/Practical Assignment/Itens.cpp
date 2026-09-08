#include <iostream>
#include <sstream>

using namespace std;
#include "Itens.h"

Itens::Itens(const int &x, const int &y,const string &id):simbolo('&'),x(x), y(y),id(id),duracao(20){
    //cout<<"Item criado"<<endl;
}

Itens::Itens(const Itens &i){
    x=i.x;
    y=i.y;
    id=i.id;
    simbolo=i.simbolo;
    duracao=i.duracao;
}

ostream &operator<<(ostream &os, Itens &i){
    os<<i.listaItens();
    return os;
}

Itens &Itens::operator=(const Itens &i){
    if(this==&i)
        return *this;
    x=i.x;
    y=i.y;
    id=i.id;
    simbolo=i.simbolo;
    duracao=i.duracao;
    return *this;
}

string Itens::getID()const{
    return this->id;
}

void Itens::setID(const string &new_id){
    this->id = new_id;
}

string Itens::listaItens(){
    return "ID: "+this->id+" "+" "+ "Duracao" + " "+to_string(this->duracao)+" "+" "+"Simbolo:" + " "+this->simbolo+" "+" "+"Posicao(x,y):" +to_string(this->x)+","+to_string(this->y);
}

char Itens::getSimbolo()const{
    return simbolo;
}

void Itens::decrementaDuracao(){
    duracao--;
}

int Itens::getDuracao(){
    return duracao;
}

int Itens::getX()const{
    return x;
}

int Itens::getY()const{
    return y;
}

void Itens::setX(const int &x){
    this->x = x;
}

void Itens::setY(const int &y){
    this->y = y;
}

Itens::~Itens(){
    cout<<"Item destruido"<<endl;
}
