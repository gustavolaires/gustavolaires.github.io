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

  int x1,x2,y1,y2;
  int linha = image.rows;
  int coluna = image.cols;
 
  cout << "Digite a coordenada de uma ponto: (Ex: 4 5)(Limites: x=";
  cout << coluna << " e y=" << linha<< ")"<< endl;
  cin>>x1;
  cin>>y1;

  cout<< x1 << " " << y1 << endl;

  cout<<"Digite outra coordenada de uma ponto: (Ex: 4 5)(Limites: x=";
  cout<< coluna << " e y=" << linha << ")" << endl;
  cin>>x2;
  cin>>y2;

  cout<< x2 << " " << y2 << endl;

  int xl,xh,yl,yh;

  if(x1<x2) {
    xl = x1;
    xh = x2;
    }
  else {
    xl = x2;
    xh = x1;
    }

  if(y1<y2) {
    yl = y1;
    yh = y2;
    }
  else {
    yl = y2;
    yh = y1;
    }

  for(int i=yl;i<yh;i++){
    for(int j=xl;j<xh;j++){
      
      int valor = image.at<uchar>(i,j);
      image.at<uchar>(i,j) = 255 - valor;
    }
  }
  
  imshow("Imagem Carregada", image);
  waitKey();

  return 0;
}
