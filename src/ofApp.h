#pragma once

#include "ofMain.h"
#include "oeClasses.h"
#include "oeScreenManager.h"
#include "ofxXmlBasedProjectSettings.h"
#include "ofxXmlSettings.h"
#include "ofxImGui.h"
#include "ofxFlashMessages.h"


class ofApp : public ofBaseApp {
public:
    void setup();
    void listMaskDirectory();
    void update();
    void draw();
    void drawScreensGuiPart();
    void drawScreenToSlotGuiPart();
    void drawSlotGuiPart();
    void drawScenesGuiPart();
    void drawDMXLightsGuiPart();
    void drawMasterGuiPart();
    void drawFeedbackGuiPart();
    
    
    void updateMasks(int number, ofImage tempMask, string maskString);
    
    void saveSceneSettings(int sceneNumber);
    void prepareInnerSceneSettingsForSave(int tagNumber, int sceneNumber);

    void loadSceneSettings();
    void saveScreenSettings();
    void loadScreenSettings();
    void saveSlotSettings();
    void saveSlotSettings(int sceneNumber);
    void loadSlotSettings();
    void loadSlotSettings(int sceneNumber);
    void saveScreenToSlotSettings();
    void saveScreenToSlotSettings(int sceneNumber);
    void loadScreenToSlotSettings();
    void loadScreenToSlotSettings(int sceneNumber);

    void adjustMasterSpeed(float mspeed);
    void startScene(int sceneNumber);
    void startTransitionForScreen(int screenNumber);  
    void addScreen(ofPoint position, int width, int height, int pointsX, int pointsY, int pixelsPerGridDivision);
    void removeScreen(int tempScreenNumber);
    vector <int> getPrechosenSlots();
    void addSlot();
    void addScene();
    void addDMXLight();
    
    void prepareContentFbos();
    void resetFlashMessages();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void addFeedback(string _message, ofLogLevel _level);
    void addFeedback(string _message, ofLogLevel _level, bool _showFlashMessage);

    ofImage mask;

    ofDirectory bezierDir, maskDir, videoDir, imageDir;
    // #### GUI ####
    ofxImGui::Gui gui;

    ofxXmlSettings slotData, screenToSlotData, screenData, sceneData;


    long flashMessageTimer, actualTime;
    int screenCreatePositionAndSize[4], screenCreateGrid[3], removeScreenNumber;
    bool show_test_window, actualScreenSoloMode, showGridOnControlScreen;

    vector <ofImage> masks;
    vector <string> maskPaths;
    // debug 
    bool debug, verbose, logToFile;

    string sceneMessage;

    vector <string> tempSceneName;
    vector <int> tempSceneNumber;

    int actualScene;
    // fbos holding the actual image
    vector <ofFbo> contentFbos;

    vector <oeSlot*> slots;
    vector <oeScene*> scenes;
    vector <int> slotTypes;
    vector <int> targetSlotTypes;
    vector <ImVec4> slotColors;
    vector <ImVec4> targetSlotColors;

    ImVec4 startSlotColor;
    int sceneCounter;
    vector <int> slotNumberForScreen;
    vector <int> targetSlotNumberForScreenUnApproved;
    vector <int> targetSlotNumberForScreen;
    vector <int> targetSlotNumberAlphaForScreen;
    vector <int> fadeTimeForScreen;
    vector <long> fadeTimerForScreen;
    vector <bool> videoPaused;
    vector <bool> videoLooping;



	vector <float> videoSpeed;
    vector <bool> targetVideoPaused;
    vector <bool> targetVideoLooping;
    vector <float> targetVideoSpeed;


    float masterSpeed, masterSpeedMultiplier, masterAlpha;

    oeBezierEditor bezier;

    int maskBlur;

    ofxXmlBasedProjectSettings settings;

    oeScreenManager screenManager;
    int actualScreen, firstMonitorWidth, firstMonitorHeight;


    char maskName[128];

    string help;

    ofxFlashMessages flashMessages;
    bool useFeedbackWindow;
    ofFbo feedbackFBO;
    ofTexture textureSource;
    GLuint textureSourceID;
};
