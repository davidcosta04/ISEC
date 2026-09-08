//
// Created by rodri on 04/12/2024.
//

#include <vector>
#include <memory>
#include "Caravana_Comercio.h"
#include "Simulador.h"

Comercio::Comercio(const int &x, const int &y, const char &simbolo): Caravana(x,y,simbolo,10,70,20,40,200,40){
    //podem faltar coisas, a medida q se faz vai se vendo
}

Comercio* Comercio::clone(const int &x,const int&y, const char simbolo)const{
    return new Comercio(x,y,simbolo);
}

Comercio::Comercio(const Comercio &c): Caravana(c){
    Caravana::copia(c);
}

string Comercio::getTipoCaravana() const{
    return "Comercio";
}

void Comercio::lista_caravana() const{
    cout << "Tipo de caravana: " << "Comercio" << endl;
    Caravana::lista_caravana();

}

bool Comercio::TempestadeAreia() {
    int capacidadeAtual = getMercadoria();
    int capacidadeMaxima = getMaxMercadoria();

    if (capacidadeAtual > capacidadeMaxima / 2) {//se a mercadoria for maior q 50% do total
        if (rand() % 100 < 50) {//50% prob destruicao
            return true; // Caravana destruida
        }
    } else {
        if (rand() % 100 < 25) {// 25% prob de ser destruida caso mercadoria menor q metade do total
            return true; // Caravana destruída
        }
    }

    //Se nao destruir perde 25% da mercadoria
    int novaCarga = capacidadeAtual * 0.75; //retira 25%
    setMercadoria(novaCarga);
    return false; // Caravana sobreviveu
}

void Comercio::DecrementaCoisasPorInstante(){
    //fim do instante
    if(getTripulantes()<getMaxTripulantes()/2){
        setQuantidadeAgua(getQuantidadeAgua()-1);
    }else{
        setQuantidadeAgua(getQuantidadeAgua()-2);
    }
    if(getQuantidadeAgua()<0)
        setQuantidadeAgua(0);
    if(getQuantidadeAgua()==0)
        setTripulantes(getTripulantes()-1);
    if(getTripulantes()<0)
        setTripulantes(0);
}

void Comercio::TrataMovimentoAleatorio(Simulador* s){
    DeslocamentoAutonomo(s);
    DecrementaCoisasPorInstante();
}

void Comercio::DeslocamentoAutonomo(Simulador* s){
    if(!isEstadoAuto())
        return;//se nao estiver em autonomo nao faz nada
     int aleatorio = rand() % 2+1; // numero aleatorio entre 1 2, sao os movimentos por instante
          for(int i=0;i<aleatorio;i++) {
              Caravana *CaravanaEscolhida=nullptr;
              int diferenca_linha = 0, diferenca_coluna = 0, flag = 0;
              // VARIAVEIS PARA OS ITENS
              int diferenca_linha_item=0, diferenca_coluna_item=0, flag_item=0;
              Itens *ItemEscolhido=nullptr;

              if (getTripulantes() == 0) {//se nao tiver tripulantes move aleatorio diz queeee
                  DeslocamentoSemTripulantes(s);
              }else {
                  for (Caravana *outraCaravana: s->getCaravanas()) {//tenho acesso as caravanas do simulador
                      if (outraCaravana == this) {
                          continue; // Ignora a propria caravana para nao se comparar a ela
                      }

                      if (flag == 0) {
                          CaravanaEscolhida = outraCaravana;
                          diferenca_linha = abs(outraCaravana->getX() - getX());
                          diferenca_coluna = abs(outraCaravana->getY() - getY());
                          flag = 1;//so na primeira vez recebe os valores
                      }

                      if (abs(outraCaravana->getX() - getX()) < diferenca_linha &&
                          abs(outraCaravana->getY() - getY()) < diferenca_coluna) {
                          diferenca_linha = abs(
                                  outraCaravana->getX() -
                                  getX());//apenas atualiza valores se houver caravana mais proxima
                          diferenca_coluna = abs(outraCaravana->getY() - getY());
                          CaravanaEscolhida = outraCaravana;
                      }
                  }
                  if (diferenca_linha == 1 && diferenca_coluna == 1)
                      return;//ja esta ao lado de uma caravana entao nao mexe

                  if (diferenca_linha == 2 ||
                      diferenca_coluna == 2) { // Considerei aproximar a caravana que esteja ate uma distancia de 2
                      if (CaravanaEscolhida->getX() > getX()) {
                          s->moverCaravana(getSimbolo(), "B", "persegue");
                          return;
                      } else if (CaravanaEscolhida->getX() < getX()) {
                          s->moverCaravana(getSimbolo(), "C", "persegue");
                          return;
                      }

                      if (CaravanaEscolhida->getY() > getY()) {
                          s->moverCaravana(getSimbolo(), "D", "persegue");
                          return;
                      } else if (CaravanaEscolhida->getY() < getY()) {
                          s->moverCaravana(getSimbolo(), "E", "persegue");
                          return;
                      }
                  }
                  //VERIFICAR OS ITENS !!!!
                  for (Itens *item: s->getItens()) {
                      if (flag_item == 0) {
                          ItemEscolhido = item;
                          diferenca_linha_item = abs(item->getX() - getX());
                          diferenca_coluna_item = abs(item->getY() - getY());
                          flag_item = 1;//so na primeira vez recebe os valores
                      }

                      if (abs(item->getX() - getX()) < diferenca_linha_item &&
                          abs(item->getY() - getY()) < diferenca_coluna_item) {
                          diferenca_linha_item = abs(
                                  item->getX() -
                                  getX());//apenas atualiza valores se houver caravana mais proxima
                          diferenca_coluna_item = abs(item->getY() - getY());
                          ItemEscolhido = item;
                      }
                  }
                  //ISTO SERVE PARA ELE APROXIMAR ATE AO ITEM CASO ESTEJA A UMA DISTANCIA DE 2
                  if (diferenca_linha_item == 2 || diferenca_coluna_item == 2) { // Considerei aproximar a caravana que esteja ate uma distancia de 2
                      if (ItemEscolhido->getX() > getX()) {
                          s->moverCaravana(getSimbolo(), "B", "persegue");
                          return;
                      } else if (ItemEscolhido->getX() < getX()) {
                          s->moverCaravana(getSimbolo(), "C", "persegue");
                          return;
                      }

                      if (ItemEscolhido->getY() > getY()) {
                          s->moverCaravana(getSimbolo(), "D", "persegue");
                          return;
                      } else if (ItemEscolhido->getY() < getY()) {
                          s->moverCaravana(getSimbolo(), "E", "persegue");
                          return;
                      }
                  }
                  MoveAleatorio(s);//move aleatorio se nao fizer nd
              }
          }
    }
void Comercio::DeslocamentoSemTripulantes(Simulador* s){
    setInstantesSemTripulantes(getInstantesSemTripulantes()+1);//incrementa os instantes
    if (getInstantesSemTripulantes() >= 5) {
        setEstaMorto(true); // morrer
        return;
    }
    //Sem tripulantes nao gasta agua entao nao faz nada
    // Movimentacao random
    MoveAleatorio(s);
}

