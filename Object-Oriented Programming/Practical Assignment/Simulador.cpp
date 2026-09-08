#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;
#include "Simulador.h"

Simulador::Simulador():configurado(false),combates_ganhos(0), instantes_passados(0){}

bool Simulador::configSimulador(string filename) {
    if(configurado){
        cout<<"Simulador ja esta configurado"<<endl;
        return false;
    }
    if(filename.empty()) {
        cout << "Indique o nome do ficheiro" << endl;
        return false;
    }
    ifstream file(filename);
    if (!file) {
        cout << "Nao existe nenhum ficheiro com o nome: " << filename << endl;
        return false;
    }

    string s,f_linhas,f_colunas;
    int nlinhas, ncolunas, n;


    file>>f_linhas>>nlinhas;

    if(nlinhas<=0){
        throw invalid_argument("O numero de linhas do ficheiro deve ser positivo.");
    }
    file>>f_colunas>>ncolunas;

    if(ncolunas<=0){
        throw invalid_argument("O numero de colunas do ficheiro deve ser positivo.");
    }
    linhas = nlinhas;
    colunas = ncolunas;



    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip until the first newline


    b = make_unique<Buffer>(linhas, colunas);

    for (int i = 0; i < linhas && getline(file, s); i++) {


        for (int j = 0; j < colunas; j++) {
            try{
                b->getChar(i,j)=s[j];
                // Adiciona caravanas do mapa na classe atraves de um vetor
                if (isdigit(s[j])) {
                    char SimboloCaravana = s[j];
                    AdicionaCaravana(i,j,SimboloCaravana, '-'); // tipo "-" significa vir do mapa a caravana, sendo o tipo criado de forma aleatoria (nao sei se e assim)
                }
                //barbaras
                if (s[j] == '!') {
                    char SimboloCaravana = s[j];
                    AdicionaCaravana(i,j,SimboloCaravana, 'B');
                }
                //Adiciona cidade se for letras
                if (s[j] >= 'a' && s[j] <= 'z') {
                    char SimboloCidade = s[j];
                    AdicionaCidade(i, j, SimboloCidade);
                }
                //cout << "caravanas " << caravanas.size() << "cidades" << cidades.size() << endl;
            }catch(const std::exception &e){
                cerr<<e.what()<<endl;
            }

        }
    }


    while (getline(file, s)) {

        istringstream iss(s);
        if (iss >> s >> n) {

            if (s == "moedas")
                moedas = n;
            else if (s == "instantes_entre_novos_itens")
                instantes_entre_novos_itens = n;
            else if (s == "duracao_item")
                duracao_item = n;
            else if (s == "max_itens")
                max_itens = n;
            else if (s == "preco_venda_mercadoria")
                preco_venda_mercadoria = n;
            else if (s == "preco_compra_mercadoria")
                preco_compra_mercadoria = n;
            else if (s == "preco_caravana")
                preco_caravana = n;
            else if (s == "instantes_entre_novos_barbaros")
                instantes_entre_novos_barbaros = n;
            else if (s == "duracao_barbaros")
                duracao_barbaros = n;
        }
        else{
            throw invalid_argument("A configuracao do ficheiro esta incorreta. Deve ser: atributo valor");
        }

    }
    configurado = true;
    combates_ganhos=0;
    instantes_passados=0;//faco isto para garantir que tudo comeca de novo no caso de eu terminar a fase 2 e voltar sem terminar o programa
    atualizarPrecosCidades(preco_compra_mercadoria, preco_venda_mercadoria, preco_caravana);
    atualizarDuracaoBarbaros(duracao_barbaros);
    Caravana::setContador(Caravana::getContador()); //isto garante que o contador esta sempre de acordo com o numero de caravanas do simulador
    //tenho que fazer isto  pois os precos so sao lidos depois das cidades serem criadas. eu sei que as cidades tem todas o mesmo preco, mas assim esta preparado para nao ter o mesmo preco
    //cout<<*b;
    b->imprimeBuffer();
    return true;
}

void Simulador::atualizarPrecosCidades(int precoCompra, int precoVenda, int precoCaravana) {
    for (Cidade &cidade : cidades) {
        cidade.setPrecoCompra(precoCompra);
        cidade.setPrecoVenda(precoVenda);
        cidade.setPrecoCaravana(precoCaravana);
    }
}

void Simulador::atualizarDuracaoBarbaros(int valor){
    for (Barbara &barbaro : barbaras) {
        barbaro.setDuracaoBarbara(valor);
    }
}


bool Simulador::getConfigured() const {
    return configurado;
}

