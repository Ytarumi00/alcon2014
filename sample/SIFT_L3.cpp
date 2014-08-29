#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace std;
using namespace cv;

static int maskMatchesByTrainImgIdx( const vector<DMatch>& matches, int trainImgIdx, vector<char>& mask )
{
	int countkey = 0;
	mask.resize( matches.size() );
	fill( mask.begin(), mask.end(), 0 );
	for( size_t i = 0; i < matches.size(); i++ )
	{
		if( matches[i].imgIdx == trainImgIdx && (int)matches[i].distance < 250){ //distanceによるふるい分け
			mask[i] = 1;
			countkey++;
		}
	}
	return countkey;
}
//bool CountKeyPoint(vector<Mat> src, Mat ref, int& NumKey){

//  /* ----------detecting keypoints---------- */
//  SiftFeatureDetector detector(500);
//  vector<KeyPoint> keypoints_src, keypoints_ref;
//  detector.detect(src, keypoints_src);
//  detector.detect(ref, keypoints_ref);

//  /* ----------computing descriptors---------- */
//  SiftDescriptorExtractor extractor;
//  Mat descriptors_src, descriptors_ref;
//  extractor.compute(src, keypoints_src, descriptors_src);
//  extractor.compute(ref, keypoints_ref, descriptors_ref);

//  /* ----------matching descriptors---------- */
//  BFMatcher matcher(NORM_L2);
//  vector<DMatch> matches;
//  matcher.match(descriptors_src, descriptors_ref, matches);

//  vector<char> matchesMask(matches.size(),0);
//  for (int i = 0; i < matches.size(); i++)
//  {
//    if ((int)matches[i].distance < 200)
//      matchesMask[i] = 1;

//  }
//  Mat dst;
//  drawMatches(src,keypoints_src,ref,keypoints_ref,matches,dst,Scalar(255,0,0),Scalar(0,255,0),matchesMask);
//  namedWindow("dst",CV_WINDOW_NORMAL);
//  imshow("dst",dst);
//  waitKey();
//  return true;
//}
bool CountKeyPoint(const vector<Mat>& src, Mat ref, int& NumKey){

	/* ----------detecting keypoints---------- */
	SiftFeatureDetector detector(1000);
	vector<vector<KeyPoint> > keypoints_src;
	vector<KeyPoint> keypoints_ref;
	Mat detectMask = Mat::zeros(ref.size(),CV_8U);
	int IW = 100; //この数だけrefの上面を無視してキーを抽出
	Mat roi(detectMask,Rect(0,IW,ref.cols,ref.rows-IW));
	roi = Scalar(255,255,255);
	detector.detect(src, keypoints_src);
	detector.detect(ref, keypoints_ref, detectMask);

	/* ----------computing descriptors---------- */
//  SiftDescriptorExtractor extractor;
  Ptr<DescriptorExtractor> extractor;
	extractor = DescriptorExtractor::create("SIFT");
	vector<Mat> descriptors_src;
	Mat descriptors_ref;
	extractor->compute(src, keypoints_src, descriptors_src);
	extractor->compute(ref, keypoints_ref, descriptors_ref);

	/* ----------matching descriptors---------- */
	BFMatcher matcher(NORM_L2);
	matcher.add(descriptors_src);
	matcher.train();
	vector<DMatch> matches;
	matcher.match(descriptors_ref,matches);
//  matcher.match(descriptors_src, descriptors_ref, matches);

	vector<char> matchesMask;
	Mat drawImg;
	namedWindow("drawImg",WINDOW_NORMAL);
	int countkey;
	int max = 0;
	for( size_t i = 0; i < src.size(); i++ )
	{
		if( !src[i].empty() )
		{
			countkey = maskMatchesByTrainImgIdx( matches, (int)i, matchesMask);
			if(max < countkey){
					max = countkey;
					NumKey = i;
				}
			drawMatches( ref, keypoints_ref, src[i], keypoints_src[i],
					matches, drawImg, Scalar(255, 0, 0), Scalar(0, 255, 255), matchesMask );
			imshow("drawImg",drawImg);
			cout  << "srcnum: " << i << " countkey: " <<  countkey << endl;	//デバッグ用
			waitKey();
		}
	}
//マッチングしたkeyの数がmaxの数が条件を満たせばtrue
	if(max>10)
		return true;
	else
		return false;
}
int main(int argc, char** argv)
{

	vector<Mat> img_src;
	Mat img_ref;
	int NumKey;

	img_src.push_back(imread("../ref/Level3/scenario4_foreground0.png",CV_LOAD_IMAGE_GRAYSCALE));
	img_src.push_back(imread("../ref/Level3/scenario4_foreground1.png",CV_LOAD_IMAGE_GRAYSCALE));
	img_src.push_back(imread("../ref/Level3/scenario4_foreground2.png",CV_LOAD_IMAGE_GRAYSCALE));
	img_src.push_back(imread("../ref/Level3/scenario4_foreground3.png",CV_LOAD_IMAGE_GRAYSCALE));
	img_ref = imread("../ref/Level3/scenario4_reference.png",CV_LOAD_IMAGE_GRAYSCALE);

	bool ans = CountKeyPoint(img_src, img_ref, NumKey);
	cout << "NumKeh: " << NumKey << " bools: " << ans << endl;	//デバッグ用
}
