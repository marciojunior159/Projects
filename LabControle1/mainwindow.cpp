#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "funcoes.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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

float t=0;
void MainWindow::timerEvent(QTimerEvent *e)
{
   ui->customPlot->graph(0)->addData(t, funcQuadrada(3, 5, t));
   //ui->customPlot->graph(0)->removeDataBefore(key- 60);
   ui->customPlot->xAxis->setRange(t + 0.25, 10, Qt::AlignRight);
   ui->customPlot->replot();
   t+=0.01;
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
