#include <iostream>

#include <fstream>
#include <array>
#include <cstdlib>

#include <limits>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>

#include "point.h"
#include "line.h"
#include "utils.h"

using namespace std;

array<Point, 4> getCornerPoints(unsigned int *picArray, int width, int height, int offset){
	if (offset == 0)
		offset = 4;
    array<Point, 4> points;
	for(int k = 0; k < 4; k++){
        int x = 0, y = 0;
		switch(k){
			case 0:	x = offset - 1; y = offset - 1; break;
			case 1:	x = offset - 1; y = width - offset - 1; break;
			case 2:	x = height - offset - 1; y = offset - 1; break;
			case 3:	x = height - offset - 1; y = width - offset - 1; break;
		}
		bool flag = 0;
		int count = 1, cnt = 1, i = x, j = y;
		while (!flag){
			while (!flag && (cnt > 0)){
				if (picArray[i * width + j] == 0){
					flag = 1;
                    points[k].setX(i);
                    points[k].setY(j);
                }
                cnt = cnt - 1;
                switch(k){
                    case 0:	i++; j--; break;
                    case 1:	i++; j++; break;
                    case 2:	i--; j--; break;
                    case 3:	i--; j++; break;
                }
			}
			switch(k){
				case 0:	y++; break;
				case 1:	y--; break;
				case 2:	y++; break;
				case 3:	y--; break;
			}
			i = x;
			j = y;
			count = count + 1;
			cnt = count;
		}
	}
	return points;
}