void Simulador::AdicionaCaravana(int x, int y, char simbolo, char tipo) {
    if(!guardaSimbolosCaravanas.empty()) {
        simbolo=guardaSimbolosCaravanas.back();//atribui o ultimo valor do vetor ao simbolo para reutilizar simbolos ja apagados
        guardaSimbolosCaravanas.pop_back(); //apaga o simbolo depois de reutilizado
    }//posso meter aqui porque quando le o mapa vai estar vazio

    if(caravanas.size()==9){
        cerr << "Limite de 9 caravanas no mapa atingido." << endl;
        return;
    }
    if (tipo == '-') {//caravana vem do mapa, sortear o tipo dela com um random
        int aletorio = rand() % 3 + 1; //cria um numero entre 1 e 3
        switch (aletorio) {
            case 1:
                caravanas.push_back(new Comercio(x, y, simbolo)); //conforme dado na aula teorica
                break;
            case 2:
                caravanas.push_back(new Militar(x, y, simbolo));
                break;
            case 3:
                caravanas.push_back(new Secreta(x, y, simbolo));
                break;

        }
    }else if (tipo == 'B'){
        int nova_barbara = barbaras.size() + 1;
        string id_barbara = "c" + to_string(nova_barbara);
        barbaras.push_back(Barbara(x, y, id_barbara, duracao_barbaros));
    }else if (tipo == 'S'){
        caravanas.push_back(new Secreta(x, y, simbolo));
    }else if (tipo == 'M'){
        caravanas.push_back(new Militar(x, y, simbolo));
    }else if (tipo == 'C'){
        caravanas.push_back(new Comercio(x, y, simbolo));
    }
    //se quiser adicionar novas caravanas continuar isto
}

void Simulador::AdicionaCidade(int x, int y, char simbolo){
    cidades.push_back(Cidade(x, y, simbolo));
}

void Simulador::CompraCaravanaNaCidade(char simbolo_cidade, char tipo_caravana){
    if(!configurado){
        cout<<"Simulador nao configurado, faca config 'nomeficheiro.txt'"<<endl;
        return;
    }
    if(isdigit(simbolo_cidade)){
        cout << "Cidade invalida" << endl;
        return;
    }
    if(tipo_caravana == 'S' || tipo_caravana == 'M' || tipo_caravana == 'C') {
        for (Cidade &cidade: cidades) {
            if (cidade.getSimbolo() == simbolo_cidade) {
                int valor = cidade.PodeComprarCaravana(tipo_caravana, moedas);
                if (valor > 0) {
                    string simbolo_string= to_string(caravanas.size()+1);
                    char simbolo=simbolo_string[0];//esta salganhada toda para  converter o numero da caravana em char
                    AdicionaCaravana(cidade.getX(), cidade.getY(), simbolo, tipo_caravana);
                    cidade.adicionarCaravana(caravanas.back());//manda a ultima caravana para a cidade
                    moedas -= valor; //retira o custo da caravana
                    cout << "Caravana comprada. Moedas a menos -> " << valor << endl;
                    return;
                } else if (valor == -1) {
                    cerr << "Tipo de caravana indisponivel nessa cidade ou moedas insuficientes." << endl;
                    return;
                }
            }
        }
    }
    else
        cout << "Tipo de caravana inserido invalido" << endl;
}


void Simulador::getPrecosCidade(){
    if(!configurado){
        cout<<"Simulador nao configurado, faca config 'nomeficheiro.txt'"<<endl;
        return;
    }
    //metemos a funcao aqui porque o preco e igual em todas as cidades senao era na class cidade
    cout << "Preco venda mercadoria: " << preco_venda_mercadoria << endl;
    cout << "Preco compra mercadoria: " << preco_compra_mercadoria << endl;
    cout << "Preco caravana: " << preco_caravana << endl;
}

bool Simulador::VerificaObstaculos(char id, const string &direcao){
    for (Caravana *caravana : caravanas) {
        if (caravana->getSimbolo() == id) {
            int posX_antiga=caravana->getX(), posY_antiga=caravana->getY(); // guarda coordenadas antinas para depois voltar a meter igual
            caravana->move_Com_Utilizador(direcao, linhas, colunas);
            char caracter_buffer = b->getChar(caravana->getX(),caravana->getY());
            if(caracter_buffer == '+' || isdigit(caracter_buffer) || caracter_buffer == '!') {
                caravana->setX(posX_antiga);
                caravana->setY(posY_antiga);
                return false; // esta la outra caravana ou montanha entao nao faz nada pois nao pode ir para la
            }
            if(caracter_buffer == '&' && caravana->getTipoCaravana()!="Comercio"){
                caravana->setX(posX_antiga);
                caravana->setY(posY_antiga);
                return false; // esta la um item e esta caravana nao o come
            }
            caravana->setX(posX_antiga);
            caravana->setY(posY_antiga); // faz isto para voltar tudo ao normal
        }
    }
    return true;
}

