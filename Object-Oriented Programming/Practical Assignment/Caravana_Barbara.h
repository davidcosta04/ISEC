//
// Created by rodri on 05/12/2024.
//

#ifndef TRABALHOPRATICO_CARAVANA_BARBARA_H
#define TRABALHOPRATICO_CARAVANA_BARBARA_H
#include <string>
using namespace std;

class Simulador; //inventei por causa dos conflitos dos includes

//Fiz uma classe independente da Caravana pq a prof disse que era melhor
class Barbara{
    int x, y;
    char simbolo;
    string id;
    int barbaros;
    bool esta_morto;
    int InstantesRestantes;

    int quantidade_agua;
    int max_agua;



public:
    Barbara(const int &x, const int &y, string &id, const char &simbolo='!');
    int getX() const;
    int getY() const;

    Barbara(const Barbara &b);

    int getAgua() const;
    int getBarbaros() const;

    void setAgua(int valor);
    void setBarbaros(int valor);
    bool getEstaMorto() const;
    void setX(int valor);
    void setY(int valor);
    void setDuracaoBarbara(int valor);
        string getID()const;
    void move_Com_Coordenadas(const string& direcao, int maxLinhas, int maxColunas);
    void MoveAleatorio(Simulador* s);
    bool TempestadeAreia();
    void DeslocamentoAutonomo(Simulador* s);
    void DecrementaCoisasPorInstante();
    void TrataMovimentoAleatorio(Simulador* s);
        //mais coisas
};


#endif //TRABALHOPRATICO_CARAVANA_BARBARA_H
