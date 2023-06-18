#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {

	Mat img;
	img = imread("ice cream.png", 1);

	Mat cal_img(img.rows, img.cols, CV_8UC3);
	Mat opencv_img(img.rows, img.cols, CV_8UC3);

	cvtColor(img, opencv_img, cv::COLOR_BGR2YUV);
	int c_Y, c_U, c_V, c_Red, c_Green, c_Blue;

	for (int h = 1; h < img.rows; h++) {
		for (int w = 1; w < img.cols; w++) {

			c_Red = (int)img.at<Vec3b>(h, w)[2];
			c_Green = (int)img.at<Vec3b>(h, w)[1];
			c_Blue = (int)img.at<Vec3b>(h, w)[0];


			c_Y = 0.257 * c_Red + 0.504 * c_Green + 0.098 * c_Blue + 16;
			c_U = -0.148 * c_Red - 0.291 * c_Green + 0.439 * c_Blue + 128;
			c_V = 0.439 * c_Red - 0.368 * c_Green - 0.071 * c_Blue + 128;


			cal_img.at<Vec3b>(h, w)[0] = c_Y;
			cal_img.at<Vec3b>(h, w)[1] = c_U;
			cal_img.at<Vec3b>(h, w)[2] = c_V;

		}
	}

	namedWindow("원본", WINDOW_AUTOSIZE);
	imshow("원본", img);

	namedWindow("직접계산", WINDOW_AUTOSIZE);
	imshow("직접계산", cal_img);

	namedWindow("opencv", WINDOW_AUTOSIZE);
	imshow("opencv", opencv_img);

	waitKey();


	return 0;
}