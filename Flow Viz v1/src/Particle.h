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
    void draw();
    
    ofVec3f pos, vel, acc;
    
    
    
    
};