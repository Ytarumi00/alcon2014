#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main( int argc, char** argv )
{    Mat src;
	Mat _src;
	Mat src_gray;
	Mat result;

	// the first command-line parameter must be a filename of the binary (black-n-white) image

	src=imread("../ref/Level1/reference_sample.png");
	_src=imread("../ref/Level1/reference_sample.png", 1);
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
	imwrite("F:\\abc.jpg",result);
	//threshold(_src,_src,80,255,CV_THRESH_BINARY);
	namedWindow( "Components", 1 );
	imshow( "Components",result);
	for(int i = 0; i < contours.size(); i++){
		cout << contours[contoursID][i] << endl;
	}
	waitKey(0);
}
