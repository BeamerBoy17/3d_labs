#pragma once
#include <windows.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14

struct POINT3
{
    int x;
    int y;
    int z;
};

POINT Point(int x, int y);
POINT3 PointEx(int x, int y, int z);
int Round(double x);
double Abs(double X);
double DegreeToRad(double Angle);
POINT3 GlobalPointToScreen(POINT3 World_Point, double Angle_Horizontal, double Angle_Vertical, int R);
POINT Perspective(POINT3 PT, double d);
void Piramid(HDC hdc, POINT* PT, int COUNT);