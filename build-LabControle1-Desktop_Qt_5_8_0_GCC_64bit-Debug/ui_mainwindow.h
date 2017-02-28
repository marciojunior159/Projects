/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QRadioButton *radioButtonMalhaAberta;
    QRadioButton *radioButtonMalhaFechada;
    QGroupBox *groupBoxSaida;
    QCheckBox *checkBox;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_7;
    QCheckBox *checkBox_8;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_4;
    QLabel *label_4;
    QGroupBox *groupBoxEntrada;
    QDoubleSpinBox *SpinBoxNivel;
    QDoubleSpinBox *SpinBoxTensao;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_3;
    QSpinBox *spinBoxCanal;
    QLabel *label_5;
    QDoubleSpinBox *SpinBoxPeriodo;
    QLabel *label_6;
    QComboBox *comboBoxSinal;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QCustomPlot *customPlot;
    QCustomPlot *plotS1;
    QCustomPlot *plotS2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1200, 700);
        MainWindow->setMinimumSize(QSize(1200, 700));
        MainWindow->setMaximumSize(QSize(1200, 700));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        radioButtonMalhaAberta = new QRadioButton(centralWidget);
        radioButtonMalhaAberta->setObjectName(QStringLiteral("radioButtonMalhaAberta"));
        radioButtonMalhaAberta->setGeometry(QRect(20, 70, 117, 22));
        radioButtonMalhaFechada = new QRadioButton(centralWidget);
        radioButtonMalhaFechada->setObjectName(QStringLiteral("radioButtonMalhaFechada"));
        radioButtonMalhaFechada->setGeometry(QRect(150, 70, 151, 22));
        groupBoxSaida = new QGroupBox(centralWidget);
        groupBoxSaida->setObjectName(QStringLiteral("groupBoxSaida"));
        groupBoxSaida->setGeometry(QRect(360, 100, 321, 91));
        groupBoxSaida->setMaximumSize(QSize(1200, 1200));
        checkBox = new QCheckBox(groupBoxSaida);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(10, 30, 71, 20));
        checkBox_6 = new QCheckBox(groupBoxSaida);
        checkBox_6->setObjectName(QStringLiteral("checkBox_6"));
        checkBox_6->setGeometry(QRect(87, 50, 71, 20));
        checkBox_3 = new QCheckBox(groupBoxSaida);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setGeometry(QRect(164, 30, 71, 20));
        checkBox_7 = new QCheckBox(groupBoxSaida);
        checkBox_7->setObjectName(QStringLiteral("checkBox_7"));
        checkBox_7->setGeometry(QRect(164, 50, 71, 20));
        checkBox_8 = new QCheckBox(groupBoxSaida);
        checkBox_8->setObjectName(QStringLiteral("checkBox_8"));
        checkBox_8->setGeometry(QRect(241, 50, 71, 20));
        checkBox_5 = new QCheckBox(groupBoxSaida);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));
        checkBox_5->setGeometry(QRect(10, 50, 71, 20));
        checkBox_2 = new QCheckBox(groupBoxSaida);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(87, 30, 71, 20));
        checkBox_4 = new QCheckBox(groupBoxSaida);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));
        checkBox_4->setGeometry(QRect(241, 30, 71, 20));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(170, 0, 341, 51));
        QFont font;
        font.setFamily(QStringLiteral("DejaVu Sans"));
        font.setPointSize(28);
        font.setBold(true);
        font.setWeight(75);
        label_4->setFont(font);
        groupBoxEntrada = new QGroupBox(centralWidget);
        groupBoxEntrada->setObjectName(QStringLiteral("groupBoxEntrada"));
        groupBoxEntrada->setGeometry(QRect(20, 100, 331, 241));
        SpinBoxNivel = new QDoubleSpinBox(groupBoxEntrada);
        SpinBoxNivel->setObjectName(QStringLiteral("SpinBoxNivel"));
        SpinBoxNivel->setEnabled(false);
        SpinBoxNivel->setGeometry(QRect(210, 40, 69, 31));
        SpinBoxNivel->setMaximum(30);
        SpinBoxNivel->setSingleStep(0.01);
        SpinBoxTensao = new QDoubleSpinBox(groupBoxEntrada);
        SpinBoxTensao->setObjectName(QStringLiteral("SpinBoxTensao"));
        SpinBoxTensao->setEnabled(false);
        SpinBoxTensao->setGeometry(QRect(80, 40, 69, 31));
        SpinBoxTensao->setMinimum(-4);
        SpinBoxTensao->setMaximum(4);
        SpinBoxTensao->setSingleStep(0.01);
        label_2 = new QLabel(groupBoxEntrada);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(170, 40, 41, 31));
        label = new QLabel(groupBoxEntrada);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 40, 51, 31));
        label_3 = new QLabel(groupBoxEntrada);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 90, 31, 31));
        spinBoxCanal = new QSpinBox(groupBoxEntrada);
        spinBoxCanal->setObjectName(QStringLiteral("spinBoxCanal"));
        spinBoxCanal->setEnabled(false);
        spinBoxCanal->setGeometry(QRect(80, 140, 48, 31));
        spinBoxCanal->setMaximum(7);
        label_5 = new QLabel(groupBoxEntrada);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 140, 41, 31));
        SpinBoxPeriodo = new QDoubleSpinBox(groupBoxEntrada);
        SpinBoxPeriodo->setObjectName(QStringLiteral("SpinBoxPeriodo"));
        SpinBoxPeriodo->setEnabled(false);
        SpinBoxPeriodo->setGeometry(QRect(230, 140, 51, 31));
        SpinBoxPeriodo->setSingleStep(0.1);
        SpinBoxPeriodo->setValue(1);
        label_6 = new QLabel(groupBoxEntrada);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(170, 140, 51, 31));
        comboBoxSinal = new QComboBox(groupBoxEntrada);
        comboBoxSinal->setObjectName(QStringLiteral("comboBoxSinal"));
        comboBoxSinal->setEnabled(false);
        comboBoxSinal->setGeometry(QRect(80, 90, 201, 31));
        pushButton = new QPushButton(groupBoxEntrada);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(60, 200, 80, 22));
        pushButton_2 = new QPushButton(groupBoxEntrada);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(190, 200, 80, 22));
        customPlot = new QCustomPlot(centralWidget);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        customPlot->setGeometry(QRect(780, 20, 381, 271));
        plotS1 = new QCustomPlot(centralWidget);
        plotS1->setObjectName(QStringLiteral("plotS1"));
        plotS1->setGeometry(QRect(20, 360, 381, 271));
        groupBoxSaida->raise();
        plotS2 = new QCustomPlot(centralWidget);
        plotS2->setObjectName(QStringLiteral("plotS2"));
        plotS2->setGeometry(QRect(410, 360, 381, 271));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1200, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        radioButtonMalhaAberta->setText(QApplication::translate("MainWindow", "Malha Aberta", Q_NULLPTR));
        radioButtonMalhaFechada->setText(QApplication::translate("MainWindow", "Malha Fechada", Q_NULLPTR));
        groupBoxSaida->setTitle(QApplication::translate("MainWindow", "Canais de Saida", Q_NULLPTR));
        checkBox->setText(QApplication::translate("MainWindow", "Canal 0", Q_NULLPTR));
        checkBox_6->setText(QApplication::translate("MainWindow", "Canal 5", Q_NULLPTR));
        checkBox_3->setText(QApplication::translate("MainWindow", "Canal 2", Q_NULLPTR));
        checkBox_7->setText(QApplication::translate("MainWindow", "Canal 6", Q_NULLPTR));
        checkBox_8->setText(QApplication::translate("MainWindow", "Canal 7", Q_NULLPTR));
        checkBox_5->setText(QApplication::translate("MainWindow", "Canal 4", Q_NULLPTR));
        checkBox_2->setText(QApplication::translate("MainWindow", "Canal 1", Q_NULLPTR));
        checkBox_4->setText(QApplication::translate("MainWindow", "Canal 3", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Pseudo Controle", Q_NULLPTR));
        groupBoxEntrada->setTitle(QApplication::translate("MainWindow", "Entrada", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "N\303\255vel", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Tens\303\243o", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Sinal", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Canal", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Periodo", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "Enviar", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "Cancelar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
