#ifndef FUNCOES_H
#define FUNCOES_H

double funcDegrau(double A, double t, double offset);
double funcSerra(double A, double T, double t, double offset);

double funcAleatoria1(double t);
double funcAleatoria2(double t);
double funcSenoidal(double A, double T, double t, double offset);
double funcQuadrada(double A, double T, double t, double offset);

double funcAlturaTensao(double alt);
double funcSensor(double leituras);

#endif // FUNCOES_H
