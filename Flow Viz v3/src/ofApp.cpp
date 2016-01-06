#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
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
    
    //Flags to tell us if data is loaded or not
    xLoaded = false;
    yLoaded = false;
    zLoaded = false;
    allLoaded = false;

    
    //----------SLICING----------
    Slicer.startSlicer(scaleFactor, &slicesZ, 2, &zLoaded);
    loadString = "Z-Axis";
    loadColor = ofColor::blue;
    
    
    //0 = X slices, 1 = Y slices, 2 = Z slices
    sliceDirection = 2;
    
    viewingSlice = 0;
    nonViewingTrans = 1.0;
    drawMode = 3;
    
    //set up font
    axesFont.load("geosans.ttf", 40);
    textFont.load("geosans.ttf", 30);

    titleImg.load("titleDescription cropped.png");
    velocityKey.load("velocityKey.png");
    velocityKey.setAnchorPercent(1.0, 0.0);
    
    camGuide.load("camGuide.png");
    camGuide.setAnchorPercent(0.0, 1.0);
    
    flowDirection.load("flowDirection.png");

    
    
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
    

    //particle stuff
    particlesOn = false;
    particleSys.dataDim = dataDim;
    particleSys.boundingBoxDim = boundingBoxDim;
    particleSys.scaleFactor = scaleFactor;
    particleSys.setup();
    

    //Camera settings
    //    camera.setDistance(2000);
    float angleAboveHoriz = -30;
    float dist = 2000;
    
    globalCamPos = ofVec3f(0, 0, dist);
    globalCamPos.rotate(angleAboveHoriz, ofVec3f(1, 0, 0));
    
    camera.setGlobalPosition(globalCamPos);
    camera.lookAt(ofVec3f(0));
    
    camera.setTranslationKey(' ');

    setupCamera = false;
    
}

//--------------------------------------------------------------
void ofApp::update(){

    //Cheap camera Hack: ofEasyCam resets itself after the 2nd frame to
    //whatever default distance it wants so we'll set it back after that bug
    if(ofGetFrameNum() == 2 && !setupCamera){

        camera.setGlobalPosition(globalCamPos);
        camera.lookAt(ofVec3f(0));
        setupCamera = true;

    }
    
    //0 = Z slices, 1 = X slices, 2 = Y slices
    if(sliceDirection == 0){
        viewingSlice = (int)round(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, dataDim.x - 1, true));
    } else if(sliceDirection == 1){
        viewingSlice = (int)round(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, dataDim.y - 1, true));
    } else if(sliceDirection == 2){
        viewingSlice = (int)round(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, dataDim.z - 1, true));
    }
    
    
    
    if(Slicer.isThreadRunning()){

        Slicer.update();
        
        float noiseScale = 0.18;
        float noiseSpeed = 1.25;
        for(int i = 0; i < numCircles; i++){
            
            circleHeights[i] = ofSignedNoise(ofGetElapsedTimef() * noiseSpeed + (noiseScale * i));
            
        }
        
        loadingTime = ofGetElapsedTimeMillis();
        
    }
    

    //if slicer isn't running, check to see if we need to load the other directions
    if(!allLoaded && !Slicer.isThreadRunning()){
        
        //BUT give it a little extra time to make sure the thread has fully wrapped up the last slice event
        if(!xLoaded && ofGetElapsedTimeMillis() - loadingTime > 100){
            loadString = "X-Axis";
            loadColor = ofColor::red;
            Slicer.startSlicer(scaleFactor, &slicesX, 0, &xLoaded);
            
        } else if(!yLoaded && ofGetElapsedTimeMillis() - loadingTime > 100){
            loadString = "Y-Axis";
            loadColor = ofColor::green;
            Slicer.startSlicer(scaleFactor, &slicesY, 1, &yLoaded);
            
            //since x is now done, pass it to the particle system
            particleSys.loadData(&slicesX);
//            particlesOn = true;
        }
        
        
        if(xLoaded && yLoaded && zLoaded){
            allLoaded = true;
        }
    }
    

    if(particlesOn){
        
        if(ofGetFrameNum() % 2 == 0){
            particleSys.addParticles(1);
        }
        particleSys.update();
        
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
    if(!particlesOn){
        if(sliceDirection == 0 && xLoaded){
            if(drawMode == 0){
                
                for(int i = 0; i < slicesX.size(); i++){
                    slicesX[i].draw();
                }
                
            } else if(drawMode == 1) {
                
                slicesX[viewingSlice].drawPoints();
                
            } else if(drawMode == 2) {
                
                slicesX[viewingSlice].drawWire();
                
            } else if(drawMode == 3) {
                
                slicesX[viewingSlice].draw();
                
            } else if(drawMode == 4) {
                
                slicesX[viewingSlice].drawVectors();
                
            }
        } else if(sliceDirection == 1 && yLoaded){
            if(drawMode == 0){
                
                for(int i = 0; i < slicesY.size(); i++){
                    slicesY[i].draw();
                }
                
            } else if(drawMode == 1) {
                
                slicesY[viewingSlice].drawPoints();
                
            } else if(drawMode == 2) {
                
                slicesY[viewingSlice].drawWire();
                
            } else if(drawMode == 3) {
                
                slicesY[viewingSlice].draw();
                
            } else if(drawMode == 4) {
                
                slicesY[viewingSlice].drawVectors();
                
            }
        } else if(sliceDirection == 2 && zLoaded){
            if(drawMode == 0){
                
                for(int i = 0; i < slicesZ.size(); i++){
                    slicesZ[i].draw();
                }
                
            } else if(drawMode == 1) {
                
                slicesZ[viewingSlice].drawPoints();
                
            } else if(drawMode == 2) {
                
                slicesZ[viewingSlice].drawWire();
                
            } else if(drawMode == 3) {
                
                slicesZ[viewingSlice].draw();
                
            } else if(drawMode == 4) {
                
                slicesZ[viewingSlice].drawVectors();
                
            }
        }
    }

    drawAxes();
    drawBoundingBox();
    
    if(particlesOn){
        
        particleSys.draw();
        
        
    }

    
    
    ofPopMatrix();

    camera.end();


    
    
    drawUI();


    
    
    
//    ofSetColor(255);
//    ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), 600, 15);
//    ofDrawBitmapString("Particles: " + ofToString(particleSys.particles.size()), 600, 30);

    
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

        drawLoadingWidget();
        
    }

    
    
    
}

