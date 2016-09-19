#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  int objectColor;
  int nobjects;

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

  // busca objetos presentes
  objectColor = 50;
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
		// achou um objeto
		nobjects++;
		p.x=j;
		p.y=i;
		floodFill(image,p,objectColor);
	  }
	}
  }

  std::cout << "Foram encontrados " << nobjects <<" objetos." << std::endl;

  imshow("image", image);
  waitKey();
  return 0;
}
