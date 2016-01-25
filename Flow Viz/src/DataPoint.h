//
//  DataPoint.h
//  Flow Viz v1
//
//  Created by Adiel Fernandez on 12/24/15.
//
//

#ifndef __Flow_Viz_v1__DataPoint__
#define __Flow_Viz_v1__DataPoint__

#include <iostream>
#include "ofMain.h"

#pragma once

#endif /* defined(__Flow_Viz_v1__DataPoint__) */


class DataPoint{
    
public:
    
    DataPoint();
    
    void setup(double _x, double _y, double _z, double _u, double _v, double _w);
    
    double x, y, z, u, v, w;
    
    ofVec3f pos, vel;
    ofVec3f scaledPos, scaledVel, inflatedZVel;
    
    float scaleFactor;
    
    ofColor col;
    ofFloatColor fCol;
};