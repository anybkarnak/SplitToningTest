//
// Created by akomandyr on 15.06.16.
//

#include <iostream>
#include "toner.h"

//convert to grayskale using luminosity method
const float lumRedWeight = 0.21;
const float lumGreenWeight = 0.72;
const float lumBlueWeight = 0.07;

static inline float _Hue_2_RGB(float v1, float v2, float vH)
{
    if (vH < 0.0f) vH += 1.0f;
    if (vH > 1.0f) vH -= 1.0f;
    if ((6.0f * vH) < 1.0f) return (v1 + (v2 - v1) * 6.0f * vH);
    if ((2.0f * vH) < 1.0f) return (v2);
    if ((3.0f * vH) < 2.0f) return (v1 + (v2 - v1) * ((2.0f / 3.0f) - vH) * 6.0f);
    return (v1);
}

static inline float minF(float a, float b)
{
    return a > b ? b : a;
}

static inline float maxF(float a, float b)
{
    return a > b ? a : b;
}

static inline void RGB2HSL(const float* RGB, float* HSL)
{
    float H, S, L;

    float R = RGB[0];
    float G = RGB[1];
    float B = RGB[2];

    float var_Min = minF(R, minF(G, B));
    float var_Max = maxF(R, maxF(G, B));
    float del_Max = var_Max - var_Min;

    L = (var_Max + var_Min) / 2.0f;

    if (del_Max < 1e-6f)
    {
        H = 0.0f;
        S = 0.0f;
    }
    else
    {
        if (L < 0.5f)
            S = del_Max / (var_Max + var_Min);
        else
            S = del_Max / (2.0f - var_Max - var_Min);

        float del_R = (((var_Max - R) / 6.0f) + (del_Max / 2.0f)) / del_Max;
        float del_G = (((var_Max - G) / 6.0f) + (del_Max / 2.0f)) / del_Max;
        float del_B = (((var_Max - B) / 6.0f) + (del_Max / 2.0f)) / del_Max;

        if (R == var_Max)
            H = del_B - del_G;
        else if (G == var_Max)
            H = (1.0f / 3.0f) + del_R - del_B;
        else if (B == var_Max)
            H = (2.0f / 3.0f) + del_G - del_R;
        else
            H = 0.0f; // make GCC happy

        //H/=4;

        if (H < 0.0f) H += 1.0f;
        if (H > 1.0f) H -= 1.0f;
    }

    HSL[0] = H;
    HSL[1] = S;
    HSL[2] = L;
}

static inline void HSL2RGB(const float* HSL, float* RGB)
{
    float H = HSL[0];
    float S = HSL[1];
    float L = HSL[2];

    float var_1, var_2;

    if (S < 1e-6f)
    {
        RGB[0] = RGB[1] = RGB[2] = L;
    }
    else
    {
        if (L < 0.5f)
            var_2 = L * (1.0f + S);
        else
            var_2 = (L + S) - (S * L);

        var_1 = 2.0f * L - var_2;

        RGB[0] = _Hue_2_RGB(var_1, var_2, H + (1.0f / 3.0f));
        RGB[1] = _Hue_2_RGB(var_1, var_2, H);
        RGB[2] = _Hue_2_RGB(var_1, var_2, H - (1.0f / 3.0f));
    }
}

void tonePixel(const float* src, float* dst,
               float highlightsHue,
               float shadowsHue,
               float balance)
{

    RGB2HSL(src, dst);
    //toning
    dst[0] = dst[2] < balance ? shadowsHue : highlightsHue;

    HSL2RGB(dst, dst);
}

void split_toning(
        float* pixelsSrc,
        float* pixelsDst,
        int width,
        int height,
        float highlightsHue,
        float shadowsHue,
        float balance)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {

            tonePixel((pixelsSrc + i * width * 4 + j*4+1),
                      (pixelsDst + i * width * 4 + j*4+1),
                      highlightsHue,
                      shadowsHue,
                      balance);
        }
    }
}


void toGrayskale(const float* src, float* dst, int height, int width)
{

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            float greypix =
                    src[i * width * 4 + j * 4 + 1] * lumRedWeight +
                    src[i * width * 4 + j * 4 + 2] * lumGreenWeight +
                    src[i * width * 4 + j * 4 + 3] * lumBlueWeight;
            dst[i * width * 4 + j * 4 + 1] = greypix;
            dst[i * width * 4 + j * 4 + 2] = greypix;
            dst[i * width * 4 + j * 4 + 3] = greypix;
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