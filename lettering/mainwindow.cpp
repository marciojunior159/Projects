#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);

    // carrega banco para detecção de face
    face_cascade.load( face_cascade_name );
    eyes_cascade.load( eyes_cascade_name );

    // Nomeia abas do .ui
    ui->tabWidget->setTabText(0,"Original");
    ui->tabWidget->setTabText(1,"Canny");
    ui->tabWidget->setTabText(2,"Com Limites");
    ui->tabWidget->setTabText(3,"Linhas");
    ui->tabWidget->setTabText(4,"Bordes simples");
    ui->tabWidget->setTabText(5,"Cor reduzida");
    ui->tabWidget->setTabText(6,"Simples Final");
    ui->tabWidget->setTabText(7,"Colorido Final");

    // adiciona elementos ao vetor de cores
    colors.push_back(Scalar(255, 144, 30));
    colors.push_back(Scalar(235, 206, 135));
    colors.push_back(Scalar(255, 0, 0));
    colors.push_back(Scalar(255, 105, 65));
    colors.push_back(Scalar(230, 224, 176));
    colors.push_back(Scalar(112, 25, 25));
    colors.push_back(Scalar(250, 206, 135));
    colors.push_back(Scalar(237, 149, 100));

    // upload inicial (imagem-teste)
    image_path = "..\\lettering\\imagens\\test.jpg";

}

MainWindow::~MainWindow()
{
    delete ui;
}

// inicializa (e reinicializa) todas as matrizes e variáveis globais
void MainWindow::clearAll(){
    BlacknWhiteResult = Mat::zeros(size, CV_8U);
    allTogether = Mat::zeros(size, CV_8U);
    cannyImage = Mat::zeros(size, CV_8U);
    faceMask = Mat::zeros(size, CV_8U);
    horGrid = Mat::zeros(size, CV_8U);
    verGrid = Mat::zeros(size, CV_8U);
    limitant = Mat::zeros(size, CV_8U);
    centralGrid = Mat::zeros(size, CV_8U);
    ColoredResult = Mat(height, width, CV_8UC3, Scalar(230,255,255));
    simpleContours = Mat(height, width, CV_8UC3, Scalar::all(255));
    dictionary.clear();
    porygon.clear();
    word_pos = 0; letter_pos = 0;
    fourPointContours.clear();
}

// Imprime as coordenadas do ponto no plano
void printPoint(Point p){
    stringstream ss;
    string s;
    ss << "(" << p.x << ", " << p.y << ")";
    ss >> s;
    cout << s << endl;
}

// percorre a matriz e cria um pseudo-contorno a partir do primeiro elemento não-branco encontrado
void MainWindow::findNonwhiteArea(Mat InputImage, Mat * OutputArray, int height, int width){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(InputImage.at<uchar>(i, j) < 255){
                OutputArray->at<uchar>(i, j) = 255;
                break;
            }
        }
    }for(int i = 0; i < height; i++){
        for(int j = width-1; j > 0; j--){
            if(InputImage.at<uchar>(i, j) < 255){
                OutputArray->at<uchar>(i, j) = 255;
                break;
            }
        }
    }for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(InputImage.at<uchar>(j, i) < 255){
                OutputArray->at<uchar>(j, i) = 255;
                break;
            }
        }
    }for(int i = 0; i < width; i++){
        for(int j = height-1; j > 0; j--){
            if(InputImage.at<uchar>(j, i) < 255){
                OutputArray->at<uchar>(j, i) = 255;
                break;
            }
        }
    }
}

// Carrega o arquivo texto
void MainWindow::LoadLetters(){
    char ch;
    ifstream infile;
    string s;
    infile.open("..\\lettering\\textos\\texto.txt");
    while(infile >> noskipws >> ch) // To get you all the lines.
    {
        //cout << ch;
        if(ch != ' ' && ch != '\n'){
            set_of_letters.push_back(ch);
            s = s + ch;
        }else if((ch == ' ' || ch == '\n') && !s.empty()){
            dictionary.push_back(s);
            s.clear();
        }
    }

    // ordena palavras por tamanho (maior para menor)
    struct compare {
        bool operator()(const std::string& first, const std::string& second) {
            return first.size() > second.size();
        }
    };
    compare c;
    sort(dictionary.begin(), dictionary.end(), c);

    infile.close();
}

