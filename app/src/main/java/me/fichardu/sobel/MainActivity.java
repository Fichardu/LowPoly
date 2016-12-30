package me.fichardu.sobel;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private TextView pointText;
    private TextView accuracyText;

    private Bitmap sourceBitmap;
    private int accuracy = 50;
    private int pointCount = 100;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        pointText = (TextView) findViewById(R.id.point_text);
        accuracyText = (TextView) findViewById(R.id.accuracy_text);
        sourceBitmap = ((BitmapDrawable) getResources().getDrawable(R.drawable.test)).getBitmap();

        pointText.setText(String.valueOf(pointCount));
        accuracyText.setText(String.valueOf(accuracy));
    }

    public void pointAdd(View view) {
        pointCount += 10;
        pointText.setText(String.valueOf(pointCount));
    }

    public void pointDec(View view) {
        pointCount -= 10;
        if (pointCount < 0) {
            pointCount = 0;
        }
        pointText.setText(String.valueOf(pointCount));
    }

    public void accuracyAdd(View view) {
        accuracy += 10;
        accuracyText.setText(String.valueOf(accuracy));
    }

    public void accuracyDec(View view) {
        accuracy -= 10;
        if (accuracy < 0) {
            accuracy = 0;
        }
        accuracyText.setText(String.valueOf(accuracy));
    }

    public void fill(View view) {
        Bitmap copy = nativeLowPoly(sourceBitmap, pointCount, accuracy, true);
        ImageView sobelImage = (ImageView) findViewById(R.id.sobel_image);
        sobelImage.setImageBitmap(copy);
    }

    public void stroken(View view) {
        Bitmap copy = nativeLowPoly(sourceBitmap, pointCount, accuracy, false);
        ImageView sobelImage = (ImageView) findViewById(R.id.sobel_image);
        sobelImage.setImageBitmap(copy);

    }

    public void nativeSobel(View view) {
        int width = sourceBitmap.getWidth();
        int height = sourceBitmap.getHeight();
        int[] pixels = new int[width * height];
        sourceBitmap.getPixels(pixels, 0, width, 0, 0, width, height);
        int[] sobelPoints = LowPoly.sobel(pixels, width, height, 0);

        int[] sobelPixels = new int[width * height];
        Arrays.fill(sobelPixels, Color.BLACK);
        for (int i = 0; i < sobelPoints.length; i+=2) {
            sobelPixels[sobelPoints[i] + sobelPoints[i+1]*width] = Color.WHITE;
        }

        Bitmap copy = Bitmap.createBitmap(sobelPixels, sourceBitmap.getWidth(), sourceBitmap.getHeight
                (), Bitmap.Config.ARGB_8888);
        ImageView sobelImage = (ImageView) findViewById(R.id.sobel_image);
        sobelImage.setImageBitmap(copy);
    }

    public void nativeDelaunay(View view) {
        int width = sourceBitmap.getWidth();
        int height = sourceBitmap.getHeight();

        int[] triangles = LowPoly.delaunay(width, height, pointCount);

        int x1, x2, x3, y1, y2, y3;

        Paint paint = new Paint();
        paint.setAntiAlias(true);
        paint.setStyle(Paint.Style.STROKE);

        Bitmap copy = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(copy);
        canvas.drawColor(Color.WHITE);
        Path path = new Path();

        for (int i = 0; i < triangles.length; i += 6) {
            x1 = triangles[i];
            y1 = triangles[i +1];
            x2 = triangles[i +2];
            y2 = triangles[i +3];
            x3 = triangles[i +4];
            y3 = triangles[i +5];

            path.reset();
            path.moveTo(x1, y1);
            path.lineTo(x2, y2);
            path.lineTo(x3, y3);
            path.close();
            canvas.drawPath(path, paint);
        }

        ImageView sobelImage = (ImageView) findViewById(R.id.sobel_image);
        sobelImage.setImageBitmap(copy);
    }


    private Bitmap nativeLowPoly(Bitmap bitmap, int pointCount, int accuracy, boolean fill) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();

        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
        int[] triangles = LowPoly.lowPoly(pixels, width, height, accuracy, pointCount);

        int x1, x2, x3, y1, y2, y3, cx, cy;

        Paint paint = new Paint();
        paint.setAntiAlias(true);
        if (fill) {
            paint.setStyle(Paint.Style.FILL_AND_STROKE);
        } else {
            paint.setStyle(Paint.Style.STROKE);
        }

        Bitmap lowPoly = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(lowPoly);
        canvas.drawColor(Color.WHITE);
        Path path = new Path();

        for (int i = 0; i < triangles.length; i += 6) {
            x1 = triangles[i];
            y1 = triangles[i +1];
            x2 = triangles[i +2];
            y2 = triangles[i +3];
            x3 = triangles[i +4];
            y3 = triangles[i +5];

            cx = (x1 + x2 + x3) / 3;
            cy = (y1 + y2 + y3) / 3;
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

    private void sobel(View view) {
        int[] pixels = Sobel.getImage(sourceBitmap, 40);
        Bitmap copy = Bitmap.createBitmap(pixels, sourceBitmap.getWidth(), sourceBitmap.getHeight
                (), Bitmap.Config.ARGB_8888);
        ImageView sobelImage = (ImageView) findViewById(R.id.sobel_image);
        sobelImage.setImageBitmap(copy);
    }

    private void lowPoly(View view) {
        List<int[]> pixels = Sobel.getSobelPixels(sourceBitmap, 40);

    }

}
