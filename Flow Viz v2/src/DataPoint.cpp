//
//  DataPoint.cpp
//  Flow Viz v1
//
//  Created by Adiel Fernandez on 12/24/15.
//
//

#include "DataPoint.h"


DataPoint::DataPoint(){
    
    
}

void DataPoint::setup(double _x, double _y, double _z, double _u, double _v, double _w){
    
    x = _x;
    y = _y;
    z = _z;
    u = _u;
    v = _v;
    w = _w;
    
    pos.set(x, y, z);
    vel.set(u, v, w);
    
    scaledPos = pos * scaleFactor;
    scaledVel = vel * scaleFactor;
    inflatedZVel.set(scaledVel.x, scaledVel.y, scaledVel.z * 5);

    
}