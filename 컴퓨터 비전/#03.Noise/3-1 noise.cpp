#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//Salt&Pepper noise 함수
void sap(Mat& img, double noise)
{
    int img_rows = img.rows;
    int img_cols = img.cols;
    int img_channel = img.channels();
    //noise 계수를 통해 noise 확률을 바꿀 수 있다.
    int sp_noise = (int)((double)(img_rows * img_cols * img_channel) * noise);


    for (int i = 0; i < sp_noise; i++)
    {
        int rand_r = rand() % img_rows;
        int rand_c = rand() % img_cols;
        int rand_ch = rand() % img_channel;

        uchar* pixel = img.ptr<uchar>(rand_r) + (rand_c * img_channel) + rand_ch;
        //salt noist ->255, pepper noise ->0
        *pixel = (rand() % 2 == 1) ? 255 : 0;
    }
}
// 벡터를 만들어서 9개의 픽셀을 받아온다
void filtered_Sort(int filter[])
{
    int num, i, j;
    for (i = 0; i < 9; i++) {
        num = filter[i];
        for (j = i - 1; j >= 0 && num < filter[j]; j--) {
            filter[j + 1] = filter[j];
        }
        filter[j + 1] = num;
    }
}

int main() {

    Mat img_input;
    Mat spimg;
    Mat mfimg;
    img_input = imread("Lena.png", IMREAD_COLOR);
    //흑백으로 변환
    cvtColor(img_input, img_input, cv::COLOR_RGB2GRAY);
    double noise;
    double sigma;
    int filtersize;

    cout << "Salt&Pepper noise 확률을 double형으로 입력하시오. 예) 5% -> 0.05 입력 \n";
    cin >> noise;
    cout << "GaussianNoise의 sigma 값을 int형으로 입력하시오. 예) 30 입력\n";
    cin >> sigma;
    cout << "Smoothing filter의 size를 3x3 일경우 3, 5x5 일경우 5를 입력하시오\n";
    cin >> filtersize;

    spimg = img_input.clone();
    sap(spimg, noise);


    if (img_input.empty())
    {
        cout << "파일을 읽어올수 없습니다." << endl;
        exit(1);
    }

    //GaussianNoise
    Mat img_noise(img_input.size(), CV_16SC1);
    double average = 0.0;

    //난수함수
    randn(img_noise, Scalar::all(average), Scalar::all(sigma));

    Mat gnimg;
    img_input.convertTo(gnimg, CV_16SC1);
    //두 영상의 가중치 합
    addWeighted(gnimg, 1.0, img_noise, 1.0, 0.0, gnimg);
    gnimg.convertTo(gnimg, img_input.type());


    //GaussianSmoothing
    Mat gsimg;
    GaussianBlur(gnimg, gsimg, Size(3, 3), 1.5);

    //moving average filter

    int filter1[3][3] = { {1,1,1},
                        { 1,1,1 },
                        { 1,1,1 } };

    int filter2[5][5] = { { 1,1,1,1,1 },
                        { 1,1,1,1,1 },
                        { 1,1,1,1,1 },
                        { 1,1,1,1,1 },
                        { 1,1,1,1,1 } };


    Mat img_aver;
    long int sum;
    img_aver = Mat(spimg.rows, spimg.cols, CV_8UC1);


    for (int y = 0; y < spimg.rows; y++)
    {
        for (int x = 0; x < spimg.cols; x++)
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
                    else if (result_y > spimg.rows - 1) result_y = spimg.rows - 1;

                    if (result_x < 0) result_x = 0;
                    else if (result_x > spimg.cols - 1) result_x = spimg.cols - 1;

                    //filter 크기별 계산
                    if (filtersize == 3)
                        sum += spimg.at<uchar>(result_y, result_x) * filter1[filtersize / 2 + i][filtersize / 2 + j];
                    else if (filtersize == 5)
                        sum += spimg.at<uchar>(result_y, result_x) * filter2[filtersize / 2 + i][filtersize / 2 + j];

                }
            }

            sum = sum / (double)(filtersize * filtersize);

            if (sum > 255) sum = 255;
            if (sum < 0) sum = 0;

            img_aver.at<uchar>(y, x) = sum;
        }
    }


    //3x3 median filter

    int median_filter[9];

    mfimg = spimg.clone();

    for (int y = 0; y < spimg.rows; y++)
        for (int x = 0; x < spimg.cols; x++)
            mfimg.at<uchar>(y, x) = 0.0;

    for (int y = 1; y < spimg.rows - 1; y++) {
        for (int x = 1; x < spimg.cols - 1; x++) {

            //9개의 3x3 filter

            median_filter[0] = spimg.at<uchar>(y - 1, x - 1);
            median_filter[1] = spimg.at<uchar>(y, x - 1);
            median_filter[2] = spimg.at<uchar>(y + 1, x - 1);
            median_filter[3] = spimg.at<uchar>(y - 1, x);
            median_filter[4] = spimg.at<uchar>(y, x);
            median_filter[5] = spimg.at<uchar>(y + 1, x);
            median_filter[6] = spimg.at<uchar>(y - 1, x + 1);
            median_filter[7] = spimg.at<uchar>(y, x + 1);
            median_filter[8] = spimg.at<uchar>(y + 1, x + 1);

            // 중앙값을 찾기 위해 창을 정렬한다
            filtered_Sort(median_filter);

            // 필터의 정렬된 값중 중앙값을 할당한다
            mfimg.at<uchar>(y, x) = median_filter[4];
        }
    }


    namedWindow("Original", WINDOW_AUTOSIZE);
    namedWindow("GaussianNoise", WINDOW_AUTOSIZE);
    namedWindow("SaltAndPepper", WINDOW_AUTOSIZE);
    namedWindow("Smoothing filter", WINDOW_AUTOSIZE);
    namedWindow("MedianFiltered", WINDOW_AUTOSIZE);

    imshow("Original", img_input);
    imshow("GaussianNoise", gnimg);
    imshow("SaltAndPepper", spimg);
    imshow("Smoothing filter", img_aver);
    imshow("MedianFiltered", mfimg);

    waitKey(0);
    return 0;
}

