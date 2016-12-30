#include "Delaunay.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
// CircumCircle() :
//   Return true if a point (xp,yp) is inside the circumcircle made up
//   of the points (x1,y1), (x2,y2), (x3,y3)
//   The CircumCircle centre is returned in (xc,yc) and the radius r
//   Note : A point on the edge is inside the circumcircle
////////////////////////////////////////////////////////////////////////
int CircumCircle(double xp, double yp, double x1, double y1, double x2,
                 double y2, double x3, double y3, double &xc, double &yc, double &r) {
    double m1, m2, mx1, mx2, my1, my2;
    double dx, dy, rsqr, drsqr;

    double fabsy1y2 = abs(y1 - y2);
    double fabsy2y3 = abs(y2 - y3);
    /* Check for coincident points */
    if (fabsy1y2 < EPSILON && fabsy2y3 < EPSILON) {
        return (false);
    }
    if (fabsy1y2 < EPSILON) {
        m2 = -(x3 - x2) / (y3 - y2);
        mx2 = (x2 + x3) / 2.0;
        my2 = (y2 + y3) / 2.0;
        xc = (x2 + x1) / 2.0;
        yc = m2 * (xc - mx2) + my2;
    } else if (fabsy2y3 < EPSILON) {
        m1 = -(x2 - x1) / (y2 - y1);
        mx1 = (x1 + x2) / 2.0;
        my1 = (y1 + y2) / 2.0;
        xc = (x3 + x2) / 2.0;
        yc = m1 * (xc - mx1) + my1;
    } else {
        m1 = -(x2 - x1) / (y2 - y1);
        m2 = -(x3 - x2) / (y3 - y2);
        mx1 = (x1 + x2) / 2.0;
        mx2 = (x2 + x3) / 2.0;
        my1 = (y1 + y2) / 2.0;
        my2 = (y2 + y3) / 2.0;
        xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        yc = (fabsy1y2 > fabsy2y3) ? m1 * (xc - mx1) + my1 : m2 * (xc - mx2) + my2;
    }
    dx = x2 - xc;
    dy = y2 - yc;
    rsqr = dx * dx + dy * dy;
    r = (int) sqrt(rsqr);
    dx = xp - xc;
    dy = yp - yc;
    drsqr = dx * dx + dy * dy;
    return drsqr <= rsqr;
}

void SuperTriangle(int pointNum, POINT point[], POINT superTriagle[]) {
    int xmin, xmax, ymin, ymax, xmid, ymid;
    int dx, dy, dmax;

    xmin = point[0].x;
    ymin = point[0].y;
    xmax = xmin;
    ymax = ymin;
    for (int i = 1; i < pointNum; i++) {
        if (point[i].x < xmin) xmin = point[i].x;
        if (point[i].x > xmax) xmax = point[i].x;
        if (point[i].y < ymin) ymin = point[i].y;
        if (point[i].y > ymax) ymax = point[i].y;
    }
    dx = xmax - xmin;
    dy = ymax - ymin;
    dmax = (dx > dy) ? dx : dy;
    xmid = (xmax + xmin) / 2;
    ymid = (ymax + ymin) / 2;
    /*
     Set up the supertriangle
     his is a triangle which encompasses all the sample points.
     The supertriangle coordinates are added to the end of the
     vertex list. The supertriangle is the first triangle in
     the triangle list.
    */
    superTriagle[0].x = xmid - 20 * dmax;
    superTriagle[0].y = ymid - dmax;
    superTriagle[1].x = xmid;
    superTriagle[1].y = ymid + 20 * dmax;
    superTriagle[2].x = xmid + 20 * dmax;
    superTriagle[2].y = ymid - dmax;
}
///////////////////////////////////////////////////////////////////////////////
// Triangulate() :
//   Triangulation subroutine
//   Takes as input NV vertices in array point
//   Returned is a list of ntri triangular faces in the array v
//   These triangles are arranged in a consistent clockwise order.
//   The triangle array 'v' should be malloced to 3 * nv
//   The vertex array point must be big enough to hold 3 more points
//   The vertex array must be sorted in increasing x values say
//
//   qsort(p,nv,sizeof(XYZ),XYZCompare);
///////////////////////////////////////////////////////////////////////////////

