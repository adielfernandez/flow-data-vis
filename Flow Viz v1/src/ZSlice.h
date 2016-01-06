//
//  ZSlice.h
//  Flow Viz v1
//
//  Created by Adiel Fernandez on 12/24/15.
//
//

#ifndef __Flow_Viz_v1__ZSlice__
#define __Flow_Viz_v1__ZSlice__

#include <iostream>
#include "ofMain.h"
#include "DataPoint.h"
#include "Tri.h"

#pragma once

#endif /* defined(__Flow_Viz_v1__ZSlice__) */


class ZSlice{
    
public:
    
    ZSlice();
    void setup();
    void setupMesh();
    void update();
    void draw();
    void drawPoints();
    void drawWire();
    void drawVectors();
    
    void setTrans(float trans);
    
    ofVec2f sliceDim;
    
    vector<DataPoint> dataList;
    
    double minX, maxX, minY, maxY, minZ, maxZ;
    double minU, maxU, minV, maxV, minW, maxW;
    
    double zValue;
    
    ofVboMesh mesh;
    vector<Tri> triangles;
    
    
    float scaleFactor;
    
    float arrowScale;
    
    
};
