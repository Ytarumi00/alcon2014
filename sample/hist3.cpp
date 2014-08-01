#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/contrib/contrib.hpp"

#include "opencv2/nonfree/nonfree.hpp"
#include<iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	//画像ファイルの読み込み
//  if(argc != 1){
//    cout << "画像が見つかりません" << endl;
//    return -1;
//  }
	const char *imagename = argv[1];
	Mat Img = imread(imagename);
	Mat HsvImg;

	//カラー画像をHSV画像に変換する
	cvtColor(Img, HsvImg, CV_BGR2HSV);

	//HSV画像をHとSとVに分割する
	Mat Channels[3];
	split(HsvImg, Channels);

	// ヒストグラムを計算する
	int histogramSize[1];
	float histValueRange[2];
	const float* histValueRanges[1];
	int firstCannel[1];

	histogramSize[0] = 256;
	histValueRange[0] = 0.0;
	histValueRange[1] = 255.0;
	histValueRanges[0] = histValueRange;
	firstCannel[0] = 0;

	int oneChannel = 1;
	MatND histogram;
	int oneDimensionHistogram = 1;
	calcHist(&Channels[0],
			oneChannel,
			firstCannel,
			Mat(),
			histogram,
			oneDimensionHistogram,
			histogramSize,
			histValueRanges);

	// ヒストグラムの最大値を調べる
	double maxValue = 0;
	double minValue = 0;
	minMaxLoc(histogram, &minValue, &maxValue);

	// ヒストグラムを書き込む画像を白く塗りつぶす
	Mat histogramImage(histogramSize[0], histogramSize[0], CV_8U, Scalar(255));

	int highestPoint = 256;
	// ヒストグラムを描画
	for (int i = 0; i < histogramSize[0]; ++i)
	{
		float binVal = histogram.at<float>(i);
//    int intensity = static_cast(binVal*highestPoint/maxValue);
		int intensity = binVal*highestPoint/maxValue;
		line(histogramImage, cv::Point(i, histogramSize[0]), cv::Point(i, histogramSize[0]-intensity),Scalar::all(0));
	}


	namedWindow("HIST", CV_WINDOW_AUTOSIZE);
	imshow("HIST", histogramImage);
	namedWindow("H", CV_WINDOW_AUTOSIZE);
	imshow("H", Channels[0]);


	waitKey();

	return 0;
}
