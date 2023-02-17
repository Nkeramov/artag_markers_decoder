#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include "point.h"

using namespace std;

Point::Point(){
    this->x = 0.0;
    this->y = 0.0;
}

Point::Point(double x, double y){
    this->x = x;
    this->y = y;
}

double Point::getX(){
    return this->x;
}

double Point::getY(){
    return this->y;
}

void Point::setX(double x){
    this->x = x;
}

void Point::setY(double y){
    this->y = y;
}

string Point::toString(){
    std::stringstream ss;
    ss << "(" << x << ";" << y << ")";
    return ss.str();
}