// Retorna a próxima letra do arquivo texto
string MainWindow::NextLetter(){
    string output; int l_size = set_of_letters.size();
    if (letter_pos == l_size){
        letter_pos = 1;
        output = set_of_letters.at(0);
    }else{
        letter_pos++;
        output = set_of_letters.at(letter_pos-1);
        //output = to_string(letter_pos-1);
    }

    return output;
}

// retorna próxima palavra do vetor
string MainWindow::NextWord(){
    int d_size = dictionary.size();
    cout << " on pos " << word_pos << " of " << d_size << endl;
    //cout << "\ndictionary word is " << dictionary[word_pos] << " on pos " << word_pos << endl;
    string output;
    if (word_pos >= d_size){
        word_pos = 1;
        output = dictionary.at(0);
    }else{
        word_pos++;
        output = dictionary.at(word_pos-1);
        //output = to_string(letter_pos-1);
    }

    return output;
}

// retorna próxima palavra do vetor
string MainWindow::NextWordRevolution(){
    int d_size = weighted_dictionary.size();
    if (d_size == 0) return "Lopes da Silva";
    string output;
    if (wword_pos >= d_size){
        wword_pos = 1;
        output = weighted_dictionary[0].second;
    }else{
        wword_pos++;
        output = weighted_dictionary[wword_pos-1].second;
    }

    return output;
}

// retorna próxima cor do vetor
Scalar MainWindow::NextColor(){
    int c_size = colors.size();
    if (color_pos == c_size){
        color_pos = 0;
        return colors.at(0);
    }else{
        color_pos++;
        return colors.at(color_pos-1);
    }
}

// Calcula a distância entre dois pontos
float distance(Point a, Point b){
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

// imprime coordenadas de um vetor point2f de tamanho 4
void printPoint2f(vector<Point2f> p2f, string name){
    cout << name << ": (" << p2f[0].x << ", " << p2f[0].y << ")  (" << p2f[1].x << ", " << p2f[1].y << ")  ("
        << p2f[2].x << ", " << p2f[2].y << ")  (" << p2f[3].x << ", " << p2f[3].y << ")" << endl;
}

// reordena o vetor point2f para a ordem top-left, bottom-left, bottom-right, top-right (necessário para realizar a transformação
vector<Point2f> reorder_4p(vector<Point2f> vec, bool higher_height){
    vector<Point2f> out_x = vec, out_y = vec;
    printPoint2f(out_x, "outx antes");
    printPoint2f(out_y, "outy antes");

    for(int i = 0; i < 4; i++){
        for(int k = i+1; k < 4; k++){
            if (out_x[i].x > out_x[k].x){
                iter_swap(out_x.begin()+i, out_x.begin()+k);
                i--;
                break;
            }
        }
    }for(int i = 0; i < 4; i++){
        for(int k = i+1; k < 4; k++){
            if (out_y[i].y > out_y[k].y){
                iter_swap(out_y.begin()+i, out_y.begin()+k);
                i--;
                break;
            }
        }
    }

    printPoint2f(out_x, "outx depois");
    printPoint2f(out_y, "outy depois");

    if (higher_height){
        if(out_y[1].x < out_y[0].x){
            iter_swap(out_y.begin(), out_y.begin()+1);
        }
        iter_swap(out_y.begin()+1, out_y.begin()+3);
        if(out_y[2].x < out_y[1].x)
            iter_swap(out_y.begin()+1, out_y.begin()+2);

        printPoint2f(out_y, "outy retornado");
        return out_y;
    }else{
        if(out_x[1].y < out_x[0].y){
            iter_swap(out_x.begin(), out_x.begin()+1);
        }
        //iter_swap(out_x.begin()+1, out_x.begin()+3);
        if(out_x[3].y > out_x[2].y)
            iter_swap(out_x.begin()+3, out_x.begin()+2);

        printPoint2f(out_x, "outx retornado");
        return out_x;
    }


}

// redimensiona o vetor point2f a partir de máximos de x e y chamados
vector<Point2f> resize_4p(vector<Point2f> vec, int max_x, int max_y){
    //printPoint2f(vec, "out inside");
    int max_local_x = 0, max_local_y = 0, v_size = vec.size();
    for (int i=0; i < v_size; i++){
        if(vec[i].x > max_local_x) max_local_x = vec[i].x;
        if(vec[i].y > max_local_y) max_local_y = vec[i].y;
    }
    cout << "locals: " << max_local_x << " " << max_local_y << endl;
    cout << "generals: " << max_x << " " << max_y << endl;

    for (int i=0; i < v_size; i++){
        vec[i].x *= (float(max_x)/float(max_local_x));
        vec[i].y *= (float(max_y)/float(max_local_y));
    }
    return vec;
}

/* FUNÇÃO: detecta a face na imagem, apaga a mesma da imagem com grid e preenche a mesma numa imagem secundária
 * código retirado dos tutoriais do open-cv
 * InputImage -> imagem de entrada
 * OutputLimits -> matriz que recebe o contorno da face
 * OutputMask -> matriz que recebe a máscara da face
*/
void MainWindow::storeFaceArea(Mat InputImage, Mat * OutputLimits, Mat * OutputMask)
{
    std::vector<Rect> faces;
    Mat frame_gray = InputImage.clone();
    //cvtColor(image, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( *OutputLimits, center, Size( 0.8*faces[i].width/2, 1.2*faces[i].height/2 ), 0, 0, 360, Scalar(255), 1, 8, 0 );
        ellipse( *OutputMask, center, Size( 0.8*faces[i].width/2, 1.2*faces[i].height/2 ), 0, 0, 360, Scalar(255), -1, 8, 0 );
        /* Código abaixo encontra os olhos (mantenho para caso veja necessidade de usar)
        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;
        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( gridImage, eye_center, radius, Scalar(50), 4, 8, 0 );
        }*/
    }
}

