#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "funcoes.h"

#include <QMessageBox>
#include <chrono>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pid1(0,0,0), pid2(0,0,0)
{
    //Matriz L=observador.Calcula_L(complex<double>(0.97,0.0), complex<double>(0.0,0.0));
    Matriz L=observador.Calcula_L(complex<double>(0.8,0.0), complex<double>(0.0,0.0));

    //quanser= new Quanser("10.13.99.69", 20081);
    quanser= new Quanser("127.0.0.1", 20074);
    //quanser= new Quanser("192.168.0.33", 20081);
    //quanser= new Quanser("192.168.0.13", 20081);
    //quanser= new Quanser("192.168.0.7", 20081);
    //quanser= new Quanser("192.168.0.5", 20081);

    fucao= "";
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

    ui->doubleSpinBox_kp_2->setDecimals(3);
    ui->doubleSpinBox_ki_2->setDecimals(3);
    ui->doubleSpinBox_kd_2->setDecimals(3);

    ui->plotS1->addGraph();
    ui->plotS1->addGraph();
    ui->plotS1->addGraph();
    ui->plotS1->addGraph();
    ui->plotS1->addGraph();
    ui->plotS1->addGraph();
    ui->plotS1->addGraph();
    ui->plotS1->addGraph();
    ui->plotS1->graph(0)->setPen(QPen(Qt::blue));
    ui->plotS1->graph(1)->setPen(QPen(Qt::red));
    ui->plotS1->graph(2)->setPen(QPen(Qt::green));
    ui->plotS1->graph(3)->setPen(QPen(Qt::yellow));
    ui->plotS1->graph(4)->setPen(QPen(Qt::magenta));
    ui->plotS1->graph(5)->setPen(QPen(Qt::cyan));
    ui->plotS1->graph(6)->setPen(QPen(Qt::black));
    ui->plotS1->graph(7)->setPen(QPen(Qt::gray));

    ui->plotS1->xAxis->setLabel("tempo(ms)");
    ui->plotS1->yAxis->setAutoTickStep(false);
    ui->plotS1->yAxis->setTickStep(5);
    ui->plotS1->yAxis->setRange(-1, 30);
    ui->plotS1->legend->setVisible(true);
    ui->plotS1->legend->setFont(QFont("Helvetica", 9));
    ui->plotS1->legend->setRowSpacing(-5);
    ui->plotS1->graph(0)->setName("L1");
    ui->plotS1->graph(1)->setName("SP1");
    ui->plotS1->graph(2)->setName("L2");
    ui->plotS1->graph(3)->setName("SP2");
    ui->plotS1->graph(4)->setName("E1");
    ui->plotS1->graph(5)->setName("E2");
    ui->plotS1->graph(6)->setName("Lo1");
    ui->plotS1->graph(7)->setName("Lo2");
    ui->plotS1->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    ui->plotS2->addGraph();
    ui->plotS2->addGraph();
    ui->plotS2->addGraph();

    ui->plotS2->addGraph();
    ui->plotS2->addGraph();
    ui->plotS2->addGraph();

    ui->plotS2->addGraph();
    ui->plotS2->addGraph();
    ui->plotS2->addGraph();

    ui->plotS2->graph(0)->setPen(QPen(Qt::blue));
    ui->plotS2->graph(1)->setPen(QPen(Qt::red));
    ui->plotS2->graph(2)->setPen(QPen(Qt::green));

    ui->plotS2->graph(3)->setPen(QPen(Qt::magenta));
    ui->plotS2->graph(4)->setPen(QPen(Qt::cyan));
    ui->plotS2->graph(5)->setPen(QPen(Qt::yellow));

    ui->plotS2->graph(6)->setPen(QPen(Qt::darkMagenta));
    ui->plotS2->graph(7)->setPen(QPen(Qt::darkCyan));
    ui->plotS2->graph(8)->setPen(QPen(Qt::darkYellow));

    ui->plotS2->xAxis->setLabel("tempo(ms)");
    ui->plotS2->yAxis->setAutoTickStep(false);
    ui->plotS2->yAxis->setTickStep(1);
    ui->plotS2->yAxis->setRange(-5, 5);
    ui->plotS2->legend->setVisible(true);
    ui->plotS2->legend->setFont(QFont("Helvetica", 9));
    ui->plotS2->legend->setRowSpacing(-3);

    ui->plotS2->graph(0)->setName("Tensão saturada");
    ui->plotS2->graph(1)->setName("Tensão calculada");
    ui->plotS2->graph(2)->setName("Observador");

    ui->plotS2->graph(3)->setName("P (mestre)");
    ui->plotS2->graph(4)->setName("I (mestre)");
    ui->plotS2->graph(5)->setName("D (mestre)");

    ui->plotS2->graph(6)->setName("P (escravo)");
    ui->plotS2->graph(7)->setName("I (escravo)");
    ui->plotS2->graph(8)->setName("D (escravo)");
    ui->plotS2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    controle= new std::thread(&MainWindow::Controle, this);
    recebe= new std::thread(&MainWindow::Recebe, this);

    startTimer(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButtonMalhaAberta_clicked()
{
    Amplitude= 0;
    ui->labelTensaoNivel->setText("Tensão");
    ui->spinBoxCanal->setEnabled(true);
    ui->comboBoxSinal->setEnabled(true);
    ui->SpinBoxTensaoNivel->setValue(0);
    ui->SpinBoxTensaoNivel->setMinimum(-4);
    ui->SpinBoxTensaoNivel->setMaximum(4);
    on_comboBoxSinal_activated(QString("Degrau"));

    ui->comboBoxTipodeControle->setDisabled(true);
    ui->doubleSpinBox_kp->setDisabled(true);
    ui->doubleSpinBox_ki->setDisabled(true);
    ui->doubleSpinBox_kd->setDisabled(true);
    ui->radioButtonGanho->setDisabled(true);
    ui->radioButtonTempo->setDisabled(true);

    ui->comboBoxTipodeControle_2->setDisabled(true);
    ui->doubleSpinBox_kp_2->setDisabled(true);
    ui->doubleSpinBox_ki_2->setDisabled(true);
    ui->doubleSpinBox_kd_2->setDisabled(true);
    ui->radioButtonGanho_2->setDisabled(true);
    ui->radioButtonTempo_2->setDisabled(true);

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
    Amplitude= 0;
    ui->labelTensaoNivel->setText("Nivel");
    ui->spinBoxCanal->setEnabled(true);
    ui->comboBoxSinal->setEnabled(true);
    ui->SpinBoxTensaoNivel->setValue(0);
    ui->SpinBoxTensaoNivel->setMinimum(0);
    ui->SpinBoxTensaoNivel->setMaximum(30);
    on_comboBoxSinal_activated(QString("Degrau"));

    ui->comboBoxTipodeControle->setEnabled(true);
    ui->comboBoxTipodeControle->setCurrentIndex(0);
    ui->doubleSpinBox_kp->setEnabled(true);
    ui->radioButtonGanho->setEnabled(true);
    ui->radioButtonTempo->setEnabled(true);

    ui->SpinBoxPeriodoOffset->setEnabled(true);
    ui->comboBoxSinalOrdem->setEnabled(true);

    ui->radioButtonMalhaAberta->setChecked(false);
    ui->radioButtonMalhaFechada->setChecked(true);
}

void MainWindow::on_comboBoxSinal_activated(const QString &sinal)
{
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

void MainWindow::timerEvent(QTimerEvent *t)
{
    ui->label_altura->update();
    ui->label_altura_2->update();

    ui->plotS1->xAxis->setRange(tempo + 0.25, 25, Qt::AlignRight);
    ui->plotS2->xAxis->setRange(tempo + 0.25, 25, Qt::AlignRight);

    ui->plotS1->replot();
    ui->plotS2->replot();

    mutex_.lock();
    ui->plotS1->graph(0)->removeDataBefore(tempo-30);
    ui->plotS2->graph(0)->removeDataBefore(tempo-30);
    mutex_.unlock();

    if(ui->checkBoxL1_SP1->isChecked()){
        ui->plotS1->graph(0)->setVisible(1);
        ui->plotS1->graph(1)->setVisible(1);
    }else{
        ui->plotS1->graph(0)->setVisible(0);
        ui->plotS1->graph(1)->setVisible(0);
    }

    if(ui->checkBoxL2_SP2->isChecked()){
        ui->plotS1->graph(2)->setVisible(1);
        ui->plotS1->graph(3)->setVisible(1);
    }else{
        ui->plotS1->graph(2)->setVisible(0);
        ui->plotS1->graph(3)->setVisible(0);
    }

    if(ui->checkBox_E1->isChecked()){
        ui->plotS1->graph(4)->setVisible(1);
    }
    else{
        ui->plotS1->graph(4)->setVisible(0);
    }

    if(ui->checkBox_E2->isChecked()){
        ui->plotS1->graph(5)->setVisible(1);
    }
    else{
        ui->plotS1->graph(5)->setVisible(0);
    }

    if(ui->checkBox_PIDe->isChecked()){
        ui->plotS2->graph(6)->setVisible(1);
        ui->plotS2->graph(7)->setVisible(1);
        ui->plotS2->graph(8)->setVisible(1);
    }else{
        ui->plotS2->graph(6)->setVisible(0);
        ui->plotS2->graph(7)->setVisible(0);
        ui->plotS2->graph(8)->setVisible(0);
    }

    if(ui->checkBox_PIDm->isChecked()){
        ui->plotS2->graph(3)->setVisible(1);
        ui->plotS2->graph(4)->setVisible(1);
        ui->plotS2->graph(5)->setVisible(1);
    }else{
        ui->plotS2->graph(3)->setVisible(0);
        ui->plotS2->graph(4)->setVisible(0);
        ui->plotS2->graph(5)->setVisible(0);
    }

    if(ui->checkBox_Tensoes->isChecked()){
        ui->plotS2->graph(0)->setVisible(1);
        ui->plotS2->graph(1)->setVisible(1);
    }else{
        ui->plotS2->graph(0)->setVisible(0);
        ui->plotS2->graph(1)->setVisible(0);
    }

    if(ui->checkBox_Observador->isChecked()){
        ui->plotS2->graph(2)->setVisible(1);
    }else{
        ui->plotS2->graph(2)->setVisible(0);
    }

    if(ui->checkBox_Observador->isChecked())
    {
        ui->plotS1->graph(6)->setVisible(1);
        ui->plotS1->graph(7)->setVisible(1);
    }
    else
    {
        ui->plotS1->graph(6)->setVisible(0);
        ui->plotS1->graph(7)->setVisible(0);
    }
}

auto now = std::chrono::high_resolution_clock::now();

void MainWindow::Controle()
{
    double tensaoCalculado, st= 0, pv, tensao; // dt= 0
    Matriz L(2, 1);
    while(1)
    {
        int canal= ui->spinBoxCanal->value();

        double sensores[2];
        for(int i=0; i<2; i++)
            sensores[i]= quanser->readAD(i);

        //ST = referencia; //PV = leitura da altura dos tanques

        if(fucao == "Degrau")
            st= funcDegrau(Amplitude, tempo, offset);
        else if(fucao == "Senoidal")
            st= funcSenoidal(Amplitude, Periodo, tempo, offset);
        else if(fucao == "Onda quadrada")
            st= funcQuadrada(Amplitude, Periodo, tempo, offset);
        else if(fucao == "Dente de serra")
            st= funcSerra(Amplitude, Periodo, tempo, offset);
        else if(fucao == "Aleatorio") // intervalo
        {
            if(ui->radioButtonMalhaAberta->isChecked())
                st= funcAleatoria1(tempo);
            else
                st= funcAleatoria2(tempo);
        }
        else
            st= 0;

        mutex_.lock();
        ui->plotS1->graph(0)->addData(tempo, funcSensor(sensores[0]));
        ui->plotS1->graph(2)->addData(tempo, funcSensor(sensores[1]));
        mutex_.unlock();

        ui->label_altura->setText(QString::number(funcSensor(sensores[0])));
        ui->label_altura_2->setText(QString::number(funcSensor(sensores[1])));

        if(ui->radioButtonMalhaAberta->isChecked())
        {
            pv = funcSensor(sensores[0]);
            tensaoCalculado = st;
            tensao = trava(st, pv);

            quanser->writeDA(canal, tensao);

            mutex_.lock();
            ui->plotS1->graph(1)->addData(tempo, 0);
            ui->plotS1->graph(3)->addData(tempo, 0);
            mutex_.unlock();
        }
        else if(ui->radioButtonMalhaFechada->isChecked())
        {
            double st2, erro1, erro2, pv1;

            if(ui->comboBoxSinalOrdem->currentText() == "Primeira"){
                pv=pv1= funcSensor(sensores[0]);
                erro1= st - pv;

                if(ui->comboBoxTipodeControle->currentText() == "PI-D")
                    tensao = pid1.Controle(erro1,pv1,0.1);
                else
                    tensao = pid1.Controle(erro1, 0.1);

                mutex_.lock();
                ui->plotS1->graph(1)->addData(tempo, st);
                ui->plotS2->graph(3)->addData(tempo, pid1.getP());
                ui->plotS2->graph(4)->addData(tempo, pid1.getI());
                ui->plotS2->graph(5)->addData(tempo, pid1.getD());
                mutex_.unlock();
            }
            else if(ui->comboBoxSinalOrdem->currentText() == "Segunda (2C)")
            {
                pv1 = funcSensor(sensores[0]);
                pv = funcSensor(sensores[1]);

                erro1 = st - pv;
                erro2 = st2 - pv1;

                if(ui->comboBoxTipodeControle->currentText() == "PI-D")
                {
                    st2 = pid1.Controle(erro1,pv1,0.1);
                    tensao = pid2.Controle(erro2,pv,0.1);
                }
                else
                {
                    st2 = pid1.Controle(erro1,0.1);
                    tensao = pid2.Controle(erro2, 0.1);
                }

                mutex_.lock();
                ui->plotS1->graph(1)->addData(tempo, st2);
                ui->plotS1->graph(3)->addData(tempo, st);
                ui->plotS1->graph(4)->addData(tempo, abs(erro2));
                ui->plotS1->graph(5)->addData(tempo, abs(erro1));

                ui->plotS2->graph(3)->addData(tempo, pid1.getP());
                ui->plotS2->graph(4)->addData(tempo, pid1.getI());
                ui->plotS2->graph(5)->addData(tempo, pid1.getD());
                ui->plotS2->graph(6)->addData(tempo, pid2.getP());
                ui->plotS2->graph(7)->addData(tempo, pid2.getI());
                ui->plotS2->graph(8)->addData(tempo, pid2.getD());
                mutex_.unlock();
            }else{
                //segunda ordem com apenas um controlador
                pv1 = funcSensor(sensores[0]);
                pv = funcSensor(sensores[1]);

                erro1 = st - pv;

                if(ui->comboBoxTipodeControle->currentText() == "PI-D")
                {
                    tensao = pid1.Controle(erro1,pv,0.1);
                }
                else
                {
                    tensao = pid1.Controle(erro1,0.1);
                }

                mutex_.lock();
                ui->plotS1->graph(3)->addData(tempo, st);
                ui->plotS1->graph(4)->addData(tempo, 0);
                ui->plotS1->graph(5)->addData(tempo, abs(erro1));

                ui->plotS2->graph(3)->addData(tempo, pid1.getP());
                ui->plotS2->graph(4)->addData(tempo, pid1.getI());
                ui->plotS2->graph(5)->addData(tempo, pid1.getD());
                mutex_.unlock();
            }

            tensaoCalculado = tensao;
            tensao = trava(tensao, pv1);
            pid1.antWindUP(tensaoCalculado, tensao);
            pid2.antWindUP(tensaoCalculado, tensao);

            quanser->writeDA(canal, tensao);
        }

        if(ui->checkBox_observacao->isChecked())
        {
            if(ui->radioButton_Polos->isChecked()){
                //calcula L com os polos
                L = observador.Calcula_L(complex<double>(ui->doubleSpinBox_p1_real->value(), ui->doubleSpinBox_p1_img->value()), complex<double>(ui->doubleSpinBox_p2_real->value(), ui->doubleSpinBox_p2_img->value()));
                ui->labelL->setText("["+ QString::number(L[0][0]) +"    "+ QString::number(L[0][1])+"]");
                //mostra L
                observador.setL(L);
                Matriz y(1,1), u(1,1), x(2,1);
                u[0][0]= tensao;
                y[0][0]= funcSensor(sensores[1]);
                x= observador.Observa(y,u);
                mutex_.lock();
                ui->plotS1->graph(6)->addData(tempo, x[0][0]);
                ui->plotS1->graph(7)->addData(tempo, x[1][0]);
                mutex_.unlock();
            }else if(ui->radioButton_matrizGanhos){
                //calcula os polos com L
                L[0][0]=ui->doubleSpinBox_L1->value();
                L[0][1]=ui->doubleSpinBox_L2->value();
                vector<complex<double>> polos = observador.Calcula_Polos(L);

                ui->labelPolos->setText("("+ QString::number(polos[0].real()) + "+i"+QString::number(polos[0].imag()) +", "+QString::number(polos[1].real()) + "+i"+QString::number(polos[1].imag())+")");
                //mostra os polos
                observador.setL(L);
                Matriz y(1,1), u(1,1), x(2,1);
                u[0][0]= tensao;
                y[0][0]= funcSensor(sensores[1]);
                x= observador.Observa(y,u);
                mutex_.lock();
                ui->plotS1->graph(6)->addData(tempo, x[0][0]);
                ui->plotS1->graph(7)->addData(tempo, x[1][0]);
                mutex_.unlock();

            }

        }

        mutex_.lock();
        ui->plotS2->graph(0)->addData(tempo, tensao);
        ui->plotS2->graph(1)->addData(tempo, tensaoCalculado);
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

        //dt= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-now).count()/1000.0;
        //now = std::chrono::high_resolution_clock::now();
        tempo+=0.1;

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

void MainWindow::on_pushButtonEnviar_clicked()
{
    fucao= ui->comboBoxSinal->currentText().toStdString();
    Amplitude = ui->SpinBoxTensaoNivel->value();
    Periodo = ui->SpinBoxPeriodo->value();
    offset=ui->SpinBoxPeriodoOffset->value();

    st_ant = Amplitude;
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
        pid1.setConstantes(ui->doubleSpinBox_kp->isEnabled()?ui->doubleSpinBox_kp->value():0,
                          ui->doubleSpinBox_ki->isEnabled()?ui->doubleSpinBox_ki->value():0,
                          ui->doubleSpinBox_kd->isEnabled()?ui->doubleSpinBox_kd->value():0);
    }else if(ui->radioButtonTempo->isChecked()){
        pid1.setConstantesT(ui->doubleSpinBox_kp->isEnabled()?ui->doubleSpinBox_kp->value():0,
                           ui->doubleSpinBox_ki->isEnabled()?ui->doubleSpinBox_ki->value():0,
                           ui->doubleSpinBox_kd->isEnabled()?ui->doubleSpinBox_kd->value():0);
    }
    if(ui->radioButtonGanho_2->isChecked()){
        pid2.setConstantes(ui->doubleSpinBox_kp_2->isEnabled()?ui->doubleSpinBox_kp_2->value():0,
                          ui->doubleSpinBox_ki_2->isEnabled()?ui->doubleSpinBox_ki_2->value():0,
                          ui->doubleSpinBox_kd_2->isEnabled()?ui->doubleSpinBox_kd_2->value():0);
    }else if(ui->radioButtonTempo_2->isChecked()){
        pid2.setConstantesT(ui->doubleSpinBox_kp_2->isEnabled()?ui->doubleSpinBox_kp_2->value():0,
                           ui->doubleSpinBox_ki_2->isEnabled()?ui->doubleSpinBox_ki_2->value():0,
                           ui->doubleSpinBox_kd_2->isEnabled()?ui->doubleSpinBox_kd_2->value():0);
    }
    if(ui->comboBoxSinalOrdem->currentText()=="Segunda (2C)" || ui->comboBoxSinalOrdem->currentText()=="Segunda (1C)"){
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
    fucao= "";
    Amplitude = 0;
    Periodo = 0;
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

void MainWindow::on_comboBoxTipodeControle_activated(const QString &tipo)
{
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

void MainWindow::on_comboBoxSinalOrdem_activated(const QString &ordem)
{
    if(ordem == "Primeira"){
        ui->comboBoxTipodeControle_2->setEnabled(false);
        ui->comboBoxTipodeControle_2->setCurrentIndex(0);
        ui->doubleSpinBox_kp_2->setEnabled(false);
        ui->doubleSpinBox_ki_2->setEnabled(false);
        ui->doubleSpinBox_kd_2->setEnabled(false);
        ui->radioButtonGanho_2->setEnabled(false);
        ui->radioButtonTempo_2->setEnabled(false);
    }
    else if(ordem=="Segunda (1C)"){
        ui->comboBoxSinalfaixats->setEnabled(true);
        ui->comboBoxSinalfaixa_tr->setEnabled(true);

        ui->comboBoxTipodeControle_2->setEnabled(false);
        ui->comboBoxTipodeControle_2->setCurrentIndex(0);
        ui->doubleSpinBox_kp_2->setEnabled(false);
        ui->doubleSpinBox_ki_2->setEnabled(false);
        ui->doubleSpinBox_kd_2->setEnabled(false);
        ui->radioButtonGanho_2->setEnabled(false);
        ui->radioButtonTempo_2->setEnabled(false);
    }
    else if(ordem=="Segunda (2C)"){
        ui->comboBoxSinalfaixats->setEnabled(true);
        ui->comboBoxSinalfaixa_tr->setEnabled(true);

        ui->lcdNumber_mp->setEnabled(true);
        ui->lcdNumber_tr->setEnabled(true);
        ui->lcdNumber_ts->setEnabled(true);
        ui->lcdNumber_tp->setEnabled(true);

        ui->comboBoxTipodeControle_2->setEnabled(true);
        ui->comboBoxTipodeControle_2->setCurrentIndex(0);

        ui->doubleSpinBox_kp_2->setEnabled(true);

        ui->radioButtonGanho_2->setEnabled(true);
        ui->radioButtonTempo_2->setEnabled(true);
    }
}

void MainWindow::on_comboBoxTipodeControle_2_activated(const QString &tipo)
{
    if(tipo == "P"){

        ui->doubleSpinBox_ki_2->setEnabled(false);
        ui->doubleSpinBox_kd_2->setEnabled(false);

        ui->doubleSpinBox_ki_2->setValue(0);
        ui->doubleSpinBox_kd_2->setValue(0);

    }else if(tipo == "PI"){

        ui->doubleSpinBox_ki_2->setEnabled(true);
        ui->doubleSpinBox_kd_2->setEnabled(false);

        ui->doubleSpinBox_kd_2->setValue(0);


    }else if(tipo == "PD"){

        ui->doubleSpinBox_ki_2->setEnabled(false);
        ui->doubleSpinBox_kd_2->setEnabled(true);

        ui->doubleSpinBox_ki_2->setValue(0);

    }else if(tipo == "PID"){

        ui->doubleSpinBox_ki_2->setEnabled(true);
        ui->doubleSpinBox_kd_2->setEnabled(true);

    }else if(tipo == "PI-D"){

        ui->doubleSpinBox_ki_2->setEnabled(true);
        ui->doubleSpinBox_kd_2->setEnabled(true);
    }
}

void MainWindow::on_radioButtonGanho_2_clicked(bool checked)
{
    if(checked == true){
        ui->label_kd_2->setText("Ganho (Kd)");
        ui->label_ki_2->setText("Ganho (Ki)");
    }
}

void MainWindow::on_radioButtonTempo_2_clicked(bool checked)
{
    if(checked == true){
        ui->label_kd_2->setText("Tempo (Td)");
        ui->label_ki_2->setText("Tempo (Ti)");
    }
}

void MainWindow::on_checkBox_observacao_clicked(bool checked)
{
    if(checked == true){
        //desativar pid
        ui->comboBoxSinalOrdem->setCurrentText("Segunda (1C)");
        ui->doubleSpinBox_kp->setValue(ui->doubleSpinBox_kp_obs->value());

        ui->doubleSpinBox_kp_obs->setEnabled(true);
        ui->doubleSpinBox_p1_real->setEnabled(true);
        ui->doubleSpinBox_p1_img->setEnabled(true);
        ui->doubleSpinBox_p2_img->setEnabled(true);
        ui->doubleSpinBox_p2_img->setEnabled(true);
        ui->doubleSpinBox_L1->setEnabled(true);
        ui->doubleSpinBox_L2->setEnabled(true);

    }else{
        ui->doubleSpinBox_kp_obs->setEnabled(false);
        ui->doubleSpinBox_p1_real->setEnabled(false);
        ui->doubleSpinBox_p1_img->setEnabled(false);
        ui->doubleSpinBox_p2_img->setEnabled(false);
        ui->doubleSpinBox_p2_img->setEnabled(false);
        ui->doubleSpinBox_L1->setEnabled(false);
        ui->doubleSpinBox_L2->setEnabled(false);
    }
}
