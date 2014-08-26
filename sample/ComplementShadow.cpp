//com_shadowが補完した影画像
#include<iostream>
#include<opencv2/opencv.hpp>
#include<math.h>
using namespace cv;
using namespace std;

void makeShadowMask(Mat& smask, Point pt, double rad, float w = 50){
	int sign = 1;
//    for(int j = -5; j < 5;j++){
//      for(int h = -5; h < 5;h++){
//        smask.data[smask.cols*(pt.y+j)+pt.x+h]=255;
//      }
//    }
	if(sin(rad)<0)
		sign = -1;
//  else{
	cout << "rad: " << rad << " cos(rad): " << cos(rad)  << " sin(rad): " << sin(rad) << endl;
		if(cos(rad) >= 0){
			for(int y = 0; y < (int)w*sin(rad)*sign; y++){
				for(int x = 0; x < (int)w*cos(rad); x++){
					smask.data[smask.cols*(pt.y-y)+pt.x+x] = 255;
				}
			}
		}
		else{
			for(int y = 0; y < (int)w*sin(rad)*sign; y++){
				for(int x = 0; x > (int)w*cos(rad); x--){
					smask.data[smask.cols*(pt.y-y)+pt.x+x] = 255;
				}
			}
		}
//  }
}

int main( int argc, char** argv )
{    Mat src;
	Mat _src;
	Mat src_gray;
	Mat result;

	// the first command-line parameter must be a filename of the binary (black-n-white) image

	src=imread("../ref/Level1/reference_sample.png");
	_src=imread("../ref/Level1/reference_sample.png", 1);
	Mat shadow = imread("../ref/tmp/shadow_img.png",0); //グレースケール入力
	Mat com_shadow = shadow.clone();
	Mat dst = src.clone();
	Mat mask=Mat::zeros(src.rows, src.cols, CV_8UC3);
	Mat shadow_mask = Mat::zeros(shadow.rows, shadow.cols, CV_8U);
	cvtColor(src, src_gray, CV_BGR2GRAY);
	threshold(src_gray,src,80,255,CV_THRESH_BINARY);
	//Canny(src,src,100,500);
	namedWindow( "Source", 1 );
	imshow( "Source", src );
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours( src, contours, hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_NONE );
	Scalar color( 255, 0, 0 );
	cout<<contours.size()<<"\n";
	int num=contours.size();
	int contoursID;
	for( int idx=0; idx <num; idx++ )
	{   double area=contourArea(contours[idx]);
		if(area>97000&&area<1042440){
			Scalar color(255,255,255);
			drawContours( mask, contours, idx, color,-1, 8);
			cout<<area<<" "<<idx<<"\n";
			contoursID = idx;
		}
	}
	_src.copyTo(result,mask);
	//threshold(_src,_src,80,255,CV_THRESH_BINARY);
	namedWindow( "Components", 1 );
	imshow( "Components",result);

	double radian;
	Scalar linecolor(0,0,200);
	int linelength=30,thickness=1,lineType=1,Shift=0;
	vector<double> nrad;

	//法線ベクトルの算出
	for(int i = 0; i < contours[contoursID].size(); i++){
		if(i != contours[contoursID].size()-1){
			radian = atan2(contours[contoursID][i+1].y-contours[contoursID][i].y,contours[contoursID][i+1].x-contours[contoursID][i].x);
		}
		//radian>0なら+90,radian<0なら-90度
		if(radian>=0)
		{
			nrad.push_back(radian+M_PI/2);
		}
		else{
			nrad.push_back(radian-M_PI/2);
		}
//    line(dst,contours[contoursID][i],Point(contours[contoursID][i].x+50*cos(radian),contours[contoursID][i].y+50*sin(radian)),Scalar(0,0,200),3,4 );
	}
	int flag = 0;
	int shadowlen = 100;
	for(int i = 0; i < contours[contoursID].size(); i++){
		//8近傍探索
		flag = 0;
		for(int j = -1; j < 1; j++){
			for(int h = -1; h < 1; h++){
//        if(shadow.at<uchar>(contours[contoursID][i].y+h,contours[contoursID][i].x+j)<128){
				if(shadow.data[shadow.cols*(contours[contoursID][i].y+h)+contours[contoursID][i].x+j]<128){
					flag = 1;
				}
			}
		}
		if(flag == 1){
//      for(int j = 0; j < (int)shadowlen*sin(nrad[i]);j++){
//        for(int h = 0; h < (int)shadowlen*cos(nrad[i]);h++){
//      for(int j = -5; j < 5;j++){
//        for(int h = -5; h < 5;h++){
//          shadow_mask.data[shadow_mask.cols*(contours[contoursID][i].y+j)+contours[contoursID][i].x+h]=255;
//        }
//      }
			makeShadowMask(shadow_mask,contours[contoursID][i],nrad[i]);
		}
	}
	for(int y = 0; y < com_shadow.cols; y++){//横
		for(int x = 0; x < com_shadow.rows; x++){//縦
			if(shadow_mask.data[y*shadow_mask.cols+x] == 255){
				com_shadow.data[y*shadow_mask.cols+x]=0;
			}
		}
	}
	namedWindow("shadow");
	imshow("shadow",shadow);
	namedWindow("complemnt_shadow",WINDOW_NORMAL);
	imshow("complemnt_shadow",com_shadow);
	namedWindow("shadow_mask",WINDOW_NORMAL);
	imshow("shadow_mask",shadow_mask);
	waitKey(0);
}

