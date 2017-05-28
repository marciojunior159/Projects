#ifndef OBSERVADOR_H
#define OBSERVADOR_H

#pragma once
#include <matriz.h>
#include <complex>

using std::complex;

#include <iostream>

using namespace std;

class Observador
{
public:
    Observador();
    Matriz Calcula_L(complex<double> p1, complex<double> p2);
    Matriz Observa(Matriz y, Matriz u);
private:
    Matriz G, H, C, L, x;
    Matriz W_inv;
};

#endif // OBSERVADOR_H
