#include <iostream>
#include <math.h>

using namespace std;

double funcDegrau(double amp, double tempo)
{
    if(tempo>=0)
        return amp;

    return 0;
}

double funcSerra(double tempo, double amplitude, double periodo)
{
    double novoTempo = fmod(tempo, periodo);

    return amplitude/periodo*novoTempo;
}

struct aleatorio {
  double amplitude;
  double intervalo;
};

aleatorio funcAleatoria()
{
    struct aleatorio resultado;

    resultado.amplitude = (rand() % 4000 + 0)/ 1000;
    resultado.intervalo = (rand() % 100 + 0)/10.0;

    return resultado;
}

//Nao esquecer das travas
