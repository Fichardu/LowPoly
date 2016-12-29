#ifndef Delaunay_H
#define Delaunay_H

#include <iostream>
#include <stdlib.h> // for C qsort 
#include <cmath>
#include <time.h> // for random

const double EPSILON = 0.000001;

struct TRIANGLE{
  int p1, p2, p3;
};

struct EDGE{
  int p1, p2;
};

struct POINT{
  double x, y;
};

int POINTCompare(const void *v1, const void *v2);
int Triangulate(int pNum, POINT point[], TRIANGLE triangle[], int &tNum);
int CircumCircle(double, double, double, double, double, double, double,
                 double, double &, double &, double &);

#endif