bool Simulador::VerificaCaravanaEstaOuVaiSairCidade(char id){
    for (Caravana *caravana: caravanas) {
        if (caravana->getSimbolo() == id) {
            for (Cidade &cidade: cidades) {
                if (cidade.getX()==caravana->getX()&&cidade.getY()==caravana->getY()) {
                    cidade.removerCaravana(caravana); //Se estiver numa cidade remove
                    b->setChar(cidade.getX(), cidade.getY(), cidade.getSimbolo()); // mantem o simbolo da cidade
                    return true;
                }
            }
            b->setChar(caravana->getX(), caravana->getY(), '.'); // deixa vazio o local onde estava
            break;
        }
    }
    return false;
}

bool Simulador::VerificaSeCaravanaEntrouEmCidade(char id){
    for (Caravana *caravana: caravanas) {
        if (caravana->getSimbolo() == id) {
            // Atualiza a nova posicao no mapa
            for (Cidade &cidade: cidades) { //se for cidade adiciona a la
                if (cidade.getX()==caravana->getX()&&cidade.getY()==caravana->getY()) {
                    cidade.adicionarCaravana(caravana);
                    b->setChar(cidade.getX(), cidade.getY(), cidade.getSimbolo());
                    return true;
                }
            }
            //se nao estiver em cidade atualiza posicao com simbolo da caravana
            b->setChar(caravana->getX(), caravana->getY(), caravana->getSimbolo()); // atualiza posicao
            break;
        }
    }
    return false;
}

void Simulador::moverCaravana(char id, const string &direcao, const string tipo_move) {
    if(!configurado){
        cout<<"Simulador nao configurado, faca config 'nomeficheiro.txt'"<<endl;
        return;
    }
    if(isalpha(id)){
        cout << "Caravana invalida" << endl;
        return;
    }
    if(VerificaObstaculos(id, direcao)) {
        for (Caravana *caravana: caravanas) {
            if (caravana->getSimbolo() == id) {
                VerificaCaravanaEstaOuVaiSairCidade(id);//verifica se a caravana esta na cidade e se estiver apaga a do vetor
                caravana->move_Com_Utilizador(direcao, linhas, colunas);
                VerificaSeCaravanaEntrouEmCidade(id); //verifica se a posicao para onde a caravana foi e uma cidade e se for adiciona a
                if(tipo_move=="user") {//se for o user incrementa, senao é a caravana q trata disso (se for aleatorio)
                    instantes_passados++;//incrementa o numero de instantes
                    VerificaCombates();
                    MostraMapaAtualizado();
                }
                return;
            }
        }
        cout << "Caravana "<<id<<" nao existe" << endl;
    }
    if(tipo_move=="user")
        MostraMapaAtualizado(); //mostra mapa mesmo que seja errado as coordenadas
}

bool Simulador::VerificaBarbaraPodeMovimentar(string &id, const string &direcao){
    for (Barbara &barbara : barbaras) {
        if (barbara.getID() == id) {
            int posX_antiga=barbara.getX(), posY_antiga=barbara.getY(); // guarda coordenadas antinas para depois voltar a meter igual
            barbara.move_Com_Coordenadas(direcao, linhas, colunas);
            char caracter_buffer = b->getChar(barbara.getX(),barbara.getY());
            if(caracter_buffer == '+' || isdigit(caracter_buffer) || caracter_buffer == '!' || isalpha(caracter_buffer)) {
                barbara.setX(posX_antiga);
                barbara.setY(posY_antiga);
                return false; // esta la outra caravana ou montanha entao nao faz nada pois nao pode ir para la
            }
            barbara.setX(posX_antiga);
            barbara.setY(posY_antiga); // faz isto para voltar tudo ao normal
        }
    }
    return true;
}

void Simulador::moverBarbara(string &id, const string &direcao) {
    if(VerificaBarbaraPodeMovimentar(id, direcao)) {
        for (Barbara &barbara: barbaras) {
            if (barbara.getID() == id) {
                b->setChar(barbara.getX(), barbara.getY(), '.');
                barbara.move_Com_Coordenadas(direcao, linhas, colunas);
                b->setChar(barbara.getX(), barbara.getY(), '!');
                return;
            }
        }
    }
}


