#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;

  image = imread("imagem.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data) cout << "imagem.png não abriu!" << endl;

  imshow("Imagem Carregada", image);
  waitKey();

  int linha = image.rows;
  int coluna = image.cols;

  Mat temp(image,Rect(0,0,coluna/2,linha/2));
  Mat temp1(image,Rect(coluna/2,0,coluna/2,linha/2));
  Mat temp2(image,Rect(0,linha/2,coluna/2,linha/2));
  Mat temp3(image,Rect(coluna/2,linha/2,coluna/2,linha/2));

  Mat imgTrocada = image.clone();

  temp.copyTo(imgTrocada(Rect(coluna/2,linha/2,coluna/2,linha/2)));
  temp1.copyTo(imgTrocada(Rect(0,linha/2,coluna/2,linha/2)));
  temp2.copyTo(imgTrocada(Rect(coluna/2,0,coluna/2,linha/2)));
  temp3.copyTo(imgTrocada(Rect(0,0,coluna/2,linha/2)));

  imshow("Imagem Carregada 2", imgTrocada);
  waitKey();

  return 0;
}
