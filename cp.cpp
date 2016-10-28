#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
  vector<int> yrange;
  vector<int> xrange;

  Mat image, points;
  Mat can;

  int width, height, gray;
  int x, y;

  int step = 5;
  //int centro = step*2;
  
  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }

  srand(time(0));

  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/step);
  yrange.resize(width/step);
  
  iota(xrange.begin(), xrange.end(), 0); 
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*step + step/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*step + step/2;
  }

  points = Mat(height, width, CV_8U, Scalar(255));

  for(uint i=0; i<xrange.size(); i++){
    for(uint j=0; j<yrange.size(); j++){

      x = xrange[i];
      y = yrange[j];
      gray = image.at<uchar>(x,y);
      circle(points,
             cv::Point(y,x),
             step,
             CV_RGB(gray,gray,gray),
             -1,
             CV_AA);
    }
  }

  Mat inter;

  points.copyTo(inter);

  imshow("Imagem intermediaria", inter);

  int raio = 3;
  int inc = 200/raio;
  int value = 0;

  do{

  Canny(image, can, value, 3*value);

  bool pt = false;
  
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){

      if( can.at<uchar>(i,j) == 255 ){

	gray = image.at<uchar>(i,j);
        circle(points,
               cv::Point(j,i),
               raio,
               CV_RGB(gray,gray,gray),
               -1,
               CV_AA);
	
	j=j + rand() % (2*raio);
	pt=true;

	}
      
      }

    if(pt==true) {
      i=i + rand() % (2*raio);
      pt=false;
      }
    }

  raio = raio - 1;
  value = value + inc;

  }while(raio>0);

  imshow("Original", image);
  imshow("Pontilhismo", points);  
	
  waitKey();
  return 0;
}
