#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "funcoes.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //quanser= new Quanser("10.13.97.69", 20072);

    ui->setupUi(this);
    ui->comboBox->addItem("Degrau");
    ui->comboBox->addItem("Senoidal");
    ui->comboBox->addItem("Onda quadrada");
    ui->comboBox->addItem("Dente de serra");
    ui->comboBox->addItem("Aleatorio");
      
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


void MainWindow::on_radioButton_clicked()
{
    if(ui->radioButton->isChecked())
    {
        ui->doubleSpinBox->setEnabled(true);
        ui->doubleSpinBox_2->setEnabled(false);
        ui->comboBox->setEnabled(true);
        ui->spinBox->setEnabled(true);
    }
}

void MainWindow::on_radioButton_2_clicked()
{
    if(ui->radioButton_2->isChecked())
    {
        ui->doubleSpinBox->setEnabled(false);
        ui->doubleSpinBox_2->setEnabled(true);
        ui->comboBox->setEnabled(true);
    }
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    QString sinal = ui->comboBox->currentText();

    if(sinal == "Degrau")
    {
        ui->spinBox_2->setEnabled(false);
    }
    else if(sinal == "Senoidal")
    {
        ui->spinBox_2->setEnabled(true);
    }
    else if(sinal == "Onda quadrada")
    {
        ui->spinBox_2->setEnabled(true);
    }
    else if(sinal == "Dente de serra")
    {
        ui->spinBox_2->setEnabled(true);
    }
    else if(sinal == "Aleatorio")
    {
        ui->spinBox_2->setEnabled(false);
    }
}

float t=0, lt=0;
#include <time.h>
#include <QDebug>

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
        ui->customPlot->graph(0)->addData(t, funcSenoidal(3, 2, t));
        //ui->customPlot->graph(0)->removeDataBefore(key- 60);
        ui->customPlot->xAxis->setRange(t + 0.25, 10, Qt::AlignRight);

        usleep(1*10E3);
        t+=0.01;
    }
}
