#include "funcoes.h"

#include <iostream>
#include <math.h>
#include <time.h>
#include <cstdlib>

using namespace std;

#define PI 3.1415

double funcDegrau(double amp, double tempo)
{
    if(tempo>=0)
        return amp;

    return 0;
}

double funcSerra(double amplitude, double periodo, double tempo)
{
    double novoTempo = fmod(tempo, periodo);

    return amplitude/periodo*novoTempo;
}

aleatorio last_resultado= {0,0};
double funcAleatoria(double tempo)
{
    if(tempo>last_resultado.intervalo)
    {
        last_resultado.amplitude = (rand() % 8000 )/ 1000.0 - 4.0;
        last_resultado.intervalo = (rand() % 100 )/50.0+tempo + 0.1;
    }
    return last_resultado.amplitude;
}

double funcSenoidal(double amp, double periodo, double tempo){

    return (amp/2)*sin(2*PI*1/periodo*tempo) + amp/2;
    //periodo em radianos
    if((fmod(tempo, periodo)) < periodo/2){
        return (amp/2)*sin(2*PI*periodo*tempo) + amp/2;
    }else{
        return 0;
    }
}

double funcQuadrada(double amp, double periodo, double tempo){

    if( fmod(tempo,periodo) < periodo/2){
        return amp;
    }else{
        return 0;
    }

}
//Nao esquecer das travas
