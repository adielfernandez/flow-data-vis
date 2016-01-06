#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //jot down the time to see how long setup takes
    cout << "Start Time: " << ofGetElapsedTimeMillis() << endl;
    
    ofLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(300);
    ofSetVerticalSync(false);
    
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    
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
    
        
    viewingSlice = 0;

    
    boundingBoxDim.set(maxX, maxY, maxZ);
    boundingBoxDim *= scaleFactor;
    

    
    //----------SLICING----------
    Slicer.startSlicer(scaleFactor, &slices, &sliceDirection, 2);
        
    //0 = X slices, 1 = Y slices, 2 = Z slices
    sliceDirection = 2;
    
    viewingSlice = 0;
    nonViewingTrans = 1.0;
    drawMode = 0;
    
    //set transparencies of data set to
//    for(int i = 0; i < slices.size(); i++){
//        slices[i].setTrans(nonViewingTrans);
//    }
    
    //set up font
    axesFont.load("geosans.ttf", 40);
    textFont.load("geosans.ttf", 30);

    titleImg.load("titleDescription cropped.png");
    velocityKey.load("velocityKey.png");
    velocityKey.setAnchorPercent(1.0, 0.0);
    
    camGuide.load("camGuide.png");
    camGuide.setAnchorPercent(0.0, 1.0);
    
    camera.setDistance(2000);
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
    
    ofDisableArbTex();
    

}

//--------------------------------------------------------------
void ofApp::update(){

    //0 = Z slices, 1 = X slices, 2 = Y slices
    if(sliceDirection == 0){
        viewingSlice = (int)round(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, dataDim.x - 1, true));
    } else if(sliceDirection == 1){
        viewingSlice = (int)round(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, dataDim.y - 1, true));
    } else if(sliceDirection == 2){
        viewingSlice = (int)round(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, dataDim.z - 1, true));
    }
    
    
    //reset the transparencies (method only does anything if the transparency is different)
    if(drawMode == 0){
        for(int i = 0; i < slices.size(); i++){
            slices[i].setTrans(nonViewingTrans);
        }
    } else {
        for(int i = 0; i < slices.size(); i++){
            slices[i].setTrans(1.0);
        }
    }
    
    
    if(Slicer.isThreadRunning()){
//    if(true){
        Slicer.update();
        
        float noiseScale = 0.18;
        float noiseSpeed = 1.25;
        for(int i = 0; i < numCircles; i++){
            
            circleHeights[i] = ofSignedNoise(ofGetElapsedTimef() * noiseSpeed + (noiseScale * i));
            
        }
        
    }
    
    
    
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

    drawUI();

    
    
//    ofSetColor(255);
//    ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), 600, 15);
    

    
}


//--------------------------------------------------------------
void ofApp::drawUI(){

    float imgScale = 0.80;
    
    ofSetColor(255);
    titleImg.draw(20, 10, titleImg.getWidth() * imgScale, titleImg.getHeight() * imgScale);

    float velKeyScale = 0.70;
    velocityKey.draw(ofGetWidth() - 20, 10, velocityKey.getWidth() * velKeyScale, velocityKey.getHeight() * velKeyScale);
    
    float camGuideScale = 0.60  ;
    ofSetColor(255);
    camGuide.draw(25, ofGetHeight() - 20, camGuide.getWidth() * camGuideScale, camGuide.getHeight() * camGuideScale);
    
    
    if(Slicer.isThreadRunning()){
//    if(true){
        string load = "Slicing Data...";
        
        float boxPosY = 400;
        float boxMargin = 20;
        float width = boxMargin * 2 + textFont.stringWidth(load);
        
        float textPosY = boxPosY + boxMargin + textFont.stringHeight(load);
        
        float circleAmp = 50;
        float circleSpacing = 180/numCircles;
        float circlePosY = textPosY + circleAmp + 10;

        float height = boxMargin * 2 + textFont.stringHeight(load) + circleAmp * 2 - 5;
        
        ofSetColor(255, 80);
        ofDrawRectangle(ofGetWidth()/2 - width/2, boxPosY, -10, width, height);
        
        ofPushStyle();
        ofSetColor(255, 255);
        ofSetLineWidth(3);
        ofNoFill();
        ofDrawRectangle(ofGetWidth()/2 - width/2, boxPosY, -10, width, height);
        ofPopStyle();
        
        ofSetColor(255, 255);
        textFont.drawString(load, ofGetWidth()/2 - textFont.stringWidth(load)/2, textPosY);
        
        
        for(int i = 0; i < numCircles; i++){
            
            ofSetColor(255, 255);
            ofDrawCircle((ofGetWidth()/2 - (numCircles/2 * circleSpacing)) + i * circleSpacing, circlePosY + circleHeights[i] * circleAmp, 2);
            
            
        }
        
        
    }

    
    
    
}


