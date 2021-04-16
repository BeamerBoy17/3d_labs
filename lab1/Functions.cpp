
#include "Functions.h"

POINT Point(int x, int y)
{
    POINT res;
    res.x = x;
    res.y = y;
    return res;
}

POINT3 PointEx(int x, int y, int z)
{
    POINT3 res;
    res.x = x;
    res.y = y;
    res.z = z;
    return res;
}

int Round(double x)
{
    if (x < 0) x -= 0.5; else x += 0.5;
    return (int)x;
}

double Abs(double X)
{
    if (X < 0) X = (double)-X;
    return X;
}

double DegreeToRad(double Angle)
{
    return (((double)Angle * PI) / 180);
}

POINT3 GlobalPointToScreen(POINT3 World_Point, double Angle_Horizontal, double Angle_Vertical, int R)
{
    int M1[4];
    double M2[4][4];

    //Перевод в радианы

    double A = DegreeToRad(Angle_Horizontal);
    double B = DegreeToRad(Angle_Vertical);

    //Задаю вектор

    M1[0] = World_Point.x;
    M1[1] = World_Point.y;
    M1[2] = World_Point.z;
    M1[3] = 1;

    //Задаю матрицу видовых преобразований

    M2[0][0] = -sin(A);
    M2[1][0] = cos(A);
    M2[2][0] = 0;
    M2[3][0] = 0;

    M2[0][1] = -cos(B) * cos(A);
    M2[1][1] = -cos(B) * sin(A);
    M2[2][1] = sin(B);
    M2[3][1] = 0;

    M2[0][2] = -sin(B) * cos(A);
    M2[1][2] = -sin(B) * sin(A);
    M2[2][2] = -cos(B);
    M2[3][2] = R;

    M2[0][3] = 0;
    M2[1][3] = 0;
    M2[2][3] = 0;
    M2[3][3] = 1;

    int M3[4];
    int S = 0;

    //Умножаю вектор на матрицу

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            S = S + Round(M1[j] * (double)M2[i][j]);
        M3[i] = S; S = 0;
    }

    POINT3 res = PointEx(Abs(M3[0]), Abs(M3[1]), Abs(M3[2]));
    return res;
}

POINT Perspective(POINT3 PT, double d)
{
    int x = Round(d * ((double)PT.x / (double)PT.z));
    int y = Round(d * ((double)PT.y / (double)PT.z));

    return Point(x, y);
}

void Piramid(HDC hdc, POINT* PT, int COUNT)
{
    //Прочертиваю линии от вершины пирамиды к углам (вершина всегда в нулевом элементе массива)

    for (int i = 1; i < COUNT; i++)
    {
        MoveToEx(hdc, PT[0].x, PT[0].y, NULL);
        LineTo(hdc, PT[i].x, PT[i].y);
    }

    //Соединяю все углы между собой (условие для последнего угла)

    for (int i = 1; i < COUNT; i++)
    {
        MoveToEx(hdc, PT[i].x, PT[i].y, NULL);
        if (i != COUNT - 1)
            LineTo(hdc, PT[i + 1].x, PT[i + 1].y);
        else
            LineTo(hdc, PT[1].x, PT[1].y);
    }
}