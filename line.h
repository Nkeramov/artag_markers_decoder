#include <iostream>
#include <vector>
#include <string>

#include "point.h"

using namespace std;

#pragma once

const double EPS = 1e-9;

class Line{
    private:
        Point p1, p2;
        double k, b;
    public:
        Line(Point p1, Point p2);
        Point getStartPoint();
        Point getEndPoint();
        string toString(int digits);
        int getIntersection(Line ab, double &x, double &y);
        double getLength();
        vector<Point> divideLine(int split_points);
        Point getCenter();
};