void Simulador::MostraMapaAtualizado(){
        cout << "Mapa atualizado" << endl;
        ShowBuffer();
}
void Simulador::MostraCaravana(char s){
    if(!configurado){
        cout<<"Simulador nao configurado, faca config 'nomeficheiro.txt'"<<endl;
        return;
    }

    if(isalpha(s)){
        cout << "Caravana invalida" << endl;
        return;
    }
    cout << "nr caravanas existentes: " << caravanas.size() << endl;
    for (Caravana *caravana : caravanas) {
        if(caravana->getSimbolo() == s) {
            caravana->lista_caravana();
            return;
        }
    }
    cout << "Caravana "<<s<<" nao encontrada" << endl; //caso ela nao exista
}

void Simulador::VendeTodaMercadoriaNaCidade(char simbolo){

    if(!configurado){
        cout<<"Simulador nao configurado, faca config 'nomeficheiro.txt'"<<endl;
        return;
    }
    if(simbolo == '\0' || isalpha(simbolo) || simbolo == ' '){
        cout << "Valor introduzido invalido" << endl;
        return;
    }
    for(Cidade &cidade : cidades){
        int verifica = cidade.VendeMercadoria(simbolo); //retorna as moedas extra ou 0 se nao tiver mercadoria ou -1 se nao existir
        if(verifica>0){
            cout << "Toda a mercadoria da Caravana " << simbolo << " foi vendido" << endl;
            cout << "Moedas extra -> " << verifica << endl;
            moedas+=verifica;
            return;
        }else if(verifica==-1){
            cout << "Nao existe mercadoria para vender" << endl;
            return;
        }
    }
    //Se chegar aqui é porque nao estava em cidade
    cout << "A caravana nao esta numa cidade" << endl;
}

void Simulador::CompraMercadoriaNaCidade(char simbolo, int toneladas){

    if(!configurado){
        cout<<"Simulador nao configurado, faca config 'nomeficheiro.txt'"<<endl;
        return;
    }
    if(simbolo == '\0' || isalpha(simbolo) || simbolo == ' '){
        cout << "Valor introduzido invalido" << endl;
        return;
    }
    for(Cidade &cidade : cidades){
        int verifica = cidade.CompraMercadoria(simbolo,toneladas, moedas);
        if(verifica>0){
            cout << toneladas << " de mercadoria compradas" << endl;
            cout << "Moedas a menos -> " << verifica << endl;
            moedas-=verifica;
            return;
        }else if(verifica==-1){
            cout << "Excede o limite de mercadoria" << endl;
            return;
        }else if(verifica==-2){
            cout << "Nao tem moedas disponiveis" << endl;
            return;
        }
    }
    //Se chegar aqui é porque nao estava em cidade
    cout << "A caravana nao esta numa cidade" << endl;
}

void Simulador::CompraTripulantesNaCidade(char simbolo, int quantidade){
    if(!configurado){
        cout<<"Simulador nao configurado, faca config 'nomeficheiro.txt'"<<endl;
        return;
    }
    if(simbolo == '\0' || isalpha(simbolo) || simbolo == ' '){
        cout << "Valor introduzido invalido" << endl;
        return;
    }
    for(Cidade &cidade : cidades){
        int verifica = cidade.CompraTripulantes(simbolo,quantidade, moedas);
        if(verifica>0){
            cout << quantidade << " tripulantes comprados" << endl;
            cout << "Moedas a menos -> " << verifica << endl;
            moedas-=verifica;
            return;
        }else if(verifica==-1){
            cout << "Excede o limite de tripulantes" << endl;
            return;
        }else if(verifica==-2){
            cout << "Nao tem moedas disponiveis" << endl;
            return;
        }
    }
    //Se chegar aqui é porque nao estava em cidade
    cout << "A caravana nao esta numa cidade" << endl;
}

void Simulador::listarCaravanasCidade(char simboloCidade) const {
    if(!configurado){
        cout<<"Simulador nao configurado, faca config 'nomeficheiro.txt'"<<endl;
        return;
    }
    if(isdigit(simboloCidade)){
        cout << "Cidade invalida" << endl;
        return;
    }
    for (const Cidade &cidade : cidades) {
        if (cidade.getSimbolo() == simboloCidade) {
            cidade.listarCaravanas();
            return;
        }
    }
    cout << "Cidade " << simboloCidade << " nao encontrada." << endl;
}

bool Simulador::VerificaPosicaoDisponivelBuffer(int &x, int &y){
    if(b->getChar(x,y)=='.')
        return true;
    return false;
}

bool Simulador::AdicionaBarbara(int x, int y){
    if(!configurado){
        cout<<"Simulador nao configurado, faca config 'nomeficheiro.txt'"<<endl;
        return false;
    }
    if(VerificaPosicaoDisponivelBuffer(x,y)) {
        int nova_barbara = barbaras.size() + 1;
        string id_barbara = "c" + to_string(nova_barbara);
        barbaras.push_back(Barbara(x, y, id_barbara, duracao_barbaros));
        b->setChar(x,y,'!');
        cout << "Barbara adicionada." << endl;
        b->imprimeBuffer();//mostra buffer com barbara adicionada
        return true;
    }
    return false;
}

