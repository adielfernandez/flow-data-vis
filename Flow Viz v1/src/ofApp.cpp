#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //jot down the time to see how long setup takes
    cout << "Start Time: " << ofGetElapsedTimeMillis() << endl;
    
    
    ofSetFrameRate(300);
    ofSetVerticalSync(false);
    
    ofEnableDepthTest();
    
    //number of data points in each direction
    dataDim.set(96, 65, 48);
    
    scaleFactor = 400.0;
    
    //    Data Ranges:
    //    x:  0.0 to 3.10887
    //    y: -1.0 to 1.0
    //    z:  0.0 to 1.53807
    //    u: -0.00010852 to 0.823162
    //    v: -0.0997344  to 0.122762
    //    w: -0.126819   to 0.143105
    
    //maximum values for each dimension
    //(found algorithmically from previous data vis)
    minX = 0.0;
    maxX = 3.10887;
    
    //Y is -1 to 1 in data set, but we'll shift over
    //by 1 to keep all data in first quadrant
    minY = 0.0;
    maxY = 2.0;
    
    minZ = 0.0;
    maxZ = 1.53807;
    
    minU = -0.00010852;
    maxU = 0.823162;
    
    minV = -0.0997344;
    maxV = 0.122762;
    
    minW = -0.126819;
    maxW = 0.143105;
    
    viewingSlice = 0;
    lastViewingSlice = 0;
    
    boundingBoxDim.set(maxX, maxY, maxZ);
    boundingBoxDim *= scaleFactor;
    
    
    //Path to the comma delimited file
	string filePath = "uvw.txt";
    
	
	//Load file placed in bin/data
	ofFile file(filePath);
	
	if(!file.exists()){
		ofLogError("The file " + filePath + " is missing");
	}
    ofBuffer buffer(file);
    
    int currentLine = 0;
    
	//Read file line by line
	for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        
        
        string line = *it;
        //Split line into strings
		vector<string> words = ofSplitString(line, " ");
        
        //PARSE STRINGS TO DOUBLES
        //X
        istringstream ssX(words[0]);
        double x;
        ssX >> x;
        
        //Y
        istringstream ssY(words[1]);
        double y;
        ssY >> y;
        y = y + 1.0;
        
        //Z
        istringstream ssZ(words[2]);
        double z;
        ssZ >> z;
        
        //U
        istringstream ssU(words[3]);
        double u;
        ssU >> u;
        
        //V
        istringstream ssV(words[4]);
        double v;
        ssV >> v;
        
        //W
        istringstream ssW(words[5]);
        double w;
        ssW >> w;

        //now that values are parsed, put them in a data point
        DataPoint d;
        d.scaleFactor = scaleFactor;
        d.setup(x, y, z, u, v, w);  //assigns values and sets pos and vel
        
        d.fCol.set(ofMap(d.u, minU, maxU, 0.0, 1.0), ofMap(d.v, minV, maxV, 0.0, 1.0), ofMap(d.w, minW, maxW, 0.0, 1.0));
        d.col.set(d.fCol.r * 255, d.fCol.g * 255, d.fCol.b * 255);
//        d.fCol.set(1.0, 0.8, 0.0);
        
        //then put that data point in the vector for the appropriate slice
        //but first figure out what slice we're in
        int linesPerSlice = dataDim.x * dataDim.y;  //6240

        int thisSliceIndex = floor(currentLine/linesPerSlice);
        
        //then create a ZSlice object when we get to a new slice
        //the instant we get to the first line of a new slice
        if(currentLine % linesPerSlice == 0){
            
            ZSlice thisSlice;
            thisSlice.setup();
            thisSlice.sliceDim.set(dataDim.x, dataDim.y);
            thisSlice.zValue = d.z;
            thisSlice.scaleFactor = scaleFactor;
            slices.push_back(thisSlice);
            
        }
        
        //add DataPoint to the vector of the current slice
        slices[thisSliceIndex].dataList.push_back(d);
        
        
        currentLine++;
        
	}
    

    
    
    //once all the ZSlices have been filled with their data points,
    //we can continue and do other things with them like setting up their meshes
    for(int i = 0; i < slices.size(); i++){
        
        slices[i].setupMesh();
        slices[i].setTrans(1.0);
    }
    
    
    
    viewingSlice = 0;
    nonViewingTrans = 0.03;
    drawMode = 0;
    
    //set transparencies of data set to
    for(int i = 0; i < slices.size(); i++){
        slices[i].setTrans(1.0);
    }
    
    
    
    camera.setDistance(1500);
    camera.setTranslationKey(' ');
    
    //set up the heads of the axis cones
    coneX.setRadius(15);
    coneX.setHeight(45);
    coneX.setResolution(8, 1, 0);
    coneX.setTopColor(ofColor(255, 0, 0));
    
    coneY.setRadius(15);
    coneY.setHeight(45);
    coneY.setResolution(8, 1, 0);
    coneY.setTopColor(ofColor(0, 255, 0));
    
    coneZ.setRadius(15);
    coneZ.setHeight(45);
    coneZ.setResolution(8, 1, 0);
    coneZ.setTopColor(ofColor(0, 0, 255));
    

    
    //jot down the time to see how long setup takes
    cout << "End Time: " << ofGetElapsedTimeMillis() << endl;

}

