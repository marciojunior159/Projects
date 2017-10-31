#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "opencv2/opencv.hpp"
#include <algorithm>

using namespace std;
using namespace cv;

vector<char> set_of_letters;
vector<string> dictionary;
vector<Point> points_vec;
vector< vector<Point> > polygon_vec;

int letter_pos = 0;
int thrs = 120, max_thresh = 255, reduction = 4, max_reduction = 128;
Mat image, chImage, polyImage, smaller, bigger, testImage1, testImage2, testImage3, allTogether;
int width, height;

// Imprime as coordenadas do ponto no plano
void printPoint(Point p){
    stringstream ss;
    string s;
    ss << "(" << p.x << ", " << p.y << ")";
    ss >> s;
    cout << s << endl;
}

// Carrega o arquivo texto
void LoadLetters(){
    char ch;
    ifstream infile;
    stringstream ss;
    string s;
    infile.open("..\\trabalho-pdi\\texto.txt");
    while(infile >> noskipws >> ch) // To get you all the lines.
    {
        //cout << ch;
        ss << ch;
        if(ch != ' ' && ch != '\n'){
            set_of_letters.push_back(ch);
            ss >> s;
            dictionary.push_back(s);
            ss.str(string());
        }
    }
    infile.close();
}

// Retorna a próxima letra do arquivo texto
string NextLetter(){
    string output;
    if (letter_pos == set_of_letters.size()){
        letter_pos = 0;
        output = set_of_letters.at(0);
    }else{
        letter_pos++;
        output = set_of_letters.at(letter_pos-1);
        //output = to_string(letter_pos-1);
    }

    return output;
}

// Calcula a distância entre dois pontos
float distance(Point a, Point b){
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

void process(int, void*){

    bigger = Mat::zeros(image.size(), CV_8U);
    chImage = Mat::zeros(image.size(), CV_8UC3);
    polyImage = Mat::zeros(image.size(), CV_8UC3);
    testImage1 = Mat::zeros(image.size(), CV_8UC3);

    if (reduction == 0)
        reduction = 1;
    Size small_size(width/reduction, height/reduction);
    resize(image, smaller, small_size);
    Canny(smaller, smaller, thrs, 3*thrs);

    // Mapeamento dos pontos: pequena -> grande
    for(int i = 0; i < small_size.height; i++){
      for(int j = 0; j < small_size.width; j++){
          if (smaller.at<uchar>(i,j) > 0){
              bigger.at<uchar>(i*reduction, j*reduction) = smaller.at<uchar>(i,j);
              points_vec.push_back(Point(j*reduction, i*reduction));
          }
      }
    }//fim

    // definição do ponto inicial (o mais a esquerda da imagem)
    int c_pos=0;
    int min_x=bigger.size().width;
    for(int p = 0; p < points_vec.size(); p++){
        if (points_vec[p].y < min_x){
            c_pos = p;
            min_x = points_vec[p].y;
        }
    }

    // troca ponto encontrada com aquele que está no início do vetor
    iter_swap(points_vec.begin(), points_vec.begin() + c_pos);

    // reordenação do vetor pela distância
    for(int p = 0; p < points_vec.size(); p++){
        float dist = 8001.0;
        for(int p2 = p+1; p2 < points_vec.size(); p2++){
            if (distance(points_vec[p], points_vec[p2]) < dist){
                iter_swap(points_vec.begin() + p + 1, points_vec.begin() + p2);
                dist = distance(points_vec[p], points_vec[p+1]);
            }
        }
    }//fim

    // Imprime um caracter do arquivo-texto para cada ponto em ordem
    vector<Point>::iterator it;
    for (it = points_vec.begin(); it != points_vec.end(); ++it){
        //cout << "y x " << it->y << " " << it->x << endl;
        putText(chImage, NextLetter(), *it, FONT_HERSHEY_COMPLEX, 0.4, CV_RGB(0,255,255));
    }// FIM

    for (it = points_vec.begin(); it != points_vec.end()-1; ++it){
        //cout <<"xy "<< it->x << " " << it->y;
        //putText(polyImage, NextLetter(), *it, FONT_HERSHEY_COMPLEX, 0.4, CV_RGB(255,255,0));
        if (distance(*it, *(it+1)) < 2*reduction)
          line(polyImage, *it, *(it+1), CV_RGB(255,255,0));
    }// FIM

    vector<Point> tmpvec;
    int cont = 0;
    for (it = points_vec.begin(); it != points_vec.end()-1; ++it){
        tmpvec.push_back(*it);
        cont++;
        if (distance(*it, *(it+1)) > 2*reduction || cont > 2 ){
            //if (cont > 7)
            polygon_vec.push_back(tmpvec);
            tmpvec.clear();
            cont=0;
        }
    }

    fillPoly(testImage1, polygon_vec, CV_RGB(255,255,0));

    imshow( "Source", testImage1 );


}

int main(){
  LoadLetters();

  image= imread("..\\trabalho-pdi\\imagens\\test.jpg",CV_LOAD_IMAGE_GRAYSCALE);

  width=image.size().width;
  height=image.size().height;
  //bigger = Mat(height, width, CV_8U, Scalar(0));
  //chImage = Mat(height, width, CV_8UC3, Scalar(0));
  //polyImage = Mat(height, width, CV_8UC3, Scalar(0));
  //testImage1 = Mat(height, width, CV_8UC3, Scalar(0));
  allTogether = Mat(height*4, width*4, CV_8UC3, Scalar(0));
  cout << "w/h = " << width << "/" << height <<  endl;
  Size size(width, height);

  namedWindow("Source", WINDOW_AUTOSIZE );
  //imshow( "Source", testImage1 );
  createTrackbar( "Threshold - Canny:", "Source", &thrs, max_thresh, process );
  createTrackbar( "Reduction factor:", "Source", &reduction, max_reduction, process );
  process(0,0);
  waitKey(0);

  /* SALVA ARQUIVOS DE RESULTADO E ANÁLISE
   * points = pontos encontrados
   * letters = resultado
  */
  imwrite("..\\trabalho-pdi\\imagens\\points.png", bigger);
  imwrite("..\\trabalho-pdi\\imagens\\letters.png", chImage);
  imwrite("..\\trabalho-pdi\\imagens\\polly.png", polyImage);
  imwrite("..\\trabalho-pdi\\imagens\\polly1.png", testImage1);
  return 0;
}