// mostra algumas matrizes selecionadas
void MainWindow::updatePictures()
{
    int w_ = ui->labelPoints->width();
    int h_ = ui->labelPoints->height();

    if (w_ < width || h_ < height){
        // Ibagem original
        ui->labelOriginal->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(input_image.data), input_image.cols, input_image.rows, input_image.step,
                                                               QImage::Format_RGB888).rgbSwapped().scaled(w_,h_,Qt::KeepAspectRatio)));
        // Imagem do canny
        ui->labelPoints->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(cannyImage.data), cannyImage.cols, cannyImage.rows, cannyImage.step,
                                                             QImage::Format_Indexed8).convertToFormat(QImage::Format_RGB888).scaled(w_,h_,Qt::KeepAspectRatio)));
        // Imagem do canny com grades
        ui->labelChars->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(debugAllTogether.data), debugAllTogether.cols, debugAllTogether.rows, debugAllTogether.step,
                                                            QImage::Format_Indexed8).convertToFormat(QImage::Format_RGB888).scaledToHeight(h_, Qt::FastTransformation)));
        // Imagem do canny com grades e sem o rosto
        ui->labelLines->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(allTogether.data), allTogether.cols, allTogether.rows, allTogether.step,
                                                            QImage::Format_Indexed8).convertToFormat(QImage::Format_RGB888).scaled(w_,h_,Qt::KeepAspectRatio)));
        // Imagem dos contornos simplificados
        ui->labelSimple->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(simpleContours.data), simpleContours.cols, simpleContours.rows, simpleContours.step,
                                                             QImage::Format_RGB888).rgbSwapped().scaled(w_,h_,Qt::KeepAspectRatio)));
        // Imagem dos contornos simplificados
        ui->labelReduced->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(background.data), background.cols, background.rows, background.step,
                                                             QImage::Format_Indexed8).convertToFormat(QImage::Format_RGB888).scaled(w_,h_,Qt::KeepAspectRatio)));
        // Imagem final versão preto e branco
        ui->labelPoly->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(BlacknWhiteResult.data), BlacknWhiteResult.cols, BlacknWhiteResult.rows, BlacknWhiteResult.step,
                                                           QImage::Format_Indexed8).convertToFormat(QImage::Format_RGB888).scaled(w_,h_,Qt::KeepAspectRatio)));
        // Imagem final
        ui->labelContour->setPixmap(QPixmap::fromImage(QImage((const uchar*)(ColoredResult.data), ColoredResult.cols, ColoredResult.rows, ColoredResult.step,
                                                              QImage::Format_RGB888).rgbSwapped().scaled(w_,h_,Qt::KeepAspectRatio)));
    }else{
        // Ibagem original
        ui->labelOriginal->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(input_image.data), input_image.cols, input_image.rows, input_image.step,
                                                               QImage::Format_RGB888).rgbSwapped()));
        // Imagem do canny
        ui->labelPoints->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(cannyImage.data), cannyImage.cols, cannyImage.rows, cannyImage.step,
                                                             QImage::Format_Indexed8).convertToFormat(QImage::Format_RGB888)));
        // Imagem do canny com grades
        ui->labelChars->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(debugAllTogether.data), debugAllTogether.cols, debugAllTogether.rows, debugAllTogether.step,
                                                            QImage::Format_Indexed8).convertToFormat(QImage::Format_RGB888)));
        // Imagem do canny com grades e sem o rosto
        ui->labelLines->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(allTogether.data), allTogether.cols, allTogether.rows, allTogether.step,
                                                            QImage::Format_Indexed8).convertToFormat(QImage::Format_RGB888)));
        // Imagem dos contornos simplificados
        ui->labelSimple->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(simpleContours.data), simpleContours.cols, simpleContours.rows, simpleContours.step,
                                                             QImage::Format_RGB888).rgbSwapped()));
        // Imagem dos contornos simplificados
        ui->labelReduced->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(background.data), background.cols, background.rows, background.step,
                                                             QImage::Format_Indexed8).convertToFormat(QImage::Format_RGB888)));
        // Imagem final versão preto e branco
        ui->labelPoly->setPixmap(QPixmap::fromImage(QImage((unsigned char*)(BlacknWhiteResult.data), BlacknWhiteResult.cols, BlacknWhiteResult.rows, BlacknWhiteResult.step,
                                                           QImage::Format_Indexed8).convertToFormat(QImage::Format_RGB888)));
        // Imagem final
        ui->labelContour->setPixmap(QPixmap::fromImage(QImage((const uchar*)(ColoredResult.data), ColoredResult.cols, ColoredResult.rows, ColoredResult.step,
                                                              QImage::Format_RGB888).rgbSwapped()));
    }
    ui->labelChars->update();
    ui->labelPoints->update();
    ui->labelLines->update();
    ui->labelPoly->update();
    ui->labelOriginal->update();
    ui->labelContour->update();
    ui->labelSimple->update();
}

