#include <jni.h>
#include <string>
#include "Sobel.h"
#include "Delaunay.h"

extern "C"
jstring
Java_me_fichardu_sobel_LowPoly_stringFromJNI(JNIEnv *env, jclass type) {

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
jintArray
Java_me_fichardu_sobel_LowPoly_lowPoly(JNIEnv *env, jclass type, jintArray pixels_, jint width,
                                       jint height, jint accuracy, jint randomCount) {
    jint *pixels = env->GetIntArrayElements(pixels_, NULL);

    int length = width * height;
    int *sobelPixels = new int[length];
    int sobelSize = SobelPixel(width, height, pixels, sobelPixels, 40);
    env->ReleaseIntArrayElements(pixels_, pixels, 0);

    int ntri = 0;
    int pointCount = sobelSize;
    POINT* points = new POINT[pointCount + 3];

    for (int i = 0; i < sobelSize; ++i) {
        points[i].x = sobelPixels[i] % width;
        points[i].y = sobelPixels[i] / width;
    }

    TRIANGLE* triangles = new TRIANGLE[3*pointCount];
    qsort(points, pointCount, sizeof(POINT), POINTCompare);
    Triangulate(pointCount, points, triangles, ntri);

    int triLen = ntri * 6;
    int* delaunay = new int[triLen];
    for (int i = 0; i < ntri; ++i) {
        POINT p1 = points[triangles[i].p1];
        POINT p2 = points[triangles[i].p2];
        POINT p3 = points[triangles[i].p3];
        int start = i*6;
        delaunay[start + 1] = (int) p1.x;
        delaunay[start + 2] = (int) p1.y;
        delaunay[start + 3] = (int) p2.x;
        delaunay[start + 4] = (int) p2.y;
        delaunay[start + 5] = (int) p3.x;
        delaunay[start + 6] = (int) p3.y;
    }

    delete[] points;
    delete[] triangles;
    delete[] sobelPixels;

    jintArray data = env->NewIntArray(triLen);
    env->SetIntArrayRegion(data, 0, triLen, delaunay);
    return data;
}


