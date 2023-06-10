#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stack>


#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>

using namespace cv;
using namespace std;

int main()
{
//Q3
    cv::Mat img = cv::imread("D:\\####Uni Fourth Sem\\DS\\A4\\Test_Images\\Student Test Images\\t1.bmp", cv::IMREAD_GRAYSCALE);
    cv::Mat img2 = cv::imread("D:\\OneDrive - FAST National University\\Desktop\\ConsoleApplication1\\ConsoleApplication1\\kashif.bmp", cv::IMREAD_GRAYSCALE);

    if (img.empty()) {
        cout << "Failed to read image!" << std::endl;
        return -1;
    }
    else
        cout << "Alhamdulillah, Image read successfully!" << endl;

    
    int rows = img.rows;
    int cols = img.cols;
    int** imgArray = new int* [rows];
    for (int i = 0; i < rows; i++) {
        imgArray[i] = new int[cols];
        uchar* p = img.ptr<uchar>(i);
        for (int j = 0; j < cols; j++) {
            imgArray[i][j] = static_cast<int>(p[j]);
        }
    }

    if (img2.empty()) {
        cout << "Failed to read image!" << std::endl;
        return -1;
    }
    else
        cout << "Alhamdulillah, Image read successfully!" << endl;


    int rows2 = img2.rows;
    int cols2 = img2.cols;
    int** imgArray2 = new int* [rows2];
    for (int i = 0; i < rows2; i++) {
        imgArray2[i] = new int[cols2];
        uchar* p = img2.ptr<uchar>(i);
        for (int j = 0; j < cols2; j++) {
            imgArray2[i][j] = static_cast<int>(p[j]);
        }
    }

    double sum = 0;
    double X = 0;
    double n = img.rows * img.cols;

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            int num = imgArray[i][j] - imgArray2[i][j];
            num = num * num;
            sum += num;
        }
    }

    X = sum / n;
    cout << X << endl;


    double Y = 0;
    double Z = 255;
    Y = 100 * (1 - (X / (Z * Z)));
    cout << Y << endl;







    return 0;


}