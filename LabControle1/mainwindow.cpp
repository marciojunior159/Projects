#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSettings>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    x(500),
    y(500),
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


    startTimer(100);

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
    string sinal;
    //QMessageBox::QPrivateSignal(this, "Title",);
    sinal = QSettings::value("settings/comboText","").toString();
    ui->comboBox->setCurrentText(sinal);

    //if("Title" == "Degrau"){ui->spinBox_2->setEnabled(false);}if("Title" == "Senoidal"){ui->spinBox_2->setEnabled(true);}

#include <QDebug>
#include "funcoes.h"

float t=0;
void MainWindow::timerEvent(QTimerEvent *e)
{
   t+=0.1;
   qDebug() << funcSerra(t, 3, 1) << endl;
   ui->customPlot->graph(0)->addData(t, func(3, 3, t));
   //ui->customPlot->graph(0)->removeDataBefore(key- 60);
   ui->customPlot->xAxis->setRange(t + 0.25, 1, Qt::AlignRight);
   ui->customPlot->replot();

}
