package me.fichardu.sobel;

import android.graphics.Bitmap;
import android.graphics.Color;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by xf on 2016/12/22
 */

public class Sobel {

    public static int[] getImage(Bitmap bitmap, int throttle) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();

        int[] copy = new int[width * height];
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int gx = -grayPixel(x-1, y-1, bitmap) + grayPixel(x+1, y-1, bitmap)
                        -2*grayPixel(x-1, y, bitmap) + 2*grayPixel(x+1, y, bitmap)
                        -grayPixel(x-1, y+1, bitmap) + grayPixel(x+1, y+1, bitmap);
                int gy = -grayPixel(x-1, y+1, bitmap) + grayPixel(x-1, y-1, bitmap)
                        -2*grayPixel(x, y+1, bitmap) + 2*grayPixel(x, y-1, bitmap)
                        -grayPixel(x+1, y+1, bitmap) + grayPixel(x+1, y-1, bitmap);
                int magnitude = Math.abs(gx) + Math.abs(gy);
                copy[x+y*width] = magnitude > throttle ? Color.WHITE : Color.BLACK;
            }
        }
        return copy;
    }

    public static List<int[]> getSobelPixels(Bitmap bitmap, int throttle) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();

        List<int[]> pixelList = new ArrayList<>();
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int gx = -grayPixel(x-1, y-1, bitmap) + grayPixel(x+1, y-1, bitmap)
                        -2*grayPixel(x-1, y, bitmap) + 2*grayPixel(x+1, y, bitmap)
                        -grayPixel(x-1, y+1, bitmap) + grayPixel(x+1, y+1, bitmap);
                int gy = -grayPixel(x-1, y+1, bitmap) + grayPixel(x-1, y-1, bitmap)
                        -2*grayPixel(x, y+1, bitmap) + 2*grayPixel(x, y-1, bitmap)
                        -grayPixel(x+1, y+1, bitmap) + grayPixel(x+1, y-1, bitmap);
                int magnitude = Math.abs(gx) + Math.abs(gy);
                if (magnitude > throttle) {
                    pixelList.add(new int[] {x, y});
                }
            }
        }
        return pixelList;
    }

    private static int grayPixel(int x, int y, Bitmap bitmap) {
        if (x < 0 || x >= bitmap.getWidth() || y < 0 || y >= bitmap.getHeight()) {
            return 0;
        } else {
            return grayColor(bitmap.getPixel(x, y));
        }
    }

    private static int grayColor(int color) {
        return (Color.red(color) + Color.green(color) + Color.blue(color)) / 3;
    }


}
