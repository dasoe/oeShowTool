#include "oeClasses.h"	
#include "ofGstVideoPlayer.h"


//--------------------------------------------------------------

void oeBezierEditor::init(int contentWidthTemp, int contentHeightTemp, int maskWidthTemp, int maskHeightTemp) {
    contentWidth = contentWidthTemp;
    contentHeight = contentHeightTemp;
    maskWidth = maskWidthTemp;
    maskHeight = maskHeightTemp;

    ofLogNotice("maskFbo width:" + ofToString(maskWidth) + ", height: " + ofToString(maskHeight));
    maskFBO.allocate(maskWidth, maskHeight, GL_RGB);
    myBezier.setReactToMouseAndKeyEvents(false);
    isActive = false;
}

//--------------------------------------------------------------

void oeBezierEditor::setActive(bool active) {
    myBezier.setReactToMouseAndKeyEvents(active);
    isActive = active;
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------

bool oeBezierEditor::getActive() {
    return isActive;
}


//--------------------------------------------------------------

void oeBezierEditor::loadBezier(string maskName) {
    myBezier.loadXmlPoints(maskName);
    ofLogNotice("bezier mask " + maskName + " loaded");
}

//--------------------------------------------------------------

void oeBezierEditor::saveMask(int blur) {
    ofImage image;
    image = grabMask(blur);
    //
    ofFileDialogResult saveFileResult = ofSystemSaveDialog("bin/data/masks/example.jpg", "Maske sichern (Bsp.: 'XXX.jpg')");
    if (saveFileResult.bSuccess) {
        image.save(saveFileResult.filePath);
        ofLogNotice("mask: image saved");
    }
}


//--------------------------------------------------------------

ofImage oeBezierEditor::grabMask(int blur) {
    //ofLogNotice("grabMask()");
    ofImage image;
    //image.allocate(contentWidth, contentHeight, OF_IMAGE_COLOR_ALPHA);
    ofPixels pixels;
    //ofLogNotice("grabMask() read To Pixels");
    maskFBO.readToPixels(pixels);
    image.setFromPixels(pixels);
    // cvImage.allocate(contentWidth, contentHeight);
    // ofLogNotice("grabMask() set CV IMage from pixels Pixels");

    cvImage.setFromPixels(pixels);
    cvImage.blur(blur);
    cvImage.resize(contentWidth, contentHeight);
    cvImage.flagImageChanged();
    ofLogNotice("cvimage size: " + ofToString(cvImage.getWidth()));
    image.setFromPixels(cvImage.getPixels());
    ofLogNotice("image size: " + ofToString(image.getWidth()));



    //screenGrab.grabScreen(0, 0, 1024, 768);
    return image;
}

//--------------------------------------------------------------

void oeBezierEditor::update() {
}

//--------------------------------------------------------------

void oeBezierEditor::draw() {
    if (isActive) {
        maskFBO.begin();
        ofClear(0);
        ofSetColor(255);
        myBezier.draw();
        maskFBO.end();
        ofSetColor(255);

        maskFBO.draw(0, 0);

    }
}
//--------------------------------------------------------------

void oeBezierEditor::drawHelp() {

    myBezier.drawHelp();
}

//####################################
// Slots! types: 
// 0: color, 1: image, 2: video

//--------------------------------------------------------------

void oeSlot::init(int contentWidthTemp, int contentHeightTemp, bool useGstreamer) {
    
	// load shader (brightness aso adjusting)
	brcosaShader.load("shaders/brcosa_GLSL");

	ofVideoPlayer newVideo1;
    ofVideoPlayer newVideo2;
	if (useGstreamer) {
		newVideo1.setPlayer(ofPtr<ofGstVideoPlayer>(new ofGstVideoPlayer));
		newVideo2.setPlayer(ofPtr<ofGstVideoPlayer>(new ofGstVideoPlayer));
	}

	rewindOnTransition = true;
    video.push_back(newVideo1);
    video.push_back(newVideo2);
    mainVideo = 0;
	contrast = 1.0;
	brightness = 1.0;
	saturation = 1.0;
    contentWidth = contentWidthTemp;
    contentHeight = contentHeightTemp;
    type = 1;
    color = ofFloatColor(0.4, 0.5, 0.2, 1.0);
}

//--------------------------------------------------------------

void oeSlot::setSlotType(int transType) {
    type = transType;
}

//--------------------------------------------------------------

void oeSlot::setTargetSlotType(int transType) {
    targetType = transType;
}


//--------------------------------------------------------------

string oeSlot::getInfo() {
    string info = "";
    if (getSlotType() == 0) {
        info = "COLOR ";
    } else if (getSlotType() == 1) {
        string cleanedPath = imagePath;
        ofStringReplace(cleanedPath, "images/", "");
        info = "IMAGE " + cleanedPath;
    } else if (getSlotType() == 2) {
        string cleanedPath = video[mainVideo].getMoviePath();
        ofStringReplace(cleanedPath, "videos/", "");
        info = "VIDEO " + cleanedPath;
    }
    return info;
}

//--------------------------------------------------------------

string oeSlot::getTargetInfo() {
    string info = "";
    if (getTargetSlotType() == 0) {
        info = "COLOR ";
    } else if (getTargetSlotType() == 1) {
        string cleanedPath = targetImagePath;
        ofStringReplace(cleanedPath, "images/", "");
        info = "IMAGE " + cleanedPath;
    } else if (getTargetSlotType() == 2 && video[!mainVideo].isLoaded()) {
        string cleanedPath = video[!mainVideo].getMoviePath();
        ofStringReplace(cleanedPath, "videos/", "");
        info = "VIDEO " + cleanedPath;
    }
    return info;
}

//--------------------------------------------------------------

string oeSlot::getMoviePath() {
    return video[mainVideo].getMoviePath();
}



//--------------------------------------------------------------

int oeSlot::getSlotType() {
    return type;
}

//--------------------------------------------------------------

int oeSlot::getTargetSlotType() {
    return targetType;
}

void oeSlot::prepareContent() {

    if (type == 2) {
        video[mainVideo].update();
    }
    if (targetType == 2 && video[!mainVideo].isLoaded()) {
        // target video
        video[!mainVideo].update();
    }

}

//--------------------------------------------------------------

bool oeSlot::getIsActive() {
    actualTime = ofGetElapsedTimeMillis();
    if (actualTime - slotIsActiveTimer > 300) {
        return false;
    } else {
        return true;
    }
}


//--------------------------------------------------------------

void oeSlot::update() {
    actualTime = ofGetElapsedTimeMillis();

    if (targetAlpha < 255) {

        float increment = (float) (actualTime - fadeTimerForSlot) / (float) (fadeTimeForSlot);
        //  ofLogNotice("fade increment: " + ofToString(ofLerp(0, 255, increment)));
        if (increment > 1.0) {
            increment = 1.0;
        }
        targetAlpha = ofLerp(0, 255, increment);
        if (targetAlpha > 254) {
            ofLogNotice("Transition of slot  DONE.");
            targetAlpha = 255;

            color = targetColor;

            image = targetImage;
            ofLogNotice("image swapped!");
            imagePath = targetImagePath;
            targetImagePath = "";
            targetImage.clear();

            // swap Videos
            ofLogNotice("videos swapped!");
            mainVideo = !mainVideo;
            // close the non-main-video
            video[!mainVideo].close();

            type = targetType;
            // set slot and target slot the same
        }
    }


}
//--------------------------------------------------------------

void oeSlot::loadImage(string path) {
    ofLogNotice("try to load image " + ofToString(path));
    image.load(path);
    imagePath = path;
    ofLogNotice("loaded!");
}

//--------------------------------------------------------------

void oeSlot::loadTargetImage(string path) {
    ofLogNotice("try to load image " + ofToString(path));
    targetImage.load(path);
    targetImagePath = path;
    ofLogNotice("loaded!");
}

//--------------------------------------------------------------

void oeSlot::loadVideo(string path) {
    ofLogNotice("try to load video " + ofToString(path));
    video[mainVideo].loadAsync(path);
    video[mainVideo].play();
    video[mainVideo].setVolume(0);
    setVideoLoopState(OF_LOOP_NORMAL);
    ofLogNotice("loaded!");
}

//--------------------------------------------------------------

void oeSlot::loadTargetVideo(string path) {
	ofLogNotice("try to load TARGET video " + ofToString(path));
	targetVideoPath = path;
	video[!mainVideo].loadAsync(path);
	video[!mainVideo].play();
	video[!mainVideo].setVolume(0);
	setTargetVideoLoopState(OF_LOOP_NORMAL);
	ofLogNotice("length: " + ofToString(video[!mainVideo].getDuration()));
	ofLogNotice("size video vector: " + ofToString(video.size()));
	ofLogNotice("loaded!");
}

//--------------------------------------------------------------

void oeSlot::setVideoLoopState(ofLoopType state) {
    video[mainVideo].setLoopState(state);
}

//--------------------------------------------------------------

void oeSlot::setTargetVideoLoopState(ofLoopType state) {
    video[!mainVideo].setLoopState(state);
}

//--------------------------------------------------------------

void oeSlot::restartVideo() {
    video[mainVideo].firstFrame();
}

//--------------------------------------------------------------

void oeSlot::restartTargetVideo() {
    video[!mainVideo].firstFrame();
}

//--------------------------------------------------------------

void oeSlot::setVideoPaused(bool paused) {
    video[mainVideo].setPaused(paused);
}

//--------------------------------------------------------------

void oeSlot::setVideoSpeed(float speed) {
    video[mainVideo].setSpeed(speed);
}

//--------------------------------------------------------------

void oeSlot::setTargetVideoSpeed(float speed) {
    video[!mainVideo].setSpeed(speed);

}

//--------------------------------------------------------------

void oeSlot::setTargetVideoPaused(bool paused) {
    video[!mainVideo].setPaused(paused);
}

//--------------------------------------------------------------

void oeSlot::setVideoPosition(float percent) {
	video[mainVideo].setPosition(percent/100.0);
}

//--------------------------------------------------------------

float oeSlot::getVideoPosition() {
	return video[mainVideo].getPosition();
}

//--------------------------------------------------------------

void oeSlot::selectSource() {

}

//--------------------------------------------------------------

void oeSlot::startTransitionForSlot() {
	// start transition - in case something has changed
	if (
		(type != targetType) ||
		(type == 0 && color != targetColor) ||
		(type == 1 && imagePath != targetImagePath) ||
		(type == 2 && video[mainVideo].getMoviePath() != targetVideoPath)
		)
	{
		ofLogNotice("transition of Slot started");
		fadeTimerForSlot = ofGetElapsedTimeMillis();
		targetAlpha = 0;
		if (type == 2 && rewindOnTransition) {
			restartTargetVideo();
		}
    }
}

//--------------------------------------------------------------

//--------------------------------------------------------------

ofFbo oeSlot::getContent() {
    slotIsActiveTimer = ofGetElapsedTimeMillis();
	ofEnableAlphaBlending();

    ofFbo tempFbo;

    tempFbo.allocate(contentWidth, contentHeight, GL_RGBA);
    tempFbo.begin();


	bool doShader = false;
	if (contrast != 1.0 || brightness != 1.0 || saturation != 1.0) {
		doShader = true;
	}
	if (doShader) {
		brcosaShader.begin();
		//tex1 = image.getTexture();
		brcosaShader.setUniform3f("avgluma", 0.62, 0.62, 0.62);
		//brcosaShader.setUniformTexture("tex0", tex1, 0);
		brcosaShader.setUniform1f("contrast", contrast);
		brcosaShader.setUniform1f("brightness", brightness);
		brcosaShader.setUniform1f("saturation", saturation);
		brcosaShader.setUniform1f("alpha", 1.0);
	}

    ofClear(0, 0, 0, 0);

    if (type == 0) {
		if (doShader) {
			brcosaShader.end();
		}
        ofSetColor(color);
		ofLogVerbose("drawing main COLOR type: " + ofToString(type) + ", targetType: " + ofToString(targetType) + ", targetAlpha: " + ofToString(targetAlpha));
        ofDrawRectangle(0, 0, contentWidth, contentHeight);
        ofSetColor(255, 255, 255, 255);
		if (doShader) {
			brcosaShader.begin();
		}
    } else if (type == 1) {
		ofLogVerbose("drawing main IMAGE type: " + ofToString(type) + ", targetType: " + ofToString(targetType) + ", targetAlpha: " + ofToString(targetAlpha));
		image.draw(0, 0);

    } else if (type == 2) {
        ofSetColor(255, 255, 255, 255);
		ofLogVerbose("drawing main VIDEO type: " + ofToString(type) + ", targetType: " + ofToString(targetType) + ", targetAlpha: " + ofToString(targetAlpha));
		video[mainVideo].draw(0, 0, contentWidth, contentHeight);
    }
	if (doShader) {
		brcosaShader.end();
	}
	if (doShader) {
		brcosaShader.begin();
		//tex1 = image.getTexture();
		brcosaShader.setUniform3f("avgluma", 0.62, 0.62, 0.62);
		//brcosaShader.setUniformTexture("tex0", tex1, 0);
		brcosaShader.setUniform1f("contrast", contrast);
		brcosaShader.setUniform1f("brightness", brightness);
		brcosaShader.setUniform1f("saturation", saturation);
		brcosaShader.setUniform1f("alpha", targetAlpha/255.0);
	}
	if (targetAlpha < 255) {
        if (targetType == 0) {
			if (doShader) {
				brcosaShader.end();
			}
            ofSetColor(targetColor, targetAlpha);
			ofLogVerbose("drawing TARGET COLOR type: " + ofToString(type) + ", targetType: " + ofToString(targetType) + ", targetAlpha: " + ofToString(targetAlpha));
			ofDrawRectangle(0, 0, contentWidth, contentHeight);
			if (doShader) {
				brcosaShader.begin();
			}
        } else if (targetType == 1) {
            ofSetColor(255, 255, 255, targetAlpha);
			ofLogVerbose("drawing TARGET IMAGE type: " + ofToString(type) + ", targetType: " + ofToString(targetType) + ", targetAlpha: " + ofToString(targetAlpha));
			targetImage.draw(0, 0);

        } else if (targetType == 2) {
			ofLogVerbose("drawing TARGET VIDEO type: " + ofToString(type) + ", targetType: " + ofToString(targetType) + ", targetAlpha: " + ofToString(targetAlpha));

            ofSetColor(255, 255, 255, targetAlpha);

			if (video[!mainVideo].getCurrentFrame() > 2) {
				video[!mainVideo].draw(0, 0, contentWidth, contentHeight);
			}
        }
        ofSetColor(255, 255, 255, 255);
    }
	if (doShader) {
		brcosaShader.end();
	}
	tempFbo.end();


    return tempFbo;
}


//--------------------------------------------------------------

void oeScene::addScene(vector <int> tempTargetScreenToSlotValues) {
    ofLogNotice("add scene with " + ofToString(tempTargetScreenToSlotValues.size()) + " slot to screen elements");
    setSlots(tempTargetScreenToSlotValues);
    name = "new";
}

string oeScene::getName() {
    return name;
}

//--------------------------------------------------------------

int oeScene::getNumber() {
    return number;
}

//--------------------------------------------------------------

void oeScene::setName(string tempName) {
    ofLogNotice("set name for scene: " + tempName);
    name = tempName;
}

//--------------------------------------------------------------

void oeScene::setNumber(int tempNumber) {
    ofLogNotice("set number for scene: " + ofToString(tempNumber));
    number = tempNumber;
}

//--------------------------------------------------------------

void oeScene::update() {

}


//--------------------------------------------------------------

void oeScene::setSlots(vector <int> tempTargetScreenToSlotValues) {
    ofLogNotice("set " + ofToString(tempTargetScreenToSlotValues.size()) + " slots for scene: ");

    slotNumberForScreen = tempTargetScreenToSlotValues;
}

//--------------------------------------------------------------

vector <int> oeScene::getSlots() {
    return slotNumberForScreen;
}
