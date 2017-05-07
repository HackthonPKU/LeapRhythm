//
//  Bezier.hpp
//  Wave
//
//  Created by tegusi on 2017/5/6.
//
//

#ifndef Bezier_hpp
#define Bezier_hpp

#include <iostream>
#include <string.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <stdio.h>

using namespace std;
const int p3[13][2] = { 650,772,650,230,250,0,53,414,260,670,583,548,699,63,837,67,1174,619,1354,121,948,126,676,315,650,771 };
const int p4[15][2] = { 696,772,647,30,848,6,1039,383,1008,720,536,30,223,5,46,185,48,474,336,692,473,143,1261,296,1308,242,1335,269,775,244 };


class HitPoint {
public:
    float x, y, z;
    float time;
    float duration;
    bool hit;
    HitPoint* next;
    HitPoint* former;
    HitPoint() {
        next = NULL;
        former = NULL;
        hit = false;
    }
    HitPoint(float xx, float yy, float zz, float t):x(xx), y(yy), z(zz), time(t) {
        duration = 0.1;
        next = NULL;
        former = NULL;
        hit = false;
    }
};

class flow {
public:
    float crt_time;
    flow* next;
    HitPoint* hit_point;
    flow() {
        next = NULL;
        hit_point = NULL;
    }
};

struct Point1 {
    float x, y;
};

#define Width 800
#define Height 600


float Distance(Point1 p[], int n);
void BezierCurveSplitting(Point1
                          P[], Point1 Q[], Point1 R[], int n);
void DisplayBezierCurve(Point1 P[], int n, float delta, list<HitPoint> &l);
void bezier();
#endif /* Bezier_hpp */
