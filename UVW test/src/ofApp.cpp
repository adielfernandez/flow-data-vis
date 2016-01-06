#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(300);
    ofSetVerticalSync(false);
    
    
    //Path to the comma delimited file
	string filePath = "uvw.txt";

	
	//Load file placed in bin/data
	ofFile file(filePath);
	
	if(!file.exists()){
		ofLogError("The file " + filePath + " is missing");
	}
    ofBuffer buffer(file);

    
    
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
        xVals.push_back(x);
        
        //Y
        istringstream ssY(words[1]);
        double y;
        ssY >> y;
        yVals.push_back(y);
        
        //Z
        istringstream ssZ(words[2]);
        double z;
        ssZ >> z;
        zVals.push_back(z);
        
        //U
        istringstream ssU(words[3]);
        double u;
        ssU >> u;
        uVals.push_back(u);
        
        //V
        istringstream ssV(words[4]);
        double v;
        ssV >> v;
        vVals.push_back(v);
        
        //W
        istringstream ssW(words[5]);
        double w;
        ssW >> w;
        wVals.push_back(w);
        
        
	}
    
    
    minX = *min_element(xVals.begin(), xVals.end());
    maxX = *max_element(xVals.begin(), xVals.end());
    
    minY = *min_element(yVals.begin(), yVals.end());
    maxY = *max_element(yVals.begin(), yVals.end());
    
    minZ = *min_element(zVals.begin(), zVals.end());
    maxZ = *max_element(zVals.begin(), zVals.end());
    
    minU = *min_element(uVals.begin(), uVals.end());
    maxU = *max_element(uVals.begin(), uVals.end());
    
    minV = *min_element(vVals.begin(), vVals.end());
    maxV = *max_element(vVals.begin(), vVals.end());

    minW = *min_element(wVals.begin(), wVals.end());
    maxW = *max_element(wVals.begin(), wVals.end());
    
    
    counter  = 0;
    
    dataDimensions.set(96, 65, 48);
    
    zSlice = 0;
    zSpeed = 1.5;
    
    
}

//--------------------------------------------------------------
void ofApp::update(){

    
//    zSlice = floor(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 48, true));
    
    
    zSpeed = ofMap(ofGetMouseX(), 0, ofGetWidth(), -0.2, 0.2);
    
    zFloat += zSpeed;
    
    if(zFloat < 0){
        zFloat = 48;
    } else if(zFloat > 48) {
        zFloat = 0;
    }
    
    zSlice = floor(zFloat);
    
    
    //COUNTER INCREMENTING
    counter++;
    
    if(counter > xVals.size()-1){
        counter = 0;
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackgroundGradient(ofColor(100), ofColor(0));
    

//    drawRawCircles();
    
    //need to get X and Y slots for current Z slice
    int startSlot = dataDimensions.x * dataDimensions.y * zSlice;
    int endSlot = dataDimensions.x * dataDimensions.y * (zSlice + 1);

    
    ofPushMatrix();
    ofTranslate(50, 50);
    
    //plot out a bunch of squares based on the current slice
    for(int i = startSlot; i < endSlot; i++){
        
        float scale = 270.0;
        
        float mappedX = xVals[i] * scale;
        float mappedY = (yVals[i] + 1) * scale;
        float mappedZ = zVals[i] * scale;
        float mappedU = ofMap(uVals[i], minU, maxU, 0, 255);
        float mappedV = ofMap(vVals[i], minV, maxV, 0, 255);
        float mappedW = ofMap(wVals[i], minW, maxW, 0, 255);
        
        ofSetColor(mappedU, mappedV, mappedW);
        ofDrawRectangle(mappedX, mappedY, 9, 12);
        
        
        
    }
    
    ofPopMatrix();
    
    

    
    
    
    
    //---------- DEBUG STRINGS ----------
    
    ofSetColor(255);
    ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), 15, 15);
    ofDrawBitmapString("Current Z-Slice: " + ofToString(zSlice), 15, 30);
    
