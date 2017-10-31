#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "opencv2/opencv.hpp"
#include <algorithm>
using namespace std;
using namespace cv;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    // Vetores
    vector<char> set_of_letters;
    vector<string> dictionary;
    vector< vector<Point> > porygon, fourPointContours;
    vector<Scalar> colors;

    // Detecção de face
    String face_cascade_name = "C:\\OpenCV3.3.0\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
    String eyes_cascade_name = "C:\\OpenCV3.3.0\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml";
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;

    // Variáveis globais
    int letter_pos = 0, word_pos = 0, color_pos = 0; //auxiliares das funções de chamada de propriedade
    int thrs = 120, max_thresh = 255; // para o canny
    int blurr = 3; // tamanho da matriz de borramento
    int width, height; // dimensões da imagem
    Size size, small_size;
    String file;

    // Matrizes
    Mat image, BlacknWhiteResult, allTogether, cannyImage, facelessGridImage, face;
    Mat horGrid, verGrid, centralGrid, ColoredResult, simpleContours, limitant;
    ~MainWindow();

protected:
    void LoadLetters();
    string NextLetter();
    void grid_it_up(int hor, int ver);
    void detectAndDisplay();
    void removeUnnecessaryGrid();
    void clearAll();
    void joinGrids();
    void removeFace();
    void addBelowLine();
    void simplifyContours(vector<Vec4i> *);
    string NextWord();
    Scalar NextColor();
    void findNonwhiteArea();
    void drawResult(vector<Vec4i>);
private slots:
    void on_loadButton_clicked();
    void on_applyButton_clicked();
    void on_spinCanny_valueChanged(int arg1);

    void on_spinKernel_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
