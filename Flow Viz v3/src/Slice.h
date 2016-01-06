//
//  Slice.h
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


#pragma once

#endif /* defined(__Flow_Viz_v1__ZSlice__) */


class Slice{
    
public:
    
    Slice();
    void setupMesh(int dimensionA, int dimensionB);
    void draw();
    void drawPoints();
    void drawWire();
    void drawVectors();
    
    void setTrans(float trans);

    float oldTrans;
    
    
    vector<DataPoint> dataList;
    
    ofVboMesh mesh;
    

    
    
    float scaleFactor;
    const float arrowScale = 0.35;
    
    
};
