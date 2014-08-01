#include <cv.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char** argv )
{
    Mat src, hsv;
    if( argc != 2 || !(src=imread(argv[1], 1)).data )
        return -1;

    cvtColor(src, hsv, CV_BGR2HSV);

    // 色相を 30 分割レベルで，
    // 彩度を 32 分割レベルで量子化します
    int hbins = 30, sbins = 32;
    int histSize[] = {hbins, sbins};
    // cvtColor にあるように，色相の範囲は 0 から 179 です．
    float hranges[] = { 0, 180 };
    // 彩度の範囲は 0 （黒-灰色-白）から
    // 255 （純粋なスペクトルカラー）までです．
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    MatND hist;
    // 0 番目と 1 番目のチャンネルからヒストグラムを求めます．
    int channels[] = {0, 1};

    calcHist( &hsv, 1, channels, Mat(), // マスクは利用しません
        hist, 2, histSize, ranges,
        true, // ヒストグラムは一様です
        false );
    double maxVal=0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);

    int scale = 10;
    Mat histImg = Mat::zeros(sbins*scale, hbins*10, CV_8UC3);

    for( int h = 0; h < hbins; h++ )
        for( int s = 0; s < sbins; s++ )
        {
            float binVal = hist.at<float>(h, s);
            int intensity = cvRound(binVal*255/maxVal);
            rectangle( histImg, Point(h*scale, s*scale),
                         Point( (h+1)*scale - 1, (s+1)*scale - 1),
                         Scalar::all(intensity),
                         CV_FILLED );
        }

    namedWindow( "Source", 1 );
    imshow( "Source", src );

    namedWindow( "H-S Histogram", 1 );
    imshow( "H-S Histogram", histImg );

    waitKey();
}