//--------------------------------------------------------------
void ofApp::update(){

    viewingSlice = (int)round(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, dataDim.z - 1, true));


    
    lastViewingSlice = viewingSlice;
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackgroundGradient(ofColor(75), ofColor(0));
    
    
    camera.begin();
    
    ofPushMatrix();
    ofTranslate(-ofVec3f(maxX, maxY, maxZ)/2 * scaleFactor);
    
    //0 = draw bulk
    //1 = data points
    //2 = interpolated mesh
    //3 = interpolated image
    //4 = vectors
    
    if(drawMode == 0){
        
        
        for(int i = 0; i < slices.size(); i++){
            slices[i].draw();
        }
        
    } else if(drawMode == 1) {
        
        slices[viewingSlice].drawPoints();
        
    } else if(drawMode == 2) {
        
        slices[viewingSlice].drawWire();
        
    } else if(drawMode == 3) {
        
        slices[viewingSlice].draw();
        
    } else if(drawMode == 4) {
        
        slices[viewingSlice].drawVectors();
        
    }
    


    drawAxes();
    drawBoundingBox();
    
    ofPopMatrix();


    
    camera.end();
    
    
    ofSetColor(255);
    ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), 15, 15);
    

    
}

//--------------------------------------------------------------
void ofApp::drawAxes(){
    
    
    ofVec3f axisLengths = boundingBoxDim/2;
//    ofVec3f axisLengths = ofVec3f(100, 100, 100);

    
    ofSetLineWidth(2);
    
    //X-Axis (Red)
    ofSetColor(255, 0, 0);
    ofDrawLine(0, 0, 0, axisLengths.x, 0, 0);
    
    
    ofPushMatrix();
    ofTranslate(axisLengths.x, 0);
    ofRotate(90, 0, 0, 1);
    
    coneX.draw();
    
    ofPopMatrix();
    
    //Y-Axis (Green)
    ofSetColor(0, 255, 0);
    ofDrawLine(0, 0, 0, 0, axisLengths.y, 0);
    
    
    ofPushMatrix();
    ofTranslate(0, axisLengths.y);
    ofRotate(180, 0, 0, 1);
    
    coneY.draw();
    
    ofPopMatrix();
    
    //Z-Axis (Blue)
    ofSetColor(0, 0, 255);
    ofDrawLine(0, 0, 0, 0, 0, axisLengths.z);
    
    
    ofPushMatrix();
    ofTranslate(0, 0, axisLengths.z);
    ofRotate(-90, 1, 0, 0);
    
    coneZ.draw();
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::drawBoundingBox(){

    ofSetLineWidth(1);
    ofSetColor(255, 255 * 0.5);
    
    //XY Plane
    ofDrawLine(ofVec3f(0, 0, 0), ofVec3f(boundingBoxDim.x, 0, 0));
    ofDrawLine(ofVec3f(0, boundingBoxDim.y, 0), ofVec3f(boundingBoxDim.x, boundingBoxDim.y, 0));
    ofDrawLine(ofVec3f(0, 0, 0), ofVec3f(0, boundingBoxDim.y, 0));
    ofDrawLine(ofVec3f(boundingBoxDim.x, 0, 0), ofVec3f(boundingBoxDim.x, boundingBoxDim.y, 0));

    //XY +Z Plane
    ofDrawLine(ofVec3f(0, 0, boundingBoxDim.z), ofVec3f(boundingBoxDim.x, 0, boundingBoxDim.z));
    ofDrawLine(ofVec3f(0, boundingBoxDim.y, boundingBoxDim.z), ofVec3f(boundingBoxDim.x, boundingBoxDim.y, boundingBoxDim.z));
    ofDrawLine(ofVec3f(0, 0, boundingBoxDim.z), ofVec3f(0, boundingBoxDim.y, boundingBoxDim.z));
    ofDrawLine(ofVec3f(boundingBoxDim.x, 0, boundingBoxDim.z), ofVec3f(boundingBoxDim.x, boundingBoxDim.y, boundingBoxDim.z));
    
    //bridge above two rectangles
    ofDrawLine(ofVec3f(0, 0, 0), ofVec3f(0, 0, boundingBoxDim.z));
    ofDrawLine(ofVec3f(boundingBoxDim.x, 0, 0), ofVec3f(boundingBoxDim.x, 0, boundingBoxDim.z));
    ofDrawLine(ofVec3f(0, boundingBoxDim.y, 0), ofVec3f(0, boundingBoxDim.y, boundingBoxDim.z));
    ofDrawLine(ofVec3f(boundingBoxDim.x, boundingBoxDim.y, 0), ofVec3f(boundingBoxDim.x, boundingBoxDim.y, boundingBoxDim.z));
    
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == '0'){
        for(int i = 0; i < slices.size(); i++){
            slices[i].setTrans(nonViewingTrans);
        }
        drawMode = 0;
    }
    
    if(key != '0' && drawMode == 0){
        for(int i = 0; i < slices.size(); i++){
            slices[i].setTrans(1.0);
        }
        
        cout << "reset transparencies" << endl;
    }
    
    if(key == '1'){
        drawMode = 1;
    } else if(key == '2'){
        drawMode = 2;
    } else if(key == '3'){
        drawMode = 3;
    } else if(key == '4'){
        drawMode = 4;
    }
    
    if(key == 'r' || key == 'R'){
        camera.reset();
    }
    

    cout << "draw mode: " << drawMode << "  Key: " << key << endl;
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
