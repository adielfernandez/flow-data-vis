//
//  ParticleSystem.h
//  Flow Viz v3
//
//  Created by Adiel Fernandez on 1/5/16.
//
//

#ifndef __Flow_Viz_v3__ParticleSystem__
#define __Flow_Viz_v3__ParticleSystem__

#include <iostream>

#endif /* defined(__Flow_Viz_v3__ParticleSystem__) */

#include "ofMain.h"
#include "Slice.h"
#include "Particle.h"

#pragma once


class ParticleSystem{
    
public:
    
    ParticleSystem();

    void setup();
    void update();
    void draw();
    
    void loadData(vector<Slice> *_xSlices);
    void addParticles(int num);
    
    //trilinear interpolation 
    ofVec3f interpolatedVel(ofVec3f _pos);
    
    float portalDist;
    
    vector<Slice> *xSlices;
    
    ofVec3f dataDim, boundingBoxDim;
    
    float scaleFactor;

    ofVec3f distBetDataPts;
    ofVec3f cruiseVel;
    
    int numParticles;
    int maxParticles;
    vector<Particle> particles;
    
    ofVec3f leftBottom1, leftTop1, rightBottom1, rightTop1;
    ofVec3f leftBottom2, leftTop2, rightBottom2, rightTop2;
    
};




