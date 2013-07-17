/*
 *  Scanner.cpp
 *  Scride
 *
 *  Created by Serkan Sökmen on 03.05.2012
 *  Copyright 2012 Teknolab. All rights reserved.
 *
 */

#include "Scanner.h"


//------------------------------------------------------------------
Scanner::Scanner()
{
    resetScan();
}


//------------------------------------------------------------------
Scanner::~Scanner()
{
    emptyFramesCache();
}


//------------------------------------------------------------------
void Scanner::init(const ofRectangle &rect, int res, float scanTime)
{
    resetScan();
    
    scanRect.set(rect);
    step = res;
    speed = scanTime;
    
    scanLine.set(0, 0, rect.width, step);
    
    fbo.allocate(rect.width, rect.height);
    
    emptyFramesCache();
    
    for (int i=0; i<rect.height; i+=step)
    {
        ofTexture *tex = new ofTexture;
        tex->clear();
        tex->allocate(rect.width, rect.height, GL_RGB);
        //tex->setCompression(OF_COMPRESS_ARB);
        frames.push_back(tex);
    }
}


//------------------------------------------------------------------
void Scanner::startScan()
{
    bDoScan = true;
    scanPos.y = 0;
    scanIndex = 0;
}


//------------------------------------------------------------------
void Scanner::resetScan()
{
    scanIndex = 0;
    timestamp = ofGetElapsedTimef();
    scanPos.set(0, 0);
    bDoScan = false;
}


//------------------------------------------------------------------
void Scanner::update(ofPixels &pixels)
{
    if (bDoScan)
    {
        frames[scanIndex]->loadData(pixels);
        moveScanLine();
    }
    
    fbo.begin();
    ofSetColor(255, 255);
    
    ofPoint * pos = new ofPoint;
    
    // Scan line frame pixels
    for (int index=0; index<frames.size(); index++)
    {
        pos->set(scanLine.x, index*step);
        drawFrameIndex(index, pos);
    }
    
    delete pos;
    pos = 0;
    fbo.end();
    
    if (bDoScan)
    {
        ofEnableAlphaBlending();
        ofSetColor(255, 120);
        ofRect(scanPos, scanLine.width, scanLine.height);
        ofDisableAlphaBlending();
    }
    
    int frameCount = scanRect.height / step;
    if (frames.size() > frameCount)
    {
        frames.erase(frames.begin());
    }
    
    
}

//------------------------------------------------------------------
void Scanner::moveScanLine()
{
    if (scanPos.y >= scanRect.height)
    {
        scanPos.y = 0;
        scanIndex = 0;
        bDoScan = false;
    }
    else
    {
        float timePassed = ofGetElapsedTimef() - timestamp;
        
        if (timePassed > speed)
        {
            scanPos.y += step;
            timestamp = ofGetElapsedTimef();
            
            if (scanIndex < frames.size()-1)  scanIndex++;
        }
    }
}


//------------------------------------------------------------------
void Scanner::draw()
{
    ofPushMatrix();
    ofTranslate(scanRect.x, scanRect.y);
    fbo.draw(0, 0);
    ofPopMatrix();
}


//--------------------------------------------------------------
void Scanner::drawFrameIndex(int index, ofPoint *pos)
{
    frames[index]->bind();
    glBegin(GL_QUADS);
    glTexCoord2f(frames[index]->getCoordFromPoint(pos->x, pos->y).x,
                 frames[index]->getCoordFromPoint(pos->x, pos->y).y);
    glVertex2f(pos->x, pos->y);
    glTexCoord2f(frames[index]->getCoordFromPoint(pos->x + scanLine.width, pos->y).x,
                 frames[index]->getCoordFromPoint(pos->x + scanLine.width, pos->y).y);
    glVertex2f(pos->x + scanLine.width, pos->y);
    glTexCoord2f(frames[index]->getCoordFromPoint(pos->x + scanLine.width, pos->y + scanLine.height).x,
                 frames[index]->getCoordFromPoint(pos->x + scanLine.width, pos->y + scanLine.height).y);
    glVertex2f(pos->x + scanLine.width, pos->y + scanLine.height);
    glTexCoord2f(frames[index]->getCoordFromPoint(pos->x, pos->y + scanLine.height).x,
                 frames[index]->getCoordFromPoint(pos->x, pos->y + scanLine.height).y);
    glVertex2f(pos->x, pos->y + scanLine.height);
    glEnd();
    frames[index]->unbind();
}


//--------------------------------------------------------------
void Scanner::createFileName()
{
    // create a uniqe file name
	ostringstream oss;
	oss << ofGetYear() << "-";
	oss << setw(2) << setfill('0') << ofGetMonth() << "-";
	oss << setw(2) << setfill('0') << ofGetDay() << "-";
	oss << setw(2) << setfill('0') << ofGetHours() << "-";
	oss << setw(2) << setfill('0') << ofGetMinutes() << "-";
	oss << setw(2) << setfill('0') << ofGetSeconds() << ".png";
	shotName = oss.str();
}


//--------------------------------------------------------------
void Scanner::saveScanImage()
{
    createFileName();
    ofImage output;
    output.allocate(scanRect.width, scanRect.height, OF_IMAGE_COLOR);
    //output.setCompression(OF_COMPRESS_ARB);
    fbo.readToPixels(output.getPixelsRef());
    output.saveImage("shots/" + shotName);
}


//--------------------------------------------------------------
void Scanner::emptyFramesCache()
{
    for (int i=0; i<frames.size(); i++)
    {
        frames[i]->clear();
        delete frames[i];
        frames[i] = 0;
    }
    frames.clear();
}