void Simulador::ApareceBarbara() {
    while (true) {
        int x = rand() % linhas; // Gera uma linha random
        int y = rand() % colunas; // Gera uma coluna random
        if (AdicionaBarbara(x, y)) {
            break; // Sai do loop quando adicionar uma barbara, para o caso das coordenadas geradas nao servirem
        }
    }
}

int Simulador::removerCaravana(Caravana* caravana) {
    vector<Caravana*>::iterator it; // posso usar auto para simplificar esta salganhada
    for (it = caravanas.begin(); it != caravanas.end(); ++it) {
        if (*it == caravana) {
            guardaSimbolosCaravanas.push_back(caravana->getSimbolo());
            VerificaCaravanaEstaOuVaiSairCidade(caravana->getSimbolo());//remove da cidade se for o caso
            caravanas.erase(it);
            delete caravana; //para libertar memoria da caravana porque usei new ao criar
            return 1;
        }
    }
    return 0;
}

int Simulador::removerBarbara(Barbara &barbara) {
    vector<Barbara>::iterator it; // posso usar auto para simplificar esta salganhada
    for (it = barbaras.begin(); it != barbaras.end(); ++it) {
        if (it->getID() == barbara.getID()){
            b->setChar(it->getX(), it->getY(), '.');
            barbaras.erase(it);
            return 1;
        }
    }
    return 0;
}

void Simulador::TempestadeAreia(int linha, int coluna, int raio) {
    if(!configurado){
        cout<<"Simulador nao configurado, faca config 'nomeficheiro.txt'"<<endl;
        return;
    }
    if (linha < 0 || linha >= linhas || coluna < 0 || coluna >= colunas || raio < 0) {
       cout << "Parametros invalidos para a tempestade de areia." << endl;
        return;
    }

    cout << "Deu certo." << endl;
    for (Caravana* caravana : caravanas) {
        int dx = abs(caravana->getX() - linha); //calcula a distancia entre a linha recebida e a linha da caravana
        int dy = abs(caravana->getY() - coluna);//calcula a distancia entre a coluna recebida e a coluna da caravana

        if (dx <= raio && dy <= raio){
            if(caravana->TempestadeAreia())
                removerCaravana(caravana); //se for true remove
        }
    }
    for (Barbara barbara : barbaras) {
        int dx = abs(barbara.getX() - linha); //calcula a distancia entre a linha recebida e a linha da caravana
        int dy = abs(barbara.getY() - coluna);//calcula a distancia entre a coluna recebida e a coluna da caravana

        if (dx <= raio && dy <= raio){
            if(barbara.TempestadeAreia())
                removerBarbara(barbara); //se for true remove
        }
    }
    instantes_passados++;
    b->imprimeBuffer();//mostra atualizacoes
}

int Simulador::getNumCaravanas() const {
    return caravanas.size();
}

int Simulador::getMoedas() const {
    return moedas;
}

void Simulador::ShowBuffer(){
    b->imprimeBuffer();
}

void Simulador::LimpaBuffer(){
    b->clearBuffer();
    for (Caravana* caravana : caravanas) {
        delete caravana; // liberta a memoria new alocada
    }
    caravanas.clear();//aqui tenho que fazer isto para o reutilizar
    b.reset();//faco isto aqui tambem para o caso de eu terminar o jogo e voltar a configurar o mapa sem terminar o programa.
    copiasBuffer.clear(); // elimina as copias no caso de terminar a simulacao
}

void Simulador::DadosFim(){
    cout << "Pontuacao: " << moedas << endl;
    cout << "Total combates vencidos: " << combates_ganhos << endl;
    cout << "Numero de instantes ocorridos: " << instantes_passados << endl;
}

void Simulador::AdicionaMoedas(int quantidade){

    if(!configurado){
        cout<<"Simulador nao configurado, faca config 'nomeficheiro.txt'"<<endl;
        return;
    }

        setMoedas(getMoedas()+quantidade);
}

void Simulador::setMoedas(int moedas) {
    this->moedas = moedas;
}

void Simulador::LimpaSimulador() {//esta funcao serve caso saiamos da fase 2 para a fase 1 e queiramos voltar para a fase 2 sem terminar o programa
    for (Caravana* caravana : caravanas) {
        delete caravana;
    }
    caravanas.clear();
    cidades.clear();
    barbaras.clear();
    guardaSimbolosCaravanas.clear();
}