//--------------------------------------------------------------
void ofApp::drawAxes(){
    
    
    ofVec3f axisLengths = boundingBoxDim;
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
    ofSetColor(0, 128, 255);
    ofDrawLine(0, 0, 0, 0, 0, axisLengths.z);
    
    
    ofPushMatrix();
    ofTranslate(0, 0, axisLengths.z);
    ofRotate(-90, 1, 0, 0);
    
    coneZ.draw();
    
    ofPopMatrix();
    
    
    //X Axis label
    ofVec3f xLabelPos = ofVec3f(axisLengths.x + 10, -60, 0);
    float xLabelAng = getBillboardAngle(xLabelPos);
    ofVec3f xLabelVec = getBillboardVec(xLabelPos);
    
    ofSetColor(255, 0, 0);
    
    ofPushMatrix();
        ofTranslate(xLabelPos);
        ofRotate(xLabelAng, xLabelVec.x, xLabelVec.y, xLabelVec.z);
        ofRotate(180, 0, 1, 0);
        ofSetColor(255, 0, 0);
        axesFont.drawString("X max: ", 0, 0);
        ofSetColor(255);
        axesFont.drawString(" 3.109", axesFont.stringWidth("X max: "), 0);
    ofPopMatrix();
    
    //Y Axis label
    ofVec3f yLabelPos = ofVec3f(-50, axisLengths.y + 10, 0);
    float yLabelAng = getBillboardAngle(yLabelPos);
    ofVec3f yLabelVec = getBillboardVec(yLabelPos);
    

    
    ofPushMatrix();
        ofTranslate(yLabelPos);
        ofRotate(yLabelAng, yLabelVec.x, yLabelVec.y, yLabelVec.z);
        ofRotate(180, 0, 1, 0);
        ofSetColor(0, 255, 0);
        axesFont.drawString("Y max:", 0, 0);
        ofSetColor(255);
        axesFont.drawString(" 2.000", axesFont.stringWidth("Y max: "), 0);
    ofPopMatrix();
    
    //Z Axis label
    ofVec3f zLabelPos = ofVec3f(-50, -40, axisLengths.z + 10);
    float zLabelAng = getBillboardAngle(zLabelPos);
    ofVec3f zLabelVec = getBillboardVec(zLabelPos);
    

    
    ofPushMatrix();
        ofTranslate(zLabelPos);
        ofRotate(zLabelAng, zLabelVec.x, zLabelVec.y, zLabelVec.z);
        ofRotate(180, 0, 1, 0);
        ofSetColor(0, 128, 255);
        axesFont.drawString("Z max: ", 0, 0);
        ofSetColor(255);
        axesFont.drawString(" 1.538", axesFont.stringWidth("Z max: "), 0);
    ofPopMatrix();
    

    
}

//--------------------------------------------------------------
float ofApp::getBillboardAngle(ofVec3f globalPos){


    ofNode posNode;
    posNode.setGlobalPosition(globalPos);
    posNode.lookAt(camera.getGlobalPosition(), camera.getUpDir());
    ofQuaternion posQuat = posNode.getGlobalOrientation();
    
    float ang = 0;
    ofPoint vec;
    posQuat.getRotate(ang, vec);

    return ang;
    
}

//--------------------------------------------------------------
ofVec3f ofApp::getBillboardVec(ofVec3f globalPos){
    
    ofNode posNode;
    posNode.setGlobalPosition(globalPos);
    posNode.lookAt(camera.getGlobalPosition(), camera.getUpDir());
    ofQuaternion posQuat = posNode.getGlobalOrientation();
    
    float ang = 0;
    ofPoint vec;
    posQuat.getRotate(ang, vec);
    
    return vec;
    
    
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
//        for(int i = 0; i < slices.size(); i++){
//            slices[i].setTrans(0.05);
//        }
        drawMode = 0;
    }
    
    
    if(key == '1'){
//        for(int i = 0; i < slices.size(); i++){
//            slices[i].setTrans(1.0);
//        }
        drawMode = 1;
    } else if(key == '2'){
//        for(int i = 0; i < slices.size(); i++){
//            slices[i].setTrans(1.0);
//        }
        drawMode = 2;
    } else if(key == '3'){
//        for(int i = 0; i < slices.size(); i++){
//            slices[i].setTrans(1.0);
//        }
        drawMode = 3;
    } else if(key == '4'){
//        for(int i = 0; i < slices.size(); i++){
//            slices[i].setTrans(1.0);
//        }
        drawMode = 4;
    }
    
    if(key == 'r' || key == 'R'){
        camera.reset();
    }
    
    if(key == 'x' || key == 'X'){

        //make sure slicer isn't already running
        if(!Slicer.isThreadRunning()){
            Slicer.startSlicer(scaleFactor, &slices, &sliceDirection, 0);
        } else {
            cout << "wait until current slicing is done" << endl;
        }
        
    }
    
    if(key == 'y' || key == 'Y'){
        
        //make sure slicer isn't already running
        if(!Slicer.isThreadRunning()){
            Slicer.startSlicer(scaleFactor, &slices, &sliceDirection, 1);
        } else {
            cout << "wait until current slicing is done" << endl;
        }
        
    }
    
    if(key == 'z' || key == 'Z'){
        
        //make sure slicer isn't already running
        if(!Slicer.isThreadRunning()){
            Slicer.startSlicer(scaleFactor, &slices, &sliceDirection, 2);
        } else {
            cout << "wait until current slicing is done" << endl;
        }
        
    }
 
    
    
    if(key == 'l' || key == 'L'){
        lighting = !lighting;
    }
    
    
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
