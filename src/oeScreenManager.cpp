/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "oeScreenManager.h"
#include "ofxOpenCv.h"

// -----------------------------------

void oeScreenManager::setup(int firstMonitorWidthTemp, int firstMonitorHeightTemp, int contentWidthTemp, int contentHeightTemp) {
    actualScreen = 0;
    screenNumberCounter = 0;

    firstMonitorWidth = firstMonitorWidthTemp;
    firstMonitorHeight = firstMonitorHeightTemp;
    contentWidth = contentWidthTemp;
    contentHeight= contentHeightTemp;

    ofEnableAlphaBlending();

    maskShader.load("shaders/dummyVert.c", "shaders/maskShaderFrag.c");

}

// -----------------------------------

void oeScreenManager::showGridOnControlScreenChanged(bool & active) {
    ofLogNotice("------ showGridOnControlScreenChanged -------");
    showGridOnControlScreen = active;
    setWarpGridPosition();
}

// -----------------------------------

void oeScreenManager::info() {
    ofLogNotice("------ oeScreenManager: -------");
    ofLogNotice("number of screens: " + ofToString(screens.size()));
    //ofLogNotice("(ordering " + ofToString(screens.size() - 1) + ")");
    for (unsigned int i = 0; i < screens.size(); i++) {
        ofLogNotice("--- screen: " + ofToString(i + 1) + " (index " + ofToString(i) + "): ---");
        ofLogNotice("show Warp grid: " + ofToString(screens[i]->getShowWarpGrid()));
        ofLogNotice("points (x/y): " + ofToString(screens[i]->getNumXPoints()) + " / " + ofToString(screens[i]->getNumYPoints()));
        ofLogNotice("resolution: " + ofToString(screens[i]->getWarpGridResolution()));
        ofLogNotice("offset: " + ofToString(screens[i]->getOffset()));
        ofLogNotice("width: " + ofToString(screens[i]->getWidth()));
        ofLogNotice("------");

    }
    ofLogNotice("-------------------------------");
}

// -----------------------------------

void oeScreenManager::addScreen(ofPoint position, int width, int height, int pointsX, int pointsY, int pixelsPerGridDivision) {
    ofxBezierWarp* warp = new ofxBezierWarp();
    warp->allocate(width, height, pointsX, pointsY, pixelsPerGridDivision, 6408);
    warp->setShowWarpGrid(0);
    screens.push_back(warp);
    ordering.push_back(screenNumberCounter);
    screenNumberCounter++;
    // note: ordering holds index for screenNumbers not screenNumber (=screenNumber -1)

    screenPosition.push_back(position);
    screenWidth.push_back(width);    
    screenHeight.push_back(height);
    screenPresentationWidth.push_back(width);    
    screenPresentationHeight.push_back(height);
    
    
    ofLogNotice("oeScreenManager: added Screen Nr. " + ofToString(screens.size()) + " - index " + ofToString(screens.size() - 1));
    ofLogNotice("ordering : " + ofToString(ordering));
    screenNumber = ofToString(getScreenNumber());

}




// -----------------------------------

void oeScreenManager::removeScreen(int screenNumber) {
    screens.erase(screens.begin() + (screenNumber - 1));
    screenNumberCounter--;
    screenPosition.erase(screenPosition.begin() + (screenNumber - 1));
    ordering.erase(ordering.begin() + (screenNumber - 1));
}

// -----------------------------------

void oeScreenManager::toggleGrid(int screenNumber) {
    screens[screenNumber - 1]->toggleShowWarpGrid();
}

// -----------------------------------

bool oeScreenManager::getGridState(int screenNumber) {
    return screens[screenNumber - 1]->getShowWarpGrid();
}

// -----------------------------------

void oeScreenManager::moveScreenToFront(int screenNumber) {

    ofLogNotice("screenNumber: " + ofToString(screenNumber));

    ofLogNotice("ordering vorher: " + ofToString(ordering));
    // order of drawing is done by ordering
    unsigned int indexOfScreen;
    for (unsigned int i = 0; i < ordering.size(); i++) {
        if (ordering[i] == screenNumber - 1) {
            indexOfScreen = i;
        }
    }

    ordering.push_back(ordering[indexOfScreen]);
    ofLogNotice("ordering dazwischen: " + ofToString(ordering));

    ordering.erase(ordering.begin() + indexOfScreen);

    ofLogNotice("ordering nacher: " + ofToString(ordering));
}

// -----------------------------------

