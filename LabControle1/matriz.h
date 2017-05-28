#ifndef MATRIZ_H
#define MATRIZ_H

#pragma once

#include <vector>
using std::vector;

typedef unsigned int uint;

class Matriz
{
public:
    Matriz(unsigned int linhas, unsigned int colunas);
    ~Matriz();
    Matriz operator +(Matriz x);
    Matriz operator -(Matriz x);
    Matriz operator *(Matriz x);
    Matriz operator *(double k);
    Matriz operator ^(uint k);
    vector<double>& operator[](int p);
    friend Matriz operator *(double k, Matriz x);
public:
    Matriz& operator =(Matriz&& x);
    Matriz& operator =(const Matriz& x);
    Matriz(const Matriz& x);
    Matriz(const Matriz&& x);
    static Matriz Identidade(uint ord);
    const uint l, c;
private:
    vector<vector<double>> data;
};

#endif // MATRIZ_H
