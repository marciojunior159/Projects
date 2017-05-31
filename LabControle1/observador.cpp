#include "observador.h"

Observador::Observador(): G(2,2), H(2,1), C(1,2), L(2,1), x(2,1), W_inv(2,2)
{
    G[0][0]= 0,999344; G[0][1]= 0;
    G[1][0]= 0,000656; G[1][1]= 0,999344;

    H[0][0]= 0,0296334;
    H[1][0]= 0,0000292;

    C[0][0]= 0; C[0][1]= 1;

    L[0][0]= 0.5;
    L[1][0]= 0.7;

    x[0][0]=0;
    x[1][0]= 0;

    W_inv[0][0]= -1523.61; W_inv[0][1]= 1524.61;
    W_inv[1][0]= 1; W_inv[1][1]= 0;
}
Matriz Observador::Calcula_L(complex<double> p1, complex<double> p2)
{
    Matriz I= Matriz::Identidade(2), aux(2,1);
    double c[2]= {(-p1-p2).real(), (p1*p2).real()};
    aux[0][0]= 0;
    aux[1][0]= 1;
    Matriz Ackerman= (G^2)+(c[0]*G)+(c[1]*I);
    L= (Ackerman*W_inv)*aux;
    return L;
}
Matriz Observador::Observa(Matriz y, Matriz u)
{
    x= G*x+L*(y-(C*x))+H*u;
    return x;
}
