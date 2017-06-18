#include "seguidor.h"

#include <iostream>
using namespace std;

Seguidor::Seguidor() : Ga(3,3), Ha(3,1), K(1,3), Kaux(3,3), W_inv(3,3)
{
    erro=0;
//    Kaux= {{-0.006542982219667759, 0, 0.0295461},
//          {0.006521530073323866, -0.006542982219667759, 0.0000968713},
//          {0.006521530073323866, 0.9934570177803322, 0.0000968713}};
//    Kaux= Matriz::Inversa(Kaux);
    Kaux= { {- 0.5010940253652179 ,  151.8355063141445   , 1.000000000349655},
            {         -0.0        , -0.9999999999999999  , 0.9999999999999999 },
            {  33.73444720967077  ,    33.62396452078953 , 0.2214499450673878 }};

    cout << Kaux << endl;

    Ga= { {0.9934570177803322,   0,                  0.0295461},
          {0.006521530073323866, 0.9934570177803322, 0.0000968713},
          {0,                    0,                  0} };

    Ha= {{0},
         {0},
         {1}};

//    Matriz W(3,3);
//    W= {{0, 0.0295461,    0.0293528},
//        {0, 0.0000968713, 0.000288922},
//        {1, 0,            0}};
    W_inv= {{0,       0,        1},
            {50.7497, -5155.88, 0},
            {-17.0156, 5189.83, 0}};

    K= {{0.0, 0.0, 0.0}};
}

Matriz Seguidor::Calcula_K(complex<double> p1, complex<double> p2, complex<double> p3)
{
    Matriz I= Matriz::Identidade(3), aux(1,3);
    aux= {{0,0,1}};
    double c[3]= {(-p1-p2-p3).real(), (p1*p2+p2*p3+p1*p3).real(),(-p1*p2*p3).real()};
    Matriz Ackerman= (Ga^3)+(Ga^2)*c[0]+Ga*c[1]+I*c[2];
    K= aux*W_inv*Ackerman;
    K= (K+aux)*Kaux;
    return K;
}

double Seguidor::Seguir(Matriz x, Matriz r)
{
    erro+=r[0][0]-x[1][0];
    return -K[0][0]*x[0][0]-K[0][1]*x[1][0]+K[0][2]*(erro);
}
