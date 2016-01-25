//
//  ThreadedSlicer.cpp
//  Flow Viz v2
//
//  Created by Adiel Fernandez on 12/28/15.
//
//

#include "ThreadedSlicer.h"


ThreadedSlicer::ThreadedSlicer(){
    
}

ThreadedSlicer::~ThreadedSlicer(){
    
    reSlice.close();
    //when class is destroyed in ofApp, make sure the thread finishes its business
    waitForThread(true);
    
}


void ThreadedSlicer::startSlicer(float _scaleFactor, vector<Slice> *_mainThreadSlices, int _orientation, bool *_flag){

    orientation = _orientation;
    mainThreadSlices = _mainThreadSlices;
    scaleFactor = _scaleFactor;
    flag = _flag;
    
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
    
    
    startThread();
    
    
}

void ThreadedSlicer::update(){
    
    
    if(reSlice.tryReceive(tempSlices)){
        
        
        mainThreadSlices -> swap(tempSlices);

        //reset internal variables to prepare for the next slicing event
        tempSlices.clear();
        
        *flag = true;
        
        stopThread();
    }
    
    
    

}


void ThreadedSlicer::threadedFunction(){
    
    while(isThreadRunning()){
    
        //Slice orientation
        //0 for X
        //1 for Y
        //2 for Z
        
        int numSlices;
        int sliceDimA, sliceDimB;
        
        if(orientation == 0){
            numSlices = dataDim.x;
            sliceDimA = dataDim.y;
            sliceDimB = dataDim.z;
        } else if(orientation == 1){
            numSlices = dataDim.y;
            sliceDimA = dataDim.x;
            sliceDimB = dataDim.z;
        } else if(orientation == 2){
            numSlices = dataDim.z;
            sliceDimA = dataDim.x;
            sliceDimB = dataDim.y;
        }
        
        
        
        
        //create all slices simultaneously since we'll need to fill them all
        //at the same time piece by piece
        for(int i = 0; i < numSlices; i++){
            Slice thisSlice;
            thisSlice.scaleFactor = scaleFactor;
            tempSlices.push_back(thisSlice);
        }

        //Path to the comma delimited file
        string filePath = "uvw.txt";


        //Load file placed in bin/data
        ofFile file(filePath);

        if(!file.exists()){
            ofLogError("The file " + filePath + " is missing");
        }
        ofBuffer buffer(file);

        int currentPoint = 0;

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
            d.setup(x, y, z, u, v, w);

            d.fCol.set(ofMap(d.u, minU, maxU, 0.0, 1.0), ofMap(d.v, minV, maxV, 0.0, 1.0), ofMap(d.w, minW, maxW, 0.0, 1.0));
            d.col.set(d.fCol.r * 255, d.fCol.g * 255, d.fCol.b * 255);


            //since the data is formatted to read off points in X, then Y, then Z
            //we need to sort them into the correct slices. go through the line,
            //and add each point to the right slice

            //figure out which slice the data point belongs in,
            //depending on the slicing orientation
            int thisSliceIndex;
            
            int pointsPerSlice = dataDim.x * dataDim.y;
            //...Data set reads off 6240 points before moving to next Z
            
            
            int pointsPerLine = (int)dataDim.x;
            
            if(orientation == 0){
                //if we're slicing in X, each successive point in the set goes on the
                //next X slice until the data set drops to the next Y then we start over
                thisSliceIndex = currentPoint % pointsPerLine;
                
            } else if(orientation == 1){
                //if we're slicing in Y, the whole line of points goes in a Y slice
                //then the next line goes in the next slice

                thisSliceIndex = floor( (currentPoint % pointsPerSlice) / pointsPerLine );
                 
                
            } else if(orientation == 2){
                //if we're slicing in Z, we can just fill a slice completely before
                //filling up the next one
                
                thisSliceIndex = floor((currentPoint - (currentPoint % pointsPerSlice)) / pointsPerSlice );

                
            }
            
            

            
            
            //add DataPoint to the vector of the current slice
            tempSlices[thisSliceIndex].dataList.push_back(d);


            currentPoint++;

        }

        //once all the Slices have been filled with their data points,
        //we can continue and do other things with them like setting up their meshes
        for(int i = 0; i < tempSlices.size(); i++){
            
            tempSlices[i].setupMesh(sliceDimA, sliceDimB);

        }
        
        //now that everything has been prepared, send the vector to the GL thread
        reSlice.send(std::move(tempSlices));
        
    }
    

    
}