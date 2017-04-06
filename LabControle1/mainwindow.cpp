#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "funcoes.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    pid(0,0,0),
    ui(new Ui::MainWindow)
{
    quanser= new Quanser("10.13.99.69", 20081);
    //quanser= new Quanser("127.0.0.1", 20074);

    fuc= "WAIT";
    tempo= 0;
    ui->setupUi(this);
    ui->comboBoxSinal->addItem("Degrau");
    ui->comboBoxSinal->addItem("Senoidal");
    ui->comboBoxSinal->addItem("Onda quadrada");
    ui->comboBoxSinal->addItem("Dente de serra");
    ui->comboBoxSinal->addItem("Aleatorio");

    ui->doubleSpinBox_kp->setDecimals(3);
    ui->doubleSpinBox_ki->setDecimals(3);
    ui->doubleSpinBox_kd->setDecimals(3);

    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->xAxis->setLabel("tempo(ms)");
    ui->customPlot->yAxis->setLabel("tensão(V)");
    ui->customPlot->yAxis->setRange(-5, 5);
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setFont(QFont("Helvetica", 9));
    ui->customPlot->legend->setRowSpacing(-5);
    ui->customPlot->graph(0)->setName("Saturada");
    ui->customPlot->graph(1)->setName("Calculada");
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);


    ui->plotS1->addGraph();
    ui->plotS1->addGraph();    
    ui->plotS1->graph(0)->setPen(QPen(Qt::blue));
    ui->plotS1->graph(1)->setPen(QPen(Qt::red));
    ui->plotS1->xAxis->setLabel("tempo(ms)");
    ui->plotS1->yAxis->setLabel("altura(cm)");
    ui->plotS1->yAxis->setAutoTickStep(false);
    ui->plotS1->yAxis->setTickStep(5);
    ui->plotS1->yAxis->setRange(-1, 30);
    ui->plotS1->legend->setVisible(true);
    ui->plotS1->legend->setFont(QFont("Helvetica", 9));
    ui->plotS1->legend->setRowSpacing(-5);
    ui->plotS1->graph(0)->setName("Sensor 1");
    ui->plotS1->graph(1)->setName("Set Point");
    ui->plotS1->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);


    ui->plotS2->addGraph();
    ui->plotS2->addGraph();
    ui->plotS2->graph(0)->setPen(QPen(Qt::blue));
    ui->plotS2->graph(1)->setPen(QPen(Qt::red));
    ui->plotS2->xAxis->setLabel("tempo(ms)");
    ui->plotS2->yAxis->setLabel("altura(cm)");
    ui->plotS2->yAxis->setAutoTickStep(false);
    ui->plotS2->yAxis->setTickStep(5);
    ui->plotS2->yAxis->setRange(-1, 30);
    ui->plotS2->legend->setVisible(true);
    ui->plotS2->legend->setFont(QFont("Helvetica", 9));
    ui->plotS2->legend->setRowSpacing(-3);
    ui->plotS2->graph(0)->setName("Sensor 2");
    ui->plotS2->graph(1)->setName("Set Point");
    ui->plotS2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);


    controle= new std::thread(&MainWindow::Controle, this);
    recebe= new std::thread(&MainWindow::Recebe, this);
    canais[0]= true;
    canais[1]= true;
    for(int i=2; i<8; i++)
        canais[i]= false;

    startTimer(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButtonMalhaAberta_clicked()
{
    ui->labelTensaoNivel->setText("Tensão");
    ui->spinBoxCanal->setEnabled(true);
    ui->comboBoxSinal->setEnabled(true);
    ui->SpinBoxTensaoNivel->setValue(0);
    A= 0;
    ui->SpinBoxTensaoNivel->setMinimum(-4);
    ui->SpinBoxTensaoNivel->setMaximum(4);
    on_comboBoxSinal_activated(QString());

    ui->comboBoxTipodeControle->setDisabled(true);
    ui->doubleSpinBox_kp->setDisabled(true);
    ui->doubleSpinBox_ki->setDisabled(true);
    ui->doubleSpinBox_kd->setDisabled(true);
    ui->radioButtonGanho->setDisabled(true);
    ui->radioButtonTempo->setDisabled(true);

    ui->SpinBoxPeriodoOffset->setEnabled(true);
    ui->comboBoxSinalOrdem->setEnabled(false);

    ui->lcdNumber_mp->setEnabled(false);
    ui->lcdNumber_tr->setEnabled(false);
    ui->lcdNumber_ts->setEnabled(false);
    ui->lcdNumber_tp->setEnabled(false);

    ui->comboBoxSinalfaixats->setEnabled(false);
    ui->comboBoxSinalfaixa_tr->setEnabled(false);


    ui->radioButtonMalhaAberta->setChecked(true);
    ui->radioButtonMalhaFechada->setChecked(false);
}

void MainWindow::on_radioButtonMalhaFechada_clicked()
{
    ui->labelTensaoNivel->setText("Nivel");
    ui->spinBoxCanal->setEnabled(true);
    ui->comboBoxSinal->setEnabled(true);
    A= 0;
    ui->SpinBoxTensaoNivel->setValue(0);
    ui->SpinBoxTensaoNivel->setMinimum(0);
    ui->SpinBoxTensaoNivel->setMaximum(30);
    on_comboBoxSinal_activated(QString());


    ui->comboBoxTipodeControle->setEnabled(true);
    ui->comboBoxTipodeControle->setCurrentIndex(0);
    ui->doubleSpinBox_kp->setEnabled(true);
    //ui->doubleSpinBox_ki->setEnabled(true);
    //ui->doubleSpinBox_kd->setEnabled(true);
    ui->radioButtonGanho->setEnabled(true);
    ui->radioButtonTempo->setEnabled(true);

    ui->SpinBoxPeriodoOffset->setEnabled(true);
    ui->comboBoxSinalOrdem->setEnabled(true);

    ui->radioButtonMalhaAberta->setChecked(false);
    ui->radioButtonMalhaFechada->setChecked(true);
}

void MainWindow::on_comboBoxSinal_activated(const QString &arg1)
{
    QString sinal = ui->comboBoxSinal->currentText();

    if(sinal == "Degrau")
    {
        ui->SpinBoxTensaoNivel->setEnabled(true);
        ui->SpinBoxPeriodo->setEnabled(false);

        ui->lcdNumber_mp->setEnabled(true);
        ui->lcdNumber_tr->setEnabled(true);
        ui->lcdNumber_ts->setEnabled(true);
        ui->lcdNumber_tp->setEnabled(true);

        ui->comboBoxSinalfaixats->setEnabled(true);
        ui->comboBoxSinalfaixa_tr->setEnabled(true);
    }
    else if(sinal == "Senoidal")
    {
        ui->SpinBoxTensaoNivel->setEnabled(true);
        ui->SpinBoxPeriodo->setEnabled(true);

        ui->lcdNumber_mp->setEnabled(false);
        ui->lcdNumber_tr->setEnabled(false);
        ui->lcdNumber_ts->setEnabled(false);
        ui->lcdNumber_tp->setEnabled(false);

        ui->comboBoxSinalfaixats->setEnabled(false);
        ui->comboBoxSinalfaixa_tr->setEnabled(false);
    }
    else if(sinal == "Onda quadrada")
    {
        ui->SpinBoxTensaoNivel->setEnabled(true);
        ui->SpinBoxPeriodo->setEnabled(true);

        ui->lcdNumber_mp->setEnabled(true);
        ui->lcdNumber_tr->setEnabled(true);
        ui->lcdNumber_ts->setEnabled(true);
        ui->lcdNumber_tp->setEnabled(true);

        ui->comboBoxSinalfaixats->setEnabled(true);
        ui->comboBoxSinalfaixa_tr->setEnabled(true);
    }
    else if(sinal == "Dente de serra")
    {
        ui->SpinBoxTensaoNivel->setEnabled(true);
        ui->SpinBoxPeriodo->setEnabled(true);

        ui->lcdNumber_mp->setEnabled(false);
        ui->lcdNumber_tr->setEnabled(false);
        ui->lcdNumber_ts->setEnabled(false);
        ui->lcdNumber_tp->setEnabled(false);

        ui->comboBoxSinalfaixats->setEnabled(false);
        ui->comboBoxSinalfaixa_tr->setEnabled(false);
    }
    else if(sinal == "Aleatorio")
    {
        ui->SpinBoxTensaoNivel->setEnabled(false);
        ui->SpinBoxPeriodo->setEnabled(false);

        ui->lcdNumber_mp->setEnabled(true);
        ui->lcdNumber_tr->setEnabled(true);
        ui->lcdNumber_ts->setEnabled(true);
        ui->lcdNumber_tp->setEnabled(true);

        ui->comboBoxSinalfaixats->setEnabled(true);
        ui->comboBoxSinalfaixa_tr->setEnabled(true);
    }
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    ui->label_altura->update();
    ui->customPlot->xAxis->setRange(tempo + 0.25, 25, Qt::AlignRight);
    ui->plotS1->xAxis->setRange(tempo + 0.25, 25, Qt::AlignRight);
    ui->plotS2->xAxis->setRange(tempo + 0.25, 25, Qt::AlignRight);

    ui->customPlot->replot();
    ui->plotS1->replot();
    ui->plotS2->replot();

    mutex_.lock();
    ui->customPlot->graph(0)->removeDataBefore(tempo-30);
    ui->plotS1->graph(0)->removeDataBefore(tempo-30);
    ui->plotS2->graph(0)->removeDataBefore(tempo-30);
    mutex_.unlock();

    if(canais[0])
        ui->plotS1->graph(0)->setVisible(1);
    else
        ui->plotS1->graph(0)->setVisible(0);

    if(canais[1])
        ui->plotS2->graph(0)->setVisible(1);
    else
        ui->plotS2->graph(0)->setVisible(0);

    if(canais[0])
        ui->plotS1->graph(1)->setVisible(1);
    else
        ui->plotS1->graph(1)->setVisible(0);

    if(canais[1])
        ui->plotS2->graph(1)->setVisible(1);
    else
        ui->plotS2->graph(1)->setVisible(0);


}

#include <chrono>
auto now = std::chrono::high_resolution_clock::now();

void MainWindow::Controle()
{
    double tensaoCalculado, dt;
    now = std::chrono::high_resolution_clock::now();
    while(1)
    {
        //now = std::chrono::high_resolution_clock::now();
        int canal= ui->spinBoxCanal->value();

        double sensores[8];
        for(int i=0; i<2; i++)
            sensores[i]= quanser->readAD(i);

        //ST = referencia; //PV = leitura da altura dos tanques
        double tensao=0, st, erro, pv;

        if(fuc == "Degrau")
        {
            tensao= funcDegrau(A, tempo, offset);
        }
        else if(fuc == "Senoidal")
        {
            tensao= funcSenoidal(A, T, tempo, offset);
        }
        else if(fuc == "Onda quadrada")
        {
            tensao= funcQuadrada(A, T, tempo, offset);
        }
        else if(fuc == "Dente de serra")
        {
            tensao= funcSerra(A, T, tempo, offset);
        }
        else if(fuc == "Aleatorio") // intervalo
        {
            if(ui->radioButtonMalhaAberta->isChecked())
                tensao= funcAleatoria1(tempo);
            else
                tensao= funcAleatoria2(tempo);
        }
        else
        {
            tensao= 0;
        }

        mutex_.lock();
        ui->plotS1->graph(0)->addData(tempo, funcSensor(sensores[0]));
        ui->plotS2->graph(0)->addData(tempo, funcSensor(sensores[1]));
        mutex_.unlock();


        ui->label_altura->setText(QString::number(funcSensor(sensores[0])));
        //
        //qDebug() << funcSensor(sensores[0]) << endl;

//        ui->plotS3->graph(0)->addData(tempo, sensores[4]);
//        ui->plotS3->graph(1)->addData(tempo, sensores[6]);
//        ui->plotS4->graph(0)->addData(tempo, sensores[5]);
//        ui->plotS4->graph(1)->addData(tempo, sensores[7]);

        if(ui->radioButtonMalhaAberta->isChecked())
        {
            mutex_.lock();
            ui->plotS1->graph(1)->addData(tempo, 0);
            ui->plotS2->graph(1)->addData(tempo, 0);
            mutex_.unlock();
            pv = funcSensor(sensores[0]);
            //Trava #1
            tensaoCalculado = tensao;

            if(tensao>3.9)
                tensao = 3.9;
            if(tensao<-3.9)
                tensao = -3.9;
            //Trava #2 e #3
            if(pv <= 1.5 && tensao < 0){
                tensao = 0;
            }else if(pv >= 30 && tensao > 0){
                tensao = 2.75; //tensao de equilibrio
            }
            quanser->writeDA(canal, tensao);
        }
        else if(ui->radioButtonMalhaFechada->isChecked())
        {
            st = tensao;

            if(ui->comboBoxSinalOrdem->currentText() == "Primeira"){
                pv = funcSensor(sensores[0]);
                mutex_.lock();
                ui->plotS2->graph(1)->addData(tempo, 0);
                ui->plotS1->graph(1)->addData(tempo, st);
                mutex_.unlock();
            }else{
                pv = funcSensor(sensores[1]);
                mutex_.lock();
                ui->plotS1->graph(1)->addData(tempo, 0);
                ui->plotS2->graph(1)->addData(tempo, tensao);
                mutex_.unlock();
            }
            erro = st - pv;
            //tensao = funcAlturaTensao(st)+erro;
            if(ui->comboBoxTipodeControle->currentText() == "PI-D")
                tensao = pid.Controle(erro,pv,0.1);
            else
                tensao = pid.Controle(erro, 0.1);

            tensaoCalculado = tensao;
            //Travas e saturacao
            if(tensao > 3.9)
                tensao = 4;
            if(tensao<-3.9)
                tensao = -4;

            if(funcSensor(sensores[0]) <= 1.5 && tensao < 0){
                tensao = 0;
            }else if(funcSensor(sensores[0]) >= 30 && tensao > 0){
                tensao = 2.75; //tensao de equilibrio
            }

            quanser->writeDA(canal, tensao);
        }

        mutex_.lock();
        ui->customPlot->graph(0)->addData(tempo, tensao);
        ui->customPlot->graph(1)->addData(tempo, tensaoCalculado);
        mutex_.unlock();

        //tempo de subida
        if( fabs(pv-st_ant) >= fabs(faixa_tr*(st-st_ant)) && trs == 0 && flag_tr == false){
            tempoInicial = tempo;
            flag_tr = true;
        }

        if( fabs(pv-st_ant) >= fabs((1-faixa_tr)*(st-st_ant)) && flag_tr == true){
            trs = tempo - tempoInicial;
            ui->lcdNumber_tr->display(trs);
            flag_tr = false;
        }
        //mp
        if(fabs(pv-st_ant) > fabs(st-st_ant) && trs != 0)
        {
            if(fabs(pv-st)>max_mp)
            {
                max_mp= fabs(pv-st);
                mp = 100.0*(pv - st)/(st - st_ant);
                tp = tempo - tempoInicialAcom;
                ui->lcdNumber_tp->display(tp);
                ui->lcdNumber_mp->display(mp);
            }
        }
        /*
        if( fabs(pv-st_ant) >= fabs(st-st_ant) && mp == 0 && flag_mp == false){
            flag_mp = fabs(pv-st) > fabs(pv_ant-st)? false: true;
            if(fabs(pv-st)>max)
            {
                max= fabs(pv-st);
            }
            if(st!=0 && flag_mp == true){
                mp = 100.0*(pv_ant - st)/(st - st_ant);
                ui->lcdNumber_mp->display(mp);

                if(flag_tp == false){
                    tp = tempo - tempoInicialAcom;
                    ui->lcdNumber_tp->display(tp);
                    flag_tp = true;
                }
            }
        }
        if(pv <= st && mp == 0 && flag_mp == false && st_ant > st){
            flag_mp = pv < pv_ant? false: true;
            qDebug() << flag_mp << endl;

            if(st!=0 && flag_mp == true){
                mp = 100.0*(st - pv_ant)/(st_ant - st);
                ui->lcdNumber_mp->display(mp);
            }
        }
        */

        //tempo de acomodacao
        if(pv >= (1-tolerancia_ts)*st && pv_ant < (1-tolerancia_ts)*st){
            contFaixa++; //entrou por baixo
            ts = tempo;
        }
        if(pv >= (1+tolerancia_ts)*st && pv_ant < (1+tolerancia_ts)*st){
            contFaixa++; // saiu por cima
            ts = tempo;
        }
        if(pv <= (1-tolerancia_ts)*st && pv_ant > (1-tolerancia_ts)*st){
            contFaixa++; //saiu por baixo
            ts = tempo;
        }
        if(pv <= (1+tolerancia_ts)*st && pv_ant > (1+tolerancia_ts)*st){
            contFaixa++; // entrou por cima
            ts = tempo;
        }
        if(contFaixa%2==1){
            ui->lcdNumber_ts->display(ts-tempoInicialAcom);
        }

        //qDebug() << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-now).count() << "us\n";
        double t= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-now).count()/1000.0;
        now = std::chrono::high_resolution_clock::now();
        //qDebug() << pv << " " << pv_ant << " " << " " << st_ant << " " << st << endl;
        //qDebug() << t << endl;
        tempo+=0.1;
        //tempo+=0.1;
        usleep((0.1)*10E5);
        pv_ant = pv;
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
    st_ant = A;
    fuc= ui->comboBoxSinal->currentText().toStdString();
    A = ui->SpinBoxTensaoNivel->value();
    T = ui->SpinBoxPeriodo->value();
    offset=ui->SpinBoxPeriodoOffset->value();

    max_mp= 0;
    trs = 0;
    mp = 0;
    tp = 0;
    contFaixa = 0;
    flag_mp = false;
    flag_tp = false;
    tempoInicialAcom = tempo;

    ui->lcdNumber_tr->display(0);
    ui->lcdNumber_mp->display(0);
    ui->lcdNumber_tp->display(0);
    ui->lcdNumber_ts->display(0);


    if(ui->radioButtonGanho->isChecked()){
        pid.setConstantes(ui->doubleSpinBox_kp->isEnabled()?ui->doubleSpinBox_kp->value():0,
                          ui->doubleSpinBox_ki->isEnabled()?ui->doubleSpinBox_ki->value():0,
                          ui->doubleSpinBox_kd->isEnabled()?ui->doubleSpinBox_kd->value():0);
    }else if(ui->radioButtonTempo->isChecked()){
        pid.setConstantesT(ui->doubleSpinBox_kp->isEnabled()?ui->doubleSpinBox_kp->value():0,
                           ui->doubleSpinBox_ki->isEnabled()?ui->doubleSpinBox_ki->value():0,
                           ui->doubleSpinBox_kd->isEnabled()?ui->doubleSpinBox_kd->value():0);
    }

    if(ui->comboBoxSinalOrdem->currentText()=="Segunda"){
        flag_2ordem = true;
    }else{
        flag_2ordem = false;
    }

    switch(ui->comboBoxSinalfaixats->currentIndex()){
        case 0:
            tolerancia_ts = 0.02;
            break;
        case 1:
            tolerancia_ts = 0.05;
            break;
        case 2:
            tolerancia_ts = 0.07;
            break;
        case 3:
            tolerancia_ts = 0.1;
            break;
    }


    switch(ui->comboBoxSinalfaixa_tr->currentIndex()){
        case 2:
            faixa_tr = 0.0;
            break;
        case 1:
            faixa_tr = 0.05;
            break;
        case 0:
            faixa_tr = 0.1;
            break;

    }


}

void MainWindow::on_pushButtonCancel_clicked()
{
    fuc= "";
    A = 0;
    T = 0;
    offset=0;
    contFaixa= 0;
    max_mp= 0;
    trs=0;
    flag_tp = false;
    ui->lcdNumber_tr->display(0);
    ui->lcdNumber_mp->display(0);
    ui->lcdNumber_tp->display(0);
    ui->lcdNumber_ts->display(0);
}

void MainWindow::on_radioButtonGanho_clicked(bool checked)
{
    if(checked==true){
        ui->label_kd->setText("Ganho (Kd)");
        ui->label_ki->setText("Ganho (Ki)");
    }
}

void MainWindow::on_radioButtonTempo_clicked(bool checked)
{
    if(checked==true){
        ui->label_kd->setText("Tempo (Td)");
        ui->label_ki->setText("Tempo (Ti)");
    }
}

void MainWindow::on_comboBoxTipodeControle_activated(const QString &arg1)
{
    QString tipo = ui->comboBoxTipodeControle->currentText();
    if(tipo == "P"){

        ui->doubleSpinBox_ki->setEnabled(false);
        ui->doubleSpinBox_kd->setEnabled(false);

        ui->doubleSpinBox_ki->setValue(0);
        ui->doubleSpinBox_kd->setValue(0);

    }else if(tipo == "PI"){

        ui->doubleSpinBox_ki->setEnabled(true);
        ui->doubleSpinBox_kd->setEnabled(false);

        ui->doubleSpinBox_kd->setValue(0);


    }else if(tipo == "PD"){

        ui->doubleSpinBox_ki->setEnabled(false);
        ui->doubleSpinBox_kd->setEnabled(true);

        ui->doubleSpinBox_ki->setValue(0);

    }else if(tipo == "PID"){
        ui->doubleSpinBox_ki->setEnabled(true);
        ui->doubleSpinBox_kd->setEnabled(true);

    }else if(tipo == "PI-D"){

        ui->doubleSpinBox_ki->setEnabled(true);
        ui->doubleSpinBox_kd->setEnabled(true);

    }
}

void MainWindow::on_comboBoxSinalOrdem_activated(const QString &arg1)
{
    QString ordem = ui->comboBoxSinalOrdem->currentText();

    ui->lcdNumber_mp->setEnabled(true);
    ui->lcdNumber_tr->setEnabled(true);
    ui->lcdNumber_ts->setEnabled(true);
    ui->lcdNumber_tp->setEnabled(true);

    ui->comboBoxSinalfaixats->setEnabled(true);
    ui->comboBoxSinalfaixa_tr->setEnabled(true);
}
