#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "funcoes.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //quanser= new Quanser("10.13.97.69", 20072);
    quanser= new Quanser("127.0.0.1", 20072);

    fuc= "WAIT";
    tempo= 0;
    ui->setupUi(this);
    ui->comboBoxSinal->addItem("Degrau");
    ui->comboBoxSinal->addItem("Senoidal");
    ui->comboBoxSinal->addItem("Onda quadrada");
    ui->comboBoxSinal->addItem("Dente de serra");
    ui->comboBoxSinal->addItem("Aleatorio");


    ui->customPlot->addGraph();
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    ui->customPlot->yAxis->setRange(-5, 5);

    ui->plotS1->addGraph();
    ui->plotS1->addGraph();
    ui->plotS1->graph(0)->setPen(QPen(Qt::blue));
    ui->plotS1->graph(1)->setPen(QPen(Qt::red));
    ui->plotS1->xAxis->setLabel("x");
    ui->plotS1->yAxis->setLabel("y");
    ui->plotS1->yAxis->setRange(-5, 5);

    ui->plotS2->addGraph();
    ui->plotS2->addGraph();
    ui->plotS2->graph(0)->setPen(QPen(Qt::blue));
    ui->plotS2->graph(1)->setPen(QPen(Qt::red));
    ui->plotS2->xAxis->setLabel("x");
    ui->plotS2->yAxis->setLabel("y");
    ui->plotS2->yAxis->setRange(-5, 5);

    controle= new std::thread(&MainWindow::Controle, this);
    recebe= new std::thread(&MainWindow::Recebe, this);
    startTimer(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButtonMalhaAberta_clicked()
{
    fuc= ui->comboBoxSinal->currentText().toStdString();
    if(ui->radioButtonMalhaAberta->isChecked())
    {
        ui->SpinBoxTensao->setEnabled(true);
        ui->SpinBoxNivel->setEnabled(false);
        ui->spinBoxCanal->setEnabled(true);
        ui->comboBoxSinal->setEnabled(true);
    }
}

void MainWindow::on_radioButtonMalhaFechada_clicked()
{
    fuc= ui->comboBoxSinal->currentText().toStdString();
    if(ui->radioButtonMalhaFechada->isChecked())
    {
        ui->SpinBoxTensao->setEnabled(false);
        ui->SpinBoxNivel->setEnabled(true);
        ui->spinBoxCanal->setEnabled(true);
        ui->comboBoxSinal->setEnabled(true);
    }
}

void MainWindow::on_comboBoxSinal_activated(const QString &arg1)
{
    QString sinal = ui->comboBoxSinal->currentText();

    fuc= sinal.toStdString();
    if(sinal == "Degrau")
    {
        ui->SpinBoxPeriodo->setEnabled(false);
        ui->SpinBoxTensao->setEnabled(true);
    }
    else if(sinal == "Senoidal")
    {
        ui->SpinBoxPeriodo->setEnabled(true);
        ui->SpinBoxTensao->setEnabled(true);
    }
    else if(sinal == "Onda quadrada")
    {
        ui->SpinBoxPeriodo->setEnabled(true);
        ui->SpinBoxTensao->setEnabled(true);
    }
    else if(sinal == "Dente de serra")
    {
        ui->SpinBoxPeriodo->setEnabled(true);
        ui->SpinBoxTensao->setEnabled(true);
    }
    else if(sinal == "Aleatorio")
    {
        ui->SpinBoxPeriodo->setEnabled(false);
        ui->SpinBoxTensao->setEnabled(false);
    }
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    ui->customPlot->replot();
    ui->customPlot->graph(0)->removeDataBefore(tempo-12);
    ui->customPlot->xAxis->setRange(tempo + 0.25, 10, Qt::AlignRight);

    ui->plotS1->replot();
    ui->plotS1->graph(0)->removeDataBefore(tempo-12);
    ui->plotS1->xAxis->setRange(tempo + 0.25, 10, Qt::AlignRight);

    ui->plotS2->replot();
    ui->plotS2->graph(0)->removeDataBefore(tempo-12);
    ui->plotS2->xAxis->setRange(tempo + 0.25, 10, Qt::AlignRight);

    if(canais[0])
        ui->plotS1->graph(0)->setVisible(1);
    else
        ui->plotS1->graph(0)->setVisible(0);

    if(canais[1])
        ui->plotS2->graph(0)->setVisible(1);
    else
        ui->plotS2->graph(0)->setVisible(0);

    if(canais[2])
        ui->plotS1->graph(1)->setVisible(1);
    else
        ui->plotS1->graph(1)->setVisible(0);

    if(canais[3])
        ui->plotS2->graph(1)->setVisible(1);
    else
        ui->plotS2->graph(1)->setVisible(0);
    /*
    if(canais[4])
        ui->plotS1->graph(0)->setVisible(1);
    if(canais[5])
        ui->plotS1->graph(0)->setVisible(1);
    if(canais[6])
        ui->plotS1->graph(0)->setVisible(1);
    if(canais[7])
        ui->plotS1->graph(0)->setVisible(1);
    */

}

#include <chrono>
auto now = std::chrono::high_resolution_clock::now();

void MainWindow::Controle()
{
    while(1)
    {
        int canal= ui->spinBoxCanal->value();

        double sensores[8];
        for(int i=0; i<8; i++)
            sensores[i]= quanser->readAD(i);

        double val=0;

        if(fuc == "Degrau")
        {
            val= funcDegrau(A, tempo);
        }
        else if(fuc == "Senoidal")
        {
            val= funcSenoidal(A, T, tempo);
        }
        else if(fuc == "Onda quadrada")
        {
            val= funcQuadrada(A, T, tempo);
        }
        else if(fuc == "Dente de serra")
        {
            val= funcSerra(A, T, tempo);
        }
        else if(fuc == "Aleatorio") // intervalo
        {
            val= funcAleatoria(tempo);
        }

        ui->plotS1->graph(0)->addData(tempo, sensores[0]);
        ui->plotS1->graph(1)->addData(tempo, sensores[3]+0.5);
        ui->plotS2->graph(0)->addData(tempo, sensores[1]);
        ui->plotS2->graph(1)->addData(tempo, sensores[4]-0.5);
        ui->customPlot->graph(0)->addData(tempo, val);

        if(ui->radioButtonMalhaAberta->isChecked())
        {
            quanser->writeDA(canal, val);
        }
        else if(ui->radioButtonMalhaFechada->isChecked())
        {
            // Calculo malha fechada e controle
            quanser->writeDA(canal, val);
        }

        qDebug() << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-now).count() << "ms\n";
        now = std::chrono::high_resolution_clock::now();
        tempo+=0.1;

        usleep(10.0*10E3);
    }
}

