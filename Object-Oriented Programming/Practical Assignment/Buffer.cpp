#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <array>
#include<random>
#include <fstream>

using namespace std;
#include "Buffer.h"


Buffer::Buffer(int lin, int col):cursor_l(0),buffer(nullptr), cursor_c(0),linhas(lin), colunas(col), criado(true){


    buffer = new char*[linhas];
    for(int i=0;i<linhas;++i){
        buffer[i] = new char[colunas];
    }
}

ostream &operator<<(ostream &os,Buffer &b){
    ostringstream oss;
    oss<<"Linhas: "<<b.getLinhas()<<endl;
    oss<<"Colunas: "<<b.getColunas()<<endl;
    for (int i = 0; i < b.getLinhas(); i++) {
        for (int j = 0; j < b.getColunas(); j++) {
            oss<<b.getChar(i,j);
        }
        oss<<endl;
    }
    os<<oss.str();
    return os;
}





void Buffer::imprimeBuffer(){


    ostringstream oss;
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                oss<<getChar(i,j);
            }
            oss<<endl;
        }
        cout<<oss.str()<<endl;

}

string Buffer::getNomeCopia() const{
    return nomecopia;
}

Buffer::Buffer(const Buffer &b, const string &copia){
    nomecopia = copia;

    if(this == &b){
        return;
    }

    linhas = b.linhas;
    colunas = b.colunas;

    buffer = new char*[linhas];
    for(int i=0;i<linhas;++i){
        buffer[i] = new char[colunas];
    }

    for(int i=0;i<linhas;++i){
        for(int j=0;j<colunas;++j){
            buffer[i][j] = b.buffer[i][j];
        }
    }


}




void Buffer::clearBuffer() {



    for(int i=0;i<linhas;++i){
        for(int j=0;j<colunas;++j){
            buffer[i][j]=' ';
        }
    }
}





char & Buffer::getChar(const int &x, const int &y){ // o simulador vai usar esta funcao para preencher o buffer

        if(x<0 || x >=linhas || y<0 || y>=colunas){
            throw invalid_argument("Coordenadas invalidas");
        }
    return buffer[x][y];
}

char & Buffer::setChar(const int &x, const int &y, char valor){ // o simulador vai usar esta funcao para alterar o buffer

    return buffer[x][y] = valor;
}


void Buffer::mudaCursor(const int &linha,const int &coluna){


    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas) {
        cursor_l = linha;
        cursor_c = coluna;
    } else {
        throw invalid_argument("Coordenadas inválidas");
    }

}

char Buffer::printCharCursor() const {
    return buffer[cursor_l][cursor_c+1];
}






int Buffer::getLinhas() const {
    return linhas;
}

int Buffer::getColunas() const {
    return colunas;
}








Buffer::~Buffer(){

        for (int i = 0; i < linhas; i++) {
            delete[]  buffer[i];
        }
        delete []buffer;
}








