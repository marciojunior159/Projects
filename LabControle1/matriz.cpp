#include "matriz.h"

using std::move;

Matriz::Matriz(unsigned int linhas, unsigned int colunas) : l(linhas), c(colunas)
{
    data.resize(l);
    for(uint i=0; i<l; i++)
        data[i].resize(c);
}
Matriz::~Matriz()
{

}
Matriz::Matriz(const Matriz& x) : l(x.l), c(x.c), data(x.data) // Copy constructor
{

}
Matriz::Matriz(const Matriz&& x) : l(move(x.l)), c(move(x.c)), data(move(x.data)) // Move constructor
{

}
Matriz& Matriz::operator =(Matriz&& x) // Move assignment operator
{
    if(l != x.l && c != x.c)
        throw "Erro de dimensao";
    this->data= move(x.data);
    return (*this);
}
Matriz& Matriz::operator =(const Matriz& x) // Copy assignment operator
{
    if(l != x.l && c != x.c)
        throw "Erro de dimensao";
    this->data= x.data;
    return (*this);
}
Matriz Matriz::operator +(Matriz x)
{
    if(l != x.l || c != x.c)
        throw "Soma Incoerente";
    Matriz res(l, c);
    for(uint i=0; i<l; i++)
        for(uint j=0; j<c; j++)
            res[i][j]= data[i][j]+x[i][j];
    return res;
}
Matriz Matriz::operator -(Matriz x)
{
    if(l != x.l || c != x.c)
        throw "Subtracao Incoerente";
    Matriz res(l, c);
    for(uint i=0; i<l; i++)
        for(uint j=0; j<c; j++)
            res[i][j]= data[i][j]-x[i][j];
    return res;
}
Matriz Matriz::operator *(Matriz x)
{
    if(c != x.l)
        throw "Multiplicacao Incoerente";
    Matriz res(l, x.c);
    for(uint i=0; i<l; i++)
        for(uint j=0; j<x.c; j++)
        {
            res[i][j]= 0;
            for(uint k=0; k<c; k++)
                res[i][j]+= data[i][k]*x[k][j];
        }
    return res;
}
Matriz operator *(double k, Matriz x)
{
    return x*k;
}
Matriz Matriz::operator *(double k)
{
    for(uint i=0; i<l; i++)
        for(uint j=0; j<c; j++)
            data[i][j]*=k;
    return (*this);
}
Matriz Matriz::operator ^(uint k)
{
    Matriz res= (*this);
    for(uint i=1; i<k; i++)
        res= res*(*this);
    return res;
}
vector<double>& Matriz::operator[](int p)
{
    return data[p];
}

Matriz Matriz::Identidade(uint ord)
{
    Matriz res(ord, ord);
    for(uint i=0; i<ord; i++)
        for(uint j=0; j<ord; j++)
            res[i][j]= i==j;
    return res;
}
