#include "UI.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

UI::UI(Simulador &s1):s(s1){
    comandos.insert(pair<string,int>("config",1));
    comandos.insert(pair<string,int>("sair",2));
    comandos.insert(pair<string,int>("cidade",3));
    comandos.insert(pair<string,int>("move",4));
    comandos.insert(pair<string,int>("caravana",5));
    comandos.insert(pair<string,int>("terminar",6));
    comandos.insert(pair<string,int>("precos",7));
    comandos.insert(pair<string,int>("moedas",8));
    comandos.insert(pair<string,int>("vende",9));
    comandos.insert(pair<string,int>("compra",10));
    comandos.insert(pair<string,int>("tripul",11));
    comandos.insert(pair<string,int>("comprac",12));
    comandos.insert(pair<string,int>("barbaro",13));
    comandos.insert(pair<string,int>("areia",14));
    comandos.insert(pair<string,int>("auto",15));
    comandos.insert(pair<string,int>("stop",16));
    comandos.insert(pair<string,int>("prox",17));
    comandos.insert(pair<string,int>("saves",18));
    comandos.insert(pair<string,int>("loads",19));
    comandos.insert(pair<string,int>("lists",20));
    comandos.insert(pair<string,int>("dels",21));
    comandos.insert(pair<string,int>("exec",22));
}

void UI::comando(){
    string command, remainingline,last,last2;
    do {

        command.clear();
        remainingline.clear();
        last.clear();
        last2.clear(); //Limpa todas as variaveis para garantir que nao acumular lixo para futuros comandos

        cout << "Insira um comando:"<<endl;
        getline(cin, command);
        istringstream is(command);
        is >> command >> remainingline>>last>>last2;

        verifica_comandos(command,remainingline,last,last2);

    }while(true);
}

void UI::exec(const string &filename){

    if(filename.empty()) {
        cout << "Indique o nome do ficheiro" << endl;
        return;
    }


    string command, remainingline,last,last2;
    ifstream file(filename);

    if(!file){
        cout<<"Ficheiro"<<filename<< "nao encontrado"<<endl;
        return;
    }

    string linha;
    while(getline(file,linha)){
        command.clear();
        remainingline.clear();
        last.clear();
        last2.clear(); //Limpa todas as variaveis para garantir que nao acumular lixo para futuros comandos

        istringstream is(linha);
        is >> command >> remainingline>>last>>last2;
        verifica_comandos(command,remainingline,last,last2);
    }
    file.close();
}

void UI::verifica_comandos(string command, string remainingline, string last, string last2){
    auto it = comandos.find(command);
    if (it != comandos.end()) {

        switch (it->second) {
            case 1://config nomeficheiro
                s.configSimulador(remainingline);
                break;
            case 2://sair
                s.terminaPrograma();
                break;
            case 3://cidade info_caravanas
                s.listarCaravanasCidade(remainingline[0]);
                break;
            case 4://move caravana direcao
                s.moverCaravana(remainingline[0], last, "user");
                break;

            case 5://info caravana
                s.MostraCaravana(remainingline[0]);
                break;
            case 6://terminar
                s.terminaSimulacao();
                break;
            case 7://precos
                s.getPrecosCidade();
                break;
            case 8://moedas += n
                try {
                    s.AdicionaMoedas(stoi(remainingline));
                }catch(...){
                    cout<<"Insira um inteiro"<<endl;
                }
                break;
            case 9://vende mercadoria da caravana
                s.VendeTodaMercadoriaNaCidade(remainingline[0]);
                break;
            case 10://compra mercadoria para a caravana
                try {
                    s.CompraMercadoriaNaCidade(remainingline[0], stoi(last));
                }catch(...){
                    cout<<"Insira um inteiro"<<endl;
                }
                break;

            case 11://compra tripulantes para a caravana
                try {
                    s.CompraTripulantesNaCidade(remainingline[0], stoi(last));
                }catch(...){
                    cout<<"Insira um inteiro"<<endl;
                }
                break;

            case 12://compra caravana do tipo x na cidade y
                s.CompraCaravanaNaCidade(remainingline[0], last[0]);
                break;

            case 13://barbaro na posicao x y com duracao z
                try {
                    s.AdicionaBarbara(stoi(remainingline), stoi(last));
                }catch(...){
                    cout<<"Insira um inteiro nos dois parametros (x,y)"<<endl;
                }
                break;

            case 14://tempestade de areia na posicao x y com raio z
                try {
                    s.TempestadeAreia(stoi(remainingline), stoi(last), stoi(last2));
                }catch(...){
                    cout<<"Insira um inteiro nos tres parametros"<<endl;
                }
                break;

            case 15://auto numcaravana
                s.ColocaAuto(remainingline[0]);
                break;

            case 16://stop numcaravana
                s.RetiraAuto(remainingline[0]);
                break;

            case 17: {//prox
                if(remainingline.empty())
                    remainingline="1";
                s.TrataComandosAutomaticos(stoi(remainingline));
                break;
            }

            case 18://saves nomecopia
                s.guardaCopiaBuffer(remainingline);
                break;
            case 19://loads nomecopia
                s.mostraCopiaBuffer(remainingline);
                break;
            case 20://lists
                s.listaCopiasBuffer();
                break;
            case 21://dels nomecopia
                s.eliminaCopiaBuffer(remainingline);
                break;

            case 22://exec nomeficheiro
                exec(remainingline);
                break;

        }
    } else
        cout << "comando invalido\n";
}