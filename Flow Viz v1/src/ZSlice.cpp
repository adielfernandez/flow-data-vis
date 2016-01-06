//
//  ZSlice.cpp
//  Flow Viz v1
//
//  Created by Adiel Fernandez on 12/24/15.
//
//

#include "ZSlice.h"


ZSlice::ZSlice(){
    
    
    
    
}

void ZSlice::setup(){
 
    minX = 0.0;
    maxX = 3.10887;
    
    minY = -1.0;
    maxY = 1.0;
    
    minZ = 0.0;
    maxZ = 1.53807;
    
    minU = -0.00010852;
    maxU = 0.823162;
    
    minV = -0.0997344;
    maxV = 0.122762;
    
    minW = -0.126819;
    maxW = 0.143105;
    
    arrowScale = 0.35;

    
}


void ZSlice::setupMesh(){
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    //go through vector of datapoints and add vertices to mesh 6 at a time
    //(2 triangles for each unit square)
    
    for(int y = 0; y < sliceDim.y - 1; y++){
        for(int x = 0; x < sliceDim.x - 1; x++){

            
            //convert the x and y values to the indices we need in the datapoint vector
            //4 points for two triangles
            //      0___1
            //      |  /|
            //      | / |
            //      |/__|
            //      2   3
            //
            // Tri 1: 0-1-2
            // Tri 2: 1-3-2 (declare points clockwise so mesh normal is properly oriented)
            
            int i0 = y * sliceDim.x + x;
            int i1 = y * sliceDim.x + (x + 1);
            int i2 = (y + 1) * sliceDim.x + x;
            int i3 = (y + 1) * sliceDim.x + (x + 1);
            
            
            //--------------------first triangle--------------------
            mesh.addVertex(dataList[i0].scaledPos);
            mesh.addVertex(dataList[i1].scaledPos);
            mesh.addVertex(dataList[i2].scaledPos);

            mesh.addColor(dataList[i0].fCol);
            mesh.addColor(dataList[i1].fCol);
            mesh.addColor(dataList[i2].fCol);

//            mesh.addVertex(ofVec3f(ofRandom(500), ofRandom(500), ofRandom(500)));
//            mesh.addVertex(ofVec3f(ofRandom(500), ofRandom(500), ofRandom(500)));
//            mesh.addVertex(ofVec3f(ofRandom(500), ofRandom(500), ofRandom(500)));
//            mesh.addColor(ofFloatColor(ofRandomuf(), ofRandomuf(), ofRandomuf()));
//            mesh.addColor(ofFloatColor(ofRandomuf(), ofRandomuf(), ofRandomuf()));
//            mesh.addColor(ofFloatColor(ofRandomuf(), ofRandomuf(), ofRandomuf()));
            
            
            
            //--------------------second triangle--------------------
            mesh.addVertex(dataList[i1].scaledPos);
            mesh.addVertex(dataList[i2].scaledPos);
            mesh.addVertex(dataList[i3].scaledPos);
            
//            mesh.addColor(ofFloatColor(ofRandomuf(), ofRandomuf(), ofRandomuf()));
//            mesh.addColor(ofFloatColor(ofRandomuf(), ofRandomuf(), ofRandomuf()));
//            mesh.addColor(ofFloatColor(ofRandomuf(), ofRandomuf(), ofRandomuf()));
//            mesh.addVertex(ofVec3f(ofRandom(500), ofRandom(500), ofRandom(500)));
//            mesh.addVertex(ofVec3f(ofRandom(500), ofRandom(500), ofRandom(500)));
//            mesh.addVertex(ofVec3f(ofRandom(500), ofRandom(500), ofRandom(500)));
            
            mesh.addColor(dataList[i1].fCol);
            mesh.addColor(dataList[i2].fCol);
            mesh.addColor(dataList[i3].fCol);

        }
    }
    
    

    
}

void ZSlice::setTrans(float trans){
    
    for(int i = 0; i < mesh.getNumVertices(); i+= 2){
        
        ofFloatColor c = mesh.getColor(i);
        
        mesh.setColor(i, ofFloatColor(c.r, c.g, c.b, trans));
        
    }
    
    
}

