//
//  ThreadedSlicer.h
//  Flow Viz v2
//
//  Created by Adiel Fernandez on 12/28/15.
//
//

#ifndef __Flow_Viz_v2__ThreadedSlicer__
#define __Flow_Viz_v2__ThreadedSlicer__

#include <iostream>
#include "ofMain.h"
#include "Slice.h"
#include "DataPoint.h"

#pragma once


#endif /* defined(__Flow_Viz_v2__ThreadedSlicer__) */


class ThreadedSlicer: public ofThread{
    
public:
    ThreadedSlicer();
    ~ThreadedSlicer();

    void startSlicer(float _scaleFactor, vector<Slice> *_mainThreadSlices, int *_mainThreadSliceDirection, int _orientation);
    void update();
    
    //received from setup
    int orientation;
    vector<Slice> *mainThreadSlices;
    float scaleFactor;
    int *mainThreadSliceDirection;
    
    const ofVec3f dataDim = ofVec3f(96, 65, 48);
    double minX, maxX, minY, maxY, minZ, maxZ;
    double minU, maxU, minV, maxV, minW, maxW;
    
protected:
    ofThreadChannel<vector<Slice>> reSlice;
    void threadedFunction();
    vector<Slice> tempSlices;    
    
};