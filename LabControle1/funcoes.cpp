#include "funcoes.h"

#include <iostream>
#include <math.h>
#include <time.h>
#include <cstdlib>

using namespace std;

#define PI 3.1415

double periodoAle = 0;
double amplitudeAle = 0;

double funcDegrau(double amp, double tempo, double offset)
{
    if(tempo>=0)
        return amp+offset;

    return offset;
}

double funcSerra(double amplitude, double periodo, double tempo, double offset)
{
    double novoTempo = fmod(tempo, periodo);

    return (amplitude/periodo*novoTempo) + offset;
}

double funcAleatoria1(double tempo)
{
    if(tempo>periodoAle)
    {
        amplitudeAle = (rand() % 4000 )/ 1000.0;
        periodoAle = (rand() % 100 )/50.0+tempo+1;
    }
    return amplitudeAle;
}
double funcAleatoria2(double tempo)
{
    if(tempo>periodoAle)
    {
        amplitudeAle = (rand() % 30000 )/ 1000.0;
        periodoAle = (rand() % 100 )/50.0+tempo+1;
    }
    return amplitudeAle;
}

double funcSenoidal(double amp, double periodo, double tempo, double offset){

    return (amp/2)*sin(2*PI*1/periodo*tempo) + amp/2 + offset;
    //periodo em radianos
//    if((fmod(tempo, periodo)) < periodo/2){
//        return ((amp/2)*sin(2*PI*periodo*tempo) + amp/2) + offset;
//    }else{
//        return offset;
//    }
}

double funcQuadrada(double amp, double periodo, double tempo, double offset){

    if( fmod(tempo,periodo) < periodo/2){
        return amp+offset;
    }else{
        return offset;
    }

}

//funcao que retorna a tensao para controlar a vazao de entrada no modo malha fechada a partir da altura
double funcAlturaTensao(double alt){

    //aproximação linear
    double v= (alt + 11)/12;
    return alt>0?v:0;
}

//funcao que retorna a altura a partir das leituras do sensor de pressao
double funcSensor(double leituras){

    return 6.1*leituras;
}

//Nao esquecer das travas
