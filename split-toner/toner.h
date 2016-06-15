//
// Created by akomandyr on 15.06.16.
//

#ifndef SPLITTONINGTEST_TONER_H
#define SPLITTONINGTEST_TONER_H

//
//по заданию:
//
//Задание - сделать простой эффект Split Toning.
//
//Фото должно обесцветиться и раскраситься заданными цветами в "светах" и "тенях".
//
//Параметры:
//
//highlightsHue - Hue в светах (от 0 до 1)
//shadowsHue - Hue в тенях (от 0 до 1)
//balance - точка разделения на "света" и "тени" (0.5 - середина)
//
//Оформить в отдельную пару .h/.cpp файлов с возможностью вызова функции из С-окружения:

void split_toning(
        float* pixelsSrc,
        float* pixelsDst,
        int width,
        int height,
        float highlightsHue,
        float shadowsHue,
        float balance
);
void toGrayskale(const float* src, float* dst, int width, int height);
//Данные в памяти располагаются в виде ARGB (A - зарезервированный канал альфы, имеет индекс 0, R - красный, индекс 1 и т.д.).
//Значения компонент пикселей от 0 до 1.

#endif //SPLITTONINGTEST_TONER_H
