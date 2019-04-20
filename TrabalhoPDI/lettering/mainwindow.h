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
    vector< pair<int, string> > weighted_dictionary;

    // Detecção de face
    String face_cascade_name = "C:\\OpenCV3.3.0\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
    String eyes_cascade_name = "C:\\OpenCV3.3.0\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml";
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;

    // Variáveis globais
    int letter_pos = 0, word_pos = 0, wword_pos = 0, color_pos = 0; //auxiliares das funções de chamada de propriedade
    int thrs = 120, max_thresh = 255; // para o canny
    int blurr = 3; // tamanho da matriz de borramento
    int width, height; // dimensões da imagem
    Size size, small_size;
    String file, image_path;

    // Matrizes
    Mat input_image, image, BlacknWhiteResult, allTogether, cannyImage, faceMask, face, background, debugAllTogether;
    Mat horGrid, verGrid, centralGrid, ColoredResult, simpleContours, limitant, imageMask;
    ~MainWindow();

protected:
    void LoadLetters();
    string NextLetter();
    void grid_it_up(Mat* OutputHorizontal, Mat* OutputVertical, int height, int width, int hor, int ver);
    void storeFaceArea(Mat, Mat*, Mat*);
    void removeUnnecessaryGrid();
    void clearAll();
    void joinGrids(Mat, Mat, Mat, Mat, Mat*);
    void removeFace(Mat, Mat, Mat*);
    void addBelowLine();
    void simplifyContours(vector< vector<Point> > *, vector< vector<Point> > *, vector<Vec4i> *);
    string NextWord();
    Scalar NextColor();
    void findNonwhiteArea(Mat, Mat*, int, int);
    void drawResult(vector<Vec4i>);
    string NextWordRevolution();
    void updatePictures();

private slots:
    void on_applyButton_clicked();
    void on_spinCanny_valueChanged(int arg1);
    void on_spinKernel_valueChanged(int arg1);
    void on_imageSelector_clicked();
    void on_textSelector_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
