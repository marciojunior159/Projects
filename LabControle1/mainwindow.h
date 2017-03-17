#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <quanser.h>
#include <thread>
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
};

#endif // MAINWINDOW_H