// adiciona uma grade na imagem, de acordo com o número pedido pelo usuário
void MainWindow::grid_it_up(Mat * OutputHorizontal, Mat * OutputVertical, int height, int width, int hor, int ver){
    if (ver > 0)
        for(int i = width/(2*ver); i < width; i += width/ver){
            line(*OutputVertical, Point(i, 0), Point(i, height-1), Scalar(255));
        }

    if (hor > 0)
        for(int i = height/(2*hor); i < height; i += height/hor){
            line(*OutputHorizontal, Point(0, i), Point(width-1, i), Scalar(255));
        }
}

/* remove a parte da grade que não encontra-se no foco da imagem (o homo sapiens sapiens)
 * cada loop realiza a mesma operaçõa num sentido diferente
*/
void MainWindow::removeUnnecessaryGrid(){
    int ignore = false;
    //remove horizontal lines left to right
    for(int i = 0; i < height; i++){
        if(cannyImage.at<uchar>(i, 0) == 255 || cannyImage.at<uchar>(i, 1) == 255)
            ignore = !ignore;
        else if(horGrid.at<uchar>(i, 0) == 255 && !ignore){
            for(int j = 0; j < width; j++){
                if(cannyImage.at<uchar>(i, j) == 255 || cannyImage.at<uchar>(i-1, j) == 255
                        || cannyImage.at<uchar>(i+1, j) == 255 || centralGrid.at<uchar>(i, j) == 255)
                    break;
                else
                    horGrid.at<uchar>(i, j) = 0;
            }
        }
    }

    ignore = false;
    //remove horizontal lines right to left
    for(int i = 0; i < height; i++){
        if(cannyImage.at<uchar>(i, width-1) == 255 || cannyImage.at<uchar>(i, width-2) == 255)
            ignore = !ignore;
        else if(horGrid.at<uchar>(i, width-1) == 255 && !ignore){
            for(int j = width-1; j > 0; j--){
                if(cannyImage.at<uchar>(i, j) == 255 || cannyImage.at<uchar>(i-1, j) == 255
                        || cannyImage.at<uchar>(i+1, j) == 255 || centralGrid.at<uchar>(i, j) == 255)
                    break;
                else
                    horGrid.at<uchar>(i, j) = 0;

            }
        }
    }

    ignore = false;
    //remove vertical lines
    for(int j = 0; j < width; j++){
        if(cannyImage.at<uchar>(0, j) == 255){
            ignore = !ignore;
            j+=3;
        }
        else if((verGrid.at<uchar>(0, j) == 255 || verGrid.at<uchar>(1, j) == 255 ) && !ignore){
            for(int i = 0; i < height; i++){
                if (cannyImage.at<uchar>(i, j) == 255 || cannyImage.at<uchar>(i, j-1) == 255
                        || cannyImage.at<uchar>(i, j+1) == 255 || centralGrid.at<uchar>(i, j) == 255)
                    break;
                else
                    verGrid.at<uchar>(i, j) = 0;
            }
        }
    }


}