int Simulador::getLinhas() const{
    return linhas;
}

int Simulador::getColunas()const{
    return colunas;
}

int Simulador::getInstantesPassados() const{
    return instantes_passados;
}

int Simulador::setIncrementaInstantesPassados(int valor){
    return instantes_passados+=valor;
}






const vector<Caravana *> &Simulador::getCaravanas() const {
    return caravanas;
}

const vector<Itens *> &Simulador::getItens() const{
    return itens;
}

vector<Cidade>& Simulador::getCidades() {
    return cidades; // retorna o vetor de cidades
}

void Simulador::setCaravanas(const vector<Caravana *> &caravanas) {
    Simulador::caravanas = caravanas;
}

vector<Barbara> &Simulador::getBarbaras(){
    return barbaras;
}

void Simulador::setBarbaras(const vector<Barbara> &barbaras) {
    Simulador::barbaras = barbaras;
}

int Simulador::ContaCaravanasEstadoAuto(){
    int contador=0;
    for(Caravana* caravana : caravanas) {
        if (caravana->isEstadoAuto())
            contador++;
    }
    return contador;
}


void Simulador::AdicionaItens(int x, int y) {

    if(itens.size()>=5){
        cout << "Limite de 5 itens no mapa atingido." << endl;
        return;
    }

    Itens* novoItem = nullptr;

    int aleatorio = rand() % 5 + 1;
    switch (aleatorio) {
        case 1:
            novoItem = new ArcaTesouro(x, y);
            break;
        case 2:
            novoItem = new CaixaPandora(x, y);
            break;
        case 3:
            novoItem = new Jaula(x, y);
            break;

        case 4:
            novoItem = new Mina(x, y);
            break;
        case 5:
            novoItem = new Surpresa(x, y);
            break;
    }
    if(novoItem!= nullptr) {
        if (VerificaPosicaoDisponivelBuffer(x, y)) {
            itens.push_back(novoItem);
            b->setChar(x, y, novoItem->getSimbolo());
        }
    }
}



void Simulador::AdicionaItensAleatorios() {
    if (itens.size() >= max_itens){
        cout <<"numero maximo de itens atingido" << endl;
        return;
    }
    else {

        while (true) {
            int x = rand() % linhas;
            int y = rand() % colunas;
            if (VerificaPosicaoDisponivelBuffer(x, y)) {
                AdicionaItens(x, y);
                break;
            }
        }
    }
}

void Simulador::RemoveItensExpirados() {
    auto it = itens.begin();
    while (it != itens.end()) {
        cout << "Item ID: " << (*it)->getID() << ", Duracao: " << (*it)->getDuracao() << endl;
        (*it)->decrementaDuracao();
        if ((*it)->getDuracao() == 0) {
            it = itens.erase(it);
            b->setChar((*it)->getX(), (*it)->getY(), '.');
        } else {
            ++it;
        }
    }
}

void Simulador::setAcaoItens() {
    for (Itens* item : itens) {
        item->setAcao(this);
    }
}

void Simulador::removeItem(Itens *i){
    auto it = itens.begin();
    while (it != itens.end()) {
        if (*it == i) {
            itens.erase(it);
            b->setChar(i->getX(), i->getY(), '.');
            return;
        }
        ++it;
    }
    cout << "Item nao encontrado" << endl;
}



