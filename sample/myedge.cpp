#include<iostream>
#include<opencv2/opencv.hpp>
#include<math.h>
using namespace cv;
using namespace std;

void cvArrow(Mat& img, Point pt1, Point pt2, Scalar color, int thickness=1, int lineType=8, int shift=0)
{
    line(img,pt1,pt2,color,thickness,lineType,shift);
    float vx = (float)(pt2.x - pt1.x);
    float vy = (float)(pt2.y - pt1.y);
    float v = sqrt( vx*vx + vy*vy );
    float ux = vx / v;
    float uy = vy / v;
    //矢印の幅の部分
    float w=5,h=10;
    Point ptl,ptr;
    ptl.x = (int)((float)pt2.x - uy*w - ux*h);
    ptl.y = (int)((float)pt2.y + ux*w - uy*h);
    ptr.x = (int)((float)pt2.x + uy*w - ux*h);
    ptr.y = (int)((float)pt2.y - ux*w - uy*h);
    //矢印の先端を描画する
    line(img,pt2,ptl,color,thickness,lineType,shift);
    line(img,pt2,ptr,color,thickness,lineType,shift);
}
int main( int argc, char** argv )
{    Mat src;
	Mat _src;
	Mat src_gray;
	Mat result;

	// the first command-line parameter must be a filename of the binary (black-n-white) image

	src=imread("../ref/Level1/reference_sample.png");
	_src=imread("../ref/Level1/reference_sample.png", 1);
	Mat dst = src;
	Mat mask=Mat::zeros(src.rows, src.cols, CV_8UC3);
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

	for(int i = 0; i < contours[contoursID].size(); i++){
		if(i != contours[contoursID].size()-1){
			radian = atan2(contours[contoursID][i+1].y-contours[contoursID][i].y,contours[contoursID][i+1].x-contours[contoursID][i].x);
		}
		cout << contours[contoursID][i] << radian / 180 * M_PI << endl;
		//radian>0なら+90,radian<0なら-90度
		if(radian>=0)
		{
			cvArrow(dst,contours[contoursID][i],Point(contours[contoursID][i].x+linelength*cos(radian+M_PI/2),contours[contoursID][i].y+linelength*sin(radian+M_PI/2)),linecolor,thickness,lineType,Shift );
		}
		else{
			cvArrow(dst,contours[contoursID][i],Point(contours[contoursID][i].x+linelength*cos(radian-M_PI/2),contours[contoursID][i].y+linelength*sin(radian-M_PI/2)),linecolor,thickness,lineType,Shift );
		}
//    line(dst,contours[contoursID][i],Point(contours[contoursID][i].x+50*cos(radian),contours[contoursID][i].y+50*sin(radian)),Scalar(0,0,200),3,4 );
	}
	namedWindow("dst",1);
	imshow("dst",dst);
	waitKey(0);
}
