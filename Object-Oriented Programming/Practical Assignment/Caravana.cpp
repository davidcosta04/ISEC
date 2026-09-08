#include <iostream>
using namespace std;
#include "Caravana.h"
#include "Simulador.h"

//Caravana::Caravana(): simbolo(counter++),x(0),y(0) {}
int Caravana::contador = 1;//comeca em 1

Caravana::Caravana(const int &x, const int &y, const char &simbolo, int peso_mercadoria, int quantidade_agua, int tripulacao, int max_peso_mercadorias, int max_agua, int max_tripulantes): x(x), y(y),
simbolo(simbolo), id("c"+to_string(contador++)),
peso_mercadoria(peso_mercadoria),estado_auto(false), quantidade_agua(quantidade_agua), tripulacao(tripulacao),
max_peso_mercadoria(max_peso_mercadorias), max_agua(max_agua), max_tripulantes(max_tripulantes), instantesSemTripulantes(0), esta_morto(false){}




void Caravana::copia(const Caravana &c){
    x = c.x;
    y = c.y;
    id=c.id;
    tripulacao=c.tripulacao;
    max_tripulantes=c.max_tripulantes;
    max_peso_mercadoria=c.max_peso_mercadoria;
    peso_mercadoria=c.peso_mercadoria;
    quantidade_agua=c.quantidade_agua;
    max_agua=c.max_agua;
    instantesSemTripulantes=c.instantesSemTripulantes;
    esta_morto=c.esta_morto;
    estado_auto=c.estado_auto;
}

Caravana &Caravana::operator=(const Caravana &c) {
    if(this==&c)
        return *this;
    x = c.x;
    y = c.y;
    id=c.id;
    tripulacao=c.tripulacao;
    max_tripulantes=c.max_tripulantes;
    max_peso_mercadoria=c.max_peso_mercadoria;
    peso_mercadoria=c.peso_mercadoria;
    quantidade_agua=c.quantidade_agua;
    max_agua=c.max_agua;
    instantesSemTripulantes=c.instantesSemTripulantes;
    esta_morto=c.esta_morto;
    estado_auto=c.estado_auto;
    return *this;
}

ostream &operator<<(ostream &os, Caravana &c) {
    os << "Caravana: " << c.getID() << " Simbolo: " << c.getSimbolo() << " Tripulacao: " << c.getTripulantes() << " Max Tripulantes: " << c.getMaxTripulantes() << " Peso Mercadoria: " << c.getMercadoria() << " Max Peso Mercadoria: " << c.getMaxMercadoria() << " Quantidade Agua: " << c.getQuantidadeAgua() << " Max Agua: " << c.getMaxAgua() << " Instantes Sem Tripulantes: " << c.getInstantesSemTripulantes() << " Esta Morto: " << c.EstaMorto() << " Estado Auto: " << c.isEstadoAuto();
    return os;
}

char Caravana::getSimbolo() const {
    return simbolo;
}

int Caravana::getContador(){
    return contador;
}

void Caravana::setContador(int valor) {
    contador=valor;
}

int Caravana::getMercadoria() const {
    return peso_mercadoria;
}

int Caravana::getMaxMercadoria() const {
    return max_peso_mercadoria;
}

int Caravana::getTripulantes()const{
    return tripulacao;
}
int Caravana::getMaxTripulantes()const{
    return max_tripulantes;
}

int Caravana::getMaxAgua() const {
    return max_agua;
}

void Caravana::setMaxAgua(int maxAgua) {
    max_agua = maxAgua;
}

void Caravana::setMercadoria(int quantidade) {
    peso_mercadoria=quantidade;
}

void Caravana::setTripulantes(int quantidade) {
    tripulacao=quantidade;
    if(tripulacao<0)
        tripulacao=0;
}

string Caravana::getID() const{
    return id;
}

int Caravana::getX() const {
    return x;
}

int Caravana::getY() const {
    return y;
}

void Caravana::setX(int x) {
   this->x = x;
}

void Caravana::setY(int y) {
    this->y = y;
}

void Caravana::setSimbolo(char sim) {
    this->simbolo = sim;
}

