#ifndef FUNCOES_H
#define FUNCOES_H

double funcDegrau(double A, double t);
double funcSerra(double A, double T, double t);
struct aleatorio {
  double amplitude;
  double intervalo;
};

double funcAleatoria(double t);
double funcSenoidal(double A, double T, double t);
double funcQuadrada(double A, double T, double t);

#endif // FUNCOES_H
