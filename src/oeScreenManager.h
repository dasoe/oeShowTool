#pragma once
#include "ofMain.h"

#include "ofxBezierWarp.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"

class oeScreenManager {
public:

    //oeScreenManager();
    void setup(int firstMonitorWidthTemp, int firstMonitorHeightTemp, int contentWidthTemp, int contentHeightTemp);
    void update(vector <ofFbo> contentFbos);
    void draw();

    void info();
    void moveScreenToFront(int screenNumber);
    void keyPressed(int key);

    void showGridOnControlScreenChanged(bool & active);


    unsigned int getScreenNumber();
    int getScreenWidthOfScreen(int screenNumber);
    int getScreenHeightOfScreen(int screenNumber);
    int getScreenNumYPoints(int screenNumber);
    int getScreenNumXPoints(int screenNumber);
    int getScreenWarpGridResolution(int screenNumber);
    bool getGridState(int screenNumber);
    
    ofPoint getPosition(int screenNumber);  
    void setPosition(int screenNumber, ofPoint position);
    
    int getScreenPresentationWidth(int screenNumber);
    void setScreenPresentationWidth(int screenNumber, int transWidth);
    
    int getScreenPresentationHeight(int screenNumber);
    void setScreenPresentationHeight(int screenNumber, int transHeight);

    void toggleGrid(int screenNumber);
    void addScreen(ofPoint position, int width, int height, int pointsX, int pointsY, int pixelsPerGridDivision);
    void removeScreen(int screenNumber);
    void resetWarpGrid(int screenNumber);
    
    void setWarpGridPosition();
    void autosaveWarpSettings();
    void saveWarpSettings();
    void saveWarpSettingsToXML(string path);
    void loadWarpSettings(bool forceLoad);
    void loadWarpSettingsFromXML(string path, bool forceLoad);
    int countPointsInSavedWarpSettings(string path);

    vector <ofFbo> content;
    vector <ofxBezierWarp*> screens;
    vector <ofPoint> screenPosition;
    vector <int> screenWidth;
    vector <int> screenHeight;
    vector <int> screenPresentationWidth;
    vector <int> screenPresentationHeight;

    
    
    vector <ofFbo> screenContent;
    vector <int> ordering;

    ofxXmlSettings gridData;
    ofXml warpXml;

    ofShader maskShader; //Shader


    unsigned int actualScreen, screenNumberCounter, firstMonitorWidth, firstMonitorHeight, contentWidth, contentHeight;
    bool actualScreenSoloMode, showGridOnControlScreen;
    
    string screenNumber, activeScreen;

};