//
// Created by xf on 2016/12/29.
//

#include "Sobel.h"

/*
 * Find Sobel pixels
 *
 * return sobel pixels count
 */
int SobelPixel(int width, int height, int bitmap[], int sobelPixels[], int throttle) {
    int sobelPixelSize = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int gx = -GrayPixel(x-1, y-1, width, height, bitmap) + GrayPixel(x+1, y-1, width, height, bitmap)
                     -2*GrayPixel(x-1, y, width, height, bitmap) + 2*GrayPixel(x+1, y, width, height, bitmap)
                     -GrayPixel(x-1, y+1, width, height, bitmap) + GrayPixel(x+1, y+1, width, height, bitmap);
            int gy = -GrayPixel(x-1, y+1, width, height, bitmap) + GrayPixel(x-1, y-1, width, height, bitmap)
                     -2*GrayPixel(x, y+1, width, height, bitmap) + 2*GrayPixel(x, y-1, width, height, bitmap)
                     -GrayPixel(x+1, y+1, width, height, bitmap) + GrayPixel(x+1, y-1, width, height, bitmap);
            int magnitude = abs(gx) + abs(gy);
            if (magnitude > throttle) {
                sobelPixels[y*width + x] = 1;
                ++ sobelPixelSize;
            } else {
                sobelPixels[y*width + x] = 0;
            }
        }
    }
    return sobelPixelSize;
}


int GrayPixel(int x, int y, int width, int height, int bitmap[]) {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return 0;
    } else {
        return GrayColor(bitmap[width * y + x]);
    }
}

int GrayColor(int color) {
    return ((color >> 16 & 0xFF) + (color >> 8 & 0xFF) + (color & 0xFF)) / 3;
}