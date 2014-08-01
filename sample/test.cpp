#include<cv.h>
#include<highgui.h>
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
	const char *imagename1 = argv[1];
	const char *imagename2 = argv[2];
	Mat Img1 = imread(imagename1);
	Mat Img2 = imread(imagename2);
	Mat HsvImg1;
	Mat HsvImg2;

	//カラー画像をHSV画像に変換する
	cvtColor(Img1, HsvImg1, CV_BGR2HSV);
	cvtColor(Img2, HsvImg2, CV_BGR2HSV);


	//HSV画像をHとSとVに分割する
	Mat Channels1[3];
	Mat Channels2[3];
	split(HsvImg1, Channels1);
	split(HsvImg2, Channels2);

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
	MatND histogram1;
	MatND histogram2;
	int oneDimensionHistogram = 1;
	calcHist(&Channels1[0],
			oneChannel,
			firstCannel,
			Mat(),
			histogram1,
			oneDimensionHistogram,
			histogramSize,
			histValueRanges);
	calcHist(&Channels2[0],
			oneChannel,
			firstCannel,
			Mat(),
			histogram2,
			oneDimensionHistogram,
			histogramSize,
			histValueRanges);

	// ヒストグラムの最大値を調べる
	double maxValue1 = 0;
	double minValue1 = 0;
	double maxValue2 = 0;
	double minValue2 = 0;
	minMaxLoc(histogram1, &minValue1, &maxValue1);
	minMaxLoc(histogram2, &minValue2, &maxValue2);

	// ヒストグラムを書き込む画像を白く塗りつぶす
	Mat histogramImage1(histogramSize[0], histogramSize[0], CV_8U, Scalar(255));
	Mat histogramImage2(histogramSize[0], histogramSize[0], CV_8U, Scalar(255));

	int highestPoint = 256;
	// ヒストグラムを描画
	for (int i = 0; i < histogramSize[0]; ++i)
	{
		float binVal1 = histogram1.at<float>(i);
		float binVal2 = histogram2.at<float>(i);
//    int intensity = static_cast(binVal*highestPoint/maxValue);
		int intensity1 = binVal1*highestPoint/maxValue1;
		int intensity2 = binVal2*highestPoint/maxValue2;
		line(histogramImage1, cv::Point(i, histogramSize[0]), cv::Point(i, histogramSize[0]-intensity1),Scalar::all(0));
		line(histogramImage2, cv::Point(i, histogramSize[0]), cv::Point(i, histogramSize[0]-intensity2),Scalar::all(0));
	}


	namedWindow("HIST1", CV_WINDOW_AUTOSIZE);
	imshow("HIST1", histogramImage1);
	namedWindow("HIST2", CV_WINDOW_AUTOSIZE);
	imshow("HIST2", histogramImage2);
	namedWindow("H1", CV_WINDOW_AUTOSIZE);
	imshow("H1", Channels1[0]);
	namedWindow("H2", CV_WINDOW_AUTOSIZE);
	imshow("H2", Channels2[0]);
	cout << compareHist(histogram1,histogram2,CV_COMP_CORREL) << endl;
	cout << compareHist(histogram1,histogram2,CV_COMP_CHISQR) << endl;
	cout << compareHist(histogram1,histogram2,CV_COMP_INTERSECT) << endl;
	cout << compareHist(histogram1,histogram2,CV_COMP_BHATTACHARYYA) << endl;


	waitKey();

	return 0;
}
