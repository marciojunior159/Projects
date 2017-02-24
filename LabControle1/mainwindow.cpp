#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    x(500),
    y(500),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
