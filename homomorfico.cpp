#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

#define RADIUS 20

using namespace cv;
using namespace std;

Mat filter, tmp;

int dft_M, dft_N; // valores ideais dos tamanhos da imagem para calculo da DFT

const int dh_max = 1000;
const int dl_max = 100;
const int d0_max = 100;
const int c_max = 100;

int dh_slider = 100;
int dl_slider = 10;
int d0_slider = 70;
int c_slider = 10;

float dh = 10;
float dl = 1;
float d0 = 7;
float c = 1;

char TrackbarName[50];

// Filtro

void make_filter(){

  float d2;

  // cria uma matriz temporária para criar as componentes real
  // e imaginaria do filtro ideal
  tmp = Mat(dft_M, dft_N, CV_32F);

  // prepara o filtro
  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){

      d2 = ((float)i - dft_M/2.0)*((float)i - dft_M/2.0) + ((float)j - dft_N/2.0)*((float)j - dft_N/2.0);
      tmp.at<float> (i,j) = (dh-dl)*( 1.0-exp( -1.0*(float)c*(d2/(d0*d0)) ) ) + dl;
      }
    }

  // cria a matriz com as componentes do filtro e junta
  // ambas em uma matriz multicanal complexa
  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);

  imshow("Espectro de Fourier Filtro", tmp);
  }

// Chamada dos Slides

void on_trackbar_dh(int, void*){

  dh = dh_slider/10;
  make_filter();
  
  }

void on_trackbar_dl(int, void*){

  dl = dl_slider/10;
  make_filter();

  }

void on_trackbar_d0(int, void*){

  d0 = d0_slider/10;
  make_filter();

  }

void on_trackbar_c(int, void*){

  c = c_slider/10;
  make_filter();

  }

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
  Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;

  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

int main(int argc, char** argv){

  Mat imaginaryInput, complexImage, multsp;
  Mat padded, mag;
  Mat image, imagegray;
  Mat_<float> realInput, zeros;
  vector<Mat> planos;

  // guarda tecla capturada
  char key;

  // carrega imagem
  image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  //cv::log(realInput, realInput);

  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  // a função de transferência (filtro frequencial) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();

  // Gerando filtro
  make_filter();

  for(;;){

    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();

    // cria a compoente real
    realInput = Mat_<float>(padded);

    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);

    // calcula o dft
    dft(complexImage, complexImage);
 

    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    // aplica o filtro frequencial
    mulSpectrums(complexImage,filter,complexImage,0);


    // troca novamente os quadrantes
    deslocaDFT(complexImage);

    // calcula a DFT inversa
    idft(complexImage, complexImage);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    split(complexImage, planos);

    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    //calcula a exponencial da imagem
    //cv::exp(planos[0], planos[0]);


    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("filtrada", planos[0]);

    key = (char) waitKey(10);
    if( key == 27 ) break; // esc pressed!
 
    // Criando Sliders

    //---------------------------------------------------------------------

    sprintf(TrackbarName,"Delta h(x10)", dh_max);
    createTrackbar( TrackbarName, "filtrada", 
                  &dh_slider, dh_max, on_trackbar_dh);

    sprintf(TrackbarName,"Delta l(x10)", dl_max);
    createTrackbar( TrackbarName, "filtrada",
                  &dl_slider, dl_max, on_trackbar_dl);

    sprintf(TrackbarName,"C(x10)", c_max);
    createTrackbar( TrackbarName, "filtrada", 
                  &c_slider, c_max, on_trackbar_c);

    sprintf(TrackbarName,"D0(x10)", d0_max);
    createTrackbar( TrackbarName, "filtrada",
                  &d0_slider, d0_max, on_trackbar_d0);

    imshow("Original", image);

  //-------------------------------------------------------------------------
    

  }
  return 0;
}


