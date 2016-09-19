#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  int objectColor;
  int nobjects;
  int objectsCB;

  CvPoint p;
  image = imread("bolhas.png",CV_LOAD_IMAGE_GRAYSCALE);

  imshow("image", image);
  waitKey();

  if(!image.data){
    std::cout << "Imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.rows;
  height=image.cols;

  p.x=0;
  p.y=0;

  // Varedura na imagem
  objectColor = 50;
  nobjects=0;

  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){

      if(image.at<uchar>(i,j) == 255){ //Achou um objeto
        if(i==0 || j==0 || i==height-1 || j==width-1){ //Objeto nas bordas
          p.x=j;
	  p.y=i;
	  floodFill(image,p,0);
          }
	else {  //Objeto a ser contado
	  nobjects++;
	  p.x=j;
	  p.y=i;
	  floodFill(image,p,objectColor);
	  }
	}

      if(image.at<uchar>(i,j) == objectColor){ //Achou um Objeto já contado
        if(i==0 || j==0 || i==height-1 || j==width-1){ //Objeto nas bordas
	  nobjects--;  //Objeto contado por engano
          p.x=j;
	  p.y=i;
	  floodFill(image,p,0);
          }
	}

      }
    }

  imshow("image", image);
  waitKey();

  //Mudar a cor de fundo
  p.x=0;
  p.y=0;
  floodFill(image,p,255);

  imshow("image", image);
  waitKey();

  // 2 Varredura de imagem
  objectsCB = 0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){

      if(image.at<uchar>(i,j) == 0){ //Achou um buraco de objeto
        if(image.at<uchar>(i,j-1) == 50){ //Objeto do buraco não contado
	  objectsCB++;
          p.x=j-1;
	  p.y=i;
	  floodFill(image,p,100);
          }
	}

      }
    }

  std::cout << "Foram encontrados " << nobjects <<" objetos." << std::endl;
  std::cout << "Foram encontrados " << objectsCB <<" objetos com burracos." << std::endl;
  std::cout << "Foram encontrados " << nobjects - objectsCB <<" objetos sem burracos." << std::endl;

  imshow("image", image);
  waitKey();
  return 0;
}
