//
//  Hand.h
//  Wave
//
//  Created by tegusi on 2017/5/6.
//
//

#ifndef Hand_hpp
#define Hand_hpp

#include <stdio.h>
#include "Leap.h"
class HandData{
public:
    float x,y,strength;
//    static Leap::Vector handCenter1;
//    HandData() {handCenter1 = Leap::Vector(0,0,0);}
};
class Dest{
public:
    bool changed,ispause,hit,start;
    int score,combo;
    float x,y;
};
#endif /* Hand_hpp */
