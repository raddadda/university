#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main()
{
    Mat img, img_graycolor, img_aver, Sharpening_img;

    img = imread("ice cream.png", IMREAD_COLOR); //이미지 읽어오기
    if (img.empty())
    {
        cout << "No image." << endl;
        exit(1);
    }

   //흑백으로 이미지 변환

    img_graycolor = Mat(img.rows, img.cols, CV_8UC1);

    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            uchar blue = img.at<Vec3b>(y, x)[0];
            uchar green = img.at<Vec3b>(y, x)[1];
            uchar red = img.at<Vec3b>(y, x)[2];

            uchar gray = (blue + green + red) / 3.0;

            img_graycolor.at<uchar>(y, x) = gray;
        }
    }

    //moving average filter

    int filter1[3][3] = { {1,1,1},
                        { 1,1,1 },
                        { 1,1,1 } };

    int filter2[5][5] = { { 1,1,1,1,1 },
                        { 1,1,1,1,1 },
                        { 1,1,1,1,1 },
                        { 1,1,1,1,1 },
                        { 1,1,1,1,1 } };
   


    long int sum;
    img_aver = Mat(img.rows, img.cols, CV_8UC1);
    int filtersize = 3;

    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            sum = 0;
            for (int i = -1 * filtersize / 2; i <= filtersize / 2; i++)
            {
                for (int j = -1 * filtersize / 2; j <= filtersize / 2; j++)
                {

                    int result_y = y + i;
                    int result_x = x + j;

                    //boundary를 처리하기위해 테두리값을 이용하여 repetition 한다. 

                    if (result_y < 0) result_y = 0;
                    else if (result_y > img.rows - 1) result_y = img.rows - 1;

                    if (result_x < 0) result_x = 0;
                    else if (result_x > img.cols - 1) result_x = img.cols - 1;

                    //filter 크기별 계산
                    if (filtersize == 3)
                        sum += img_graycolor.at<uchar>(result_y, result_x) * filter1[filtersize / 2 + i][filtersize / 2 + j];
                    else if (filtersize == 5)
                        sum += img_graycolor.at<uchar>(result_y, result_x) * filter2[filtersize / 2 + i][filtersize / 2 + j];
                    
                }
            }

            sum = sum / (double)(filtersize * filtersize);

            if (sum > 255) sum = 255;
            if (sum < 0) sum = 0;

            img_aver.at<uchar>(y, x) = sum;
        }
    }
    //Laplacian filter 
    int Lap_filter[3][3] = 
        {{-1, -1, -1},
         { -1, 8, -1}, 
         {-1, -1, -1}};
    Mat Lap_img = Mat(img.rows, img.cols, CV_8UC1);
    filtersize = 3;

    for (int y = 0 + filtersize / 2; y < img.rows - filtersize / 2; y++)
    {
        for (int x = 0 + filtersize / 2; x < img.cols - filtersize; x++)
        {
            sum = 0;
            for (int i = -1 * filtersize / 2; i <= filtersize / 2; i++)
            {
                for (int j = -1 * filtersize / 2; j <= filtersize / 2; j++)
                {
                    sum += img_graycolor.at<uchar>(y + i, x + j) * Lap_filter[filtersize / 2 + i][filtersize / 2 + j];
                }
            }

            sum = sum / (double)(filtersize * filtersize);

            if (sum > 255) sum = 255;
            if (sum < 0) sum = 0;

            Lap_img.at<uchar>(y, x) = sum;
        }
    }
    //Sharpening filter
    Sharpening_img = Mat(img.rows, img.cols, CV_8UC1);

    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            sum = 0;
            for (int i = -1 * filtersize / 2; i <= filtersize / 2; i++)
            {
                for (int j = -1 * filtersize / 2; j <= filtersize / 2; j++)
                {

                    int result_y = y + i;
                    int result_x = x + j;

                    //boundary를 처리하기위해 테두리값을 이용하여 repetition 한다. 

                    if (result_y < 0) result_y = 0;
                    else if (result_y > img.rows - 1) result_y = img.rows - 1;

                    if (result_x < 0) result_x = 0;
                    else if (result_x > img.cols - 1) result_x = img.cols - 1;

                    //filter 크기별 계산
                    if (filtersize == 3)
                        sum += 1.9 * img_graycolor.at<uchar>(y, x) - 0.9* img_graycolor.at<uchar>(result_y, result_x) * filter1[filtersize / 2 + i][filtersize / 2 + j];
                    else if (filtersize == 5)
                        sum += 1.9 * img_graycolor.at<uchar>(y, x) - 0.9* img_graycolor.at<uchar>(result_y, result_x) * filter1[filtersize / 2 + i][filtersize / 2 + j];

                }
            }

            sum = sum / (double)(filtersize * filtersize);

            if (sum > 255) sum = 255;
            if (sum < 0) sum = 0;

            Sharpening_img.at<uchar>(y, x) = sum;
        }
    }
    

    imshow("입력 영상", img_graycolor);
    imshow("average", img_aver);
    imshow("Laplacian", Lap_img);
    imshow("Sharpening", Sharpening_img);

    while (cv::waitKey(0) == 0);
}