/* FUNÇÃO: une partes brancas de imagens numa outra
 * a, b, c, d -> imagens para unir
 * OutputArray -> imagem unida
*/
void MainWindow::joinGrids(Mat a, Mat b, Mat c, Mat d, Mat * OutputArray){
    for(int j = 0; j < width; j++)
        for(int i = 0; i < height; i++)
            if(a.at<uchar>(i, j) == 255 || b.at<uchar>(i, j) == 255
                   || c.at<uchar>(i, j) == 255 || d.at<uchar>(i, j) == 255)
                OutputArray->at<uchar>(i, j) = 255;

}

/* FUNÇÃO: remove a face da imagem
 * mask -> máscara da face (indica a região da mesma)
 * circle_limit -> limites da face
 * OutputArray -> imagem com o espaço da face retirado
*/
void MainWindow::removeFace(Mat mask, Mat circle_limit, Mat * OutputArray){
    for(int j = 0; j < width; j++)
        for(int i = 0; i < height; i++)
            if(mask.at<uchar>(i, j) == 255 && circle_limit.at<uchar>(i, j) == 0)
                OutputArray->at<uchar>(i, j) = 0;
}

/* FUNÇÃO: adiciona listras nas bordas para ajudar a detecção
 * DEPRECIADA: depois de adicionar o redimensionamento na função inicial passou a ser desnecessário
 * Só não apago por pena mesmo
*/
void MainWindow::addBelowLine(){
    int p1 = -1, p2 = -1;
    for (int i = 0; i < width-1; i++){
        if(allTogether.at<uchar>(height-1, i) == 255){
            p1 = i;
            break;
        }
    }

    for (int i = width-1; i > 0; i--){
        if(allTogether.at<uchar>(height-1, i) == 255){
            p2 = i;
            break;
        }
    }

    if(p1 > -1 && p2 > -1)
        line(allTogether, Point(p1, height-1), Point(p2, height-1), Scalar(255));
    else
        line(allTogether, Point(0, height-1), Point(width-1, height-1), Scalar(255));

    p1 = -1; p2 = -1;

    for (int i = 0; i < width-1; i++){
        if(allTogether.at<uchar>(0, i) == 255){
            p1 = i;
            break;
        }
    }

    for (int i = width-1; i > 0; i--){
        if(allTogether.at<uchar>(0, i) == 255){
            p2 = i;
            break;
        }
    }

    if(p1 > -1 && p2 > -1)
        line(allTogether, Point(p1, 0), Point(p2, 0), Scalar(255));
    else
        line(allTogether, Point(0, 0), Point(width-1, 0), Scalar(255));

}