//--------------------------------------------------------------
void ofApp::drawLoadingWidget(){
    string load = "Slicing Data: ";
    
    float boxPosY = 400;
    float boxMargin = 20;
    float width = boxMargin * 2 + textFont.stringWidth(load + loadString) + 10;
    
    float textPosY = boxPosY + boxMargin + textFont.stringHeight(load);
    
    float circleAmp = 50;
    float circleSpacing = textFont.stringWidth(load + loadString)/numCircles;
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

    ofSetColor(255);
    textFont.drawString(load, ofGetWidth()/2 - textFont.stringWidth(load + loadString)/2, textPosY);
    ofSetColor(loadColor);
    textFont.drawString(loadString, ofGetWidth()/2 - textFont.stringWidth(load + loadString)/2 + textFont.stringWidth(load) + 10, textPosY);
    
    
    for(int i = 0; i < numCircles; i++){
        
        ofSetColor(255, 255);
        ofDrawCircle((ofGetWidth()/2 - (numCircles/2 * circleSpacing)) + i * circleSpacing, circlePosY + circleHeights[i] * circleAmp, 2);
        
        
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
    
    //Flow Direction Image
    ofPushMatrix();
        ofTranslate(0, 0, boundingBoxDim.z + 10);
        ofRotate(90, 1, 0, 0);
        flowDirection.draw(0, 0, boundingBoxDim.x, flowDirection.getHeight());
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
        drawMode = 0;
    } else if(key == '1'){
        drawMode = 1;
    } else if(key == '2'){
        drawMode = 2;
    } else if(key == '3'){
        drawMode = 3;
    } else if(key == '4'){
        drawMode = 4;
    }
    
    
    
    if(key == 'r' || key == 'R'){
//        camera.reset();
        camera.setGlobalPosition(globalCamPos);
        camera.lookAt(ofVec3f(0));
    }
    
    if(key == 'x' || key == 'X'){

        //make sure the data is sliced before switching
        if(xLoaded){
            sliceDirection = 0;
        } else {
            cout << "X not yet sliced" << endl;
        }
        
    }
    
    if(key == 'y' || key == 'Y'){
        
        //make sure the data is sliced before switching
        if(yLoaded){
            sliceDirection = 1;
        } else {
            cout << "Y not yet sliced" << endl;
        }
        
    }
    
    if(key == 'z' || key == 'Z'){
        
        //make sure the data is sliced before switching
        if(zLoaded){
            sliceDirection = 2;
        } else {
            cout << "Z not yet sliced" << endl;
        }
        
    }
 
    
    
    if(key == 'p' || key == 'P'){

        particlesOn = !particlesOn;
        
    }
    
    if(key == 'c' || key == 'c'){
        
        particleSys.particles.clear();
        
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
