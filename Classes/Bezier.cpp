#include <iostream>
#include <string.h>
#include <cmath>
//#include <gl\glut.h>
//#include <graphics.h>
//#include "Leap.h"
//#include <thread>
#include <stdio.h>
#include <stdlib.h>
//#include <Windows.h>
#include "Bezier.h"
//using namespace Leap;
using namespace std;

#define Width 800
#define Height 600


list<HitPoint> l2, l3, l4;
list<HitPoint> *l;
list<HitPoint>::iterator li;

float Distance(Point1 p[], int n)	//º∆À„æ‡¿Î
{
    float dis = 0.0;
    float a = p[n].y - p[0].y;
    float b = p[0].x - p[n].x;
    float c = p[0].y * p[n].x - p[0].x *p[n].y;
    for (int i = 1; i < n; ++i)
    {
        float temp = fabs((p[i].x * a + p[i].y * b + c) / sqrt(a*a + b*b));
        if (temp > dis)
            dis = temp;
    }
    return dis;
}

void BezierCurveSplitting(Point1 P[], Point1 Q[], Point1 R[], int n)	//∑÷∏Ó«˙œﬂ
{
    int q = 0, r = n;
    Q[q++] = P[0];
    R[r--] = P[n];
    float x1, x2, y1, y2;
    for (int i = 1; i <= n; ++i)	//»° t = 0.5 ±„”⁄º∆À„
    {
        for (int j = 0; j <= n - i; ++j)
        {
            x1 = P[j].x;
            x2 = P[j + 1].x;
            y1 = P[j].y;
            y2 = P[j + 1].y;
            P[j].x = 0.5 * x1 + 0.5 * x2;
            P[j].y = 0.5 * y1 + 0.5 * y2;
        }
        Q[q++] = P[0];	//P{0}{i}
        R[r--] = P[n - i];	//P{i}{n-i}
    }
    return;
}

void DisplayBezierCurve(Point1 P[], int n, float delta, list<HitPoint> &l)
{
    
    if (Distance(P, n) <= delta)	//ª≠œﬂ
    {
        for (int i = 0; i <= n; ++i)
        {
            l.push_back(HitPoint(P[i].x, P[i].y, 100, 100));
        }
        delete P;
    }
    else
    {
        Point1 *Q = new Point1[n + 1];
        Point1 *R = new Point1[n + 1];
        BezierCurveSplitting(P, Q, R, n);	//∞—«˙œﬂ∑÷≥…¡Ω∂Œ
        
        DisplayBezierCurve(Q, n, delta, l);	//∑÷±ª≠œﬂ
        DisplayBezierCurve(R, n, delta, l);
        delete P;
    }
}

void bezier()
{
    Point1 *Pos;
    int n;
    Pos = new Point1[3];
    Pos[0].x = 400;
    Pos[0].y = 500;
    Pos[1].x = 400;
    Pos[1].y = 300;
    Pos[2].x = 600;
    Pos[2].y = 200;
    DisplayBezierCurve(Pos, 2, 0.1, l2);
    Pos = new Point1[3];
    Pos[0].x = 600;
    Pos[0].y = 200;
    Pos[1].x = 400;
    Pos[1].y = 300;
    Pos[2].x = 400;
    Pos[2].y = 500;
    DisplayBezierCurve(Pos, 2, 0.1, l2);
    Pos = new Point1[13];
    for (int i = 0; i < 13; ++i)
    {
        Pos[i].x = p3[i][0];
        Pos[i].y = p3[i][1];
    }
    DisplayBezierCurve(Pos, 12, 0.1, l3);
    Pos = new Point1[15];
    for (int i = 0; i < 15; ++i)
    {
        Pos[i].x = p4[i][0];
        Pos[i].y = p4[i][1];
    }
    DisplayBezierCurve(Pos, 14, 0.1, l4);
}

//DWORD WINAPI Fun(LPVOID lpParamter) {
//
//}