//    ofDrawBitmapString(ofToString(counter), 15, 45);
//    string vals;
//    
//    vals += "x min: " + ofToString(minX) + "\n";
//    vals += "x max: " + ofToString(maxX) + "\n";
//    vals += "y min: " + ofToString(minY) + "\n";
//    vals += "y max: " + ofToString(maxY) + "\n";
//    vals += "z min: " + ofToString(minZ) + "\n";
//    vals += "z max: " + ofToString(maxZ) + "\n";
//    vals += "u min: " + ofToString(minU) + "\n";
//    vals += "u max: " + ofToString(maxU) + "\n";
//    vals += "v min: " + ofToString(minV) + "\n";
//    vals += "v max: " + ofToString(maxV) + "\n";
//    vals += "w min: " + ofToString(minW) + "\n";
//    vals += "w max: " + ofToString(maxW) + "\n";
//    
//    ofDrawBitmapString(ofToString(vals), 15, 60);
    
}



//--------------------------------------------------------------
void ofApp::drawRawCircles(){

    ofColor col1 = ofColor::red;
    ofColor col2 = ofColor::blue;
    
    float multiplier = 100.0;
    
    
    ofSetColor(col1.getLerped(col2, 1.0f/6.0f));
    ofDrawCircle(ofGetWidth()/2 + xVals[counter] * multiplier, 50, 30);
    ofSetColor(255);
    ofDrawBitmapString(ofToString(xVals[counter]), ofGetWidth()/2 + xVals[counter] * multiplier + 30, 50);
    
    ofSetColor(col1.getLerped(col2, 2.0f/6.0f));
    ofDrawCircle(ofGetWidth()/2 + yVals[counter] * multiplier, 150, 30);
    ofSetColor(255);
    ofDrawBitmapString(ofToString(yVals[counter]), ofGetWidth()/2 + yVals[counter] * multiplier + 30, 150);
    
    ofSetColor(col1.getLerped(col2, 3.0f/6.0f));
    ofDrawCircle(ofGetWidth()/2 + zVals[counter] * multiplier, 250, 30);
    ofSetColor(255);
    ofDrawBitmapString(ofToString(zVals[counter]), ofGetWidth()/2 + zVals[counter] * multiplier + 30, 250);
    
    ofSetColor(col1.getLerped(col2, 4.0f/6.0f));
    ofDrawCircle(ofGetWidth()/2 + uVals[counter] * multiplier, 350, 30);
    ofSetColor(255);
    ofDrawBitmapString(ofToString(uVals[counter]), ofGetWidth()/2 + uVals[counter] * multiplier + 30, 350);
    
    ofSetColor(col1.getLerped(col2, 5.0f/6.0f));
    ofDrawCircle(ofGetWidth()/2 + vVals[counter] * multiplier, 450, 30);
    ofSetColor(255);
    ofDrawBitmapString(ofToString(vVals[counter]), ofGetWidth()/2 + vVals[counter] * multiplier + 30, 450);
    
    ofSetColor(col1.getLerped(col2, 6.0f/6.0f));
    ofDrawCircle(ofGetWidth()/2 + wVals[counter] * multiplier, 550, 30);
    ofSetColor(255);
    ofDrawBitmapString(ofToString(wVals[counter]), ofGetWidth()/2 + wVals[counter] * multiplier + 30, 550);
    

    
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    string msg;
    
    msg += "x length: " + ofToString(xVals.size()) + "\n";
    msg += "y length: " + ofToString(yVals.size()) + "\n";
    msg += "z length: " + ofToString(zVals.size()) + "\n";
    msg += "u length: " + ofToString(uVals.size()) + "\n";
    msg += "v length: " + ofToString(vVals.size()) + "\n";
    msg += "w length: " + ofToString(wVals.size()) + "\n";
    
    cout << msg << endl;
    
    
    
    
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
