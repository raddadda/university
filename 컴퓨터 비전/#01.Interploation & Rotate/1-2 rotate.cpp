//2016603042 최영찬 visual studio 2019

#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//rotation 함수
void Rotate(Mat& original, Mat& rotated, int degree)
{

	double h = rotated.rows;
	double w = rotated.cols;

	//회전 중심점
	int y_center = h / 2;
	int x_center = w / 2;

	// math를 include 하여 파이를 M_PI로 계산
	double seta = -degree * M_PI / 180.0;

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {

			int y_new = (x - x_center) * sin(seta) + (y - y_center) * cos(seta) + y_center;
			int x_new = (x - x_center) * cos(seta) - (y - y_center) * sin(seta) + x_center;

			if (original.channels() == 1)
			{
				if ((x_new < 0.0) || (x_new >= w) || (y_new < 0.0) || (y_new >= h))
				{
					rotated.at<uchar>(y, x) = 0;
				}
				else
				{
					uchar data = original.at<uchar>(y_new, x_new);
					rotated.at<uchar>(y, x) = data;
				}
			}
		}
	}
}

void main()
{

	//원하는 회전 각도 입력
	int degree;
	cout << "degree : ";
	cin >> degree;

	//변환할 이미지
	Mat img_in = imread("Lena.png", -1);

	if (img_in.empty())
	{
		printf("no image\n");
		return;
	}

	namedWindow("source img", WINDOW_AUTOSIZE);

	//이미지를 회색으로 변환
	cvtColor(img_in, img_in, cv::COLOR_RGB2GRAY);
	//이미지 불러오기
	imshow("source img", img_in);

	//이미지 높이,너비
	int w = img_in.cols;
	int h = img_in.rows;

	
	int imgcstate = (img_in.channels() == 1) ? CV_8UC1 : CV_8UC3;

	Mat resized_img(h, w, imgcstate, Scalar(0));

	//회전
	Rotate(img_in, resized_img, degree);

	imshow("Output Image", resized_img);
	waitKey(0);
	destroyAllWindows();
}

