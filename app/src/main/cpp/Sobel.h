//
// Created by xf on 2016/12/29.
//
#ifndef SOBEL_SOBEL_H
#define SOBEL_SOBEL_H

#include <stdlib.h>

int SobelPixel(int width, int height, int bitmap[], int sobelPixels[], int throttle);
int GrayColor(int color);
int GrayPixel(int x, int y, int width, int height, int *bitmap);

#endif //SOBEL_SOBEL_H
