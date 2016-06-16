//
// Created by akomandyr on 15.06.16.
//

#include "toner.h"

//converts colour hue to grb colour http://www.easyrgb.com/index.php?X=MATH&H=19#text19
static inline float _Hue_2_RGB(float v1, float v2, float vH)
{
    if (vH < 0.0f)
    {
        vH += 1.0f;
    }

    if (vH > 1.0f)
    {
        vH -= 1.0f;
    }

    if ((6.0f * vH) < 1.0f)
    {
        return (v1 + (v2 - v1) * 6.0f * vH);
    }

    if ((2.0f * vH) < 1.0f)
    {
        return (v2);
    }

    if ((3.0f * vH) < 2.0f)
    {
        return (v1 + (v2 - v1) * ((2.0f / 3.0f) - vH) * 6.0f);
    }

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

//http://www.easyrgb.com/index.php?X=MATH&H=18#text18 converting RGB to HSL
static inline void RGB2HSL(const float *RGB, float *HSL)
{
    float H, S, L;

    float R = RGB[0];
    float G = RGB[1];
    float B = RGB[2];

    float _min = minF(R, minF(G, B));
    float _max = maxF(R, maxF(G, B));
    float del_Max = _max - _min;

    L = (_max + _min) / 2.0f;

    if (del_Max < 1e-6f)
    {
        H = 0.0f;
        S = 0.0f;
    }
    else
    {
        if (L < 0.5f)
        {
            S = del_Max / (_max + _min);
        }
        else
        {
            S = del_Max / (2.0f - _max - _min);
        }

        float del_R = (((_max - R) / 6.0f) + (del_Max / 2.0f)) / del_Max;
        float del_G = (((_max - G) / 6.0f) + (del_Max / 2.0f)) / del_Max;
        float del_B = (((_max - B) / 6.0f) + (del_Max / 2.0f)) / del_Max;

        if (R == _max)
        {
            H = del_B - del_G;
        }
        else if (G == _max)
        {
            H = (1.0f / 3.0f) + del_R - del_B;
        }
        else if (B == _max)
        {
            H = (2.0f / 3.0f) + del_G - del_R;
        }
        else
        {
            H = 0.0f;
        }

        if (H < 0.0f)
        {
            H += 1.0f;
        }

        if (H > 1.0f)
        {
            H -= 1.0f;
        }
    }

    HSL[0] = H;
    HSL[1] = S;
    HSL[2] = L;
}

//http://www.easyrgb.com/index.php?X=MATH&H=19#text19
static inline void HSL2RGB(const float *HSL, float *RGB)
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

static inline void tonePixel(const float *src, float *dst,
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
        float *pixelsSrc,
        float *pixelsDst,
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

            tonePixel((pixelsSrc + i * width * 4 + j * 4 + 1),
                      (pixelsDst + i * width * 4 + j * 4 + 1),
                      highlightsHue,
                      shadowsHue,
                      balance);
        }
    }
}
