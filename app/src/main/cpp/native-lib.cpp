#include <jni.h>
#include <string>
#include "Sobel.h"
#include "Delaunay.h"

extern "C"
JNIEXPORT jintArray JNICALL
Java_me_fichardu_sobel_LowPoly_sobel(JNIEnv *env, jclass type, jintArray pixels_, jint width,
                                     jint height, jint randomCount) {
    jint *pixels = env->GetIntArrayElements(pixels_, NULL);

    int length = width * height;
    int *sobelPixels = new int[length];
    int sobelSize = SobelPixel(width, height, pixels, sobelPixels, 40);
    env->ReleaseIntArrayElements(pixels_, pixels, 0);

    int pointCount = sobelSize;
    POINT* points = new POINT[pointCount];

    int p = 0;
    for (int i = 0; i < length; ++i) {
        if (sobelPixels[i] > 0) {
            points[p].x = i % width;
            points[p].y = i / width;
            ++p;
        }
    }

    int* pointArray = new int[pointCount*2];
    for (int i=0; i<pointCount; ++i) {
        pointArray[i*2] = (int) points[i].x;
        pointArray[i*2+1] = (int) points[i].y;
    }
    jintArray data = env->NewIntArray(pointCount*2);
    env->SetIntArrayRegion(data, 0, pointCount*2, pointArray);
    delete[] pointArray;

    return data;
}

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

    int pointCount = sobelSize;
    POINT* points = new POINT[pointCount + 3];

    int p = 0;
    for (int i = 0; i < length; ++i) {
        if (sobelPixels[i] > 0) {
            points[p].x = i % width;
            points[p].y = i / width;
            ++p;
        }
    }

    TRIANGLE* triangles = new TRIANGLE[3*pointCount];
    qsort(points, pointCount, sizeof(POINT), POINTCompare);

    int ntri = 0;
    Triangulate(pointCount, points, triangles, ntri);

    int triLen = ntri * 6;
    int* delaunay = new int[triLen];
    for (int i = 0; i < ntri; ++i) {
        POINT p1 = points[triangles[i].p1];
        POINT p2 = points[triangles[i].p2];
        POINT p3 = points[triangles[i].p3];
        int start = i*6;
        delaunay[start + 1] = p1.x;
        delaunay[start + 2] = p1.y;
        delaunay[start + 3] = p2.x;
        delaunay[start + 4] = p2.y;
        delaunay[start + 5] = p3.x;
        delaunay[start + 6] = p3.y;
    }

    jintArray data = env->NewIntArray(triLen);
    env->SetIntArrayRegion(data, 0, triLen, delaunay);

    delete[] points;
    delete[] triangles;
    delete[] sobelPixels;
    delete[] delaunay;

    return data;
}