/* FUNÇÃO: simplifica os contornos
 * PolygonList -> vetor que guarda os contornos poligonais da imagem trabalhada
 * SimpPolygonList -> vetor que recebe os contornos após o processo de simplificação dos mesmos
 * hierarchy -> vetor que carrega a hierarquia dos contornos (necessário para excluir os contornos externos à pessoa da foto)
*/
void MainWindow::simplifyContours(vector< vector<Point> > * PolygonList, vector< vector<Point> > * SimpPolygonList, vector<Vec4i> * hierarchy){

    int max_area=0, max_area_id=-1, p_size = PolygonList->size();
    for(int i=0; i < p_size; i++){
        if(contourArea(PolygonList->at(i)) > max_area && hierarchy->at(i)[3] != -1){
            max_area = contourArea(PolygonList->at(i));
            max_area_id = i;
        }
    }
    PolygonList->erase(PolygonList->begin()+max_area_id);
    hierarchy->erase(hierarchy->begin()+max_area_id);

    // filtra apenas bordas internas
    for(vector<Vec4i>::size_type idx=0; idx<hierarchy->size(); ++idx)
    {
        if(hierarchy->at(idx)[3] != -1)
            SimpPolygonList->push_back(PolygonList->at(idx));
    }

    int f_size = SimpPolygonList->size();

    // simplificada bordas internas para 4 pontos ou menos
    for(int i = 0; i < f_size; i++){
        vector<Point> temp = SimpPolygonList->at(i);
        for(double k = 0.0; k < 100.0; k+=0.01){
            if (temp.size() <= 4)
                break;
            else
                approxPolyDP(SimpPolygonList->at(i), temp, k, true);
        }
        SimpPolygonList->at(i) = temp;
        //cout << " :: " << SimpPolygonList->at(i).size() << endl;
    }
}

/* Função pega em https://stackoverflow.com/questions/32755439/how-to-put-text-into-a-bounding-box-in-opencv
 * Foi modificada para realizar também uma transformação WARP
*/
void PutText(cv::Mat& img, const std::string& text, const cv::Rect& roi, const cv::Scalar& color, int fontFace, double fontScale, vector<Point> code, int thickness = 1, int lineType = LINE_AA)
{
    CV_Assert(!img.empty() && (img.type() == CV_8UC3 || img.type() == CV_8UC1));
    CV_Assert(roi.area() > 0);
    CV_Assert(!text.empty());

    int baseline = 0;

    // Calculates the width and height of a text string
    cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);

    // Y-coordinate of the baseline relative to the bottom-most text point
    baseline += thickness;

    // Render the text over here (fits to the text size)
    cv::Mat textImg(textSize.height + baseline, textSize.width, img.type());

    if (color == cv::Scalar::all(0)) textImg = cv::Scalar::all(255);
    else textImg = cv::Scalar::all(0);

    // Estimating the resolution of bounding image
    cv::Point textOrg((textImg.cols - textSize.width) / 2, (textImg.rows + textSize.height - baseline) / 2);

    // TR and BL points of the bounding box
    //cv::Point tr(textOrg.x, textOrg.y + baseline);
    //cv::Point bl(textOrg.x + textSize.width, textOrg.y - textSize.height);

    cv::putText(textImg, text, textOrg, fontFace, fontScale, color, thickness, lineType);

    if(roi.size().height > roi.size().width) rotate(textImg, textImg, ROTATE_90_CLOCKWISE);

    // Resizing according to the ROI
    cv::resize(textImg, textImg, roi.size());

    int min_x = 10000, min_y = 10000, c_size = code.size();
    for (int i=0; i < c_size; i++){
        if(code[i].x < min_x) min_x = code[i].x;
        if(code[i].y < min_y) min_y = code[i].y;
    }for (int i=0; i < c_size; i++){
        code[i].x -= min_x;
        code[i].y -= min_y;
    }



    cv::Mat Output1;
    if(code.size() == 4){
        // ordem: top-left, bottom-left, bottom-right, top-right
        vector<Point2f> inputQuad, outputQuad;
        outputQuad.push_back(Point2f(code[0]));
        outputQuad.push_back(Point2f(code[1]));
        outputQuad.push_back(Point2f(code[2]));
        outputQuad.push_back(Point2f(code[3]));
        bool bol = true;
        if (roi.size().height < roi.size().width){
            bol = false;
        }
        outputQuad = reorder_4p(outputQuad, bol);
        outputQuad = resize_4p(outputQuad, roi.size().width, roi.size().height);

        inputQuad.push_back(Point2f(0, 0));
        inputQuad.push_back(Point2f(0, roi.size().height));
        inputQuad.push_back(Point2f(roi.size().width, roi.size().height));
        inputQuad.push_back(Point2f(roi.size().width, 0));

        inputQuad = reorder_4p(inputQuad, bol);

        printPoint2f(inputQuad, "in_quad");
        printPoint2f(outputQuad, "out_qud");

        Mat lambda = getPerspectiveTransform(inputQuad, outputQuad);
        //cv::imshow("before", textImg);
        warpPerspective(textImg, Output1, lambda, roi.size());
        warpPerspective(textImg, textImg, lambda, textImg.size());

        //cv::imshow("after", textImg);
        //cv::imshow("bigpicture", Output1);

        //waitKey();

    }

    cv::Mat textImgMask = textImg;
    if (textImgMask.type() == CV_8UC3)
        cv::cvtColor(textImgMask, textImgMask, cv::COLOR_BGR2GRAY);

    // Creating the mask
    cv::equalizeHist(textImgMask, textImgMask);

    if (color == cv::Scalar::all(0)) cv::threshold(textImgMask, textImgMask, 1, 255, cv::THRESH_BINARY_INV);
    else cv::threshold(textImgMask, textImgMask, 254, 255, cv::THRESH_BINARY);

    // Put into the original image
    cv::Mat destRoi = img(roi);

    textImg.copyTo(destRoi, textImgMask);
}

