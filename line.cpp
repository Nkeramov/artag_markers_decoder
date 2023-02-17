#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>

#include "utils.h"
#include "line.h"

using namespace std;

Line::Line(Point p1, Point p2){
    this->p1 = p1;
    this->p2 = p2;
    k = (p1.getY() - p2.getY()) / (p1.getX() - p2.getX());
    if (abs(k) < EPS)
        k = 0;
    b = (p2.getY() * p1.getX() - p1.getY() * p2.getX()) / (p1.getX() - p2.getX());
    if (abs(b) < EPS)
        b = 0;
}

string Line::toString(int digits){
    std::stringstream ss;
    if (digits > 0){
        ss << "y = " << fixed << setprecision(digits);
        if (abs(k) >= EPS)
            ss << k << " * x";
        if (b >= EPS)
            ss << " + " << b;
        else
            if (b <= -EPS)
                ss << " - " << abs(b);
    }
    else{
        ss << "y = " << round(k) << " * x";
        if (round(b) > 0)
            ss << " + " << round(b);
        else
            if (round(b) <0)
                ss << " - " << abs(round(b));
    }
    return ss.str();
}

int Line::getIntersection(Line ab, double &x, double &y){
    if (abs(k - ab.k) >= EPS){
        x = (ab.b - b) / (k - ab.k);
        y = (k * ab.b - ab.k * b) / (k - ab.k);
        return 1;
    }
    else{
        if (abs(b - ab.b) >= EPS)
            return 0;
        return -1;
    }
}

double Line::getLength(){
    return sqrt((p2.getX() - p1.getX())*(p2.getX() - p1.getX()) + (p2.getY() - p1.getY())*(p2.getY() - p1.getY()));
}

vector<Point> Line::divideLine(int split_points){
    vector<Point> points;
    for(int i = 0; i <= split_points; i++){
        unsigned int x = round(p1.getX() + ((p2.getX() - p1.getX()) / split_points * i));
        unsigned int y = round(p1.getY() + ((p2.getY() - p1.getY()) / split_points * i));
        points.push_back(Point(x, y));
    }
    return points;
}

Point Line::getCenter(){
    return Point(round((p1.getX() + p2.getX()) / 2.0), round((p1.getY() + p2.getY()) / 2.0));
}
