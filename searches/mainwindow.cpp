#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Busca (+A)");
    // INICIALIZA OS 'ITENS' DE CADA QUADRADO, COM DOIS VALORES
    // VALOR 0 ----> TIPO DE CÉLULA (0: INICIO - 1: FIM - 2: BLOCK - 3: NORMAL)
    // VALOR 1 ----> DISTANCIA ATÉ O INÍCIO
    // VALOR 3 ----> DISTANCIA DO A ESTRELA (CALCULADA PELO stepAllstar())
    for(int x=0; x<ui->board->rowCount(); x++)
        for(int y=0; y<ui->board->columnCount(); y++){
            ui->board->setItem( x, y, new QTableWidgetItem(0));
            ui->board->setItem( x, y, new QTableWidgetItem(1));
            ui->board->setItem( x, y, new QTableWidgetItem(3));
            ui->board->item(x,y)->setData(0,3);
            ui->board->item(x,y)->setData(1,0);
            ui->board->item(x,y)->setData(3,0);
            ui->board->item(x,y)->setBackground(Qt::white);
    }sttDefined=false; endDefined=false;

    // SE UM QUADRADO FOR CLICADO, SERÁ ACIONADA A FUNÇÃO changeType(int,int)
    connect(ui->board, SIGNAL(cellPressed(int,int)),this, SLOT(changeType(int,int)));
    // SE FOR PRESSIONADO O BOTÃO DE LIMPAR, SERÁ ACIONADA A FUNÇÃO clearALL()
    connect(ui->clearAll, SIGNAL(pressed()), this, SLOT(clearALL()));
    connect(ui->reset, SIGNAL(pressed()), this, SLOT(clear()));
    // SE PRESSIONADO O BOTÃO RUN, SERÁ ACIONADA A FUNÇÃO triggerButton()
    connect(ui->startButton, SIGNAL(pressed()), this, SLOT(triggerButton()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// MÉTODO QUE MODIFICA O TIPO DO QUADRADO
void MainWindow::changeType(int row, int column)
{
    // PARA SELEÇÃO DO INÍCIO
    if(ui->Start->isChecked()){
        posX=row; posY=column;
        if(xpf.size()>0) {x_f.clear();  xpf.clear();}
        if(ypf.size()>0) {y_f.clear();  ypf.clear();}
        if (sttDefined){
            ui->board->item(sttX,sttY)->setBackground(Qt::white);
            ui->board->item(sttX,sttY)->setData(0,3);
            ui->board->item(sttX,sttY)->setData(3,0);
        }

        x_f.push_back(posX);  xpf.push_back(0);
        y_f.push_back(posY);  ypf.push_back(0);
        sttX = row; sttY = column;
        sttDefined=true;
        ui->board->item(row,column)->setBackground(Qt::green);
        ui->board->item(row,column)->setData(0,0);
    }
    // PARA SELEÇÃO DO FIM
    if(ui->End->isChecked()){
        if(endDefined){
            ui->board->item(endX,endY)->setBackground(Qt::white);
            ui->board->item(endX,endY)->setData(0,3);
            ui->board->item(endX,endY)->setData(3,0);
        }
        for(int x=0; x<ui->board->rowCount(); x++)
            for(int y=0; y<ui->board->columnCount(); y++)
                ui->board->item(x,y)->setData(1,abs(x-row)+abs(y-column));
        endDefined=true;
        endX=row; endY=column;

        ui->board->item(row,column)->setBackground(Qt::red);
        ui->board->item(row,column)->setData(0,1);

    }
    // PARA SELEÇÃO DOS BLOQUEIOS
    if(ui->Block->isChecked() && ui->board->item(row,column)->data(0).toInt() != 2){
        int comp = ui->board->item(row,column)->data(0).toInt();
        if(comp == 0 || comp == 1)
            return;
        ui->board->item(row,column)->setBackground(Qt::black);
        ui->board->item(row,column)->setData(0,2);

    }

    // DEBLOCKING
    else if(ui->board->item(row,column)->data(0).toInt() == 2){
        int comp = ui->board->item(row,column)->data(0).toInt();
        if(comp == 0 || comp == 1)
            return;
        ui->board->item(row,column)->setBackground(Qt::white);
        ui->board->item(row,column)->setData(0,3);
    }

}

void MainWindow::clear()
{
    for(int x=0; x<ui->board->rowCount(); x++)
        for(int y=0; y<ui->board->columnCount(); y++){
            if(ui->board->item(x,y)->data(0).toInt() == 3){
                ui->board->item(x,y)->setBackground(Qt::white);
                ui->board->item(x,y)->setData(0,3);
                ui->board->item(x,y)->setData(3,0);
            }else if(ui->board->item(x,y)->data(0).toInt() == 0){
                posX=x; posY=y;
            }
        }
    x_a.clear(); y_a.clear(); xpa.clear(); ypa.clear();
    x_f.erase(x_f.begin()+1, x_f.end()); y_f.erase(y_f.begin()+1, y_f.end());
    xpf.erase(xpf.begin()+1, xpf.end()); ypf.erase(ypf.begin()+1, ypf.end());
    dist_min.clear();
    ui->board->setEnabled(true);
    ui->startButton->setEnabled(true);
}

// LIMPA A TABELA
void MainWindow::clearALL()
{
    for(int x=0; x<ui->board->rowCount(); x++)
        for(int y=0; y<ui->board->columnCount(); y++){
            ui->board->item(x,y)->setBackground(Qt::white);
            ui->board->item(x,y)->setData(0,3);
            ui->board->item(x,y)->setData(1,0);
            ui->board->item(x,y)->setData(3,0);
        }
    delete &posX;
    delete &posY;
    x_a.clear(); y_a.clear(); xpa.clear(); ypa.clear();
    x_f.clear(); y_f.clear(); xpf.clear(); ypf.clear();
    dist_min.clear();
    ui->board->setEnabled(true);
    ui->startButton->setEnabled(true);
}

// PINTA QUADRADO VIZINHO DE MENOR VALOR E DESLOCA OS VALORES DE POSX E POSY PARA O MESMO
void MainWindow::stepGreedy(int row, int col)
{
    for(int x=-1; x<2; x++)
        for(int y=-1; y<2; y++){
            if(doesItExist(x+row,y+col) && !isItABlock(x+row,y+col) && notAgain(x+row,y+col)){
                dist_min.push_back(ui->board->item(x+row,y+col)->data(1).toInt());
                x_a.push_back(x+row);
                y_a.push_back(y+col);
                xpa.push_back(row);
                ypa.push_back(col);
            }
        }

    int mn = smallerNum(dist_min);
    posX = x_a[mn]; posY = y_a[mn];
    x_a.erase(x_a.begin()+mn); y_a.erase(y_a.begin()+mn);
    dist_min.erase(dist_min.begin()+mn);
    x_f.push_back(posX); y_f.push_back(posY);
    xpf.push_back(xpa[mn]); ypf.push_back(ypa[mn]);
    xpa.erase(xpa.begin()+mn); ypa.erase(ypa.begin()+mn);

    if(ui->board->item(posX,posY)->data(0).toInt() == 1)
        return;
    else
        ui->board->item(posX, posY)->setBackground(Qt::lightGray);
}

void MainWindow::stepAllstar(int row, int col)
{
    for(int x=-1; x<2; x++)
        for(int y=-1; y<2; y++){
            if(doesItExist(x+row,y+col) && !isItABlock(x+row,y+col) && !insideF(x+row,y+col)){
                int v3 = (x==0 || y==0 ? 10 : 14);
                int v4 = ui->board->item(row,col)->data(3).toInt();
                int v5 = ui->board->item(row+x,col+y)->data(3).toInt();
                if(v4+v3 < v5 || v5 == 0)
                    ui->board->item(x+row,y+col)->setData(3, v4+v3);
            }
            if(doesItExist(x+row,y+col) && !isItABlock(x+row,y+col) && notAgain(x+row,y+col)){
                int v1 = ui->board->item(x+row,y+col)->data(1).toInt();
                int v2 = ui->board->item(x+row,y+col)->data(3).toInt();
                dist_min.push_back(v1+v2);
                x_a.push_back(x+row);
                y_a.push_back(y+col);
            }
        }

    int mn = smallerNum(dist_min);
    posX = x_a[mn]; posY = y_a[mn];
    x_a.erase(x_a.begin()+mn); y_a.erase(y_a.begin()+mn);
    dist_min.erase(dist_min.begin()+mn);
    x_f.push_back(posX); y_f.push_back(posY);

    if(ui->board->item(posX,posY)->data(0).toInt() == 1)
        return;
    else
        ui->board->item(posX, posY)->setBackground(Qt::lightGray);
}

// ESSA FUNÇÃO SERIA PARA VERIFICAR SE HÁ UM QUADRADO VIZINHO NA DIREÇÃO INDICADA, COMPLETA A FUNÇÃO stepGreedy()
bool MainWindow::doesItExist(int row, int column)
{
    if(ui->board->item(row,column) != NULL)
        return true;

    return false;
}
// RECONHECE SE O OBJETO É UM BLOCK
bool MainWindow::isItABlock(int row, int col)
{
    if(ui->board->item(row,col)->data(0).toInt() == 2)
        return true;
    else
        return false;
}

// RECONHECE SE O CAMINHO JÁ FOI PERCORRIDO ANTERIORMENTE
bool MainWindow::notAgain(int row, int col)
{
    for(int i=0; i<x_f.size(); i++)
        if(row == x_f[i] && col == y_f[i])
            return false;

    if(x_a.size() > 0)
        for(int i=0; i<x_a.size(); ++i)
            if(row == x_a[i] && col == y_a[i])
                return false;

    return true;
}

bool MainWindow::insideF(int row, int col)
{
    for(int i=0; i<x_f.size(); i++)
        if(row == x_f[i] && col == y_f[i])
            return true;

    return false;
}

// RETORNA A POSIÇÃO NA LISTA DO QUADRADO MENOS DISTANTE DO FIM
int MainWindow::smallerNum(std::vector<int> aberta){

    int var = aberta[0]; int pos = 0;
    for(int i=1; i < aberta.size(); i++){
        if(aberta[i] < var){
            var = aberta[i];
            pos = i;
        }
    }

    return pos;
}

void MainWindow::Finale(int row, int col)
{
    if(ui->greedy->isChecked())
        showMeTheGreedyWay(row, col);
    else if(ui->aaa->isChecked())
        showMeTheWayPlease(row, col);
}

void MainWindow::showMeTheGreedyWay(int row, int col)
{
    if(ui->board->item(row,col)->data(0) == 0)
        return;
    else if (ui->board->item(row,col)->data(0) == 1)
        showMeTheGreedyWay(getXinF(row,col),getYinF(row,col));
    else{
        Sleep(50);
        ui->board->item(row,col)->setBackground(Qt::yellow);
        ui->board->repaint();
        showMeTheGreedyWay(getXinF(row,col),getYinF(row,col));
    }
}

void MainWindow::showMeTheWayPlease(int row, int col)
{
    if(ui->board->item(row,col)->data(0) == 0)
        return;
    else if (ui->board->item(row,col)->data(0) == 1)
        showMeTheWayPlease(getPaiX(row,col),getPaiY(row,col));
    else{
        Sleep(50);
        ui->board->item(row,col)->setBackground(Qt::yellow);
        ui->board->repaint();
        showMeTheWayPlease(getPaiX(row,col),getPaiY(row,col));
    }

}

void MainWindow::paintRest()
{
    Sleep(200);
    for(int i=0; i<x_a.size(); i++)
        ui->board->item(x_a[i],y_a[i])->setBackground(Qt::gray);
}

int MainWindow::getXinF(int row, int col)
{
    for(int i=0; i<x_f.size(); i++)
        if(row == x_f[i] && col == y_f[i])
            return xpf[i];
}
int MainWindow::getYinF(int row, int col)
{
    for(int i=0; i<x_f.size(); i++)
        if(row == x_f[i] && col == y_f[i])
            return ypf[i];
}

int MainWindow::getPaiX(int row, int col)
{
        int mejor = 10000000, x_mejor = 100, y_mejor = 100;
        for(int x=-1; x<2; ++x)
            for(int y=-1; y<2; ++y)
                if(insideF(row+x,col+y)){
                    if(ui->board->item(row+x,col+y)->data(3).toInt() < mejor){
                        mejor = ui->board->item(row+x,col+y)->data(3).toInt();
                        x_mejor = row+x;
                        y_mejor = col+y;
                    }
                }
        return x_mejor;

}

int MainWindow::getPaiY(int row, int col)
{
        int mejor = 10000000, x_mejor = 100, y_mejor = 100;
        for(int x=-1; x<2; ++x)
            for(int y=-1; y<2; ++y)
                if(insideF(row+x,col+y)){
                    if(ui->board->item(row+x,col+y)->data(3).toInt() < mejor){
                        mejor = ui->board->item(row+x,col+y)->data(3).toInt();
                        x_mejor = row+x;
                        y_mejor = col+y;
                    }
                }
        return y_mejor;

}

// MÉTODO PROCURA GULOSA
void MainWindow::Greedy()
{
    while(ui->board->item(posX,posY)->data(0).toInt(0) != 1){
        stepGreedy(posX, posY);
        ui->board->repaint();
        Sleep(10);
    }
}

// FUTURO MÉTODO A*
void MainWindow::Allstar()
{
    while(ui->board->item(posX,posY)->data(0).toInt(0) != 1){
        stepAllstar(posX, posY);
        ui->board->repaint();
        Sleep(10);
    }
}

// MÉTODO QUE ACIONA A PROCURA GULOSA OU A* QUANDO O BOTÃO É PRESSIONADO
void MainWindow::triggerButton()
{
    ui->board->setEnabled(false);
    ui->startButton->setEnabled(false);
    if(ui->greedy->isChecked())
        Greedy();
    else
        Allstar();

    Finale(posX,posY);
    paintRest();
}
