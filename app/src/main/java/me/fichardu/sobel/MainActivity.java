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

import java.util.ArrayList;
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
        Bitmap copy = lowPoly(sourceBitmap, pointCount, accuracy, true);
        ImageView sobelImage = (ImageView) findViewById(R.id.sobel_image);
        sobelImage.setImageBitmap(copy);
    }

    public void stroken(View view) {
        Bitmap copy = lowPoly(sourceBitmap, pointCount, accuracy, false);
        ImageView sobelImage = (ImageView) findViewById(R.id.sobel_image);
        sobelImage.setImageBitmap(copy);

    }

    public void nativeSobel(View view) {
        int[] pixels = Sobel.getImage(sourceBitmap, 40);
        Bitmap copy = Bitmap.createBitmap(pixels, sourceBitmap.getWidth(), sourceBitmap.getHeight
                (), Bitmap.Config.ARGB_8888);
        ImageView sobelImage = (ImageView) findViewById(R.id.sobel_image);
        sobelImage.setImageBitmap(copy);
    }


    private Bitmap lowPoly(Bitmap bitmap, int pointCount, int accuracy, boolean fill) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        List<int[]> collectors = Sobel.getSobelPixels(bitmap, 40);
        ArrayList<int[]> particles = new ArrayList<>();

        for (int i = 0; i < pointCount; i++) {
            particles.add(new int[]{(int) (Math.random() * width), (int) (Math.random() * height)});
        }

        int len = collectors.size() / accuracy;
        for (int i = 0; i < len; i++) {
            int random = (int) (Math.random() * collectors.size());
            particles.add(collectors.get(random));
            collectors.remove(random);
        }

        particles.add(new int[]{0, 0});
        particles.add(new int[]{0, height});
        particles.add(new int[]{width, 0});
        particles.add(new int[]{width, height});

        List<Integer> triangles = Delaunay.triangulate(particles);

        int x1, x2, x3, y1, y2, y3, cx, cy;

        Paint paint = new Paint();
        paint.setAntiAlias(true);
        if (fill) {
            paint.setStyle(Paint.Style.FILL_AND_STROKE);
        } else {
            paint.setStyle(Paint.Style.STROKE);
        }

        Bitmap lowpoly = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(lowpoly);
        canvas.drawColor(Color.WHITE);
        Path path = new Path();

        for (int i = 0; i < triangles.size(); i += 3) {
            x1 = particles.get(triangles.get(i))[0];
            x2 = particles.get(triangles.get(i + 1))[0];
            x3 = particles.get(triangles.get(i + 2))[0];
            y1 = particles.get(triangles.get(i))[1];
            y2 = particles.get(triangles.get(i + 1))[1];
            y3 = particles.get(triangles.get(i + 2))[1];

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

        return lowpoly;
    }

}
