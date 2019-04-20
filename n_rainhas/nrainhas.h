#ifndef NRAINHAS_H
#define NRAINHAS_H
#include <vector>

using namespace std;

class NRainhas
{
private:
    int tamTab;
    int ite;
    float T, cF, est, estF, T_f;
    float atualEnergia;
    vector<vector<int> > tabuleiro;
    vector<int> rainha;
    vector<int> nRainha;
public:
    NRainhas(int);
    NRainhas(int tam, float T, float cF, float T_f, float est, float estF);
    float showEnergia();
    void imprimir();
    int energia(vector<int>);
    bool funcaoProbabilidade(float,float);
    bool ehRepetido(vector<int>, int);
    int moverVizinho();
    void aceitarMudanca();
    bool passo();
    void processoNovo();
    int inicioAleatorio();
};

#endif // NRAINHAS_H
