//2016603042 최영찬 visual studio 2019
//release로 실행해야함.
#include <math.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//Bilinear_Interpolation함수
void Bilinear_Interpolation(Mat& original, Mat& resized, Size image_size)
{

	double rate = 0;

	//resized 가로,세로
	int x = 0;
	int y = 0;

	//바뀐 크기의 이미지의 원본 이미지 x,y픽셀
	int row_x = 0;
	int row_y = 0;

	//원래 w 크기 출력
	printf("original w : %d \n", original.rows);

	//바뀐 이미지 h,w 출력
	printf("resized height: %d \n", image_size.height);
	printf("resized width: %d \n", image_size.width);


	rate = (double)image_size.height / (double)original.rows;

	//비율 출력
	printf("rate : %f", rate);
	printf("\n");

	for (y = 0; y < image_size.height; y++) {
		for (x = 0; x < image_size.width; x++) {

			row_x = (int)(x / rate);
			row_y = (int)(y / rate);

			double fx1 = (double)x / (double)rate - (double)row_x;
			double fx2 = 1 - fx1;
			double fy1 = (double)y / (double)rate - (double)row_y;
			double fy2 = 1 - fy1;

			uchar P1 = original.at<uchar>(row_y, row_x);
			uchar P2 = original.at<uchar>(row_y, row_x + 1);
			uchar P3 = original.at<uchar>(row_y + 1, row_x);
			uchar P4 = original.at<uchar>(row_y + 1, row_x + 1);

			double w1 = fx2 * fy2; //점 P1
			double w2 = fx1 * fy2; //점 P2
			double w3 = fx2 * fy1; //점 P3
			double w4 = fx1 * fy1; //점 P4

			resized.at<uchar>(y, x) = w1 * P1 + w2 * P2 + w3 * P3 + w4 * P4;

		}
	}
}

int main()
{
	Mat img_in;


	img_in = imread("Lena_256x256.png");
	int w = img_in.cols; // 영상의 높이
	int h = img_in.rows; // 영상의 너비

	if (img_in.empty())
	{
		printf("no image\n");
		return 0;
	}
	//이미지를 회색으로 변환
	cvtColor(img_in, img_in, cv::COLOR_RGB2GRAY);

	//이미지 불러오기
	imshow("source img", img_in);



	while (1) {
		int key = waitKey(50);
		if (key == '1') {
			w += 1;
			h += 1;
		}
		else if (key == '2') {
			w -= 1;
			h -= 1;
		}
		if (key == '1' || key == '2') {

			Mat resized_img(h, w, CV_8U, Scalar(0));
			Bilinear_Interpolation(img_in, resized_img, Size(h, w));
			imshow("output image", resized_img);
		}
	}


	return 0;

}