int main(){
	vector<string> files;
	WIN32_FIND_DATA file_data;
	const string input_directory = "input";
	const string output_directory = "output";
	HANDLE h = FindFirstFile((input_directory + "/*").c_str(), &file_data);
	int i = -1;
	if(h != INVALID_HANDLE_VALUE) {
        i = 1;
        cout << "Found the following files" << '\n';
        do {
            const string filename = file_data.cFileName;
            const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
            if (filename[0] == '.')
                continue;
            if (is_directory)
                continue;
            cout << i << "  " << filename << '\n';
            files.push_back(filename);
            i++;
        } while(FindNextFile(h, &file_data));
        FindClose(h);
	}
	else {
        cout << "Error, directory with files not found";
        return 0;
	}
	cout << "Input file number for analyse : ";
    int file_number = 0;
    cin >> file_number;
    if (file_number < i && file_number > 0){
        string fname = files[file_number - 1];
        cout << "You selected : " << fname  << '\n';
        ifstream fin(input_directory + "/" + fname);
        ofstream fout(output_directory + "/picBW.txt");
        if (!fin.is_open()){
            cout << "File not found";
           return 0;
        }
        // size of artag marker 3x3, 9 cells
        const int code_size = 9;
        int width, height;
        fin >> width >> height;
        cout << "Image width = " << width << "   height = " << height << '\n';
        unsigned int picRaw[height][width], picGray[height][width], picBW[height][width];
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                fin >> picRaw[i][j];
                picGray[i][j] = rgb2gray(picRaw[i][j]);
                picBW[i][j] = (dec2rgb(picGray[i][j])[0] > round(255 / 6)) ? 0x00FFFFFFUL : 0;
           }
        }
        CreateBmp24(output_directory + "/picRaw.bmp", picRaw[0], width, height);
        CreateBmp24(output_directory + "/picBW.bmp", picBW[0], width, height);
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                unsigned int t = picBW[i][j];
                if (t == 0x00FFFFFFUL)
                    t = 0;
                else t = 1;
                fout << t << ',';
            }
            fout << '\n';
        }
        cout << "Corner points ";
        array<Point, 4> points = getCornerPoints(picBW[0], width, height, 4);
        cout << "A" << points[0].toString() << "  ";
        cout << "B" << points[1].toString() << "  ";
        cout << "C" << points[2].toString() << "  ";
        cout << "D" << points[3].toString() << "  " << '\n';
        Line segAB(points[0], points[1]);
        vector<Point> pointsAB = segAB.divideLine(code_size + 1);
        Line segCD(points[2], points[3]);
        vector<Point> pointsCD = segCD.divideLine(code_size + 1);
        Line segVert(pointsAB[5], pointsCD[5]);
        Line segAC(points[0], points[2]);
        vector<Point> pointsAC = segAC.divideLine(code_size + 1);
        Line segBD(points[1], points[3]);
        vector<Point> pointsBD = segBD.divideLine(code_size + 1);
        Line segHor(pointsAC[5], pointsBD[5]);
        double xO, yO;
        if (segVert.getIntersection(segHor, xO, yO) >0 )
            cout << "Diagonal intersection O(" << round(xO) << ";" << round(yO) << ")" << '\n';
        Line segHorUp(pointsAC[3], pointsBD[3]);
        vector<Point> pointsHorUp = segHorUp.divideLine(code_size + 1);
        Line segHorDown(pointsAC[7], pointsBD[7]);
        vector<Point> pointsHorDown = segHorDown.divideLine(code_size + 1);
        vector<Point> pointsVert = segVert.divideLine(code_size + 1);
        vector<Point> pointsHor = segHor.divideLine(code_size + 1);
        array<array<unsigned int, 3>, code_size> roiPoints;
        roiPoints[0][0] = pointsHorUp[3].getX();      roiPoints[0][1] = pointsHorUp[3].getY();
        roiPoints[1][0] = pointsVert[3].getX();       roiPoints[1][1] = pointsVert[3].getY();
        roiPoints[2][0] = pointsHorUp[7].getX();      roiPoints[2][1] = pointsHorUp[7].getY();
        roiPoints[3][0] = pointsHor[3].getX();        roiPoints[3][1] = pointsHor[3].getY();
        roiPoints[4][0] = xO;                           roiPoints[4][1] = yO;
        roiPoints[5][0] = pointsHor[7].getX();        roiPoints[5][1] = pointsHor[7].getY();
        roiPoints[6][0] = pointsHorDown[3].getX();    roiPoints[6][1] = pointsHorDown[3].getY();
        roiPoints[7][0] = pointsVert[7].getX();       roiPoints[7][1] = pointsVert[7].getY();
        roiPoints[8][0] = pointsHorDown[7].getX();    roiPoints[8][1] = pointsHorDown[7].getY();
        cout << "ROI points (x; y; color)\n";
        for(int i = 0; i < code_size; i++){
            roiPoints[i][2] = picBW[ roiPoints[i][0] ][ roiPoints[i][1] ];
            if (roiPoints[i][2] == 0x00FFFFFFUL)
                roiPoints[i][2] = 0;
            else
                roiPoints[i][2] = 1;
            cout << "(" << roiPoints[i][0] << ";" << roiPoints[i][1] << ";" << roiPoints[i][2] << ")" << "  ";
            if ((i + 1) % 3 == 0)
                cout << '\n';
        }
        unsigned int ARtagNumber = -1;
        if (roiPoints[2][2] == 0){
            ARtagNumber = 8 * roiPoints[3][2] + 4 * roiPoints[7][2] + 2 * roiPoints[1][2] + 1 * roiPoints[5][2];
        }
        else if (roiPoints[0][2] == 0){
            ARtagNumber = 8 * roiPoints[7][2] + 4 * roiPoints[5][2] + 2 * roiPoints[3][2] + 1 * roiPoints[1][2];
        }
        else if (roiPoints[6][2] == 0){
            ARtagNumber = 8 * roiPoints[5][2] + 4 * roiPoints[1][2] + 2 * roiPoints[7][2] + 1 * roiPoints[3][2];
        }
        else if (roiPoints[8][2] == 0){
            ARtagNumber = 8 * roiPoints[1][2] + 4 * roiPoints[3][2] + 2 * roiPoints[5][2] + 1 * roiPoints[7][2];
        }
        if (roiPoints[4][2] == (roiPoints[3][2] + roiPoints[7][2] + roiPoints[1][2] + roiPoints[5][2]) % 2)
            cout << "ARtag parity check error" << '\n';
        cout << "ARtag decoded number = " << ARtagNumber << '\n';
        fout.close();
        fin.close();
        return 0;
    }
    else {
        cout << "Error, you selected incorrect file number";
        return 0;
    }
    return 0;
}