// desenha a imagem final
void MainWindow::drawResult(vector<Vec4i> hierarchy){
    for(int j = 0; j < width; j++)
        for(int i = 0; i < height; i++){
            if(imageMask.at<uchar>(i, j) < 250){
                ColoredResult.at<Vec3b>(i, j) = {210, 234, 234};
            }
        }

    for(int j = 0; j < width; j++)
        for(int i = 0; i < height; i++){
            if(faceMask.at<uchar>(i, j) == 255){
                BlacknWhiteResult.at<uchar>(i, j) = cannyImage.at<uchar>(i, j);
                //if (cannyImage.at<uchar>(i, j) == 255){
                    //ColoredResult.at<Vec3b>(i, j) = {0,0,0};
                int spec = background.at<uchar>(i, j) - 45; int norm = background.at<uchar>(i, j) - 20;
                if (spec < 0) spec = 0;
                if (norm < 0) norm = 0;
                    ColoredResult.at<Vec3b>(i, j) = {spec, norm, norm};
               // }
            }
        }

    int q = 0, p_size = porygon.size();
    for(int i = 0; i < p_size; i++){
        if (hierarchy[i][3] != -1){
            Rect roi(boundingRect(Mat(fourPointContours[q])));
            PutText( ColoredResult, NextWordRevolution(), roi, NextColor(), FONT_HERSHEY_TRIPLEX, 2.0, fourPointContours[q], 8);
            //word_pos--;
            PutText( BlacknWhiteResult, NextWord(), roi, Scalar(200), FONT_HERSHEY_COMPLEX, 2.0, fourPointContours[q], 6);
            q++;
        }

    }
    add(BlacknWhiteResult, limitant, BlacknWhiteResult);

    drawContours(simpleContours, fourPointContours, -1, Scalar::all(0), 1);
}