int Triangulate(int pNum, POINT point[], TRIANGLE triangle[], int &tNum) {
    int *complete = NULL;
    EDGE *edges = NULL;
    int nedge = 0;
    int trimax, emax = 200;
    int inside;
    int i, j, k;
    double xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r;

    /* Allocate memory for the completeness list, flag for each triangle */
    trimax = 4 * pNum;
    complete = new int[trimax];
    /* Allocate memory for the edge list */
    edges = new EDGE[emax];
    /*
     Find the maximum and minimum vertex bounds.
     This is to allow calculation of the bounding triangle
    */
    SuperTriangle(pNum, point, &point[pNum]);

    triangle[0].p1 = pNum;
    triangle[0].p2 = pNum + 1;
    triangle[0].p3 = pNum + 2;
    complete[0] = false;
    tNum = 1;
    /*
     Include each point one at a time into the existing mesh
    */
    for (i = 0; i < pNum; i++) {
        xp = point[i].x;
        yp = point[i].y;
        nedge = 0;
        /*
         Set up the edge buffer.
         If the point (xp,yp) lies inside the circumcircle then the
         three edges of that triangle are added to the edge buffer
         and that triangle is removed.
        */
        for (j = 0; j < tNum; j++) {
            if (complete[j])
                continue;
            x1 = point[triangle[j].p1].x;
            y1 = point[triangle[j].p1].y;
            x2 = point[triangle[j].p2].x;
            y2 = point[triangle[j].p2].y;
            x3 = point[triangle[j].p3].x;
            y3 = point[triangle[j].p3].y;
            inside = CircumCircle(xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r);
            if (xc + r < xp)
                // Suggested
                // if (xc + r + EPSILON < xp)
                complete[j] = true;
            if (inside) {
                /* Check that we haven't exceeded the edge list size */
                if (nedge + 3 >= emax) {
                    emax += 100;
                    EDGE* p_EdgeTemp = new EDGE[emax];
                    for (int li = 0; li < nedge; li++) { // Fix by John Bowman
                        p_EdgeTemp[li] = edges[li];
                    }
                    delete[] edges;
                    edges = p_EdgeTemp;
                }
                edges[nedge + 0].p1 = triangle[j].p1;
                edges[nedge + 0].p2 = triangle[j].p2;
                edges[nedge + 1].p1 = triangle[j].p2;
                edges[nedge + 1].p2 = triangle[j].p3;
                edges[nedge + 2].p1 = triangle[j].p3;
                edges[nedge + 2].p2 = triangle[j].p1;
                nedge += 3;
                triangle[j] = triangle[tNum - 1];
                complete[j] = complete[tNum - 1];
                tNum--;
                j--;
            }
        }
        /*
         Tag multiple edges
         Note: if all triangles are specified anticlockwise then all
         interior edges are opposite pointing in direction.
        */
        for (j = 0; j < nedge - 1; j++) {
            for (k = j + 1; k < nedge; k++) {
                if ((edges[j].p1 == edges[k].p2) && (edges[j].p2 == edges[k].p1)) {
                    edges[j].p1 = -1;
                    edges[j].p2 = -1;
                    edges[k].p1 = -1;
                    edges[k].p2 = -1;
                }
                /* Shouldn't need the following, see note above */
                if ((edges[j].p1 == edges[k].p1) && (edges[j].p2 == edges[k].p2)) {
                    edges[j].p1 = -1;
                    edges[j].p2 = -1;
                    edges[k].p1 = -1;
                    edges[k].p2 = -1;
                }
            }
        }
        /*
         Form new triangles for the current point
         Skipping over any tagged edges.
         All edges are arranged in clockwise order.
        */
        for (j = 0; j < nedge; j++) {
            if (edges[j].p1 < 0 || edges[j].p2 < 0)
                continue;
            triangle[tNum].p1 = edges[j].p1;
            triangle[tNum].p2 = edges[j].p2;
            triangle[tNum].p3 = i;
            complete[tNum] = false;
            tNum++;
        }
    }
    /*
     Remove triangles with supertriangle vertices
     These are triangles which have a vertex number greater than nv
    */
    for (i = 0; i < tNum; i++) {
        if (triangle[i].p1 >= pNum || triangle[i].p2 >= pNum || triangle[i].p3 >= pNum) {
            triangle[i] = triangle[tNum - 1];
            tNum--;
            i--;
        }
    }
    delete[] edges;
    delete[] complete;
    return 0;
}


int PointCompare(const void *v1, const void *v2) {
    POINT *p1, *p2;

    p1 = (POINT *) v1;
    p2 = (POINT *) v2;
    if (p1->x < p2->x) {
        return -1;
    } else if (p1->x > p2->x) {
        return 1;
    } else {
        return 0;
    }
}




