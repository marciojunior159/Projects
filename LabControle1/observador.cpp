#include "observador.h"

complex<double> operator *(double k, complex<double> c)
{
    return complex<double>(k*c.real(), k*c.imag());
}

Observador::Observador(): G(2,2), H(2,1), C(1,2), L(2,1), x(2,1), W_inv(2,2)
{
    G[0][0]= 0.9934570177803322;   G[0][1]= 0;
    G[1][0]= 0.006521530073323866; G[1][1]= 0.9934570177803322;
    // calcular
    H[0][0]= 0.0295461;
    H[1][0]= 0.0000968713;

    C[0][0]= 0; C[0][1]= 1;

    L[0][0]= 0.0;
    L[1][0]= 0.0;

    x[0][0]= 0;
    x[1][0]= 0;
    // usando o sensor do tanque 2
    //W_inv[0]= C[0];
    //W_inv[1]= (C*G)[0];

    W_inv[0][0]= -152.3349592213091; W_inv[0][1]= 153.3382486558594;
    W_inv[1][0]= 1; W_inv[1][1]= 0;
}
vector<complex<double>> Observador::Calcula_Polos(Matriz ll)
{
    complex<double> a00(G[0][0],0), a01(G[0][1],0), a10(G[1][0],0), a11(G[1][1],0);
    complex<double> l1(ll[0][0],0), l2(ll[1][0],0);
    vector<complex<double>> polos;
    complex<double> p1= -(-a00-a11+l2+sqrt(pow(l2,2)+(2.0*a00-2*a11)*l2-4.0*a10*l1+pow(a11,2.0)-2.0*a00*a11+4.0*a01*a10+pow(a00,2)))/2.0;
    complex<double> p2=  (a00+a11-l2+sqrt(pow(l2,2)+(2.0*a00-2.0*a11)*l2-4.0*a10*l1+pow(a11,2)-2.0*a00*a11+4.0*a01*a10+pow(a00,2)))/2.0;
    polos.push_back(p1);
    polos.push_back(p2);
    return polos;
}
Matriz Observador::Calcula_L(complex<double> p1, complex<double> p2)
{
    Matriz I= Matriz::Identidade(2), aux(2,1);
    aux[0][0]= 0;
    aux[1][0]= 1;
    double c[2]= {(-p1-p2).real(), (p1*p2).real()};
    Matriz Ackerman= (G^2)+(c[0]*G)+(c[1]*I);
    L= (Ackerman*W_inv)*aux;
    return L;
}
Matriz Observador::Observa(Matriz y, Matriz u)
{
    x= G*x+L*(y-(C*x))+H*u;
    if(x[0][0] < 0)
        x[0][0]= 0;
    if(x[0][0] > 30)
        x[0][0]= 30;
    if(x[1][0] < 0)
        x[1][0]= 0;
    if(x[1][0] > 30)
        x[1][0]= 30;
    return x;
}
