//
//  Particle.cpp
//  Flow Viz v1
//
//  Created by Adiel Fernandez on 12/24/15.
//
//

#include "Particle.h"



Particle::Particle(){
    
    
    
    
}

void Particle::setup(){
    
    //create two triangles at 90 degree angles
    length = 35;
    width = 20;
    
    tip.set(length, 0, 0);
    
    //XY plane:
    tri1a.set(0, -width/2, 0);
    tri1b.set(0, width/2, 0);
    
    //XZ plane
    tri2a.set(0, 0, width/2);
    tri2b.set(0, 0, -width/2);

    
    col = ofColor(100, 255, 0, 255 * 0.5);
    
    dying = false;
    age = 0;
    deathTime = 0;
    
}

void Particle::record(){

    points.push_back(pos);

    ofColor thisCol;
    ofVec3f normVel = vel.getNormalized();
    thisCol.set(vel.x * 255, vel.y * 255, vel.z * 255);
    
    cols.push_back(ofColor(255, 100));
}

void Particle::setVel(ofVec3f _vel){
    vel = _vel;
}

void Particle::update(){

    pos += vel;
    
    //get angle between (1, 0, 0) and the velocity
    ofVec3f xNorm(1, 0, 0);
    
    rot = xNorm.angle(vel);
    
    rotAxis = xNorm.getCrossed(vel);
    
    age++;
    
    if(dying){
        deathTime++;
        col = col.getLerped(ofColor::red, 0.05);

        //slow down but preserve heading
        vel *= 0.95;
        
    }
    
    
}

void Particle::drawTrails(){
    
    if(points.size() > 1){
        
        for(int i = 0; i < points.size() - 1; i++){
            ofSetColor(cols[i]);
            ofDrawLine(points[i], points[i + 1]);
        }
    }
    
}


void Particle::draw(){
    
    
    ofSetColor(col);
    ofPushMatrix();
    
        ofTranslate(pos.x - length, pos.y, pos.z);
        ofRotate(rot, rotAxis.x, rotAxis.y, rotAxis.z);
    
            ofDrawTriangle(tip, tri1a, tri1b);
            ofDrawTriangle(tip, tri2a, tri2b);
    
    ofPopMatrix();
    

}






