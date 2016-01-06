#pragma once

#include "ofMain.h"
#include "ZSlice.h"
#include "DataPoint.h"


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
		
    ofConePrimitive coneX;
    ofConePrimitive coneY;
    ofConePrimitive coneZ;
    
    void drawAxes();
    
    
    void drawBoundingBox();
    
    ofTrueTypeFont font;
    
    ofVec3f dataDim;
    ofVec3f boundingBoxDim;
    
    vector<ZSlice> slices;
    
    double minX, maxX, minY, maxY, minZ, maxZ;
    double minU, maxU, minV, maxV, minW, maxW;
    
    
    ofEasyCam camera;
    
    int viewingSlice;
    int lastViewingSlice;
    float nonViewingTrans;
    
    float scaleFactor;
    
    
    //0 = draw bulk
    //1 = data points
    //2 = interpolated mesh
    //3 = interpolated image
    //4 = vectors
    int drawMode;
};
