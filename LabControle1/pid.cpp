#include "pid.h"

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

void PID::setConstantesT(double K, double Ti, double Td)
{
    this->Kp= Kp;
    this->Ki= Ti*Kp;
    this->Kd= Kp/Td;
}

double PID::Controle(double e, double h)
{
    I= I+Ki*e*h;
    D= Kd*(e-e_ant)/h;
    e_ant= e;
    return Kp*e+I+D;
}

double PID::Controle(double e, double y, double h)
{
    I= I+Ki*e*h;
    D= Kd*(y-e_ant)/h;
    e_ant= y;
    return Kp*e+I+D;
}
