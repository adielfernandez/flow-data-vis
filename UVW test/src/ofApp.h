#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    void drawRawCircles();
		
    ofVec3f dataDimensions;
    
    
    vector<double> xVals;
    vector<double> yVals;
    vector<double> zVals;
    vector<double> uVals;
    vector<double> vVals;
    vector<double> wVals;
    
    double minX, maxX, minY, maxY, minZ, maxZ;
    double minU, maxU, minV, maxV, minW, maxW;
    
    
    int counter;
    
    float zFloat;
    int zSlice;
    float zSpeed;
    
};
