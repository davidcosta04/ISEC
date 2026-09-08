//
// Created by david on 09/11/2024.
//

#ifndef TRABALHOPRATICO_BUFFER_H
#define TRABALHOPRATICO_BUFFER_H

#include <iostream>
using namespace std;

class Buffer{
    int cursor_l, cursor_c;
    char **buffer;
    bool criado;

    int linhas, colunas;

    string nomecopia;


public:

    Buffer(int linhas, int colunas);
    void clearBuffer();

    Buffer(const Buffer &b, const string &);

    Buffer &operator<<(int v);


    void imprimeBuffer();
    char & getChar(const int &linha, const int &coluna);
    char & setChar(const int &x, const int &y, char valor);

    void mudaCursor(const int &linha,const int &coluna);
    char printCharCursor()const;

    int getLinhas()const;
    int getColunas()const;

    string getNomeCopia()const;

    bool getCriado()const;



    char setchar(const int &linha,const int &coluna, char &ch);
    int verificaCoordenadas(const int &linha, const int &coluna)const;
    void mudaBuffer(const int &linha, const int &coluna, const char &simbolo);
    ~Buffer();
};

ostream &operator<<(ostream &os,Buffer &b);







#endif //TRABALHOPRATICO_BUFFER_H
