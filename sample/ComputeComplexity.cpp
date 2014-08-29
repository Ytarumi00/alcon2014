//com_shadowが補完した影画像
#include<iostream>
#include<opencv2/opencv.hpp>
#include<math.h>
using namespace cv;
using namespace std;


int main( int argc, char** argv )
{    Mat src;
	Mat shadow = imread("../ref/tmp/scenario6_reference_shadow.png",0); //グレースケール入力
//  Mat shadow = imread("../ref/tmp/shadow_img.png",0); //グレースケール入力
	Mat binImg;
	threshold(shadow,binImg,200,255,THRESH_BINARY);
	Mat bin_org = binImg.clone();

	//
	erode(binImg,binImg,cv::Mat(),Point(-1,1),8);
	dilate(binImg,binImg,cv::Mat(),Point(-1,1),8);
	dilate(binImg,binImg,cv::Mat(),Point(-1,1),8);
	erode(binImg,binImg,cv::Mat(),Point(-1,1),8);

	Mat _binImg = binImg.clone();
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours( _binImg, contours, hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_NONE );
	Scalar color(255,255,0);
	Mat contour_mask = Mat::zeros(binImg.rows,binImg.cols,CV_8UC3);
//  int contoursID  = 1;
	int contoursID  = 3;

	cout << "contours.size: " << contours.size() << endl;
	cout << "contoursID: " << contoursID << endl;
	//  drawContours( contour_mask, contours, contourID, color,-1, 8);
	drawContours( contour_mask, contours, contoursID, color,-1, 8);

	vector<double> nrad;
	double radian;
	//法線ベクトルの算出
	for(int i = 1; i < contours[contoursID].size(); i++){
		if(i != contours[contoursID].size()-1){
			radian = atan2(contours[contoursID][i+1].y-contours[contoursID][i-1].y,contours[contoursID][i+1].x-contours[contoursID][i-1].x);
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
	//  法線ベクトルによる複雑度
	double diff = 0;
	for(int i = 0; i < nrad.size(); i++){
		if(i == 0)
			diff += abs(nrad[0]-nrad[nrad.size()]);
		else
			diff += abs(nrad[i]-nrad[i-1]);
	}
	double complexity = diff/nrad.size();
	cout << "complexity by vec: " << complexity << endl;
	
	RotatedRect box = fitEllipse(contours[contoursID]);
	ellipse(binImg,box,Scalar(0,0,255),2,8);

	cout << "contours_area: " << contourArea(contours[contoursID]) << " ellipse: " << box.size.area() << endl;
	cout << "complexity by area: " << contourArea(contours[contoursID])/box.size.area() << endl;
	namedWindow("contour_mask",WINDOW_NORMAL);
	imshow("contour_mask",contour_mask);
	namedWindow("shadow",WINDOW_NORMAL);
	imshow("shadow",shadow);
	namedWindow("bin_org",WINDOW_NORMAL);
	imshow("bin_org",bin_org);
	namedWindow("binImg",WINDOW_NORMAL);
	imshow("binImg",binImg);
	waitKey(0);

}
