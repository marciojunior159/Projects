#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <quanser.h>
#include <thread>
#include <mutex>
#include <QTime>
#include <pid.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void timerEvent(QTimerEvent *);
    void Controle();
    void Recebe();

    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void on_checkBox_3_clicked();
    void on_checkBox_4_clicked();
    void on_checkBox_5_clicked();
    void on_checkBox_6_clicked();
    void on_checkBox_7_clicked();
    void on_checkBox_8_clicked();
    void on_radioButtonMalhaAberta_clicked();
    void on_radioButtonMalhaFechada_clicked();
    void on_comboBoxSinal_activated(const QString &arg1);

    void on_pushButtonEnviar_clicked();

    void on_pushButtonCancel_clicked();

    //void on_radioButton_clicked(bool checked);

    void on_radioButtonGanho_clicked(bool checked);

    void on_radioButtonTempo_clicked(bool checked);

    void on_comboBoxTipodeControle_activated(const QString &arg1);

    void on_comboBoxSinalOrdem_activated(const QString &arg1);

private:
    Quanser* quanser;
    Ui::MainWindow *ui;
    std::thread* controle, *recebe;
    string fuc;
    float tempo;
    bool canais[8];
    PID pid;
    double A =0;
    double T = 0;
    double offset = 0;
    float tempoInicial; //tempo de subida
    float tempoInicialAcom = 0; //tempo de acomodacao
    float pv_ant = 0;
    float ts = 0; //tempo de acomodacao
    float trs = 0; //tempo de subida
    float mp = 0; //overshoot absoluto
    float tp = 0; //tempo de pico
    bool flag_tr = false; //flag tempo de subida
    bool flag_ts = false; //flag tempo de acomodacao
    bool flag_mp = false; //flag overshoot
    bool flag_tp = false; //flag tempo de pico
    bool flag_2ordem = false;
    double st_ant=0;
    double tolerancia_ts = 0.05; //faixa de tolerancia do tempo de acomodacao
    double faixa_tr = 0;
    double max_mp=0;
    int contFaixa = 0;
    std::mutex mutex_;

};

#endif // MAINWINDOW_H
