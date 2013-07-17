/*
 *  Scanner.h
 *  Scride
 *
 *  Created by Serkan Sökmen on 03.05.2012
 *  Copyright 2012 Teknolab. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

enum soScannerResolution
{
  SO_RESOLUTION_LOSSLESS  = 1,
  SO_RESOLUTION_HIGH      = 2,
  SO_RESOLUTION_MEDIUM    = 4,
  SO_RESOLUTION_LOW       = 8
};

class Scanner
{  
  private:
    void moveScanLine();
    void drawFrameIndex(int, ofPoint*);
    void createFileName();
    void emptyFramesCache();
    void resetScan();
  
    string      shotName;
    ofFbo       fbo;
    ofRectangle scanLine, scanRect;
    ofPoint     scanPos;
    int         step;
    int         scanIndex;
    float       speed;
    float       timestamp;
    vector<ofTexture*>  frames;
    bool        bDoScan;
  
  public:
  
    Scanner();
    ~Scanner();
    
    void init(const ofRectangle &rect, int res, float scanTime);
    void update(ofPixels &pixels);
    void draw();
    void startScan();
    void saveScanImage();
  
    void setResolution(soScannerResolution res)
    {
      step = res;
      init(scanRect, step, speed);
    }
    void setScanSpeed(float time)
    {
      speed = time;
    }
  
    void setPaused(bool val)
    {
      bDoScan = val;
    }
    bool getPaused() const 
    {
      return  bDoScan;
    }
};
