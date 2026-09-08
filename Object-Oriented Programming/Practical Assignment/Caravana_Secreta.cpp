//
// Created by rodri on 04/12/2024.
//

#include "Caravana_Secreta.h"
#include "Simulador.h"

Secreta::Secreta(const int &x, const int &y, const char &simbolo): Caravana(x,y,simbolo,40,600,10,80,800,25) {
    //ESTA É SECRETA TEMOS QUE DEFINIR AS COISAS DELA
    //podem faltar coisas, a medida q se faz vai se vendo
}

Secreta* Secreta::clone(const int &x, const int &y, const char simbolo) const{
    return new Secreta(x,y,simbolo);
}

Secreta::Secreta(const Secreta &s): Caravana(s) {
    Caravana::copia(s);
}

string Secreta::getTipoCaravana() const{
    return "Secreta";
}

void Secreta::lista_caravana() const{
    cout << "Tipo de caravana: " << "Secreta" << endl;
    Caravana::lista_caravana();
}

bool Secreta::TempestadeAreia() {//ESTA IGUAL A MILITAR MUDAR AS COISAS COMO ACHARMOS INTERESSANTE :))
    int tripulantes = getTripulantes();

    if (rand() % 100 < 33) {//33% prob destruicao
        return true; // Caravana destruida
    }

    //Se nao destruir perde 10% dos tripulantes
    int novosTripulantes = tripulantes * 0.90; //retira 10%
    setTripulantes(novosTripulantes);
    return false; // Caravana sobreviveu
}


void Secreta::DecrementaCoisasPorInstante(){
    //FIM DO INSTANTE
    setQuantidadeAgua(getQuantidadeAgua()-5);

    if(getQuantidadeAgua()<0)
        setQuantidadeAgua(0);
}

void Secreta::TrataMovimentoAleatorio(Simulador* s){
    DeslocamentoAutonomo(s);
    DecrementaCoisasPorInstante();
}


//MUDEI AS CENAS, SE NAO TIVER TRIPULANTES MORRE LOGO
void Secreta::DeslocamentoAutonomo(Simulador* s){
    if(!isEstadoAuto())
        return; // Se não estiver em autónomo, não faz nada

    if(getTripulantes() == 0) { // Se não tiver tripulantes, morre logo
        DeslocamentoSemTripulantes(s);
        return;
    } else {
        Cidade* CidadeEscolhida = nullptr;
        int diferenca_linha = 0, diferenca_coluna = 0;
        int flag = 0;

        for (Cidade& cidade : s->getCidades()) {
            if (cidade.getX() == getX() && cidade.getY() == getY()) {
                return; //sai porque ja esta dentro da cidade
            }

            if (flag == 0) {
                CidadeEscolhida = &cidade;
                diferenca_linha = abs(cidade.getX() - getX());
                diferenca_coluna = abs(cidade.getY() - getY());
                flag = 1; // Só na primeira vez, recebe os valores
            }

            // Se a cidade estiver mais próxima, atualiza os valores
            if (abs(cidade.getX() - getX()) < diferenca_linha &&
                abs(cidade.getY() - getY()) < diferenca_coluna) {
                diferenca_linha = abs(cidade.getX() - getX());
                diferenca_coluna = abs(cidade.getY() - getY());
                CidadeEscolhida = &cidade;
            }
        }

        if (diferenca_linha <= 3 || diferenca_coluna <= 3) { // Considere até uma distância de 3
            // Mover em direção à cidade mais próxima, se tiver a uma distancia de 1 vai entrar na cidade
            if (CidadeEscolhida->getX() > getX()) {
                s->moverCaravana(getSimbolo(), "B", "persegue");
                return;
            } else if (CidadeEscolhida->getX() < getX()) {
                s->moverCaravana(getSimbolo(), "C", "persegue");
                return;
            }

            if (CidadeEscolhida->getY() > getY()) {
                s->moverCaravana(getSimbolo(), "D", "persegue");
                return;
            } else if (CidadeEscolhida->getY() < getY()) {
                s->moverCaravana(getSimbolo(), "E", "persegue");
                return;
            }
        }
        MoveAleatorio(s);//se nao fizer nada move aleatorio
    }
}

 void Secreta::DeslocamentoSemTripulantes(Simulador* s){
     setEstaMorto(true);
}
