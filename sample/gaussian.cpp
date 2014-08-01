#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int
main(int argc, char *argv[])
{
  cv::Mat src_img = cv::imread("../ref/Level1/level1_sample3.png", 1);
  if(src_img.empty()) return -1; 
  
  cv::Mat dst_img1, dst_img2;
  // ガウシアンを用いた平滑化
  // 入力画像，出力画像，カーネルサイズ，標準偏差x, y
  cv::GaussianBlur(src_img, dst_img1, cv::Size(11,11), 10, 10);
  cv::GaussianBlur(src_img, dst_img2, cv::Size(51,3), 80, 3);
  
  cv::namedWindow("Blur image1", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::namedWindow("Blur image2", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::imshow("Blur image1", dst_img1);
  cv::imshow("Blur image2", dst_img2);
  cv::waitKey(0);
}
