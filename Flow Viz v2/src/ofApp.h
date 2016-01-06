#pragma once

#include "ofMain.h"
#include "Slice.h"
#include "DataPoint.h"
#include "ThreadedSlicer.h"


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
		
    
    void drawUI();
    void drawAxes();
    void drawBoundingBox();
    
    float getBillboardAngle(ofVec3f globalPos);
    ofVec3f getBillboardVec(ofVec3f globalPos);
    
    //Threaded data slicing
    ThreadedSlicer Slicer;
    
    
    //UI Stuff
    ofTrueTypeFont axesFont;
    ofTrueTypeFont textFont;
    ofImage titleImg;
    ofImage velocityKey;
    ofImage camGuide;
    
    #define numCircles 25
    float circleHeights[numCircles];
    
    //Axis 
    ofConePrimitive coneX;
    ofConePrimitive coneY;
    ofConePrimitive coneZ;
    
    ofVec3f dataDim;
    ofVec3f boundingBoxDim;
    
    vector<Slice> slices;
    int sliceDirection;

    vector<Slice> slicesX;
    bool xLoaded, yLoaded, zLoaded;
    vector<Slice> slicesY;
    vector<Slice> slicesZ;
    
    
    
    
    
    //maximum values for each dimension
    //(found algorithmically from previous data vis)
    const double minX = 0.0;
    const double maxX = 3.10887;
    
    //Y is -1 to 1 in data set, but we'll shift over
    //by 1 to keep all data in first quadrant
    const double minY = 0.0;
    const double maxY = 2.0;
    
    const double minZ = 0.0;
    const double maxZ = 1.53807;
    
    const double minU = -0.00010852;
    const double maxU = 0.823162;
    
    const double minV = -0.0997344;
    const double maxV = 0.122762;
    
    const double minW = -0.126819;
    const double maxW = 0.143105;
    
    
    
    bool lighting;
    
    ofEasyCam camera;
    
    int viewingSlice;

    float nonViewingTrans;
    
    float scaleFactor;
    
    //0 = draw bulk
    //1 = data points
    //2 = interpolated mesh
    //3 = interpolated image
    //4 = vectors
    int drawMode;
};
