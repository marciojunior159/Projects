#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSettings>
#include <string>

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
}
