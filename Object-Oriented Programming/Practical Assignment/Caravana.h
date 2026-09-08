//
// Created by david on 10/11/2024.
//

#ifndef TRABALHOPRATICO_CARAVANA_H
#define TRABALHOPRATICO_CARAVANA_H
#include <iostream>

using namespace std;

class Simulador;

class Caravana{
    //VIRTUAL SERVE PARA PODERMOS ACEDER A ESSA FUNCAO NA CLASSE E SER CHAMADA DIRETAMENTE DO TIPO DE CARAVANA
    char simbolo;
    static int contador;
    int instantesSemTripulantes;
    bool esta_morto, estado_auto;
    int x,y;
    string id="c";//depois concatenamos para cada caravana especifica
    int tripulacao;
    int max_tripulantes;

    int max_peso_mercadoria;
    int peso_mercadoria;
    int quantidade_agua;
    int max_agua;
public:
    Caravana()= default;
    Caravana(const int &x, const int &y, const char &simbolo, int peso_mercadoria=0, int quantidade_agua=0, int tripulacao=0, int max_peso_mercadorias=0, int max_agua=0, int max_tripulantes=0);
    virtual ~Caravana(); // virtual para poder aceder aos tipos de caravanas e apaga las, evitando vazamentos de memoria
    virtual Caravana* clone(const int &x,const int&y, const char simbolo)const=0;
    void copia(const Caravana &c);
    Caravana &operator=(const Caravana &c);
    char getSimbolo() const;
    virtual string getTipoCaravana() const=0; //cada tipo é declarado estáticamente quando sao criadas as caravanas
    string getID() const;
    int getInstantesSemTripulantes() const;

    void setInstantesSemTripulantes(int instantesSemTripulantes);

    bool EstaMorto() const;

    void setEstaMorto(bool estado);
    int getQuantidadeAgua() const;

    void setQuantidadeAgua(int quantidadeAgua);

    void setSimbolo(char simbolo);

    bool isEstadoAuto() const;
    void setEstadoAuto(bool estadoAuto);

        int getX() const;
    int getY() const;
    int getMercadoria()const;
    int getMaxMercadoria() const;
    int getTripulantes()const;
    int getMaxTripulantes()const;
    int getMaxAgua() const;

    void setMercadoria(int quantidade);
    void setTripulantes(int quantidade);
    void setMaxAgua(int quantidade);
    void setX(int x);
    void setY(int y);
    static void setContador(int valor);
    static int getContador();

    void move_Com_Utilizador(const string &direcao, int maxLinhas, int maxColunas);

    void vende_mercadoria(const string &id);

    void compra_mercadoria(const string &id, const int &quantidade);

    virtual void lista_caravana() const;
    virtual bool TempestadeAreia()=0;
    void MoveAleatorio(Simulador* s);//para usar depois
    virtual void DeslocamentoAutonomo(Simulador* s)=0; //PARA USAR PONTEIRO DO SIMULADOR METI LA EM CIMA UMA COISA ESTRANHA NAO SEI SE POSSO SE METER INCLUDE DA CONFLITO
    virtual void DeslocamentoSemTripulantes(Simulador* s)=0;
    virtual void DecrementaCoisasPorInstante()=0;
    virtual void TrataMovimentoAleatorio(Simulador* s)=0;
};

ostream &operator<<(ostream &os, Caravana &c);

#endif //TRABALHOPRATICO_CARAVANA_H
