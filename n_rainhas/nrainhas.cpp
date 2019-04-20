#include "nrainhas.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;


NRainhas::NRainhas(int tam){
    srand(time(NULL));
    T=5.00; cF=0.05; T_f=0;
    est=5.00; estF=1.08;
    tamTab=tam; ite=0;

    tabuleiro.resize(tam);
    rainha.resize(tam);
    nRainha.resize(tam);

    atualEnergia = inicioAleatorio();
    cout << endl << "Posicoes iniciais das rainhas: " << endl;
    imprimir();

}
NRainhas::NRainhas(int tam, float T, float cF, float T_f, float est, float estF){
    srand(time(NULL));
    this->T = T;
    this->cF = cF;
    this->T_f = T_f;
    this->est = est;
    this->estF = estF;
    tamTab=tam;

    tabuleiro.resize(tam);
    rainha.resize(tam);
    nRainha.resize(tam);

    atualEnergia = inicioAleatorio();
    imprimir();
}
float NRainhas::showEnergia(){
        return atualEnergia;
}
int NRainhas::inicioAleatorio(){
    for(int i=0; i<tamTab; i++){
        srand(time(NULL));

        bool repete = true;
        while(repete){
            rainha[i] = rand() % tamTab;

            if(!ehRepetido(rainha, i))
                repete = false;
        }
    }
    return energia(rainha);
 }
void NRainhas::imprimir(){
    for(int i=0; i<tamTab; i++){
        tabuleiro[i].resize(tamTab);
        for(int j=0; j<tamTab; j++){
            tabuleiro[i][j] = 0;
        }
    }

    for(int i=0; i<tamTab; i++)
        tabuleiro[i][rainha[i]] = 1;

    for(int j=0; j<tamTab; j++)
        cout << " __";
    cout << endl;

        for(int i=0; i<tamTab; i++){
            for(int j=0; j<tamTab; j++){
                if(tabuleiro[i][j] == 0)
                    cout << "|__";
                else
                    cout << "|_R";
                //cout << tabuleiro[i][j] << " ";
            }
            cout << "|";
            cout << endl;
        }
        cout <<  endl;
}
int NRainhas::energia(vector<int> rainha){
    int q=0;
    for(int i=0; i<tamTab; i++){
        for(int j=i+1; j<tamTab; j++){
            if (i == j)
                q++;
            else if(rainha[i] == rainha[j])
                q++;
            else if(i+rainha[i] == j+rainha[j])
                q++;
            else if(rainha[i]-i == rainha[j]-j)
                q++;
        }
    }
    return q;
}
bool NRainhas::funcaoProbabilidade(float T, float delta){
    if (delta < 0)
        return true;
    float C = exp(-delta/T);
    float R = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    if (R < C)
        return true;

    return false;
}
bool NRainhas::ehRepetido(vector<int> vetor, int x){
        for(int i=0; i<tamTab; i++){
            if(i != x && vetor[x] == vetor[i])
                return true;
        }
        return false;
    }
int NRainhas::moverVizinho(){

        for(int i=0; i<tamTab; i++){
            nRainha[i] = rainha[i];
        }

        int r = rand() %  tamTab;
        bool repete = true;
        while(repete){
            int velho_x = r;
            int velho_y = nRainha[r];

            r = rand() % tamTab;
            nRainha[r] = rand() % tamTab;

            if(!ehRepetido(nRainha, r))
                repete = false;
            else{
                r = velho_x;
                nRainha[r] = velho_y;
            }

        }
        return energia(nRainha);
    }
void NRainhas::aceitarMudanca(){
        for(int i=0; i<tamTab; i++){
            rainha[i] = nRainha[i];
        }
    }
bool NRainhas::passo(){
    cout << atualEnergia << " ";
    if(T > T_f){
        for(int i=0; i<est; i++){
            int novaEnergia = moverVizinho();
            int delta = novaEnergia - atualEnergia;

            if(funcaoProbabilidade(T,delta)){
                aceitarMudanca();
                atualEnergia = novaEnergia;
            }
        }
        T = T - cF;
        est = est*estF; ite++;
        return false;
    }
    T = T_f;
    return true;
}
void NRainhas::processoNovo(){
        bool sinal = false;
        printf("Variacao de energia do sistema:\n");
        while(!sinal)
            sinal = passo();

        cout << "\n\nPosicoes finais das rainhas:\n";
        imprimir();
}
