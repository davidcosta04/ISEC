//
// Created by rodri on 04/12/2024.
//

#include "Caravana_Militar.h"
#include "Simulador.h"

Militar::Militar(const int &x, const int &y, const char &simbolo): Caravana(x,y,simbolo,2,200,15,5,400,40) {
    UltimoMovimento="vazio";
    //podem faltar coisas, a medida q se faz vai se vendo
}

Militar::Militar(const Militar &m): Caravana(m)
{
    Caravana::copia(m);
   UltimoMovimento = m.UltimoMovimento;

}

Militar* Militar::clone(const int &x, const int &y, const char simbolo) const{

    auto *militar = new Militar(x,y,simbolo);
    militar->UltimoMovimento= this->UltimoMovimento;

    return militar;
}

string Militar::getTipoCaravana() const{
    return "Militar";
}

void Militar::lista_caravana() const{
    cout << "Tipo de caravana: " << "Militar" << endl;
    Caravana::lista_caravana();
}

bool Militar::TempestadeAreia() {
    int tripulantes = getTripulantes();

    if (rand() % 100 < 33) {//33% prob destruicao
        return true; // Caravana destruida
    }

    //Se nao destruir perde 10% dos tripulantes
    int novosTripulantes = static_cast<int>(tripulantes * 0.90); //retira 10%
    setTripulantes(novosTripulantes);
    return false; // Caravana sobreviveu
}

void Militar::DecrementaCoisasPorInstante(){
    //fim do instante, vamos atualizar as cenas
    if(getTripulantes()<getMaxTripulantes()/2){
        setQuantidadeAgua(getQuantidadeAgua()-1);
    }else{
        setQuantidadeAgua(getQuantidadeAgua()-3);
    }
    if(getQuantidadeAgua()<0)
        setQuantidadeAgua(0);
    if(getQuantidadeAgua()==0)
        setTripulantes(getTripulantes()-1);
    if(getTripulantes()<0)
        setTripulantes(0);
}

void Militar::TrataMovimentoAleatorio(Simulador* s){
    DeslocamentoAutonomo(s);
    DecrementaCoisasPorInstante();
}

void Militar::DeslocamentoAutonomo(Simulador* s){
    if(!isEstadoAuto())
        return;//se nao estiver em autonomo nao faz nada
    for(int i=0; i<3; i++){//desloca se ate 3 vezes por turno. pois pode haver vezes q fique parado
        Barbara *BarbaraEscolhida;
        int diferenca_linha = 0, diferenca_coluna = 0, flag = 0;
        if (getTripulantes() == 0) {//se nao tiver tripulantes move aleatorio diz queeee
            DeslocamentoSemTripulantes(s);
        }else {

            for (Barbara &outraBarbara: s->getBarbaras()) {//tenho acesso as caravanas do simulador
                if (flag == 0) {
                    BarbaraEscolhida = &outraBarbara;
                    diferenca_linha = abs(outraBarbara.getX() - getX());
                    diferenca_coluna = abs(outraBarbara.getY() - getY());
                    flag = 1;//so na primeira vez recebe os valores
                }

                if (abs(outraBarbara.getX() - getX()) < diferenca_linha &&
                    abs(outraBarbara.getY() - getY()) < diferenca_coluna) {
                    diferenca_linha = abs(
                            outraBarbara.getX() - getX());//apenas atualiza valores se houver caravana mais proxima
                    diferenca_coluna = abs(outraBarbara.getY() - getY());
                    BarbaraEscolhida = &outraBarbara;
                }
            }
            if (diferenca_linha == 1 && diferenca_coluna == 1)return;//ja esta ao lado de uma caravana entao nao mexe

            if (diferenca_linha <= 6 ||
                diferenca_coluna <= 6) { // Considerei perseguir a caravana que esteja ate uma distancia de 6
                if (BarbaraEscolhida->getX() > getX()) {
                    UltimoMovimento = "D";
                    s->moverCaravana(getSimbolo(), "B", "persegue");
                    return;
                } else if (BarbaraEscolhida->getX() < getX()) {
                    UltimoMovimento = "E";
                    s->moverCaravana(getSimbolo(), "C", "persegue");
                    return;
                }

                if (BarbaraEscolhida->getY() > getY()) {
                    UltimoMovimento = "C";
                    s->moverCaravana(getSimbolo(), "D", "persegue");
                    return;
                } else if (BarbaraEscolhida->getY() < getY()) {
                    UltimoMovimento = "B";
                    s->moverCaravana(getSimbolo(), "E", "persegue");
                    return;
                }
            }
        }//FICA PARADA SE NAO HOUVER BARBARAS
    }
}

void Militar::DeslocamentoSemTripulantes(Simulador* s) {
    setInstantesSemTripulantes(getInstantesSemTripulantes() + 1);//incrementa os instantes
    if (getInstantesSemTripulantes() >= 7) {
        setEstaMorto(true); // morrer
        return;
    }
    setQuantidadeAgua(getQuantidadeAgua()-1);//mesmo sem tripulantes gasta agua
    // Ultimo movimento da caravana
    if(UltimoMovimento!="vazio")
        s->moverCaravana(getSimbolo(), UltimoMovimento, "ultimo_movimento");
    //senao fica quieta que foi o seu ultimo movimento
}