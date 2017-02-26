#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "funcoes.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //quanser= new Quanser("10.13.97.69", 20072);

    fuc= "WAIT";
    tempo= 0;
    ui->setupUi(this);
    ui->comboBoxSinal->addItem("Degrau");
    ui->comboBoxSinal->addItem("Senoidal");
    ui->comboBoxSinal->addItem("Onda quadrada");
    ui->comboBoxSinal->addItem("Dente de serra");
    ui->comboBoxSinal->addItem("Aleatorio");
      
    ui->customPlot->addGraph();

    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    //ui->customPlot->xAxis->setRange(-5, 5);
    ui->customPlot->yAxis->setRange(-5, 5);

    controle= new std::thread(&MainWindow::Controle, this);
    startTimer(10);

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
    }
    else if(sinal == "Senoidal")
    {
        ui->SpinBoxPeriodo->setEnabled(true);
    }
    else if(sinal == "Onda quadrada")
    {
        ui->SpinBoxPeriodo->setEnabled(true);
    }
    else if(sinal == "Dente de serra")
    {
        ui->SpinBoxPeriodo->setEnabled(true);
    }
    else if(sinal == "Aleatorio")
    {
        ui->SpinBoxPeriodo->setEnabled(false);
    }
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    ui->customPlot->replot();
}

void MainWindow::Controle()
{
    while(1)
    {
        //double val= 3.1;
        //quanser->writeDA(val);

        double A= ui->SpinBoxTensao->value();
        double T= ui->SpinBoxPeriodo->value();
        if(fuc == "Degrau")
        {
            ui->customPlot->graph(0)->addData(tempo, funcDegrau(A, tempo));
        }
        else if(fuc == "Senoidal")
        {
            ui->customPlot->graph(0)->addData(tempo, funcSenoidal(A, T, tempo));
        }
        else if(fuc == "Onda quadrada")
        {
            ui->customPlot->graph(0)->addData(tempo, funcQuadrada(A, T, tempo));
        }
        else if(fuc == "Dente de serra")
        {
            ui->customPlot->graph(0)->addData(tempo, funcSerra(A, T, tempo));
        }
        else if(fuc == "Aleatorio")
        {
            aleatorio z= funcAleatoria();
            // intervalo
            ui->customPlot->graph(0)->addData(tempo, z.amplitude);
        }
        ui->customPlot->graph(0)->removeDataBefore(tempo-12);
        ui->customPlot->xAxis->setRange(tempo + 0.25, 10, Qt::AlignRight);
        usleep(1*10E3);
        tempo+=0.01;
    }
}

bool canal0 = false;
bool canal1 = false;
bool canal2 = false;
bool canal3 = false;
bool canal4 = false;
bool canal5 = false;
bool canal6 = false;
bool canal7 = false;

void MainWindow::on_checkBox_clicked()
{
    canal0 = ui->checkBox->isChecked();
}

void MainWindow::on_checkBox_2_clicked()
{
    canal1 = ui->checkBox_2->isChecked();
}

void MainWindow::on_checkBox_3_clicked()
{
    canal2 = ui->checkBox_3->isChecked();
}
void MainWindow::on_checkBox_4_clicked()
{
    canal3 = ui->checkBox_4->isChecked();
}

void MainWindow::on_checkBox_5_clicked()
{
    canal4 = ui->checkBox_5->isChecked();
}

void MainWindow::on_checkBox_6_clicked()
{
    canal5 = ui->checkBox_6->isChecked();
}

void MainWindow::on_checkBox_7_clicked()
{
    canal6 = ui->checkBox_7->isChecked();
}

void MainWindow::on_checkBox_8_clicked()
{
    canal7 = ui->checkBox_8->isChecked();
}