void Simulador::Combate(Caravana* caravana, Barbara& barbara) {
    // Sorteio dos valores para combate
    int sorteioCaravana, sorteioBarbara;
    while(true){
        sorteioCaravana = rand() % (caravana->getTripulantes() + 1); //sorteia um numero entre 0 e o nr tripulantes
        sorteioBarbara = rand() % (barbara.getBarbaros() + 1); //sorteia um numero entre 0 e o nr tripulantes

        if(sorteioBarbara!=sorteioCaravana)
            break; // isto é so para garantir que nao sao sorteados numeros iguais
    }
    if (sorteioCaravana > sorteioBarbara) { //significa que a caravana do utilizador venceu
        int tripulantes_atualizados_da_vencedora=caravana->getTripulantes()*0.8; //foram retirados 20%
        int diferenca_entre_tripulacoes_para_calcular_a_derrotada=caravana->getTripulantes()-tripulantes_atualizados_da_vencedora;
        caravana->setTripulantes(tripulantes_atualizados_da_vencedora); //desconta 20% na vencedora
        barbara.setBarbaros(barbara.getBarbaros()-2*diferenca_entre_tripulacoes_para_calcular_a_derrotada); //desconta o dobro dos tripulantes descontados na vencedora

        combates_ganhos++;//o utilizador venceu um combate, entao incrementa

    }else if(sorteioBarbara > sorteioCaravana){//significa que a barbara venceu
        int tripulantes_atualizados_da_vencedora=barbara.getBarbaros()*0.8; //foram retirados 20%
        int diferenca_entre_tripulacoes_para_calcular_a_derrotada=barbara.getBarbaros()-tripulantes_atualizados_da_vencedora;
        barbara.setBarbaros(tripulantes_atualizados_da_vencedora); //desconta 20% na vencedora
        caravana->setTripulantes(caravana->getTripulantes()-2*diferenca_entre_tripulacoes_para_calcular_a_derrotada); //desconta o dobro dos tripulantes descontados na vencedora
    }
    //Faz a validacao se as caravanas morreram independendentemente de quem vence
    if(caravana->getTripulantes()==0 && barbara.getBarbaros()==0){ //se ambas forem removidas nao e preciso trocar nada de nenhuma (aguas e isso)
        removerCaravana(caravana);
        removerBarbara(barbara);
        return;
    }
    if(caravana->getTripulantes()==0){//manda agua para a outra caravana e morre logo
        barbara.setAgua(barbara.getAgua()+caravana->getQuantidadeAgua()); //a validacao esta nos sets
        removerCaravana(caravana);
    }
    if(barbara.getBarbaros()==0){//manda agua para a outra caravana e morre logo
        caravana->setQuantidadeAgua(caravana->getQuantidadeAgua()+barbara.getAgua()); //a validacao esta nos sets
        removerBarbara(barbara);
    }
}



void Simulador::VerificaCombates() {
    if (barbaras.empty() || caravanas.empty()) {
        return;  // Se qualquer vetor estiver vazio, sai da funcao sem fazer nada
    }
    for (Barbara& barbara : barbaras) {
        for (Caravana* caravana : caravanas) { // em cada barbara verifico se todas as caravanas do user sao vizinhas
            int dx = abs(caravana->getX() - barbara.getX()); //para verificar se existem caravanas a vizinhas, abs é para o valor ser sempre positivo
            int dy = abs(caravana->getY() - barbara.getY());

            // Verifica se esta em cima, baixo, direita ou esquerda
            if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
                Combate(caravana, barbara);
            }
        }
    }
}

void Simulador::TrataComandosAutomaticos(int nr_instantes) {
    if(!configurado){
        cout << "Simulador nao configurado, faca config 'nomeficheiro.txt'" << endl;
        return;
    }
    cout << "Quantidade caravanas em estado auto: " << ContaCaravanasEstadoAuto() << endl;
    //CHAMAR TRATAMOVIMENTOALEATORIO
    for (int i = 0; i < nr_instantes; i++) {
        if(caravanas.empty()){
            cout << "Instantes interrompidos. Nao ha caravanas. Compre numa cidade" << endl;
            return;
        }
        if(caravanas.empty() && moedas==0){
            cout << "Perdeu. Nao ha caravanas nem dinheiro" << endl;
            terminaSimulacao();
            return;
        }
        for (Caravana *caravana: caravanas) {
            caravana->TrataMovimentoAleatorio(this);//isto faz a movimentacao das caravanas
            if (caravana->EstaMorto())
                removerCaravana(caravana); //remove caravanas que tenham morrido
        }
        for (Barbara &barbara: barbaras) {
            barbara.TrataMovimentoAleatorio(this);//isto faz a movimentacao das caravanas
            if (barbara.getEstaMorto())
                removerBarbara(barbara); //remove caravanas que tenham morrido
        }

        cout << "instante: " << instantes_passados++ << endl;

        VerificaCombates();
        if (instantes_passados % instantes_entre_novos_itens == 0) {
            if (itens.size() < max_itens) {
                AdicionaItensAleatorios();
            }
        }

        setAcaoItens();

        RemoveItensExpirados();
        if (instantes_passados % instantes_entre_novos_barbaros ==0) { //a cada 40(configuravel) instantes aparece uma barbara
            ApareceBarbara();
        }

        MostraMapaAtualizado();
    }
}

void Simulador::ColocaAuto(char sim){
    if(!configurado){
        cout<<"Tem de configurar o simulador primeiro"<<endl;
        return;
    }
    if(isalpha(sim) || sim=='\0' || sim==' '){
        cout << "Caravana invalida" << endl;
        return;
    }
    for(Caravana* caravana : caravanas){
        if(caravana->getSimbolo()==sim){
            caravana->setEstadoAuto(true);
            return;
        }
    }
    cout<<"Caravana"<<sim<< "nao encontrada"<<endl;
}

