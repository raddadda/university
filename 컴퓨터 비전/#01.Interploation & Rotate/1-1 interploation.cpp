//2016603042 �ֿ��� visual studio 2019
//release�� �����ؾ���.
#include <math.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//Bilinear_Interpolation�Լ�
void Bilinear_Interpolation(Mat& original, Mat& resized, Size image_size)
{

	double rate = 0;

	//resized ����,����
	int x = 0;
	int y = 0;

	//�ٲ� ũ���� �̹����� ���� �̹��� x,y�ȼ�
	int row_x = 0;
	int row_y = 0;

	//���� w ũ�� ���
	printf("original w : %d \n", original.rows);

	//�ٲ� �̹��� h,w ���
	printf("resized height: %d \n", image_size.height);
	printf("resized width: %d \n", image_size.width);


	rate = (double)image_size.height / (double)original.rows;

	//���� ���
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

			double w1 = fx2 * fy2; //�� P1
			double w2 = fx1 * fy2; //�� P2
			double w3 = fx2 * fy1; //�� P3
			double w4 = fx1 * fy1; //�� P4

			resized.at<uchar>(y, x) = w1 * P1 + w2 * P2 + w3 * P3 + w4 * P4;

		}
	}
}

int main()
{
	Mat img_in;


	img_in = imread("Lena_256x256.png");
	int w = img_in.cols; // ������ ����
	int h = img_in.rows; // ������ �ʺ�

	if (img_in.empty())
	{
		printf("no image\n");
		return 0;
	}
	//�̹����� ȸ������ ��ȯ
	cvtColor(img_in, img_in, cv::COLOR_RGB2GRAY);

	//�̹��� �ҷ�����
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
