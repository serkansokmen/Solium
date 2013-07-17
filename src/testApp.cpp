#include "testApp.h"

// Init video related
int camWidth = 640;
int camHeight = 480;


//--------------------------------------------------------------
void testApp::setup(){

    ofBackground(0);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    int fps = 30;
    ofSetFrameRate(fps);
    
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth, camHeight);
    
    ofRectangle scanRect(200, 100, camWidth, camHeight);
    
    // Init scanner
    scanner = new Scanner;
    scanner->init(scanRect, 2, 0.005f);
    
    // Setup GUI
    bGuiVisible = true;
    bStartScan = false;
    bDrawVideo = false;
    
    setupGUI();
}

//--------------------------------------------------------------
void testApp::update(){
    
    vidGrabber.update();
	
    if(vidGrabber.isFrameNew()){
        scanner->update(vidGrabber.getPixelsRef());
    }
    
    if (bSaveScan){
        scanner->saveScanImage();
        bSaveScan = false;
    }
}

//--------------------------------------------------------------
void testApp::draw(){

	scanner->draw();
    
    if (bDrawVideo){
        int cw = camWidth/4;
        int ch = camHeight/4;
        vidGrabber.draw(ofGetWidth()-cw - 20, 20, cw, ch);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	static int latency = 0;
    
    if (key == OF_KEY_UP)
        latency++;
    if (key == OF_KEY_DOWN)
        latency--;
    
    if (latency < 0)
        latency = 0;

    if (key == 's'){
        bGuiVisible = !bGuiVisible;
        gui->setVisible(bGuiVisible);
    }}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::setupGUI(){
    float dim = 24;
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float width = 320;
    float length = width - xInit;
    
    gui = new ofxUICanvas(0, 0, width, ofGetHeight());
    gui->setDrawPaddingOutline(false);
    gui->setDrawWidgetPadding(false);
    gui->addWidgetDown(new ofxUILabel("SOLIUM", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUISpacer(length - xInit, 2));
    
    vector<string> names;
	names.push_back("LOSSLESS");
    names.push_back("HIGH");
	names.push_back("NORMAL");
	names.push_back("LOW");
    
    gui->addWidgetDown(new ofxUIRadio(dim, dim, "RESOLUTION", names, OFX_UI_ORIENTATION_VERTICAL));
    gui->addWidgetDown(new ofxUISlider(length - xInit, dim, 0.005f, 1.0f, 0.005f, "SPEED"));
    gui->addWidgetDown(new ofxUISpacer(length - xInit, 2));
    gui->addWidgetDown(new ofxUIButton(dim, dim, bStartScan, "SCAN"));
    gui->addWidgetDown(new ofxUIToggle(dim, dim, scanner->getPaused(), "PAUSE"));
    gui->addWidgetDown(new ofxUIButton(dim, dim, bSaveScan, "SAVE"));
    gui->addWidgetDown(new ofxUIToggle(dim, dim, bDrawVideo, "SHOW VIDEO INPUT"));
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->loadSettings("GUI/gui_settings.xml");
    gui->setVisible(bGuiVisible);
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	if (e.widget->getName() == "LOSSLESS")
        scanner->setResolution(SO_RESOLUTION_LOSSLESS);
    else if (e.widget->getName() == "HIGH")
        scanner->setResolution(SO_RESOLUTION_HIGH);
    else if (e.widget->getName() == "MEDIUM")
        scanner->setResolution(SO_RESOLUTION_MEDIUM);
    else if (e.widget->getName() == "LOW")
        scanner->setResolution(SO_RESOLUTION_LOW);
    
    else if (e.widget->getName() == "SPEED")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        scanner->setScanSpeed(slider->getScaledValue());
    }
    
    else if (e.widget->getName() == "SCAN")
        scanner->startScan();
    else if (e.widget->getName() == "PAUSE")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        scanner->setPaused(!toggle->getValue());
    }
    else if (e.widget->getName() == "SHOW VIDEO INPUT")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bDrawVideo = toggle->getValue();
    }
    
    else if (e.widget->getName() == "SAVE")
        bSaveScan = true;
}

//--------------------------------------------------------------
void testApp::exit()
{
    delete scanner;
    scanner = 0;
    
	gui->saveSettings("GUI/gui_settings.xml");
    delete gui;
}