void oeScreenManager::update(vector <ofFbo> contentFbos) {
    for (unsigned int i = 0; i < screens.size(); i++) {
        screens[i]->begin();
        ofClear(0, 0, 0, 0);

        //maskShader.begin();		

        //shader.setUniform1f( "time", time );	//Pass float parameter "time" to shader
        //Pass mask to shader (fbo2)
        //maskShader.setUniformTexture( "texture1", mask.getTextureReference(), 1 ); 
        //Last parameter 1 is OpenGL texture identifier 

        contentFbos[i].draw(0, 0,screenWidth[i],screenHeight[i]);

        //maskShader.end();
        screens[i]->end();
        //ofLogNotice("gridDivisionX von " + ofToString(i) + ": " + ofToString(screens[i]->getGridDivisionsX()));
    }
}




// -----------------------------------

void oeScreenManager::draw() {
    
    if (getScreenNumber() > 0) {
        if (actualScreenSoloMode) {
            if (actualScreen > 0) {
                screens[actualScreen - 1]->setWarpGridResolution(10);
                screens[actualScreen - 1]->draw(screenPosition[actualScreen - 1].x, screenPosition[actualScreen - 1].y, screenPresentationWidth[actualScreen - 1], screenPresentationHeight[actualScreen - 1]);
            } else {
                ofLogWarning("solo Mode but no screen chosen!");
            }
        } else {
            for (unsigned int i = 0; i < ordering.size(); i++) {
                screens[ordering[i]]->setWarpGridResolution(10);
                screens[ordering[i]]->draw(screenPosition[ordering[i]].x, screenPosition[ordering[i]].y, screenPresentationWidth[i], screenPresentationHeight[i]);
            }
        }
    }

}

// -----------------------------------

int oeScreenManager::getScreenNumXPoints(int screenNumber) {
    return screens[screenNumber - 1]->getNumXPoints();
}
int oeScreenManager::getScreenNumYPoints(int screenNumber) {
    return screens[screenNumber - 1]->getNumYPoints();
}

// -----------------------------------

ofPoint oeScreenManager::getPosition(int screenNumber) {
    return screenPosition[screenNumber - 1];
}
void oeScreenManager::setPosition(int screenNumber, ofPoint position) {    
    screenPosition[screenNumber - 1] = position;
    setWarpGridPosition();    
}

void oeScreenManager::setScreenPresentationWidth(int screenNumber, int transWidth) {
    screenPresentationWidth[screenNumber - 1] = transWidth;
    setWarpGridPosition();
    
}
int oeScreenManager::getScreenPresentationWidth(int screenNumber) {
    return screenPresentationWidth[screenNumber - 1];
}
void oeScreenManager::setScreenPresentationHeight(int screenNumber, int transHeight) {
     screenPresentationHeight[screenNumber - 1] = transHeight;
    setWarpGridPosition();
     
}
int oeScreenManager::getScreenPresentationHeight(int screenNumber) {
    return screenPresentationHeight[screenNumber - 1];
}

// -----------------------------------

int oeScreenManager::getScreenWarpGridResolution(int screenNumber) {
    return screens[screenNumber - 1]->getWarpGridResolution();
}


// -----------------------------------

unsigned int oeScreenManager::getScreenNumber() {
    return screens.size();
}

// -----------------------------------

int oeScreenManager::getScreenWidthOfScreen(int screenNumber) {
    return screens[screenNumber - 1]->getWidth();
}

// -----------------------------------

int oeScreenManager::getScreenHeightOfScreen(int screenNumber) {
    return screens[screenNumber - 1]->getHeight();
}

//--------------------------------------------------------------

void oeScreenManager::keyPressed(int key) {
    ofLogNotice("key Pressed in oeScreenManager. actualScreen is " + ofToString(actualScreen));
    ofLogNotice(ofToString(key));
    switch (key) {
        case OF_KEY_LEFT:
            screenPosition[actualScreen - 1].x--;
            break;
        case OF_KEY_RIGHT:
            screenPosition[actualScreen - 1].x++;
            break;
        case OF_KEY_UP:
            screenPosition[actualScreen - 1].y--;
            break;
        case OF_KEY_DOWN:
            screenPosition[actualScreen - 1].y++;
            break;
    }
    setWarpGridPosition();
    //screens[actualScreen - 1]->setWarpGridPosition(screenPosition[actualScreen - 1].x, screenPosition[actualScreen - 1].y, screens[actualScreen - 1]->getWidth(), screens[actualScreen - 1]->getHeight());
}


//--------------------------------------------------------------

