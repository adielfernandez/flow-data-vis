//
//  Slice.cpp
//  Flow Viz v1
//
//  Created by Adiel Fernandez on 12/24/15.
//
//

#include "Slice.h"


Slice::Slice(){
    
    
    
    
}

void Slice::setupMesh(int dimensionA, int dimensionB){
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    //go through vector of datapoints and add vertices to mesh 6 at a time
    //(2 triangles for each unit square)
    
    
    
    for(int b = 0; b < dimensionB - 1; b++){        //Y
        for(int a = 0; a < dimensionA - 1; a++){    //X

            
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
            
            int i0 = b * dimensionA + a;
            int i1 = b * dimensionA + (a + 1);
            int i2 = (b + 1) * dimensionA + a;
            int i3 = (b + 1) * dimensionA + (a + 1);
            
            
            //--------------------first triangle--------------------
            mesh.addVertex(dataList[i0].scaledPos);
            mesh.addVertex(dataList[i1].scaledPos);
            mesh.addVertex(dataList[i2].scaledPos);

            mesh.addColor(dataList[i0].fCol);
            mesh.addColor(dataList[i1].fCol);
            mesh.addColor(dataList[i2].fCol);
            
            //--------------------second triangle--------------------
            mesh.addVertex(dataList[i1].scaledPos);
            mesh.addVertex(dataList[i2].scaledPos);
            mesh.addVertex(dataList[i3].scaledPos);

            mesh.addColor(dataList[i1].fCol);
            mesh.addColor(dataList[i2].fCol);
            mesh.addColor(dataList[i3].fCol);

        }
    }
    
    

    
}

void Slice::setTrans(float trans){
    
    //only reset transparencies if they're different from what they used to be
    if(trans != oldTrans){
    
        for(int i = 0; i < mesh.getNumVertices(); i++){
            ofFloatColor c = mesh.getColor(i);
            mesh.setColor(i, ofFloatColor(c.r, c.g, c.b, trans));
        }
    
    }
    
    oldTrans = trans;
    
}


void Slice::draw(){

    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
//    glPointSize(5.0);
//    mesh.setMode(OF_PRIMITIVE_POINTS);
    mesh.draw();
    
}

void Slice::drawPoints(){

    glPointSize(3.0);
    mesh.setMode(OF_PRIMITIVE_POINTS);
    mesh.draw();
    
}

void Slice::drawWire(){
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    mesh.drawWireframe();
}

void Slice::drawVectors(){

    for(int i = 0; i < dataList.size(); i++){
        
        ofSetLineWidth(2.0);
        ofSetColor(dataList[i].col);
        ofDrawLine(dataList[i].scaledPos, (dataList[i].scaledPos + (dataList[i].inflatedZVel) * arrowScale));
        
    }
    
}








