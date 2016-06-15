//
// Created by akomandyr on 15.06.16.
//

#include <iostream>
#include "toner.h"

//convert to grayskale using luminosity method
const float lumRedWeight = 0.21;
const float lumGreenWeight = 0.72;
const float lumBlueWeight = 0.07;

void split_toning(
        float* pixelsSrc,
        float* pixelsDst,
        int width,
        int height,
        float highlightsHue,
        float shadowsHue,
        float balance
)
{
    std::cout << "works";
}

void toGrayskale(const float* src, float* dst, int height, int width)
{

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            float greypix =
                    src[i * width*4 + j * 4 + 1] * lumRedWeight +
                    src[i * width*4 + j * 4 + 2] * lumGreenWeight +
                    src[i * width*4 + j * 4 + 3] * lumBlueWeight;
            dst[i * width*4 + j * 4 + 1] = greypix;
            dst[i * width*4 + j * 4 + 2] = greypix;
            dst[i * width*4 + j * 4 + 3] = greypix;
        }
    }
}

void matrixmult(float* src1, float* src2, float* res, int n)
{

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            for (int k = 0; k < n; ++k)
            {
                res[i * n + j] += src1[i * n + k] * src2[k * n + j];
            };
        }
    }
}