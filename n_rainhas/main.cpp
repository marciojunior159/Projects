#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "nrainhas.h"

using namespace std;

int main(int argc, char *argv[])
{
    bool continuar=true;
    int tam;
    while(continuar){
        printf("Tamanho do tabuleiro: ");
        scanf("%d", &tam);
        NRainhas novo = NRainhas(tam);
        //NRainhas novo = NRainhas(tam,5.00,0.05,0.0,5.0,1.005);
        novo.processoNovo();
        char resp;
        cout << "\nCONTINUAR? (S/N)"; cin >> resp;
        if(resp != 'S' && resp != 's')
            continuar=false;

        cout << string(50, '\n');
    }
}
