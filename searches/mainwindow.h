#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <stdlib.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setBoard();

public slots:
    void changeType(int row, int column);
    void clear();
    void clearALL();
    void stepGreedy(int,int);
    void stepAllstar(int,int);
    bool doesItExist(int,int);
    bool isItABlock(int,int);
    bool notAgain(int,int);
    bool insideF(int,int);
    int smallerNum(std::vector<int>);
    void Finale(int,int);
    void showMeTheGreedyWay(int,int);
    void showMeTheWayPlease(int,int);
    void paintRest();
    int getXinF(int,int);
    int getYinF(int,int);
    int getPaiX(int,int);
    int getPaiY(int,int);
    void Greedy();
    void Allstar();
    void triggerButton();

private:
    Ui::MainWindow *ui;
    int posX, posY; // POSIÇÃO ATUAL DA PROCURA
    int sttX, sttY; // POSIÇÃO START
    bool sttDefined;
    int endX, endY; // POSIÇÃO END
    bool endDefined;
    QColor s,e,f,a,m;
    std::vector<int> x_f, y_f, x_a, y_a, xpf, ypf, xpa, ypa, dist_min;

};

#endif // MAINWINDOW_H
