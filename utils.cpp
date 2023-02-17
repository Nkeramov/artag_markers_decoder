#include <cmath>
#include <limits>
#include <vector>
#include <array>
#include <string>

#include "utils.h"

using namespace std;

double rad2deg(double x){
    return x * 180.0 / PI;
}

double deg2rad(double x){
    return x * PI / 180.0;
}

int sgn(int x){
	return x >= 0 ? 1 : -1;
}

int constraint(int x, int a, int b){
	if (x < a)
		return a;
    if (x > b)
        return b;
	return x;
}

char hexDigitToChar(uint8_t p){
    if (p >= 0 && p <= 9)
        return '0' + p;
    if (p >= 10 && p <= 15)
        return 'A' + (p - 10);
    return '-';
}

uint8_t hexCharToDigit(char c){
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

int str2hex(string s){
    int k = 0, p = 1;
    for(int i = s.size() - 1; i >= 0; i--){
        k += hexCharToDigit(s[i]) * p;
        p *= 16;
    }
    return k;
}

std::array<uint8_t, 3> dec2rgb(uint32_t x){
    uint8_t r = (uint32_t)(x & 0x00FF0000UL) >> 16;
    uint8_t g = (uint32_t)(x & 0x0000FF00UL) >> 8;
    uint8_t b = (uint32_t)(x & 0x000000FFUL);
    std::array<uint8_t, 3> a = {r, g, b};
    return a;
}

uint32_t rgb2dec(uint8_t r, uint8_t g, uint8_t b){
    return ((uint32_t)r << 16) + ((uint32_t)g << 8) + (uint32_t)b;
}

unsigned int  rgb2gray(uint32_t x){
    std::array<uint8_t, 3> c = dec2rgb(x);
    uint8_t k = round(((uint32_t)c[0] + (uint32_t)c[1] + (uint32_t)c[2]) / 3.0);
    return rgb2dec(k, k, k);
}

int CreateBmp24(string fname, unsigned int *pic, int width, int height){
	HANDLE hFile;
	DWORD RW;
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	BYTE Palette [1024];
	memset (Palette, 0, 1024);
	memset (&bfh, 0, sizeof(bfh));
	// bmp 'BM'
	bfh.bfType = 0x4D42;
	// palette occupies 1Kb, but is not used
	bfh.bfOffBits = sizeof(bfh) + sizeof(bih) + 1024;
	bfh.bfSize = bfh.bfOffBits + sizeof(RGBTRIPLE) * width * height + height * (width % 4);
	memset (&bih, 0, sizeof(bih));
	bih.biSize = sizeof(bih);
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biHeight = height;
	bih.biWidth = width;
	bih.biPlanes = 1;
	hFile = CreateFile (fname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return -1;
	// write headers
	WriteFile (hFile, &bfh, sizeof (bfh), &RW, NULL);
	WriteFile (hFile, &bih, sizeof (bih), &RW, NULL);
	// write palette
	WriteFile (hFile, Palette, 1024, &RW, NULL);
	// write array with points colors
	for (int i = height - 1; i >= 0; i--){
		for (int j = 0; j < width; j++){
			RGBTRIPLE color;
			uint32_t x = pic[i * width + j];
			color.rgbtRed = (x & 0x00FF0000UL) >> 16;
            color.rgbtGreen = (uint32_t)(x & 0x0000FF00UL) >> 8;
            color.rgbtBlue = (uint32_t)(x & 0x000000FFUL);
			WriteFile (hFile, &color, sizeof(color), &RW, NULL);
		}
		WriteFile (hFile, Palette, width % 4, &RW, NULL);
	}
	CloseHandle(hFile);
	return 0;
}
