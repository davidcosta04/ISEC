//
// Created by david on 19/11/2024.
//

#ifndef TRABALHOPRATICO_SIMULADOR_H
#define TRABALHOPRATICO_SIMULADOR_H

#include "Cidade.h"
#include "Caravana.h"
#include "Buffer.h"
#include "Caravana_Barbara.h"
#include "Caravana_Comercio.h"
#include "Caravana_Militar.h"
#include "Caravana_Secreta.h"
#include <memory>
#include <vector>
#include "Itens.h"
#include "ArcaTesouro.h"
#include "CaixaPandora.h"
#include "Jaula.h"
#include "Mina.h"
#include "Surpresa.h"

class Simulador{

    int linhas,colunas;

    bool configurado;
    int moedas,instantes_entre_novos_itens,duracao_item,max_itens,preco_venda_mercadoria,preco_compra_mercadoria,preco_caravana,instantes_entre_novos_barbaros,duracao_barbaros, instantes_passados, combates_ganhos;


    vector<Cidade> cidades;
    vector<Caravana *> caravanas;//ponteiro para caravana para poder fazer new tipo_caravana
    vector<Barbara> barbaras; //barbaras a parte das caravanas como foi aconselhado
    vector<char> guardaSimbolosCaravanas;//uso isto para voltar a usar simbolos das caravanas uma vez q so consigo usar numeros ate 9 (1 char)
    unique_ptr<Buffer> b;
    std::vector<std::unique_ptr<Buffer>> copiasBuffer; //guardar copias do buffer

    vector<Itens*> itens;


public:
    Simulador();

    bool configSimulador(string);

    bool getConfigured()const;

    int getLinhas()const;
    int getColunas()const;

    int getInstantesPassados() const;
    int setIncrementaInstantesPassados(int valor);
    const vector<Caravana *> &getCaravanas() const;
    const vector<Itens *> &getItens() const;

    void setCaravanas(const vector<Caravana *> &caravanas);

    void setMoedas(int moedas);

    vector<Barbara> &getBarbaras();

        void setBarbaras(const vector<Barbara> &barbaras);

    char getSimbolo(const int &x, const int &y) const;
    int getMoedas() const;

    void atualizarPrecosCidades(int precoCompra, int precoVenda, int precoCaravana);
    void MostraMapaAtualizado();

        void moverCaravana(char id, const string &direcao, const string tipo_move);
    bool VerificaObstaculos(char id, const string &direcao);
    void MostraCaravana(char s);
    void AdicionaCaravana(int x, int y, char simbolo, char tipo);
    bool VerificaCaravanaEstaOuVaiSairCidade(char id);
    bool VerificaSeCaravanaEntrouEmCidade(char id);
        void AdicionaCidade(int x, int y, char simbolo);
    void listarCaravanasCidade(char simboloCidade) const;
    int getNumCaravanas() const;
    void getPrecosCidade();
    void VendeTodaMercadoriaNaCidade(char simbolo);
    void CompraMercadoriaNaCidade(char simbolo, int toneladas);
    void CompraTripulantesNaCidade(char simbolo, int quantidade);
    void CompraCaravanaNaCidade(char simbolo_cidade, char tipo_caravana);
    bool AdicionaBarbara(int x, int y);
        void ShowBuffer();
    bool VerificaPosicaoDisponivelBuffer(int &x, int &y);
    void TempestadeAreia(int linha, int coluna, int raio);
    int removerCaravana(Caravana* caravana);
    int removerBarbara(Barbara &barbara);
    void TrataComandosAutomaticos(int nr_instantes);
    void ColocaAuto(char sim);
    void RetiraAuto(char sim);
    int ContaCaravanasEstadoAuto();
    void moverBarbara(string &id, const string &direcao);
    bool VerificaBarbaraPodeMovimentar(string &id, const string &direcao);
    void atualizarDuracaoBarbaros(int valor);
    void ApareceBarbara();
    vector<Cidade> &getCidades();
        void LimpaBuffer();
    void DadosFim();
    void AdicionaMoedas(int quantidade);

    bool terminaPrograma() const;//sair
    bool terminaSimulacao();//terminar


    /*Combate*/
    void Combate(Caravana* caravana, Barbara& barbara);
    void VerificaCombates();

    /*Itens*/
    void AdicionaItens(int x, int y);
    void AdicionaItensAleatorios();
    void RemoveItensExpirados();
    void setAcaoItens();
    void removeItem(Itens *item);

    /*Copias Buffer*/

    bool guardaCopiaBuffer(const string &);//saves nomecopia
    bool listaCopiasBuffer();//lists
    bool mostraCopiaBuffer(const string &);//loads nomecopia
    int procuraNomeCopia(const string &);//funcao auxiliar para nao inserir copias com o mesmo nome
    bool eliminaCopiaBuffer(const string &);//dels nomecopia

    ~Simulador();//destrutor
    void LimpaSimulador();
};

#endif //TRABALHOPRATICO_SIMULADOR_H