void oeScreenManager::setWarpGridPosition() {
    ofLogNotice("setWarpGridPosition in oeScreenManager. actualScreen is " + ofToString(actualScreen));

    if (actualScreen) {
        if (showGridOnControlScreen) {
            screens[actualScreen - 1]->setWarpGridPosition(100, 100, (firstMonitorWidth - 200), (firstMonitorHeight - 200));
            ofLogNotice("set warp Grid to 100, 100, " + ofToString((firstMonitorWidth - 200)) + ", " + ofToString((firstMonitorHeight - 200)));
        } else {
            screens[actualScreen - 1]->setWarpGridPosition(screenPosition[actualScreen - 1].x, screenPosition[actualScreen - 1].y, screenPresentationWidth[actualScreen - 1], screenPresentationHeight[actualScreen - 1]);
        }
    }
}

//--------------------------------------------------------------
// prepare the save of warp settings

void oeScreenManager::saveWarpSettings() {
    ofLogNotice("------- save WarpSettings! --------");
    saveWarpSettingsToXML("screenManager/warpGrid/" + ofToString(actualScreen) + "/grid.xml");
}

//--------------------------------------------------------------
// prepare the automatic save of warp settings (used for undo)

void oeScreenManager::autosaveWarpSettings() {
    ofLogNotice("------- autoSave WarpSettings! --------");
    auto t = time(nullptr);
    auto tm = *localtime(&t);

    ostringstream oss;
    oss << put_time(&tm, "%d-%m-%Y %H-%M-%S");
    saveWarpSettingsToXML("warp/undo/warp_" + oss.str() + ".xml");
}


//--------------------------------------------------------------
// actual saving of warp settings

void oeScreenManager::saveWarpSettingsToXML(string path) {
    vector<GLfloat> controlPoints;

    controlPoints = screens[actualScreen - 1]->getControlPoints();
    warpXml.clear();
    ofLogNotice("XML-Datei für Warp geleert. Nun neu anlegen...");

    ofLogNotice("control points: " + ofToString(controlPoints.size()));

    ofXml point;
    point.addChild("CONTROLPOINTS");
    point.setTo("CONTROLPOINTS");
    for (unsigned int i = 0; i < controlPoints.size(); i++) {
        point.addValue("PT", controlPoints[i]);
    }
    warpXml.addChild("SETTINGS");
    warpXml.setTo("SETTINGS");

    warpXml.addXml(point);
    ofLogNotice("XML-Datei für Warp angelegt");

    warpXml.save(path);
    ofLogNotice("XML-Datei für Warp gespeichert: " + path);

}

//--------------------------------------------------------------
int oeScreenManager::countPointsInSavedWarpSettings(string path) {

	if (warpXml.load(path)) {
		ofLogNotice("XML-Datei für Warp geladen. Pfad:" + ofToString(path));
	}
	else {
		ofLogWarning("XML-Datei für Warp existiert nicht? Pfad:" + ofToString(path) + "Warp-Punkte können nicht geladen werden");
	}
	if (warpXml.exists("SETTINGS")) {
		warpXml.setTo("SETTINGS");
	}
	if (warpXml.exists("CONTROLPOINTS")) {
		warpXml.setTo("CONTROLPOINTS");
	}
	return warpXml.getNumChildren();
}


//--------------------------------------------------------------
void oeScreenManager::resetWarpGrid(int screenNumber) {
    screens[screenNumber-1]->resetWarpGrid();
}

//--------------------------------------------------------------
void oeScreenManager::loadWarpSettings(bool forceLoad) {
	loadWarpSettingsFromXML("screenManager/warpGrid/" + ofToString(actualScreen) + "/grid.xml", forceLoad);
}

//--------------------------------------------------------------
void oeScreenManager::loadWarpSettingsFromXML(string path, bool forceLoad) {
	//we load our  file
	int numberofPoints = countPointsInSavedWarpSettings(path);
	vector<GLfloat> controlPoints;

	if (numberofPoints == screens[actualScreen - 1]->getControlPoints().size() || forceLoad) {

		//ofLogNotice("num Children: " + ofToString(warpXml.getNumChildren()));
		for (int j = 0; j< screens[actualScreen - 1]->getControlPoints().size(); j++) {
			//ofLogNotice(ofToString(warpXml.getValue<float>("PT[" + ofToString(j) + "]")));
			controlPoints.push_back(warpXml.getValue<float>("PT[" + ofToString(j) + "]"));
		}

		screens[actualScreen - 1]->setControlPoints(controlPoints);
	} else {
		ofLogWarning("Warp-Gitter in settings.xml stimmt nicht mit der Anzahl der gespeicherten Punkte in warp/warp.xml überein! warp-seetings daher nicht automatisch geladen.");
		//screens[actualScreen - 1]->setWarpGrid(warpPointsAmmountX, warpPointsAmmountY);
	}
}