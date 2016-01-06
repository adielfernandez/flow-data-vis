//
//  Tri.h
//  Flow Viz v1
//
//  Created by Adiel Fernandez on 12/24/15.
//
//

#ifndef __Flow_Viz_v1__Tri__
#define __Flow_Viz_v1__Tri__

#include <iostream>

#endif /* defined(__Flow_Viz_v1__Tri__) */

#include "ofMain.h"
#pragma once


class Tri{
    
public:
    
    Tri();
    
    void setup(ofVec3f _v1, ofVec3f _v2, ofVec3f _v3);
    void update();
    void draw();
    void drawOutline();
    
    bool exploded;
    
    
    ofVec3f v1, v2, v3;
    ofVec3f center;
    
    ofVec3f cenTov1, cenTov2, cenTov3;
    
    ofVec3f awayFromCenter;
    
    ofVec3f t1, t2, t3;
    ofVec3f normal;
    
    ofVec3f vel, acc;
    ofVec3f rotAxis;
    float rotAmt, rotSpeed;
    
    ofMesh triangle;
    
    float trans;
    float fadeSpeed;
    
};