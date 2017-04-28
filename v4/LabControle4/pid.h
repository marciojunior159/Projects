#ifndef PID_H
#define PID_H


class PID
{
public:
    PID(double Kp, double Ki, double Kd);
    void setConstantes(double Kp, double Ki, double Kd);
    void setConstantesT(double K, double Ti, double Td);
    double Controle(double e, double h);
    double Controle(double e, double y, double h);
private:
    double Kp, Ki, Kd;
    double I, D, e_ant;
};

#endif // PID_H
