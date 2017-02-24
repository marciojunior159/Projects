#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    Ui::MainWindow *ui;
    QVector<double> x, y;
};

#endif // MAINWINDOW_H
