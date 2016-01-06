//
//  Tri.cpp
//  Flow Viz v1
//
//  Created by Adiel Fernandez on 12/24/15.
//
//

#include "Tri.h"

Tri::Tri(){
    
    
    
}


void Tri::setup(ofVec3f _v1, ofVec3f _v2, ofVec3f _v3){
    
    //get vertices
    v1 = _v1;
    v2 = _v2;
    v3 = _v3;
    
    //at setup, texCoords = vertices
    t1 = _v1;
    t2 = _v2;
    t3 = _v3;
    
    rotAmt = 0;
    rotSpeed = 0.9;
    
    rotAxis = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
    rotAxis.normalize();
    
    
    //need to decompose 3 vertices into a center point, the relative positions
    //of the corners wrt the center and their normal
    center = (v1 + v2 + v3)/3;
    
    cenTov1 = v1 - center;
    cenTov2 = v2 - center;
    cenTov3 = v3 - center;
    
    triangle.addVertex(center + cenTov1);
    triangle.addVertex(center + cenTov2);
    triangle.addVertex(center + cenTov3);
    
    triangle.addTexCoord(t1);
    triangle.addTexCoord(t2);
    triangle.addTexCoord(t3);
    
    triangle.addNormal(normal);
    triangle.addNormal(normal);
    triangle.addNormal(normal);
    
    
    trans = 255;
    fadeSpeed = ofRandom(0.5, 4);
    
}

void Tri::update(){
    
    if(exploded){
        
        //set acceleration
        acc.set(0, 0.05);
        
        //add bulk speed and acceleration (only if they're within view)
        if(center.y < ofGetWindowHeight() + 100){
            center += vel;
            vel += acc;
        }
        
        //rotate 3 corners according to rot axis
        cenTov1.rotate(rotSpeed, rotAxis);
        cenTov2.rotate(rotSpeed, rotAxis);
        cenTov3.rotate(rotSpeed, rotAxis);
        
        //update mesh with proper vertices
        triangle.setVertex(0, center + cenTov1);
        triangle.setVertex(1, center + cenTov2);
        triangle.setVertex(2, center + cenTov3);
        
        if(trans > 0){
            trans -= fadeSpeed;
        }
        
    }
    
    
    
    
}


void Tri::draw(){
    
    
    ofPushMatrix();
    
    
    ofSetColor(255, trans);
    triangle.draw();
    
    ofPopMatrix();
    
    
}


//not used
void Tri::drawOutline(){
    
    
    ofSetColor(255, trans);
    ofSetLineWidth(1);
    triangle.drawWireframe();
    
    
    
}








