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
    void antWindUP(double cal, double sat);
    double getP();
    double getI();
    double getD();
private:
    bool windUp= false;
    double Kp, Ki, Kd;
    double I, D, e_ant;
    double p, i, d;
};

#endif // PID_H