/* É aqui que a acção começa mesmo !!
 * Passo a passo, chamando as funções acima, para realizar o processo
*/
void MainWindow::on_applyButton_clicked()
{
    // carrega imagem
    Mat gray_image;
    input_image = imread(image_path, CV_LOAD_IMAGE_COLOR);
    cvtColor(input_image, gray_image, CV_BGR2GRAY);

    // processo que aumenta a imagem, centralizando a atual numa maior de fundo branco
    image = Mat(10+input_image.rows, 10+input_image.cols, CV_8U, Scalar(255));
    gray_image(Rect(0,0,gray_image.cols,gray_image.rows)).copyTo(image(Rect(5,5,gray_image.cols,gray_image.rows)));

    // filtro de borramento (testando o melhor tamanho ainda)
    blur(image, image, Size(blurr,blurr));
    threshold(image, imageMask, 250, 255, THRESH_BINARY);
    imwrite("..\\lettering\\imagens\\mask1.png", imageMask);
    //threshold(image, imageMask, 254, 255, THRESH_BINARY);
    //imwrite("..\\lettering\\imagens\\mask2.png", imageMask);

    // define dimensões
    width=image.size().width;
    height=image.size().height;
    size = Size(width, height);
    cout << "w/h = " << width << "/" << height <<  endl;

    // limpa tudo
    clearAll();
    // carrega o arquivo texto
    LoadLetters();
    // faz canny
    Canny(image, cannyImage, thrs, 3*thrs);
    // adiciona a grade na imagem
    grid_it_up(&horGrid, &verGrid, height, width, 16, 8);
    findNonwhiteArea(image, &limitant, height, width);
    storeFaceArea(image, &centralGrid, &faceMask);
    imwrite("..\\lettering\\imagens\\s03.png", horGrid);
    imwrite("..\\lettering\\imagens\\s04.png", verGrid);
    imwrite("..\\lettering\\imagens\\s05.png", centralGrid);
    removeUnnecessaryGrid();
    joinGrids(horGrid, verGrid, centralGrid, cannyImage, &allTogether);
    debugAllTogether = allTogether.clone();
    removeFace(faceMask, centralGrid, &allTogether);
    vector<Vec4i> hierarchy;
    findContours(allTogether, porygon, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    simplifyContours(&porygon, &fourPointContours, &hierarchy);
    threshold(input_image, background, 100, 255, THRESH_BINARY);
    cvtColor(background, background, CV_BGR2GRAY);
    drawResult(hierarchy);

    updatePictures();

    imwrite("..\\lettering\\imagens\\s02.png", cannyImage);
    imwrite("..\\lettering\\imagens\\s01.png", limitant);
    imwrite("..\\lettering\\imagens\\s06.png", horGrid);
    imwrite("..\\lettering\\imagens\\s07.png", verGrid);
    imwrite("..\\lettering\\imagens\\s08.png", debugAllTogether);
    imwrite("..\\lettering\\imagens\\s09.png", allTogether);
    imwrite("..\\lettering\\imagens\\s15.png", BlacknWhiteResult);
    imwrite("..\\lettering\\imagens\\s14.png", simpleContours);
    imwrite("..\\lettering\\imagens\\positivo.png", ColoredResult);
}

// muda o limite de canny
void MainWindow::on_spinCanny_valueChanged(int arg1)
{
    thrs = arg1;
}

void MainWindow::on_spinKernel_valueChanged(int arg1)
{
    blurr = arg1;
}

void MainWindow::on_imageSelector_clicked()
{
    image_path = QFileDialog::getOpenFileName(this,
     tr("Open Image"), "..\\lettering\\imagens", tr("Image Files (*.png *.jpg *.bmp)")).toStdString();

}

void MainWindow::on_textSelector_clicked()
{
    QString text_path = QFileDialog::getOpenFileName(this,
            tr("Open Text"), "..\\lettering\\textos", tr("Text Files (*.txt)"));

    char ch;
    ifstream infile;
    string s, word;
    infile.open(text_path.toStdString());
    bool before_comma = true;
    while(infile >> noskipws >> ch) // To get you all the lines.
    {
        //cout << ch;
        if(ch == ','){
            before_comma = false;
            word = s;
            s.clear();
        }else if(before_comma){
            s = s + ch;
        }else if(!before_comma && ch != '\n'){
            if (ch != ' ') s = s + ch;
        }
        else if(ch == '\n'){
            cout << stoi(s) << " " << word <<  endl;
            weighted_dictionary.push_back(make_pair(stoi(s), word));
            word.clear();
            s.clear();
            before_comma = true;
        }

    }

    // ordena palavras por peso (maior para menor)
    struct compare {
        bool operator()(const std::pair<int, string>& first, const std::pair<int, string>& second) {
            //return first.first > second.first;
            return first.second.size() > second.second.size();
        }
    };
    compare c;
    sort(weighted_dictionary.begin(), weighted_dictionary.end(), c);

    infile.close();

}
