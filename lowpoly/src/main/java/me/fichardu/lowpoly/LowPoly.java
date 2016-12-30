package me.fichardu.lowpoly;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;

/**
 * Created by xf on 2016/12/29
 */

public class LowPoly {

    static {
        System.loadLibrary("lowPoly");
    }

    public static native int[] lowPoly(int[] pixels, int width, int height, int accuracy, int pointCount);

    public static native int[] sobel(int[] pixels, int width, int height);

    public static native int[] delaunay(int width, int height, int pointCount);


    public static Bitmap getLowPolyBitmap(Bitmap bitmap, int accuracy, int randomPointCount) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();

        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
        int[] triangles = LowPoly.lowPoly(pixels, width, height, accuracy, randomPointCount);

        Paint paint = new Paint();
        paint.setAntiAlias(true);
        paint.setStyle(Paint.Style.FILL_AND_STROKE);

        Bitmap lowPoly = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
        Canvas canvas = new Canvas(lowPoly);
        canvas.drawColor(Color.WHITE);
        Path path = new Path();

        for (int i = 0; i < triangles.length; i += 6) {
            int x1 = triangles[i];
            int y1 = triangles[i +1];
            int x2 = triangles[i +2];
            int y2 = triangles[i +3];
            int x3 = triangles[i +4];
            int y3 = triangles[i +5];

            int cx = (x1 + x2 + x3) / 3;
            int cy = (y1 + y2 + y3) / 3;

            int color = bitmap.getPixel(cx, cy);
            paint.setColor(color);

            path.reset();
            path.moveTo(x1, y1);
            path.lineTo(x2, y2);
            path.lineTo(x3, y3);
            path.close();
            canvas.drawPath(path, paint);
        }

        return lowPoly;
    }

}