void ZSlice::update(){
    
    
    
}


void ZSlice::draw(){
    
    mesh.draw();
    
}

void ZSlice::drawPoints(){
    
    glPointSize(3.0);
    mesh.drawVertices();
    
}

void ZSlice::drawWire(){
    

    mesh.drawWireframe();
    
}

void ZSlice::drawVectors(){


    


    for(int i = 0; i < dataList.size(); i++){
        
        ofSetLineWidth(2.0);
        ofSetColor(dataList[i].col);
        ofDrawLine(dataList[i].scaledPos, (dataList[i].scaledPos + (dataList[i].inflatedZVel) * arrowScale));

        
        
    }


    
    
}


//  unused code to draw triangles at points
//float triHeight = 5;
//ofVec3f tip = (dataList[i].scaledPos + (dataList[i].scaledVel) * arrowScale);
//ofDrawTriangle(dataList[i].scaledPos.x, dataList[i].scaledPos.y + triHeight, dataList[i].scaledPos.z, dataList[i].scaledPos.x, dataList[i].scaledPos.y - triHeight, dataList[i].scaledPos.z, tip.x, tip.y, tip.z);
//
//ofDrawTriangle(dataList[i].scaledPos.x, dataList[i].scaledPos.y, dataList[i].scaledPos.z + triHeight, dataList[i].scaledPos.x, dataList[i].scaledPos.y, dataList[i].scaledPos.z - triHeight, tip.x, tip.y, tip.z);


// unused (semi-faulty) code for drawing lots of cones

////create cones for each of the data points,
////translate them, rotate them, add them to single mesh
//for(int i = 3200; i < 3300; i+=5){
//    
//    //        float coneLength = ofMap(dataList[i].vel.lengthSquared(), ofVec3f(minU, minV, minW).lengthSquared(), ofVec3f(maxU, maxV, maxW).lengthSquared(), 3, 20);
//    
//    ofConePrimitive head;
//    head.setRadius(5);
//    head.setResolution(4, 1, 0);
//    head.setHeight(15);
//    
//    float coneLength = 10;
//    
//    //figure out vectors
//    ofVec3f conePos = (dataList[i].pos + (dataList[i].vel * arrowScale)) * scaleFactor;
//    
//    //direction we want cone to point in
//    ofVec3f coneDirection = dataList[i].vel.getNormalized();
//    
//    ofVec3f tipVert = conePos + coneDirection * coneLength;
//    
//    //construct simple cone vertices, first the tip of the cone
//    arrowMesh.addVertex(tipVert);
//    arrowMesh.addColor(arrowColor);
//    
//    //then add vertices around the cone base
//    int numCapVerts = 4;
//    
//    ofVec3f thisVert;
//    
//    for(int i = 0; i < numCapVerts; i++){
//        //start by rotating the tipVert 90 (direction is arbitrary)
//        thisVert = (coneDirection * coneLength).rotate(90, ofVec3f(1, 0, 0));
//        
//        //then rotate it around the direction of the cone
//        thisVert.rotate(i * 360/numCapVerts , tipVert.getNormalized());
//        
//        //now add it to the base position of the cone
//        thisVert += conePos;
//        
//        
//        //now add it to the mesh
//        arrowMesh.addVertex(thisVert);
//        arrowMesh.addColor(ofFloatColor(0.0, 0.0, 1.0));
//    }
//    
//    //add indices to connect the points
//    //        arrowMesh.addIndex(0);
//    //        arrowMesh.addIndex(1);
//    //        arrowMesh.addIndex(2);
//    //        arrowMesh.addIndex(0);
//    //        arrowMesh.addIndex(2);
//    //        arrowMesh.addIndex(3);
//    //        arrowMesh.addIndex(0);
//    //        arrowMesh.addIndex(3);
//    //        arrowMesh.addIndex(4);
//    //        arrowMesh.addIndex(0);
//    //        arrowMesh.addIndex(4);
//    //        arrowMesh.addIndex(5);
//}
//
//
//arrowMesh.setMode(OF_PRIMITIVE_TRIANGLES);









