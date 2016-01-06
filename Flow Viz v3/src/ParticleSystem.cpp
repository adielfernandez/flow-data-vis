//
//  ParticleSystem.cpp
//  Flow Viz v3
//
//  Created by Adiel Fernandez on 1/5/16.
//
//

#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(){
    
    
}

void ParticleSystem::setup(){
    
    portalDist = scaleFactor * 1.5;
    distBetDataPts = boundingBoxDim/dataDim;
    
    maxParticles = 200;
    numParticles = 0;
    
    particles.reserve(maxParticles);
    
    cruiseVel.set(10, 0, 0);
        
}

void ParticleSystem::loadData(vector<Slice> *_xSlices){
    
    xSlices = _xSlices;
    
}

void ParticleSystem::addParticles(int num){
    
    for(int i = 0; i < num; i++){
        
        if(particles.size() < maxParticles){
            Particle p;
            p.pos.set(-portalDist, ofRandom(boundingBoxDim.y), ofRandom(boundingBoxDim.z));
            p.setup();
            
//            particles[numParticles] = p;
//            numParticles++;
            particles.push_back(p);
            
        }
    }
    
}

void ParticleSystem::update(){

    for(int i = 0; i < particles.size(); i++){
        
        if(particles[i].deathTime > 60 || (particles[i].age > 300 && particles[i].pos.x < 200)){
            particles.erase(particles.begin() + i);
        }
    }

    //Then go through them again to make changes. This avoids
    //doing any updates to particles that will be deleted anyways
    for(int i = 0; i < particles.size(); i++){
        
        
        //do different things depending on where we are
        if(particles[i].pos.x <= 0){
            
            //if we're between the portal and the data just cruise
            particles[i].setVel(cruiseVel);
            
        } else if(particles[i].pos.x > 0 && particles[i].pos.x < boundingBoxDim.x - distBetDataPts.x && particles[i].pos.y > 0 && particles[i].pos.y < boundingBoxDim.y && particles[i].pos.z > 0 && particles[i].pos.z < boundingBoxDim.z){
        
            particles[i].record();
            
            //if we're in the data box, set the velocity to the trilinear
            //interpolation according to the position
            particles[i].setVel(interpolatedVel(particles[i].pos));
            
            
            
        } else {
            
            particles[i].dying = true;
            
        }
        
        
        
        particles[i].update();
     
        
        
    }
    
}

void ParticleSystem::draw(){
    
    //draw entrance "portal"
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(-portalDist, 0);
    ofRotate(-90, 0, 1, 0);
    ofSetColor(0, 255, 0, 40);
    ofDrawRectangle(0, 0, boundingBoxDim.z, boundingBoxDim.y);
    ofNoFill();
    ofSetColor(0, 255, 0, 128);
    ofDrawRectangle(0, 0, boundingBoxDim.z, boundingBoxDim.y);
    
    ofPopMatrix();
    ofPopStyle();
    
    
    for(int i = 0; i < particles.size(); i++){

        particles[i].drawTrails();
        particles[i].draw();
     
    }
    



}




ofVec3f ParticleSystem::interpolatedVel(ofVec3f _pos){
    
    //Find the velocity at a point in space using a tri linear interpolation
    //of the velocities from the sliced data
    //https://en.wikipedia.org/wiki/Trilinear_interpolation
    
    //first find out which two X slices it's between
    int sliceBehind, sliceAhead;
    
    sliceBehind = floor(_pos.x/distBetDataPts.x);
    sliceAhead = sliceBehind + 1;
    
    
    cout << "Slice Behind: " << sliceBehind << " Ahead: " << sliceAhead << " xPos: " << _pos.x << " Vel: ";
    
    
    //then find out which data 4 points (column/row) within
    //the Slice the point is between
    int yBelow, yAbove, zLeft, zRight;
    
    yBelow = floor(_pos.y/distBetDataPts.y);
    yAbove = yBelow + 1;
    
    zLeft = floor(_pos.z/distBetDataPts.z);
    zRight = zLeft + 1;
    
    //now convert these row and column numbers to indices in the slice vector
    //X SLICE DATA FORMAT: points are listed out into columns of Y
    //before moving to next column in Z
    int iLowerLeft, iLowerRight, iUpperLeft, iUpperRight;
    
    iLowerLeft = dataDim.y * zLeft + yBelow;
    iUpperLeft = iLowerLeft + 1;
    
    iLowerRight = dataDim.y * zRight + yBelow;
    iUpperRight = iLowerRight + 1;
    
    
    //now that we have the indices, get the velocities at
    //those indices and start interpolating
    
    //but first, find out what percentage of X, Y and Z the particle is
    //within the unit cell
    float pctX, pctY, pctZ;
    
    pctX = fmod(_pos.x, distBetDataPts.x);
    pctY = fmod(_pos.y, distBetDataPts.y);
    pctZ = fmod(_pos.z, distBetDataPts.z);

    //----------FOR BEHIND SLICE----------
    
    //get an iterator to the slice behind
    vector<Slice>::iterator backSlice = xSlices -> begin() + sliceBehind;
    
    float scaleUp = 5.0;
    
    //now get the necessary vectors

    leftBottom1 = (*backSlice).dataList[iLowerLeft].vel * scaleUp;
    leftTop1 = (*backSlice).dataList[iUpperLeft].vel * scaleUp;
    
    rightBottom1 = (*backSlice).dataList[iLowerRight].vel * scaleUp;
    rightTop1 = (*backSlice).dataList[iUpperRight].vel * scaleUp;
    
    //interpolate the Y vals on either side, then those two into the center
    ofVec3f left, right, behindCenter, frontCenter;
    
    left = leftBottom1.getInterpolated(leftTop1, pctY);
    right = rightBottom1.getInterpolated(rightTop1, pctY);

    behindCenter = left.getInterpolated(right, pctZ);
    
    //----------FOR AHEAD SLICE----------
    
    //get an iterator to the slice behind
    vector<Slice>::iterator aheadSlice = xSlices -> begin() + sliceAhead;
    
    
    //now get the necessary vectors
    leftBottom2 = (*aheadSlice).dataList[iLowerLeft].vel * scaleUp;
    leftTop2 = (*aheadSlice).dataList[iUpperLeft].vel * scaleUp;
    
    rightBottom2 = (*aheadSlice).dataList[iLowerRight].vel * scaleUp;
    rightTop2 = (*aheadSlice).dataList[iUpperRight].vel * scaleUp;
    
    //interpolate the Y vals on either side, then those two into the center
    left = leftBottom2.getInterpolated(leftTop2, pctY);
    right = rightBottom2.getInterpolated(rightTop2, pctY);
    
    frontCenter = left.getInterpolated(right, pctZ);

    
    //----------AND FINALLY----------
    //Interpolate the frontCenter and behindCenter velocities
    ofVec3f centerVel;
    centerVel = behindCenter.getInterpolated(frontCenter, pctX);
    
//    cout << "LL: " << iLowerLeft << " UL: " << iUpperLeft << " UR: " << iUpperRight << " LR: " << iLowerRight << endl;
//    cout << "LB: " << leftBottom1 << " LT: " << leftTop1 << " RT: " << rightTop1 << " RB: " << rightBottom1 << endl;
    
    cout << centerVel << endl;
    

    
    return centerVel;

}





