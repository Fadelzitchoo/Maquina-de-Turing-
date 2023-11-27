#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define MAX_FITA 1000

// Declare a classe Estado antes da classe Transicao
class Estado;

class Transicao {
private:
    char simboloX, simboloY;              // SimboloX será trocado por simboloY
    char cabecoteMove;                    // Right(R) ou Left(L)
    Estado *proximoEstado;                //Aponta pro proximo Estado
    bool travou;

public:
    Transicao(char simboloX, char simboloY, char cabecoteMove, Estado *proximoEstado)
        : simboloX(simboloX), simboloY(simboloY), cabecoteMove(cabecoteMove), proximoEstado(proximoEstado) {
            travou = false;             
        }

    Transicao() {}

    //Gets e Sets da classe

    char getSimboloX() {
        return simboloX;
    }

    char getSimboloY() {
        return simboloY;
    }

    char getMoveCabecote() {
        return cabecoteMove;
    }

    Estado *getProximoEstado() {
        return proximoEstado;
    }

    void setTravou(){
        travou = true;
    }

    bool getTravou(){
        return travou;
    }
};

class Estado {
private:
    vector<Transicao> transicoes; // Lista com transições que um estado possui
    bool ehFinal;                 // Se verdadeiro o estado é final
    int i;  //auxiliar

public:
    Estado() {
        ehFinal = false;
    }

    void addTransicao(char simboloX, char simboloY, char cabecoteMove, Estado *proximoEstado) { //Adiciona uma nova transição ao vetor de transições
        Transicao novaTransicao(simboloX, simboloY, cabecoteMove, proximoEstado);
        transicoes.push_back(novaTransicao);
    }

    void setFinal() {
        ehFinal = true;
    }

    bool getFinal() {
        return ehFinal;
    }

    Transicao *proximoEstado(char entrada) {                                                    //Olha pra todas as transições de um estado
        for (i = 0; i != transicoes.size(); ++i) {
            if (transicoes.at(i).getSimboloX() == entrada) {                                    //Aquele que a respeitar a relação entrada igual "X" de "X/Y, C"
                return &transicoes.at(i);                                                       //E retorna a transição pra main
            }
        }

        Transicao* transicaoErro = new Transicao();                                             //Caso não haja nenhuma transição, cria uma nova transição com uma flag informando fim dos caminhos
        transicaoErro->setTravou();
        return transicaoErro;
    }
    
    vector<Transicao>& getTransicoes() {                                                        //Retorna todas as transições de um estado
        return transicoes;
    }
};

void organizarFita(vector<char> &fita, const string &palavra) {
    int i;

    fita.clear();                                      // Limpa o vetor
    fita.assign(palavra.begin(), palavra.end());       // Copia a palavra para dentro do vetor

    for (i = 0; i < (MAX_FITA - fita.size()); i++) {
        fita.push_back('b');                           // Preenche com o caractere de espaço em branco 'b'
    }
}

string imprimeFita(const vector<char> &fita) {
    string conteudoFita;

    for (char c : fita) {                               //Coloca todo o conteudo do vetor de char em uma string
        if (c != 'b') {                                 //Menos os simbolos 'b'
            conteudoFita += c;
        }
    }

    return conteudoFita;
}

bool buscaCaminhosRecursiva(vector<Estado> &estados, vector<char> &fita, int itEstados, int cabecote) {
    
    if (estados.at(itEstados).getFinal()) {             //Caso base, se chegou no estado final a busca é aceita
        return true;
    }

    bool caminhoEncontrado = false;

    for (Transicao &transicao : estados.at(itEstados).getTransicoes()) {                    //Olhas as transições do estado atual

        if (!transicao.getTravou() && transicao.getSimboloX() == fita.at(cabecote)) {       //verifica se o programa não travou e o cabecote equivale ao simbolo X
            fita[cabecote] = transicao.getSimboloY();                                       //Troca o simbolo do cabecote

            int novoCabecote = cabecote;
            if (transicao.getMoveCabecote() == 'R') {                                       //Move o cabecote de acordo com informação da transicao
                novoCabecote++;                                                             
            } else if (transicao.getMoveCabecote() == 'L') {
                novoCabecote--;
            }

            int novoItEstados = transicao.getProximoEstado() - &estados.at(0);              //Obtem o interador numerico do proximo estado no vetor de estados

            caminhoEncontrado |= buscaCaminhosRecursiva(estados, fita, novoItEstados, novoCabecote);    //Recursivamente chama pra o proximo estado com novoCabecote

        }
    }

    return caminhoEncontrado;
}

int main() {

    int nEstados = 0, tTransicoes = 0, i;
    int estInicialAux, estFinalAux, quantEstFinal;
    string mudancaFita, palavraW;

    cin >> nEstados;
    cin >> tTransicoes;

    vector<Estado> estados;                 //Vetor com os estados

    for (i = 0; i < nEstados; i++) {        //Preenche o vetor de estados
        Estado estadoAuxiliar;
        estados.push_back(estadoAuxiliar);
    }

    for (i = 0; i < tTransicoes; i++) {     //Laço que cria e preenche a estrutura de transições e estados

        cin >> estInicialAux;               //Estado inicial
        cin >> mudancaFita;                 //Mudanças na fita
        cin >> estFinalAux;                 //Estado final

        size_t barraPos = mudancaFita.find('/');        //Separa a string mudancaFita em char
        size_t virgulaPos = mudancaFita.find(',');     

        string simboloXstr = mudancaFita.substr(0, barraPos);
        string simboloYstr = mudancaFita.substr(barraPos + 1, virgulaPos - barraPos - 1);
        string cabecoteMovestr = mudancaFita.substr(virgulaPos + 1);

        char simboloX = simboloXstr[0];
        char simboloY = simboloYstr[0];
        char cabecoteMove = cabecoteMovestr[0];

        estados.at(estInicialAux).addTransicao(simboloX, simboloY, cabecoteMove, &estados.at(estFinalAux)); //Adiciona estados de acordo com a entrada
    }

    cin >> quantEstFinal; // Quantos estados finais existem

    for (i = 0; i < quantEstFinal; i++) { // Define os estados finais como finais
        cin >> estFinalAux;

        estados.at(estFinalAux).setFinal();
    }

    cin >> palavraW;    //palavraW é a palavra que entra em processo de acetação

    while (palavraW != "*") {
        vector<char> fita;              //cria a fita para palavra
        bool acabouCaminhos = false;    //um bool para indicar fim dos caminhos

        organizarFita(fita, palavraW);
        int cabecote = 0;
        int itEstados = 0;

        while (!estados.at(itEstados).getFinal() && !acabouCaminhos) {
            vector<char> fita;
            organizarFita(fita, palavraW);

            bool caminhoEncontrado = buscaCaminhosRecursiva(estados, fita, 0, 0);

            if (caminhoEncontrado) {            //Aceita se a busca recursiva for um sucesso
                cout << palavraW << " aceita: " << imprimeFita(fita) << endl;
                acabouCaminhos = true;
            } else {                            //E rejeita caso contrario
                cout << palavraW << " rejeitada." << endl;
                acabouCaminhos = true;
            }

        }

        cin >> palavraW;
    }

    return 0;
}

