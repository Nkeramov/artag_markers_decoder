#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <string>
#include <windows.h>

#include "point.h"

using namespace std;

#pragma once


const double PI = acos(0) * 2;


double rad2deg(double x);

double deg2rad(double x);

int sgn(int x);

int constraint(int x, int a, int b);

char hexDigitToChar(uint8_t p);

uint8_t hexCharToDigit(char c);

int str2hex(string s);

array<uint8_t, 3> dec2rgb(uint32_t x);

uint32_t rgb2dec(uint8_t r, uint8_t g, uint8_t b);

unsigned int  rgb2gray(uint32_t x);

int CreateBmp24(string fname, unsigned int *pic, int width, int height);





