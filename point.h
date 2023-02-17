#include <iostream>

using namespace std;

#pragma once


class Point{
    private:
        double x, y;
    public:
        Point();
        Point(double x, double y);
        double getX();
        double getY();
        void setX(double x);
        void setY(double y);
        string toString();
};
