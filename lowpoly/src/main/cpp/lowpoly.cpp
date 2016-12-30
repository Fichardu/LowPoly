#include <jni.h>
#include <string>
#include "Sobel.h"
#include "Delaunay.h"


void randomize(){
    srand((time_t) time(NULL));
}

int random(int n){
    return rand()%n;
}

extern "C"
JNIEXPORT jintArray JNICALL
Java_me_fichardu_lowpoly_LowPoly_delaunay(JNIEnv *env, jclass type, jint width,
                                        jint height, jint pointCount) {
    randomize();
    POINT* points = new POINT[pointCount + 7];

    for (int i = 0; i < pointCount; ++i) {
        points[i].x = random(width);
        points[i].y = random(height);
    }

    qsort(points, pointCount, sizeof(POINT), PointCompare);

    TRIANGLE* triangles = new TRIANGLE[3*pointCount];
    int tNum = 0;
    Triangulate(pointCount, points, triangles, tNum);

    int triLen = tNum * 6;
    int* delaunayPoints = new int[triLen];
    for (int i = 0; i < tNum; ++i) {
        POINT p1 = points[triangles[i].p1];
        POINT p2 = points[triangles[i].p2];
        POINT p3 = points[triangles[i].p3];
        int start = i*6;
        delaunayPoints[start] = p1.x;
        delaunayPoints[start + 1] = p1.y;
        delaunayPoints[start + 2] = p2.x;
        delaunayPoints[start + 3] = p2.y;
        delaunayPoints[start + 4] = p3.x;
        delaunayPoints[start + 5] = p3.y;
    }

    jintArray data = env->NewIntArray(triLen);
    env->SetIntArrayRegion(data, 0, triLen, delaunayPoints);

    delete[] points;
    delete[] triangles;
    delete[] delaunayPoints;

    return data;
}

extern "C"
JNIEXPORT jintArray JNICALL
Java_me_fichardu_lowpoly_LowPoly_sobel(JNIEnv *env, jclass type, jintArray pixels_, jint width,
                                     jint height) {
    jint *pixels = env->GetIntArrayElements(pixels_, NULL);

    int length = width * height;
    int *sobelPixels = new int[length];
    int sobelSize = SobelPixel(width, height, pixels, sobelPixels, 60);
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
        pointArray[i*2] = points[i].x;
        pointArray[i*2+1] = points[i].y;
    }
    jintArray data = env->NewIntArray(pointCount*2);
    env->SetIntArrayRegion(data, 0, pointCount*2, pointArray);
    delete[] pointArray;

    return data;
}


extern "C"
jintArray
Java_me_fichardu_lowpoly_LowPoly_lowPoly(JNIEnv *env, jclass type, jintArray pixels_, jint width,
                                       jint height, jint accuracy, jint randomCount) {
    int length = width * height;
    int *sobelPixels = new int[length];
    jint *pixels = env->GetIntArrayElements(pixels_, NULL);
    // get sobel pixels
    int sobelSize = SobelPixel(width, height, pixels, sobelPixels, 40);
    env->ReleaseIntArrayElements(pixels_, pixels, 0);

    // pick out sobel pixels index
    int* sobelIndex = new int[sobelSize];
    for (int i = 0, j = 0; i < length; ++i) {
        if (sobelPixels[i] > 0) {
            sobelIndex[j++] = i;
        }
    }
    delete[] sobelPixels;

    // choose points by accuracy
    int accLength = sobelSize / accuracy;
    int pointCount = accLength + randomCount + 4;
    int *accPixels = new int[pointCount];
    randomize();
    for (int i = 0; i < accLength; ++i) {
        int randomIndex = random(sobelSize);
        accPixels[i] = sobelIndex[randomIndex];
        sobelIndex[randomIndex] = sobelIndex[sobelSize-1];
        sobelSize--;
    }
    // add random points
    for (int i = 0; i < randomCount; ++i) {
        accPixels[accLength + i] = random(length);
    }

    // add 4 corner points
    accPixels[pointCount - 4] = 0;
    accPixels[pointCount - 3] = width - 1;
    accPixels[pointCount - 2] = width * (height - 1);
    accPixels[pointCount - 1] = width * height - 1;

    POINT* points = new POINT[pointCount + 3];
    // convert array index to point structure
    for (int i = 0; i < pointCount; ++i) {
        points[i].x = accPixels[i] % width;
        points[i].y = accPixels[i] / width;
    }
    delete[] accPixels;

    qsort(points, pointCount, sizeof(POINT), PointCompare);

    TRIANGLE* triangles = new TRIANGLE[3 * pointCount];
    int tNum = 0;
    Triangulate(pointCount, points, triangles, tNum);

    // convert triangles to array
    int triLen = tNum * 6;
    int* delaunayPoints = new int[triLen];
    for (int i = 0; i < tNum; ++i) {
        POINT p1 = points[triangles[i].p1];
        POINT p2 = points[triangles[i].p2];
        POINT p3 = points[triangles[i].p3];
        int start = i * 6;
        delaunayPoints[start] = p1.x;
        delaunayPoints[start + 1] = p1.y;
        delaunayPoints[start + 2] = p2.x;
        delaunayPoints[start + 3] = p2.y;
        delaunayPoints[start + 4] = p3.x;
        delaunayPoints[start + 5] = p3.y;
    }

    jintArray data = env->NewIntArray(triLen);
    env->SetIntArrayRegion(data, 0, triLen, delaunayPoints);

    delete[] points;
    delete[] triangles;
    delete[] delaunayPoints;

    return data;
}




