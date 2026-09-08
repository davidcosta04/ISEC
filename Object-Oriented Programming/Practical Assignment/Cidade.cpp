#include "Cidade.h"
#include <iostream>

using namespace std;

Cidade::Cidade(const int &x, const int &y, const char &simbolo, int preco_compra, int preco_venda, int preco_caravana):x(x), y(y), simbolo(simbolo),
CaravanaSecretaDisponivel(true), CaravanaMilitarDisponivel(true), CaravanaComercioDisponivel(true){}

char Cidade::getSimbolo() const {
    return simbolo;
}

void Cidade::setPrecoCompra(int preco){
    preco_compra=preco;
}
void Cidade::setPrecoVenda(int preco){
    preco_venda=preco;
}
void Cidade::setPrecoCaravana(int preco){
    preco_caravana=preco;
}

void Cidade::setX(int x) {
    this->x = x;
}


void Cidade::setY(int y) {
    this->y = y;
}

int Cidade::getX() const {
    return this->x;
}

int Cidade::getY() const {
    return this->y;
}

void Cidade::setSimbolo(char sim) {
    simbolo = sim;
}

void Cidade::adicionarCaravana(Caravana *caravana) {
    caravanasNaCidade.push_back(caravana);
}

int Cidade::removerCaravana(Caravana* caravana) {
    vector<Caravana*>::iterator it; // posso usar auto para simplificar esta salganhada
    for (it = caravanasNaCidade.begin(); it != caravanasNaCidade.end(); ++it) {
        if (*it == caravana) {
            caravanasNaCidade.erase(it);
            return 1;
        }
    }
    return 0;
}

int Cidade::Caravana_Esta_Aqui(char simbolo){
    for (int i=0; i< caravanasNaCidade.size();i++) {
        if(caravanasNaCidade[i]->getSimbolo() == simbolo)
            return i;
    }
    return -1;
}

int Cidade::PodeComprarCaravana(char tipo_caravana, int saldo){
    if(saldo>=preco_caravana){
        if (tipo_caravana == 'S' && CaravanaSecretaDisponivel) {
            CaravanaSecretaDisponivel = false;
            return preco_caravana;
        } else if (tipo_caravana == 'M' && CaravanaMilitarDisponivel) {
            CaravanaMilitarDisponivel = false;
            return preco_caravana;
        } else if (tipo_caravana == 'C' && CaravanaComercioDisponivel) {
            CaravanaComercioDisponivel = false;
            return preco_caravana;
        }else {
            cout << "Caravanas disponiveis: " << "Comercio -> " << CaravanaComercioDisponivel;
            cout << ", Secreta -> " << CaravanaSecretaDisponivel;
            cout << ", Militar -> " << CaravanaMilitarDisponivel << endl;
        }
    }
    return -1;
}


void Cidade::listarCaravanas() const {
    if(caravanasNaCidade.size()>0) {
        cout << "Caravanas na cidade " << simbolo << ":" << endl;
    }else{
        cout << "Nao existem caravanas na cidade " << simbolo << "." << endl;
        return;
    }
    for (Caravana *caravana : caravanasNaCidade) {
        cout << "-------------------------------" << endl;
        caravana->lista_caravana();
    }
}

int Cidade::VendeMercadoria(char simbolo) {
    //Vende toda a mercadoria que tem
    if (Caravana_Esta_Aqui(simbolo)>=0) {
        int pos=Caravana_Esta_Aqui(simbolo);
        if(caravanasNaCidade[pos]->getMercadoria()>0) { //se nao tiver mercadoria nao faz nada
            int moedas_extra = caravanasNaCidade[pos]->getMercadoria() * preco_venda;
            caravanasNaCidade[pos]->setMercadoria(0);
            return moedas_extra;
        }else{
            return -1;//nao tem mercadoria
        }
    }
    return 0;//caravana nao esta na cidade
}

int Cidade::CompraMercadoria(char simbolo, int toneladas, int saldo){
    if (Caravana_Esta_Aqui(simbolo)>=0) {
        int pos=Caravana_Esta_Aqui(simbolo);
        int quantidade_atualizada=caravanasNaCidade[pos]->getMercadoria()+toneladas;
        if(quantidade_atualizada>caravanasNaCidade[pos]->getMaxMercadoria()){
            return -1;//excede o maximo de mercadoria
        }else{
            int moedas_a_menos=toneladas*preco_compra;
            if(saldo>=moedas_a_menos) {
                caravanasNaCidade[pos]->setMercadoria(quantidade_atualizada);
                return moedas_a_menos;
            }else{
                return -2; //nao tem moedas disponiveis
            }
        }
    }
    return 0;//caravana nao esta na cidade
}

int Cidade::CompraTripulantes(char simbolo, int quantidade, int saldo){
    if (Caravana_Esta_Aqui(simbolo)>=0) {
        int pos=Caravana_Esta_Aqui(simbolo);
        int quantidade_atualizada=caravanasNaCidade[pos]->getTripulantes()+quantidade;
        if(quantidade_atualizada>caravanasNaCidade[pos]->getMaxTripulantes()){
            return -1;//excede o maximo de tripulantes
        }else{
            int moedas_a_menos=quantidade*1; //cada tripulante custa 1 moeda e nao é configuravel
            if(saldo>=moedas_a_menos) {
                caravanasNaCidade[pos]->setTripulantes(quantidade_atualizada);
                return moedas_a_menos;
            }else{
                return -2; //nao tem moedas disponiveis
            }
        }
    }
    return 0;//caravana nao esta na cidade
}

Cidade::~Cidade(){
    caravanasNaCidade.clear(); // Limpa o vetor para reutilizar
}