void MainWindow::Recebe()
{
    while(0)
    {

       usleep(10E4);
    }
}

void MainWindow::on_checkBox_clicked()
{
    canais[0] = ui->checkBox->isChecked();
}

void MainWindow::on_checkBox_2_clicked()
{
    canais[1] = ui->checkBox_2->isChecked();
}

void MainWindow::on_checkBox_3_clicked()
{
    canais[2] = ui->checkBox_3->isChecked();
}
void MainWindow::on_checkBox_4_clicked()
{
    canais[3] = ui->checkBox_4->isChecked();
}

void MainWindow::on_checkBox_5_clicked()
{
    canais[4] = ui->checkBox_5->isChecked();
}

void MainWindow::on_checkBox_6_clicked()
{
    canais[5] = ui->checkBox_6->isChecked();
}

void MainWindow::on_checkBox_7_clicked()
{
    canais[6] = ui->checkBox_7->isChecked();
}

void MainWindow::on_checkBox_8_clicked()
{
    canais[7] = ui->checkBox_8->isChecked();
}

void MainWindow::on_pushButtonEnviar_clicked()
{
    A = ui->SpinBoxTensao->value();
    T = ui->SpinBoxPeriodo->value();
}

void MainWindow::on_pushButtonCancel_clicked()
{
    A = 0;
    T = 0;
}
