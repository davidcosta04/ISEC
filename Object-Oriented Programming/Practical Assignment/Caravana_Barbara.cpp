//
// Created by rodri on 05/12/2024.
//

#include "Caravana_Barbara.h"
#include "Simulador.h"

Barbara::Barbara(const int &x, const int &y, string &id, const char &simbolo):x(x),y(y),id(id),InstantesRestantes(0),simbolo(simbolo),quantidade_agua(100),max_agua(400),barbaros(40),esta_morto(false){

}

Barbara::Barbara(const Barbara &b){
    x = b.x;
    y = b.y;
    simbolo = b.simbolo;
    id = b.id;
    barbaros = b.barbaros;
    esta_morto = b.esta_morto;
    InstantesRestantes = b.InstantesRestantes;
    quantidade_agua = b.quantidade_agua;
    max_agua = b.max_agua;
}

bool Barbara::getEstaMorto() const{
    return esta_morto;
}
int Barbara::getX() const {
    return x;
}

int Barbara::getY() const {
    return y;
}

void Barbara::setX(int valor){
    x=valor;
}

void Barbara::setY(int valor){
    y=valor;
}

void Barbara::setDuracaoBarbara(int valor){
    InstantesRestantes=valor;
}

string Barbara::getID()const{
    return id;
}

void Barbara::move_Com_Coordenadas(const string& direcao, int maxLinhas, int maxColunas){
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
}


void Barbara::MoveAleatorio(Simulador* s){
    // Movimentacao random
    int direcao = rand() % 4; // 0: Cima, 1: Baixo, 2: Esquerda, 3: Direita

    if (direcao == 0) s->moverBarbara(id, "C");
    else if (direcao == 1) s->moverBarbara(id, "B");
    else if (direcao == 2) s->moverBarbara(id, "E");
    else if (direcao == 3) s->moverBarbara(id, "D");
}

bool Barbara::TempestadeAreia() {
    if (rand() % 100 < 25) {//25% prob destruicao
        return true; // Caravana destruida
    }

    //Se nao destruir perde 10% dos tripulantes
    int novosBarbaros = barbaros * 0.90; //retira 10%
    barbaros=novosBarbaros;
    if(barbaros==0)
        return true;//desaparece se ficar a 0;
    return false; // Caravana sobreviveu
}

void Barbara::TrataMovimentoAleatorio(Simulador* s){
    DeslocamentoAutonomo(s);
    DecrementaCoisasPorInstante();
}

void Barbara::DecrementaCoisasPorInstante(){
    //FIM DO INSTANTE
    InstantesRestantes-=1;//decrementa a vida do barbaro
    if(InstantesRestantes==0)
        esta_morto=true;
}

void Barbara::DeslocamentoAutonomo(Simulador* s){
    Caravana *CaravanaEscolhida;
    int diferenca_linha=0, diferenca_coluna=0, flag=0;
    for (Caravana *outraCaravana: s->getCaravanas()) {//tenho acesso as caravanas do simulador
        if (flag == 0) {
            CaravanaEscolhida = outraCaravana;
            diferenca_linha = abs(outraCaravana->getX() - getX());
            diferenca_coluna = abs(outraCaravana->getY() - getY());
            flag = 1;//so na primeira vez recebe os valores
        }

        if (abs(outraCaravana->getX() - getX()) < diferenca_linha &&
            abs(outraCaravana->getY() - getY()) < diferenca_coluna) {
            diferenca_linha = abs(
                    outraCaravana->getX() - getX());//apenas atualiza valores se houver caravana mais proxima
            diferenca_coluna = abs(outraCaravana->getY() - getY());
            CaravanaEscolhida = outraCaravana;
        }
    }
    if (diferenca_linha == 1 && diferenca_coluna == 1)return;//ja esta ao lado de uma caravana entao nao mexe

    if (diferenca_linha <= 8 ||
        diferenca_coluna <= 8) { // Até 8 posicoes persegue
        if (CaravanaEscolhida->getX() > getX()) {
            s->moverBarbara(id, "B");
            return;
        } else if (CaravanaEscolhida->getX() < getX()) {
            s->moverBarbara(id, "C");
            return;
        }

        if (CaravanaEscolhida->getY() > getY()) {
            s->moverBarbara(id, "D");
            return;
        } else if (CaravanaEscolhida->getY() < getY()) {
            s->moverBarbara(id, "E");
            return;
        }
    }

    MoveAleatorio(s); //caso nao encontre move aleatoio
}

int Barbara::getAgua() const{
    return quantidade_agua;
}

void Barbara::setAgua(int valor){
    quantidade_agua=valor;
    if(quantidade_agua<0)
        quantidade_agua=0;

    if(quantidade_agua>max_agua)
        quantidade_agua=max_agua;
}


int Barbara::getBarbaros() const {
    return barbaros;
}

void Barbara::setBarbaros(int nr_barbaros) {
    barbaros = nr_barbaros;
    if(barbaros<0)
        barbaros=0;
}