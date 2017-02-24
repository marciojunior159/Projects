#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <quanser.h>
#include <thread>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_comboBox_activated(const QString &arg1);


    void timerEvent(QTimerEvent *);
    void Controle();

private:
    Quanser* quanser;
    Ui::MainWindow *ui;
    std::thread* controle;
};

#endif // MAINWINDOW_H