void Caravana::lista_caravana() const {
    cout << "id: " << id << endl;
    cout << "simbolo: " << simbolo << endl;
    cout << "Peso Mercadoria: " << peso_mercadoria << endl;
    cout << "Estado automatico 0=nao, 1=sim -> " << estado_auto << endl;
    cout << "Maximo Peso Mercadoria: " << max_peso_mercadoria << endl;
    cout << "Quantidade de agua: " << quantidade_agua << endl;
    cout << "Maximo quantidade de agua: " << max_agua << endl;
    cout << "Tripulacao: " << tripulacao << endl;
    cout << "Maximo tripulantes: " << max_tripulantes << endl;
    //cout << "mostrar mais cenas se for preciso ... bla bla" << endl;
}

void Caravana::move_Com_Utilizador(const string &direcao, int maxLinhas, int maxColunas) {
    // Verificar as direções e mover a caravana
    if (direcao == "C") { // Cima
        if (x > 0) x = x - 1;
        else x = maxLinhas - 1;
    }
    //direcao=="C"&&x>0?x=x-1:x=maxLinhas-1; FAZER ASSIM
    else if (direcao == "B") { // Baixo
        if (x < maxLinhas - 1) x = x + 1;
        else x = 0;
    }
    else if (direcao == "E") { // Esquerda
        if (y > 0) y = y - 1;
        else y = maxColunas - 1;
    }
    else if (direcao == "D") { // Direita
        if (y < maxColunas - 1) y = y + 1;
        else y = 0;
    }
    else if (direcao == "CE") { // Cima-Esquerda
        if (x > 0) x = x - 1;
        else x = maxLinhas - 1;

        if (y > 0) y = y - 1;
        else y = maxColunas - 1;
    }
    else if (direcao == "CD") { // Cima-Direita
        if (x > 0) x = x - 1;
        else x = maxLinhas - 1;

        if (y < maxColunas - 1) y = y + 1;
        else y = 0;
    }
    else if (direcao == "BE") { // Baixo-Esquerda
        if (x < maxLinhas - 1) x = x + 1;
        else x = 0;

        if (y > 0) y = y - 1;
        else y = maxColunas - 1;
    }
    else if (direcao == "BD") { // Baixo-Direita
        if (x < maxLinhas - 1) x = x + 1;
        else x = 0;

        if (y < maxColunas - 1) y = y + 1;
        else y = 0;
    }
    else {
        cout << "Direcao invalida!" << endl;
    }
}

int Caravana::getInstantesSemTripulantes() const {
    return instantesSemTripulantes;
}

void Caravana::setInstantesSemTripulantes(int instantesSemTripulantes) {
    Caravana::instantesSemTripulantes = instantesSemTripulantes;
}

bool Caravana::EstaMorto() const {
    return esta_morto;
}

void Caravana::setEstaMorto(bool estado) {
    esta_morto = estado;
}

void Caravana::MoveAleatorio(Simulador* s){
    // Movimentacao random
    int direcao = rand() % 4; // 0: Cima, 1: Baixo, 2: Esquerda, 3: Direita

    if (direcao == 0 && x > 0) s->moverCaravana(getSimbolo(), "C", "aleatorio");
    else if (direcao == 1) s->moverCaravana(getSimbolo(), "B", "aleatorio");
    else if (direcao == 2) s->moverCaravana(getSimbolo(), "E", "aleatorio");
    else if (direcao == 3) s->moverCaravana(getSimbolo(), "D", "aleatorio");
}

int Caravana::getQuantidadeAgua() const {
    return quantidade_agua;
}

void Caravana::setQuantidadeAgua(int quantidadeAgua) {
    quantidade_agua = quantidadeAgua;
    if(quantidade_agua<0)
        quantidade_agua=0;

    if(quantidade_agua>max_agua)
        quantidade_agua=max_agua;
}

bool Caravana::isEstadoAuto() const {
    return estado_auto;
}

void Caravana::setEstadoAuto(bool estadoAuto) {
    estado_auto = estadoAuto;
}
Caravana::~Caravana(){}