void Simulador::RetiraAuto(char sim){

    if(!configurado){
        cout<<"Tem de configurar o simulador primeiro"<<endl;
        return;
    }

    if(isalpha(sim) || sim=='\0' || sim==' '){
        cout << "Caravana invalida" << endl;
        return;
    }
    for(Caravana* caravana : caravanas){
        if(caravana->getSimbolo()==sim){
            caravana->setEstadoAuto(false);
            return;
        }
    }
    cout<<"Caravana"<<sim<< "nao encontrada"<<endl;
}





int Simulador::procuraNomeCopia(const string &nomecopia){
    int i=0;
    for (const auto &copia : copiasBuffer){
        if(copia->getNomeCopia()==nomecopia)
            return i;

        i++;
    }
    return -1;
}

bool Simulador::guardaCopiaBuffer(const string &nomecopia){

    if(!configurado){
        cout<<"Tem de configurar o buffer primeiro"<<endl;
        return false;
    }

    if(nomecopia.empty()){
        cout<<"Parametros invalidos, insira saves 'nomecopia'"<<endl;
        return false;
    }


        if(procuraNomeCopia(nomecopia)!=-1) {
            cout<<"Ja existe uma copia com esse nome"<<endl;
            return false;
        }


        try{

        copiasBuffer.push_back(std::make_unique<Buffer>(*b,nomecopia)); // chama o construtor por copia, ja que nao podem haver 2 unique ptr a apontar para o mesmo objeto
        return true;

    } catch (const std::exception &e) {
        cerr << "Erro a fazer copia do Buffer: " << e.what() << endl;
        return false;
    }
}


bool Simulador::listaCopiasBuffer(){


    if(!configurado){
        cout<<"Tem de configurar o buffer primeiro"<<endl;
        return false;
    }

    if(copiasBuffer.empty()){
        cout<<"Nao existem copias do buffer guardadas"<<endl;
        return false;
    }
    for (const auto &copia : copiasBuffer) {
        cout << "Nome da copia: " << copia->getNomeCopia() << endl;
        copia->imprimeBuffer();
    }
    return true;
}


bool Simulador::eliminaCopiaBuffer(const string &nomecopia){

    if(nomecopia.empty()){
        cout<<"Parametros invalidos: insira dels 'nomecopia'"<<endl;
        return false;
    }

    if(!configurado){
        cout<<"Tem de configurar o simulador primeiro"<<endl;
        return false;
    }

    if(copiasBuffer.empty()){
        cout<<"Nao existem copias guardadas"<<endl;
        return false;
    }
    if(procuraNomeCopia(nomecopia)==-1){
        cout<<"Nao existe nenhuma copia com o nome:"<<nomecopia<<endl;
        return false;
    }

    try{
    auto it = copiasBuffer.begin();

    while(it != copiasBuffer.end()) {
        if ((*it)->getNomeCopia() == nomecopia) {
            copiasBuffer.erase(it);
            return true;
        }
        ++it;
    }
    }catch(...){
        cout<<"Erro ao eliminar a copia:"<<nomecopia<<endl;
    }

    return false;

}

bool Simulador::mostraCopiaBuffer(const string &nomecopia) {
    if(!configurado){
        cout<<"Tem de configurar o simulador primeiro"<<endl;
        return false;
    }
    if(copiasBuffer.empty()){
        cout<<"Nao existem copias guardadas"<<endl;
        return false;
    }

    if(nomecopia.empty()){
        cout<<"Parametros invalidos: insira loads 'nomecopia'"<<endl;
        return false;
    }

    if(procuraNomeCopia(nomecopia)==-1){
        cout<<"Nao existe nenhuma copia com o nome:"<<nomecopia<<endl;
        return false;
    }

    int pos = procuraNomeCopia(nomecopia);
    cout<<"Nome da copia: "<<copiasBuffer[pos]->getNomeCopia()<<endl;
    copiasBuffer[pos]->imprimeBuffer();

    return true;
}




bool Simulador::terminaSimulacao(){
    if(!configurado){
        cout<<"Tem de configurar o simulador primeiro: config 'nomeficheiro.txt'"<<endl;
        return false;
    }
    cout << "Simulacao terminada" << endl;
    DadosFim();
    LimpaBuffer();
    LimpaSimulador();
    configurado=false;
    return true;

}

bool Simulador::terminaPrograma() const {
    if(configurado){
        cout<<"Termine primeiro a simulacao -> 'terminar'"<<endl;
        return false;
    }
    else
        exit(0);
}


Simulador::~Simulador() {
    for (Caravana *caravana: caravanas) {
        delete caravana;
    }

    for (Itens *item: itens) {
        delete item;
    }
    caravanas.clear();
    itens.clear();
    b.reset();
    copiasBuffer.clear(); // elimina as copias no caso de terminar a simulacao
}
