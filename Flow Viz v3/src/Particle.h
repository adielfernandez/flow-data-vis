//
//  Particle.h
//  Flow Viz v1
//
//  Created by Adiel Fernandez on 12/24/15.
//
//

#ifndef __Flow_Viz_v1__Particle__
#define __Flow_Viz_v1__Particle__

#include <iostream>
#include "ofMain.h"

#pragma once


#endif /* defined(__Flow_Viz_v1__Particle__) */


class Particle{
    
public:
  
    Particle();
    void setup();
    void update();
    void setVel(ofVec3f _vel);
    void draw();
    void record();
    void drawTrails();
    
    ofVec3f pos, vel;
    
    ofVec3f tip, tri1a, tri1b, tri2a, tri2b;
    
    
    float length, width;
    float rot;
    ofVec3f rotAxis;
    
    ofColor col;
    bool dying;
    int age, deathTime;
    
    ofVboMesh mesh;
    
    vector<ofVec3f> points;
    vector<ofColor> cols;
    
};