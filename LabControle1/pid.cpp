#include "pid.h"
#include <iostream>
using namespace std;

PID::PID(double Kp, double Ki, double Kd)
{
    this->Kp= Kp;
    this->Ki= Ki;
    this->Kd= Kd;
    I=D=e_ant=0;
}
void PID::setConstantes(double Kp, double Ki, double Kd)
{
    this->Kp= Kp;
    this->Ki= Ki;
    this->Kd= Kd;
}
void PID::setConstantesT(double Kp, double Ti, double Td)
{
    this->Kp= Kp;
    this->Ki= Ti==0?0:(Kp/Ti);
    this->Kd= Kp*Td;
}
double PID::Controle(double e, double h)
{
    I= I+Ki*e*h*(!windUp); //  simpsons (e+e_ant)*h/2
    D= Kd*(e-e_ant)/h;
    e_ant= e;
    this->p = Kp*e;
    this->i = I;
    this->d = D;
    return Kp*e+I+D;
}

double PID::Controle(double e, double y, double h)
{
    I= I+Ki*e*h*(!windUp);
    D= Kd*(y-e_ant)/h;
    e_ant= y;
    this->p = Kp*e;
    this->i = I;
    this->d = D;
    return Kp*e+I+D;
}
void PID::antWindUP(double cal, double sat)
{
    if(cal != sat)
        windUp= true;
    else
        windUp= false;
}

double PID::getP(){
    return p;
}

double PID::getI(){
    return i;
}

double PID::getD(){
    return d;
}
