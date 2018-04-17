#pragma once
#include "ofMain.h"
#include "ofxBezierEditor.h"
#include "ofxOpenCv.h"

class oeBezierEditor {
public:


    void init(int contentWidthTemp, int contentHeightTemp, int maskWidthTemp, int maskHeightTemp);
    void update();
    void draw();
    void drawHelp();

    void setActive(bool active);
    bool getActive();
    void loadBezier(string maskName);
    void saveMask(int blur);
    ofImage grabMask(int blur);

    int contentWidth, contentHeight, maskWidth, maskHeight;


    ofxCvColorImage cvImage;

    ofImage screenGrab;
    ofxBezierEditor myBezier;
    bool isActive;
    ofFbo maskFBO;
};

class oeSlot {
public:


    void init(int contentWidthTemp, int contentHeightTemp);
    void update();
    void prepareContent();
    void selectSource();
    void setSlotType(int transType);
    void setTargetSlotType(int transType);

    void loadImage(string path);
    void loadTargetImage(string path);  
    
    void loadVideo(string path);
    void loadTargetVideo(string path);
    void setVideoPaused(bool paused);
    void setTargetVideoPaused(bool paused);
    void restartVideo();
    void restartTargetVideo();
    void setVideoLoopState(ofLoopType state);
    void setTargetVideoLoopState(ofLoopType state);
    void setVideoSpeed(float speed);
    void setTargetVideoSpeed(float speed);
    void startTransitionForSlot();

    int getSlotType();
    int getTargetSlotType();
    bool getIsActive();
    
    string getInfo();
    string getTargetInfo();
    string getMoviePath();

    ofFbo getContent();


    vector <ofVideoPlayer> video;
    bool mainVideo;
    ofImage image;
    string imagePath;
    string targetImagePath;
    string targetVideoPath;
    
    ofFloatColor color;
    ofImage targetImage;
    ofFloatColor targetColor;
    
    int targetAlpha;
    
    long actualTime, fadeTimerForSlot, slotIsActiveTimer;
    int fadeTimeForSlot;

    int contentWidth, contentHeight;
    int type;
    int targetType;

    ofFbo contentFbo;

};

class oeScene {
public:

    void addScene(vector <int> tempSlotNumberForScreen);
    void setSlots(vector <int> tempSlotNumberForScreen);
    vector <int>  getSlots();
    
    void update();
    string getName();
    void setName(string tempName);
    int getNumber();
    void setNumber(int tempNumber);

    string name;
    int number;
    float fadeToTime;
    vector <int> slotNumberForScreen;
    
};