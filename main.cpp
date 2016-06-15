//
// Created by akomandyr on 15.06.16.
//

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "toner.h"

using namespace std;

void convertRGB2FLOAT(const unsigned char* src, float* dest, int rows, int cols)
{
    //3 - number of channels in openCV image 4 - number of channels in needed image
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            dest[i * cols * 4 + j * 4 + 0] = 0;
            dest[i * cols * 4 + j * 4 + 1] = (float) src[i * cols * 3 + j * 3 + 1] / 255;
            dest[i * cols * 4 + j * 4 + 2] = (float) src[i * cols * 3 + j * 3 + 2] / 255;
            dest[i * cols * 4 + j * 4 + 3] = (float) src[i * cols * 3 + j * 3 + 0] / 255;
        }
    }
}

void convertFLOAT2RGB(const float* src, unsigned char* dest, int rows, int cols)
{
    //3 - number of channels in openCV image 4 - number of channels in needed image
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            dest[i * cols * 3 + j * 3 + 1] = src[i * cols * 4 + j * 4 + 1] * 255;
            dest[i * cols * 3 + j * 3 + 2] = src[i * cols * 4 + j * 4 + 2] * 255;
            dest[i * cols * 3 + j * 3 + 0] = src[i * cols * 4 + j * 4 + 3] * 255;
        }
    }
}

int main(int argc,      // Number of strings in array argv
         char* argv[])  // Array of command-line argument strings
{
    std::string path;
    int count;
    // Display each command-line argument.
    cout << "\nCommand-line arguments:\n";
    for (count = 0; count < argc; count++)
        cout << "  argv[" << count << "]   "
        << argv[count] << "\n";
    if (argc == 2)
    {
        path = argv[1];
    }
    else
    {
        path = "download11.jpg";
    }

    //input image
    cv::Mat im_rgb = cv::imread(path);

    //gray image
    cv::Mat im_gray;
    cvtColor(im_rgb, im_gray, CV_RGB2GRAY);


    cv::Mat im_gray2(cv::Size(im_rgb.cols, im_rgb.rows), CV_8UC3);

    float* firstRgbData = new float[im_rgb.rows * im_rgb.cols * 4]();

    convertRGB2FLOAT(im_rgb.data, firstRgbData, im_rgb.rows, im_rgb.cols);

    toGrayskale(firstRgbData, firstRgbData, im_rgb.rows, im_rgb.cols);

    convertFLOAT2RGB(firstRgbData, im_gray2.data, im_rgb.rows, im_rgb.cols);


    cv::imshow("gscale_me", im_gray2);
    cv::imshow("gscale", im_gray);

    if (cv::waitKey() == 27)
    {
        delete[]firstRgbData;
        return 0;
    }


    //split_toning(NULL, NULL, 1, 1, 1, 1, 1);
    return 0;
}