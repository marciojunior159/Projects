#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <quanser.h>
#include <thread>
#include <mutex>
#include <QTime>
#include <pid.h>
#include <observador.h>

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

    void on_radioButtonMalhaAberta_clicked();
    void on_radioButtonMalhaFechada_clicked();
    void on_comboBoxSinal_activated(const QString &arg1);
    void on_pushButtonEnviar_clicked();
    void on_pushButtonCancel_clicked();
    void on_radioButtonGanho_clicked(bool checked);
    void on_radioButtonTempo_clicked(bool checked);
    void on_comboBoxTipodeControle_activated(const QString &arg1);
    void on_comboBoxSinalOrdem_activated(const QString &arg1);
    void on_comboBoxTipodeControle_2_activated(const QString &arg1);
    void on_radioButtonGanho_2_clicked(bool checked);
    void on_radioButtonTempo_2_clicked(bool checked);
    void on_checkBox_observacao_clicked(bool checked);
private:
    Quanser* quanser;
    Ui::MainWindow *ui;
    std::mutex mutex_;

    PID pid1, pid2;
    Observador observador;
    std::thread* controle, *recebe;

    string fucao;
    float tempo;
    double Amplitude =0;
    double Periodo = 0;
    double offset = 0;
    double pv_ant = 0;
    double st_ant=0;

    double tempoInicial; //tempo de subida
    double tempoInicialAcom = 0; //tempo de acomodacao
    float ts = 0; //tempo de acomodacao
    float trs = 0; //tempo de subida
    float mp = 0; //overshoot absoluto
    float tp = 0; //tempo de pico
    bool flag_tr = false; //flag tempo de subida
    bool flag_ts = false; //flag tempo de acomodacao
    bool flag_mp = false; //flag overshoot
    bool flag_tp = false; //flag tempo de pico
    bool flag_2ordem = false;
    double tolerancia_ts = 0.05; //faixa de tolerancia do tempo de acomodacao
    double faixa_tr = 0;
    double max_mp=0;
    int contFaixa = 0;
};

#endif // MAINWINDOW_H
