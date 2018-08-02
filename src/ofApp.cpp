#include "ofApp.h"
#include "imgui.h"

//--------------------------------------------------------------

void ofApp::setup() {

    //only show png files
    bezierDir.allowExt("xml");
    //populate the directory object
    bezierDir.listDir("beziers/");
    bezierDir.sort();

    //only show png files
    videoDir.allowExt("mp4");
    videoDir.allowExt("mov");
    //populate the directory object
    videoDir.listDir("videos/");
    videoDir.sort();

    //only show png files
    videoDir.allowExt("jpg");
    videoDir.allowExt("png");
    //populate the directory object
    imageDir.listDir("images/");
    imageDir.sort();

    sceneCounter = 10;


    if (screenData.loadFile("screenManager/screenData.xml")) {
        ofLogNotice("screenManager/screenData.xml loaded!");
    } else {
        ofLogError("ERROR! Unable to Load screenManager/screenData.xml!");
    }
    if (sceneData.loadFile("scenes/sceneData.xml")) {
        ofLogNotice("scenes/sceneData.xml loaded!");
    } else {
        ofLogError("ERROR! Unable to Load screenManager/screenData.xml!");
    }

    startSlotColor = ImColor(114, 144, 154, 200);

    listMaskDirectory();

    //required call
    gui.setup();

    settings.addInt("someInt", 1);
    settings.addInt("firstMonitorWidth", 1024);
    settings.addInt("firstMonitorHeight", 768);
    settings.addInt("contentWidth", 1024);
    settings.addInt("contentHeight", 768);
    settings.addInt("maskWidth", 1024);
    settings.addInt("maskHeight", 768);
	settings.addBoolean("fullScreen", 0);

	
	
    settings.addString("deafultMask", "masks/test1.jpg");

    // Step 2: 
    // init the xml. The added variables will be processed
    //
    // Second parameter switches on debug stuff: When true, the variables "debug", "verbose" and "logToFile" will be added, which control the debug output
    // (everything that is sent through ofLog functions). Should be used in the main/first xml object/file
    settings.init("settings.xml", true);

    
	ofSetFullscreen(settings.getBooleanValue("fullScreen"));

	screenManager.setup(settings.getIntValue("firstMonitorWidth"), settings.getIntValue("firstMonitorHeight"), settings.getIntValue("contentWidth"), settings.getIntValue("contentHeight"));
    actualScreen = 0;

    bezier.init(settings.getIntValue("contentWidth"), settings.getIntValue("contentHeight"), settings.getIntValue("maskWidth"), settings.getIntValue("maskHeight"));

    ofEnableAlphaBlending();

    screenCreatePositionAndSize[0] = 10;
    screenCreatePositionAndSize[1] = 10;
    screenCreatePositionAndSize[2] = 800;
    screenCreatePositionAndSize[3] = 600;
    screenCreateGrid[0] = 6;
    screenCreateGrid[1] = 5;
    screenCreateGrid[2] = 10;


    // ImTextureID id = (ImTextureID)enyin::TextureProvider::GetInstance().Get(texture)->GetTextureId();    
    loadSlotSettings();
    loadScreenSettings();
    loadScreenToSlotSettings();
    loadSceneSettings();
}

//--------------------------------------------------------------

void ofApp::listMaskDirectory() {
    //only show png files
    maskDir.allowExt("jpg");
    //populate the directory object
    maskDir.listDir("masks/");
    maskDir.sort();
}


// -----------------------------------

void ofApp::prepareInnerSceneSettingsForSave(int tagNumber, int sceneNumber) {
    // set base stuff
    sceneData.setValue("scene:name", scenes[sceneNumber - 1]->getName(), tagNumber);
    sceneData.setValue("scene:number", scenes[sceneNumber - 1]->getNumber(), tagNumber);
    //    sceneData.pushTag("scene", tagNumber);
    //    // set screen nodes and according slot nodes
    //    vector <int> tempSlots = getPrechosenSlots();
    //    scenes[sceneNumber - 1]->setSlots(tempSlots);
    //    for (unsigned int j = 0; j < tempSlots.size(); j++) {
    //        int tagNum2 = sceneData.addTag("screen");
    //        ofLogNotice("add screen node " + ofToString(j));
    //        sceneData.setValue("screen:slot", tempSlots[j], tagNum2);
    //    }
    //    // set slot nodes and according settings
    //    for (int j = 0; j < slots.size(); j++) {
    //        int tagNum3 = sceneData.addTag("slot");
    //        ofLogNotice("add slot node " + ofToString(j));
    //        sceneData.setValue("slot:type", slots[j]->getSlotType(), tagNum3);
    //        sceneData.setValue("slot:color:r", slotColors[j].x, tagNum3);
    //        sceneData.setValue("slot:color:g", slotColors[j].y, tagNum3);
    //        sceneData.setValue("slot:color:b", slotColors[j].z, tagNum3);
    //        sceneData.setValue("slot:imagepath", slots[j]->imagePath, tagNum3);
    //        sceneData.setValue("slot:videopath", slots[j]->getMoviePath(slots[j]->mainVideo), tagNum3);
    //        sceneData.setValue("slot:targetVideopath", slots[j]->getMoviePath(!slots[j]->mainVideo), tagNum3);
    //    }
    //    sceneData.popTag();

    saveSlotSettings(sceneNumber);
    saveScreenToSlotSettings(sceneNumber);
}


// -----------------------------------

void ofApp::saveSceneSettings(int sceneNumber) {
    //sceneData.clear();
    //sceneData.addTag("SETTINGS");


    if (sceneData.pushTag("SETTINGS", 0)) {

        ofLogNotice(ofToString(sceneData.getNumTags("scene")));
        if (sceneData.getNumTags("scene") >= sceneNumber) {
            sceneData.clearTagContents("scene", sceneNumber - 1);
            prepareInnerSceneSettingsForSave(sceneNumber - 1, sceneNumber);
            ofLogNotice("scene: " + ofToString(sceneNumber) + " (index " + ofToString(sceneNumber - 1) + "): changed in XML");
        } else if (sceneData.getNumTags("scene") == sceneNumber - 1) {
            int tagNum = sceneData.addTag("scene");
            prepareInnerSceneSettingsForSave(tagNum, sceneNumber);
            ofLogNotice("scene: " + ofToString(sceneNumber) + " (index " + ofToString(sceneNumber - 1) + "): added to XML");
        } else {
            flashMessageTimer = ofGetElapsedTimeMillis();
            sceneMessage += "please add scene number " + ofToString(sceneData.getNumTags("scene") + 1) + " next\n";
            ofLogNotice(sceneMessage);
        }
        sceneData.popTag();
        sceneData.saveFile();

    }
}

// -----------------------------------

void ofApp::loadSceneSettings() {
    int numberOfSceneNodes;
    sceneData.pushTag("SETTINGS", 0);
    numberOfSceneNodes = sceneData.getNumTags("scene");
    ofLogNotice(ofToString(numberOfSceneNodes) + " scene nodes found");
    for (int i = 0; i < numberOfSceneNodes; i++) {
        addScene();
        scenes[i]->setName(sceneData.getValue("scene:name", "", i));
        scenes[i]->setNumber(sceneData.getValue("scene:number", 0, i));
        vector <int> tempTargetScreenToSlotValues;
        numberOfSceneNodes = sceneData.getNumTags("scene");
        sceneData.pushTag("scene", i);
        for (int j = 0; j < sceneData.getNumTags("screen"); j++) {
            tempTargetScreenToSlotValues.push_back(sceneData.getValue("screen:slot", 0, j));
        }
        scenes[i]->setSlots(tempTargetScreenToSlotValues);
        sceneData.popTag();
    }
    sceneData.popTag();
}

// -----------------------------------

void ofApp::saveScreenSettings() {
    screenData.clear();
    screenData.addTag("SETTINGS");

    if (screenData.pushTag("SETTINGS")) {

        for (unsigned int i = 0; i < screenManager.getScreenNumber(); i++) {
            ofLogNotice("screen: " + ofToString(i + 1) + " (index " + ofToString(i) + "): added to XML");

            int tagNum = screenData.addTag("screen");
            screenData.setValue("screen:x", screenManager.screenPosition[i].x, tagNum);
            screenData.setValue("screen:y", screenManager.screenPosition[i].y, tagNum);
            screenData.setValue("screen:width", screenManager.getScreenWidthOfScreen(i + 1), tagNum);
            screenData.setValue("screen:height", screenManager.getScreenHeightOfScreen(i + 1), tagNum);
            screenData.setValue("screen:presentationWidth", screenManager.getScreenPresentationWidth(i + 1), tagNum);
            screenData.setValue("screen:presentationHeight", screenManager.getScreenPresentationHeight(i + 1), tagNum);
            screenData.setValue("screen:pointsX", screenManager.getScreenNumXPoints(i + 1), tagNum);
            screenData.setValue("screen:pointsY", screenManager.getScreenNumYPoints(i + 1), tagNum);
            screenData.setValue("screen:pixelsPerGridDivision", screenManager.getScreenWarpGridResolution(i + 1), tagNum);
            screenData.setValue("screen:mask", maskPaths[i], tagNum);
        }
    }
    screenData.popTag();
    screenData.saveFile();

}

// -----------------------------------

void ofApp::loadScreenSettings() {
    int numberOfScreens;
    screenData.pushTag("SETTINGS", 0);

    numberOfScreens = screenData.getNumTags("screen");
    ofLogNotice(ofToString(numberOfScreens) + " screen nodes found");
    for (int i = 0; i < numberOfScreens; i++) {
        //the last argument of getValue can be used to specify
        //which tag out of multiple tags you are refering to.

        addScreen(ofPoint(screenData.getValue("screen:x", 0, i), screenData.getValue("screen:y", 0, i)),
                screenData.getValue("screen:width", 0, i),
                screenData.getValue("screen:height", 0, i),
                screenData.getValue("screen:pointsX", 0, i),
                screenData.getValue("screen:pointsY", 0, i),
                screenData.getValue("screen:pixelsPerGridDivision", 0, i)
                );



        ofImage tempMask = screenData.getValue("screen:mask", "", i);
        masks[i] = tempMask;
        masks[i].setImageType(OF_IMAGE_COLOR_ALPHA);
        masks[i].getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);
        maskPaths[i] = screenData.getValue("screen:mask", "", i);

        screenManager.actualScreen = i + 1;
        screenManager.setScreenPresentationWidth(screenManager.actualScreen, screenData.getValue("screen:presentationWidth", 800, i));
        screenManager.setScreenPresentationHeight(screenManager.actualScreen, screenData.getValue("screen:presentationHeight", 600, i));
        screenManager.loadWarpSettings(false);

    }
    screenManager.actualScreen = 0;
    screenData.popTag();

}

// -----------------------------------

void ofApp::loadScreenToSlotSettings() {
    loadScreenToSlotSettings(0);
}

// -----------------------------------

void ofApp::loadScreenToSlotSettings(int sceneNumber) {
    // load screen to slot settings for scene number or (in case sceneNumber == 0) "globally"
    string path = "";
    if (sceneNumber > 0) {
        ofLogNotice("trying to load Screen to slot settings - for scene " + ofToString(sceneNumber));
        path = "scenes/" + ofToString(sceneNumber);
        ofDirectory dir(path);
        if (!dir.exists()) {
            ofLogError("ERROR! no folder for Screen to slot settings - for scene " + ofToString(sceneNumber));
        }
    } else {
        ofLogNotice("trying to load main Screen to slot settings (for startup)");
        path = "slots";
    }
    path += "/screenToSlotData.xml";

    if (screenToSlotData.loadFile(path)) {
        ofLogNotice(ofToString(path) + " loaded!");
    } else {
        ofLogError("ERROR! Unable to Load " + ofToString(path) + "!");
    }

    int numberOfScreenToSlotNodes;
    screenToSlotData.pushTag("SETTINGS", 0);
    numberOfScreenToSlotNodes = screenToSlotData.getNumTags("screenToSlot");
    ofLogNotice(ofToString(numberOfScreenToSlotNodes) + " screen-to-slot nodes found");
    for (int i = 0; i < numberOfScreenToSlotNodes; i++) {
        if (targetSlotNumberForScreen[i] != screenToSlotData.getValue("screenToSlot:slot", 0, i)) {
            targetSlotNumberForScreen[i] = screenToSlotData.getValue("screenToSlot:slot", 0, i);
            startTransitionForScreen(i + 1);
        }

    }
    screenToSlotData.popTag();

}

// -----------------------------------

void ofApp::saveScreenToSlotSettings() {
    saveScreenToSlotSettings(0);
}


// -----------------------------------

void ofApp::saveScreenToSlotSettings(int sceneNumber) {
    ofLogNotice("trying to save screen-to-slot settings:");
    screenToSlotData.clear();
    screenToSlotData.addTag("SETTINGS");
    if (screenToSlotData.pushTag("SETTINGS")) {
        for (unsigned int i = 0; i < screenManager.getScreenNumber(); i++) {
            ofLogNotice("add node " + ofToString(i));
            int tagNum = screenToSlotData.addTag("screenToSlot");
            screenToSlotData.setValue("screenToSlot:slot", targetSlotNumberForScreen[i], tagNum);

        }
    }
    screenToSlotData.popTag();

    string path = "";
    if (sceneNumber > 0) {
        ofLogNotice("trying to save Screen to slot settings - for scene " + ofToString(sceneNumber));
        path = "scenes/" + ofToString(sceneNumber);
        ofDirectory dir(path);
        if (!dir.exists()) {
            dir.create(true);
        }
    } else {
        ofLogNotice("trying to save main Screen to slot settings (for startup)");
        path = "slots";
    }
    path += "/screenToSlotData.xml";
    screenToSlotData.saveFile(path);

}

// -----------------------------------

void ofApp::saveSlotSettings() {
    saveSlotSettings(0);
}

// -----------------------------------

void ofApp::saveSlotSettings(int sceneNumber) {
    ofLogNotice("prepare slot settings:");
    slotData.clear();
    slotData.addTag("SETTINGS");
    if (slotData.pushTag("SETTINGS")) {
        for (unsigned int i = 0; i < slots.size(); i++) {
            ofLogNotice("add node " + ofToString(i));

            int tagNum = slotData.addTag("slot");
            slotData.setValue("slot:type", slots[i]->getSlotType(), tagNum);
            slotData.setValue("slot:width", settings.getIntValue("contentWidth"), tagNum);
            slotData.setValue("slot:height", settings.getIntValue("contentHeight"), tagNum);
            slotData.setValue("slot:color:r", slotColors[i].x, tagNum);
            slotData.setValue("slot:color:g", slotColors[i].y, tagNum);
            slotData.setValue("slot:color:b", slotColors[i].z, tagNum);
            slotData.setValue("slot:imagepath", slots[i]->imagePath, tagNum);
            slotData.setValue("slot:videopath", slots[i]->getMoviePath(), tagNum);
            slotData.setValue("slot:paused", videoPaused[i], tagNum);
            slotData.setValue("slot:loop", videoLooping[i], tagNum);
            slotData.setValue("slot:speed", videoSpeed[i], tagNum);

            // target Paths will not be saved. later, when loading, everything is loaded to target values, then transition is called
        }
    }
    slotData.popTag();

    string path = "";
    if (sceneNumber > 0) {
        ofLogNotice("trying to save slot settings - for scene " + ofToString(sceneNumber));
        path = "scenes/" + ofToString(sceneNumber);
        ofDirectory dir(path);
        if (!dir.exists()) {
            dir.create(true);
        }
    } else {
        ofLogNotice("trying to save main slot settings (for startup)");
        path = "slots";
    }
    path += "/slotData.xml";
    slotData.saveFile(path);

}

// -----------------------------------

void ofApp::loadSlotSettings() {
    loadSlotSettings(0);
}

// -----------------------------------

void ofApp::loadSlotSettings(int sceneNumber) {
    string path = "";
    if (sceneNumber > 0) {
        ofLogNotice("trying to load slot settings - for scene " + ofToString(sceneNumber));
        path = "scenes/" + ofToString(sceneNumber);
        ofDirectory dir(path);
        if (!dir.exists()) {
            ofLogError("ERROR! no folder for slot settings - for scene " + ofToString(sceneNumber));
        }
    } else {
        ofLogNotice("trying to load main slot settings (for startup)");
        path = "slots";
    }
    path += "/slotData.xml";

    if (slotData.loadFile(path)) {
        ofLogNotice(ofToString(path) + " loaded!");
    } else {
        ofLogError("ERROR! Unable to Load " + ofToString(path) + "!");
    }

    int numberOfSlots;
    slotData.pushTag("SETTINGS", 0);
    numberOfSlots = slotData.getNumTags("slot");
    ofLogNotice(ofToString(numberOfSlots) + " slot nodes found");
    for (int i = 0; i < numberOfSlots; i++) {
        // ad a slot if called on startup (=not on loading a scene)
        // TODO: add slot addition and non-addition and so on depending on the number of slots already there and in the file
        if (sceneNumber <= 0) {
            addSlot();
        }
        // set target type in case type shall be changed
        if (slots[i]->getSlotType() != slotData.getValue("slot:type", 0, i)) {
            targetSlotTypes[i] = slotData.getValue("slot:type", 0, i);
            slots[i]->setTargetSlotType(targetSlotTypes[i]);
        }

        // TODO: from now on only change target type with transition

        // set target color in case color shall be changed
        if (
                targetSlotColors[i].x != slotData.getValue("slot:color:r", 0.0f, i) ||
                targetSlotColors[i].y != slotData.getValue("slot:color:g", 0.0f, i) ||
                targetSlotColors[i].z != slotData.getValue("slot:color:b", 0.0f, i)
                ) {
            targetSlotColors[i].x = slotData.getValue("slot:color:r", 0.0f, i);
            targetSlotColors[i].y = slotData.getValue("slot:color:g", 0.0f, i);
            targetSlotColors[i].z = slotData.getValue("slot:color:b", 0.0f, i);
            slots[i]->targetColor = ofFloatColor(targetSlotColors[i].x, targetSlotColors[i].y, targetSlotColors[i].z);
        }
        // set target image in case image shall be changed
        if (slots[i]->imagePath != slotData.getValue("slot:imagepath", "", i)) {
            slots[i]->loadTargetImage(slotData.getValue("slot:imagepath", "", i));
        }
		ofLogNotice("getmoviepath of actual slot: " + slots[i]->getMoviePath());

		ofLogNotice("movie Path in slot data: " + slotData.getValue("slot:videopath", "", i));

		// set target video in case video shall be changed
        if (slots[i]->getMoviePath() != slotData.getValue("slot:videopath", "", i)) {
            slots[i]->loadTargetVideo(slotData.getValue("slot:videopath", "", i));
        }

        videoPaused[i] = slotData.getValue("slot:paused", 0, i);
        videoLooping[i] = slotData.getValue("slot:loop", 1, i);
        videoSpeed[i] = slotData.getValue("slot:speed", 1.0f, i);

        // now do the transition for the appropriate slot
        slots[i]->startTransitionForSlot();
    }
    slotData.popTag();
    //    for (unsigned int i = 0; i < screenManager.getScreenNumber(); i++) {
    //        startTransitionForScreen(i + 1);
    //    }

}


// -----------------------------------

vector <int> ofApp::getPrechosenSlots() {
    vector <int> tempActiveSlots;
    for (unsigned int i = 0; i < screenManager.getScreenNumber(); i++) {
        if (targetSlotNumberForScreenUnApproved[i]) {
            tempActiveSlots.push_back(targetSlotNumberForScreenUnApproved[i]);
        }
    }
    return tempActiveSlots;
}

//--------------------------------------------------------------

void ofApp::update() {
    // ofLogNotice("update!");
    actualTime = ofGetElapsedTimeMillis();

    if (actualTime - flashMessageTimer > 4000) {
        resetFlashMessages();
    }

    for (unsigned int i = 0; i < slots.size(); i++) {
        slots[i]->update();
    }


    // do the calculation for actual Alpha of target fbo
    for (unsigned int i = 0; i < screenManager.getScreenNumber(); i++) {

        if (targetSlotNumberForScreen[i] != slotNumberForScreen[i]) {
            if (targetSlotNumberAlphaForScreen[i] < 255) {

                float increment = (float) (actualTime - fadeTimerForScreen[i]) / (float) (fadeTimeForScreen[i]);
                //ofLogNotice("fade increment: " + ofToString(ofLerp(0, 255, increment)));
                if (increment > 1.0) {
                    increment = 1.0;
                }
                targetSlotNumberAlphaForScreen[i] = ofLerp(0, 255, increment);
            } else if (targetSlotNumberAlphaForScreen[i] > 254) {

                targetSlotNumberAlphaForScreen[i] = 255;
                // set slot and target slot the same
                slotNumberForScreen[i] = targetSlotNumberForScreen[i];
                ofLogNotice("Transition screen " + ofToString(i) + " DONE.");
            }
        }
    }

    // prepare the fbos
    prepareContentFbos();
    screenManager.update(contentFbos);

    help = "";
}

//--------------------------------------------------------------

void ofApp::prepareContentFbos() {

    // check if mask is on edit and one screen is selected
    if (bezier.getActive() && screenManager.actualScreen > 0) {
        masks[screenManager.actualScreen - 1] = bezier.grabMask(maskBlur);
        masks[screenManager.actualScreen - 1].setImageType(OF_IMAGE_COLOR_ALPHA);
        masks[screenManager.actualScreen - 1].getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);
    }
    for (unsigned int i = 0; i < screenManager.getScreenNumber(); i++) {
        bool drawTargetContentAlso = false;
        ofFbo tempFbo;
        tempFbo.allocate(screenManager.getScreenWidthOfScreen(i + 1), screenManager.getScreenHeightOfScreen(i + 1), GL_RGBA);


        // if there is a slot for the screen, get content
        if (slotNumberForScreen[i]) {
            slots[slotNumberForScreen[i] - 1]->prepareContent();
            tempFbo = slots[slotNumberForScreen[i] - 1]->getContent();
            //  tempFbo.getTexture().setAlphaMask(masks[i].getTexture());

        }
        contentFbos[i].begin();

        ofClear(0);
        ofSetColor(255, 255, 255, 255);
        tempFbo.draw(0, 0, screenManager.getScreenWidthOfScreen(i + 1), screenManager.getScreenHeightOfScreen(i + 1));
        // if there is a target slot for the screen which is not the same, get content
        if (targetSlotNumberForScreen[i] && targetSlotNumberForScreen[i] != slotNumberForScreen[i]) {

            ofSetColor(255, 255, 255, 255);
            // ofSetColor(255, 255-targetSlotNumberAlphaForScreen[i]);
            tempFbo.draw(0, 0, screenManager.getScreenWidthOfScreen(i + 1), screenManager.getScreenHeightOfScreen(i + 1));

            // set a Flag for later use 
            drawTargetContentAlso = true;
            ofFbo tempFboTarget;
            tempFboTarget.allocate(screenManager.getScreenWidthOfScreen(i + 1), screenManager.getScreenHeightOfScreen(i + 1), GL_RGBA);
            slots[targetSlotNumberForScreen[i] - 1]->update();
            tempFboTarget = slots[targetSlotNumberForScreen[i] - 1]->getContent();
            // tempFboTarget.getTexture().setAlphaMask(masks[i].getTexture());
            ofSetColor(255, 255, 255, targetSlotNumberAlphaForScreen[i]);
            tempFboTarget.draw(0, 0, screenManager.getScreenWidthOfScreen(i + 1), screenManager.getScreenHeightOfScreen(i + 1));
            ofSetColor(255, 255, 255, 255);
        }

        contentFbos[i].end();
        // ofLogNotice("contentFbos[i]: " + ofToString(contentFbos[i].getWidth()) + "masks[i]: " + ofToString(masks[i].getWidth()));


        contentFbos[i].getTexture().setAlphaMask(masks[i].getTexture());
    }
}


//--------------------------------------------------------------

void ofApp::draw() {

    if (screenManager.actualScreen <= 0) {
        ofClear(0);
    } else {
        ofClear(50, 0, 20);
    }

    // -------------- Mask --------------------
    if (bezier.getActive()) {
        ofFill();
        ofSetColor(0, 0, 0, 255);
        ofDrawRectangle(0, 0, 1024, 768);
        ofSetColor(255, 255, 255, 255);
        ofNoFill();
        ofDrawRectangle(-1, -1, 1026, 770);
        ofFill();
        bezier.draw();
        ofSetColor(180, 180, 180, 255);

        bezier.drawHelp();
        ofFill();
        ofSetColor(255, 255, 255, 255);
    }

    // -------------- screens --------------------
    screenManager.draw();

    ofSetColor(0);
    ofDrawBitmapString(help, 5, 15);
    ofSetColor(255);
    ofDrawBitmapString(help, 6, 16);

    // -------------- GUI --------------------
    //required to call this at beginning
    gui.begin();
    {
        // -------------- GUI Screens Part --------------------
        drawScreensGuiPart();

        if (!bezier.getActive()) {
            // -------------- GUI Slot to Screens Part --------------------
            drawScreenToSlotGuiPart();

            // -------------- GUI Slots Part --------------------
            drawSlotGuiPart();

            // -------------- GUI Scenes Part --------------------
            drawScenesGuiPart();

            // -------------- GUI Scenes Part --------------------
            drawDMXLightsGuiPart();
        }

    }

    gui.end();

}

// -----------------------------------

void ofApp::drawDMXLightsGuiPart() {
    ImGui::Begin("DMX Lights");

    if (ImGui::Button("Add")) {
        addDMXLight();
    }
    ImGui::End();

}

// -----------------------------------

void ofApp::addDMXLight() {

}
// -----------------------------------

void ofApp::drawScreenToSlotGuiPart() {
    ImGui::Begin("Slots To Screens");
    if (ImGui::Button("save")) {
        saveScreenToSlotSettings();
    }

    for (unsigned int i = 0; i < screenManager.getScreenNumber(); i++) {
        string nameofSlot = "Screen " + ofToString(i + 1);
        if (ImGui::TreeNode(nameofSlot.c_str())) {

            if (slotNumberForScreen[i]) {
                string contentOfSlot = "";
                contentOfSlot += "Slot " + ofToString(slotNumberForScreen[i]) + " | " + slots[slotNumberForScreen[i] - 1]->getInfo();

                ImGui::Text(contentOfSlot.c_str());
                if (targetSlotNumberForScreen[i] != slotNumberForScreen[i]) {
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.9, 0.2, 0, 1), "- Transition!");
                }
            }
            ImGui::SliderInt("slot Number", &targetSlotNumberForScreenUnApproved[i], 0, slots.size());

            if (targetSlotNumberForScreenUnApproved[i] && targetSlotNumberForScreen[i] != targetSlotNumberForScreenUnApproved[i]) {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.07, 1.0f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.07, 0.7f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.07, 0.7f, 0.5f));
                if (ImGui::Button("->:")) {
                    targetSlotNumberForScreen[i] = targetSlotNumberForScreenUnApproved[i];
                    startTransitionForScreen(i + 1);
                }
                ImGui::PopStyleColor(3);
                ImGui::SameLine();
                ImGui::Text(slots[targetSlotNumberForScreenUnApproved[i] - 1]->getInfo().c_str());
            }
            ImGui::SliderInt("fade time", &fadeTimeForScreen[i], 0, 5000);

            ImGui::TreePop();
        } else {
            ImGui::SameLine();
            string contentOfSlot = "";

            if (slotNumberForScreen[i]) {

                contentOfSlot += ": Slot " + ofToString(slotNumberForScreen[i]) + " | " + slots[slotNumberForScreen[i] - 1]->getInfo();
            }
            ImGui::TextColored(ImVec4(0.9, 0.9, 0.9, 1), contentOfSlot.c_str());
        }
    }
    ImGui::End();
}

// -----------------------------------

void ofApp::startTransitionForScreen(int screenNumber) {

    ofLogNotice("Transition screen " + ofToString(screenNumber) + " (index " + ofToString(screenNumber - 1) + ") started.");

    targetSlotNumberAlphaForScreen[screenNumber - 1] = 0;
    fadeTimerForScreen[screenNumber - 1] = ofGetElapsedTimeMillis();

}

// -----------------------------------

void ofApp::drawScenesGuiPart() {
    ImGui::Begin("Bilder");
    if (ImGui::Button("Neues Bild")) {
        addScene();
    }
    ImGui::SameLine();

    if (ImGui::Button("end Edit")) {
        actualScene = 0;
    }


    // Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style.
    for (unsigned int i = 0; i < scenes.size(); i++) {
        ImGui::PushID(i);

        if (ImGui::Button("edit")) {
            actualScene = i + 1;
            if (actualScene > scenes.size()) {
                actualScene = scenes.size();
            }
        }
        if (actualScene == i + 1) {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.6f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.7f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.8f, 0.8f));
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.8f, 0.8f));
        }
        ImGui::SameLine();
        if (ImGui::Button("go!")) {
            startScene(i + 1);
        }
        ImGui::PopStyleColor(3);




        if (actualScene == i + 1) {
            ImGui::SameLine();
            ImGui::PushItemWidth(100);
            if (ImGui::InputInt("", &tempSceneNumber[i])) {
                scenes[i]->setNumber(tempSceneNumber[i]);
            }
            ImGui::PopItemWidth();
        } else {
            ImGui::SameLine();
            string tempNum = ofToString(scenes[i]->getNumber());
            ImGui::Text(tempNum.c_str());
            ImGui::SameLine();
            string nameofScene = scenes[i]->getName();
            ImGui::Text(nameofScene.c_str());
        }
        if (actualScene == i + 1) {
            ImGui::SameLine();
            string temp = scenes[i]->getName();
            static char buf[32] = "new";
            strcpy(buf, temp.c_str());
            ImGui::PushItemWidth(250);

            if (ImGui::InputText("name", buf, IM_ARRAYSIZE(buf))) {
                scenes[i]->setName(ofToString(buf));
            }
            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button("save")) {

                saveSceneSettings(actualScene);
            }
        }


        ImGui::PopID();
    }
    ImGui::TextColored(ImVec4(0.9, 0.2, 0, 1), sceneMessage.c_str());

    ImGui::End();

}

// -----------------------------------

void ofApp::startScene(int sceneNumber) {
    ofLogNotice("start Scene Number " + ofToString(sceneNumber) + ": try to laod slot settings...");
    loadSlotSettings(sceneNumber);
	ofLogNotice("try to load screen to slot settings...");
    loadScreenToSlotSettings(sceneNumber);
	ofLogNotice("Vector tempScreenToSlot for sceneNumber " + ofToString(sceneNumber));
    vector <int> tempScreenToSlot = scenes[sceneNumber - 1]->getSlots();
    for (unsigned int i = 0; i < tempScreenToSlot.size(); i++) {
        targetSlotNumberForScreen[i] = tempScreenToSlot[i];
        //fadeTimeForScreen[i] = 500;
		ofLogNotice("startTransitionForScreen " + ofToString(i+1));
        startTransitionForScreen(i + 1);
    }

}

// -----------------------------------

void ofApp::addScene() {

    oeScene* scene = new oeScene();
    scene->addScene(slotNumberForScreen);
    scene->setNumber(sceneCounter);
    tempSceneNumber.push_back(sceneCounter);
    scenes.push_back(scene);
    tempSceneName.push_back("new");
    sceneCounter += 10;
}

// -----------------------------------

void ofApp::drawScreensGuiPart() {

    ImGui::Begin("Screens");

    if (ImGui::CollapsingHeader("Add screen")) {
        ImGui::InputInt4("Pos/Size", screenCreatePositionAndSize);
        ImGui::InputInt3("Grid", screenCreateGrid);
        if (ImGui::Button("Add Screen")) {
            addScreen(ofPoint(screenCreatePositionAndSize[0], screenCreatePositionAndSize[1]), screenCreatePositionAndSize[2], screenCreatePositionAndSize[3], screenCreateGrid[0], screenCreateGrid[1], screenCreateGrid[2]);
        }
    }

    if (ImGui::CollapsingHeader("adjust")) {
        if (!screenManager.actualScreen) {
            // -------------- GUI show Mode Part --------------------
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0, 1.0f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0, 0.7f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0, 0.7f, 0.5f));

        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.3f, 1.0f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.3f, 0.7f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.3f, 0.7f, 0.5f));
        }
        if (ImGui::Button("Edit/Show")) {
            screenManager.actualScreen = 0;
            ofLogNotice("actual Index set to " + ofToString(screenManager.actualScreen));
        }
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        if (!screenManager.actualScreen) {
            ImGui::Text("SHOW MODE!");
        } else {
            ImGui::Text("Edit mode");
        }

        ImGui::SameLine();
        if (ImGui::Button("save")) {
            saveScreenSettings();
        }

        // -------------- GUI screens Part --------------------

        // Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style.
        for (unsigned int i = 0; i < screenManager.getScreenNumber(); i++) {
            if (i > 0) ImGui::SameLine();
            ImGui::PushID(i);
            if (screenManager.actualScreen == i + 1) {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.6f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.7f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.8f, 0.8f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.8f, 0.8f));
            }
            if (ImGui::Button("sc")) {
                screenManager.actualScreen = i + 1;
                screenManager.activeScreen = ofToString(screenManager.actualScreen);
                if (screenManager.actualScreen > screenManager.getScreenNumber()) {
                    screenManager.actualScreen = screenManager.getScreenNumber();
                }
                ofLogNotice("actual Index set to " + ofToString(screenManager.actualScreen));
            };
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
        for (unsigned int i = 0; i < screenManager.getScreenNumber(); i++) {
            if (i > 0) ImGui::SameLine();
            ImGui::PushID(i);
            if (screenManager.getGridState(i + 1)) {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0, 1.0f, 0.5f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.6f, 0.4f));
            }
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.7f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV((i + 6) / 19.0f, 0.8f, 0.7f));
            if (ImGui::Button("gr")) {
                screenManager.toggleGrid(i + 1);
                ofLogVerbose("toggled grid for screen # " + ofToString(screenManager.actualScreen));
            };
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
        // adjust position and size for active screen
        if (screenManager.actualScreen) {
            ImGui::PushItemWidth(100);
            ofPoint tempPoint = screenManager.getPosition(screenManager.actualScreen);
            screenCreatePositionAndSize[0] = tempPoint.x;
            screenCreatePositionAndSize[1] = tempPoint.y;
            if (ImGui::InputInt("X", &screenCreatePositionAndSize[0])) {
                screenManager.setPosition(screenManager.actualScreen, ofPoint(screenCreatePositionAndSize[0], screenCreatePositionAndSize[1]));
            }
            ImGui::SameLine();
            if (ImGui::InputInt("Y", &screenCreatePositionAndSize[1])) {
                screenManager.setPosition(screenManager.actualScreen, ofPoint(screenCreatePositionAndSize[0], screenCreatePositionAndSize[1]));
            }
            screenCreatePositionAndSize[2] = screenManager.getScreenPresentationWidth(screenManager.actualScreen);
            screenCreatePositionAndSize[3] = screenManager.getScreenPresentationHeight(screenManager.actualScreen);
            if (ImGui::InputInt("W", &screenCreatePositionAndSize[2])) {
                screenManager.setScreenPresentationWidth(screenManager.actualScreen, screenCreatePositionAndSize[2]);
            }
            ImGui::SameLine();
            if (ImGui::InputInt("H", &screenCreatePositionAndSize[3])) {
                screenManager.setScreenPresentationHeight(screenManager.actualScreen, screenCreatePositionAndSize[3]);
            }
            ImGui::PopItemWidth();
        }

        if (ImGui::Checkbox("SoloMode", &actualScreenSoloMode)) {
            screenManager.actualScreenSoloMode = actualScreenSoloMode;
        }
        ImGui::SameLine();
        if (ImGui::Checkbox("Grid Move", &showGridOnControlScreen)) {
            screenManager.showGridOnControlScreenChanged(showGridOnControlScreen);
        }
        ImGui::SameLine();
        if (ImGui::Button("To Front")) {
            if (screenManager.actualScreen > 0) {
                screenManager.moveScreenToFront(screenManager.actualScreen);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("reset warp grid(!)")) {
            if (screenManager.actualScreen > 0) {
                screenManager.resetWarpGrid(screenManager.actualScreen);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("info (to log)")) {
            screenManager.info();
        }
        if (screenManager.actualScreen > 0) {
            if (ImGui::Button("load Grid")) {
                screenManager.loadWarpSettings(0);
            }
            if (ImGui::Button("save Grid")) {
                screenManager.saveWarpSettings();
            }
        }

    }
    if (ImGui::CollapsingHeader("remove screen")) {
        ImGui::InputInt("screenNumber", &removeScreenNumber);
        if (ImGui::Button("r!")) {
            removeScreen(removeScreenNumber);
            removeScreenNumber = 0;
        }
    }


    // -------------- GUI Mask Part --------------------

    if (ImGui::CollapsingHeader("Mask")) {
        if (ImGui::TreeNode("Bezier Shapes")) {
            if (ImGui::Button("Toggle Mask Editor")) {
                bezier.setActive(!bezier.getActive());
            }
            if (ImGui::BeginMenu("load")) {
                //go through and print out all the paths
                for (unsigned int i = 0; i < bezierDir.size(); i++) {
                    //const char * c = bezierDir.getPath(i).c_str();
                    if (ImGui::MenuItem(bezierDir.getPath(i).c_str())) {
                        bezier.loadBezier(bezierDir.getPath(i));
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::Text("To save actual Mask: 's' in Mask Mode");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Pixel Mask")) {
            if (bezier.getActive()) {
                ImGui::SliderInt("blur", &maskBlur, 0, 100);
                ImGui::SameLine();
                if (ImGui::Button("save as Pixel Mask")) {
                    bezier.saveMask(maskBlur);
                    ofSleepMillis(500);
                    listMaskDirectory();
                }
            }
            if (screenManager.actualScreen > 0) {
                if (ImGui::BeginMenu("load Mask to actual screen")) {
                    //go through and print out all the paths
                    for (unsigned int i = 0; i < maskDir.size(); i++) {
                        //const char * c = maskDir.getPath(i).c_str();
                        if (ImGui::MenuItem(maskDir.getPath(i).c_str())) {
                            ofImage tempMask = maskDir.getPath(i);
                            masks[screenManager.actualScreen - 1] = tempMask;
                            masks[screenManager.actualScreen - 1].setImageType(OF_IMAGE_COLOR_ALPHA);
                            masks[screenManager.actualScreen - 1].getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);
                            maskPaths[screenManager.actualScreen - 1] = maskDir.getPath(i);
                        }
                    }
                    ImGui::EndMenu();
                }
            }
            ImGui::TreePop();
        }
    }

    if (ImGui::CollapsingHeader("Misc.")) {

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        if (ImGui::Button("Test Window")) {
            show_test_window = !show_test_window;
        }
        if (show_test_window) {

            ImGui::SetNextWindowPos(ofVec2f(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        } //required to call this at end
    }

    ImGui::End();

}


// -----------------------------------

void ofApp::drawSlotGuiPart() {
    ImGui::Begin("Slots");
    if (ImGui::Button("Add Slot")) {
        addSlot();
    }

    ImGui::SameLine();
    if (ImGui::Button("save")) {
        saveSlotSettings();
    }
    for (unsigned int i = 0; i < slots.size(); i++) {
        string addActiveMark = "";
        if (slots[i]->getIsActive()) {
            addActiveMark = " !";
        }
        string nameofSlot = "Slot " + ofToString(i + 1) + addActiveMark;
        slotTypes[i] = slots[i]->type;
        if (ImGui::TreeNode(nameofSlot.c_str())) {
            ImGui::Columns(2, "columns");


            ImGui::PushID(i);
            ImGui::RadioButton("Color", &slotTypes[i], 0);
            ImGui::SameLine();
            ImGui::RadioButton("Image", &slotTypes[i], 1);
            ImGui::SameLine();
            ImGui::RadioButton("Video", &slotTypes[i], 2);
            if (slotTypes[i] != slots[i]->getSlotType()) {
                ofLogNotice("Slot " + ofToString(i + 1) + " set To type " + ofToString(slotTypes[i]));
                slots[i]->setSlotType(slotTypes[i]);
            }

            if (slots[i]->getIsActive()) {
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(1.0f, 0.6f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(1.0f, 0.6f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(1.0f, 0.6f, 0.8f));
                ImGui::Button("-running-");
                ImGui::PopStyleColor(3);
            }

            // ------- Color -----
            if (slotTypes[i] == 0) {
                ImGui::PushItemWidth(150);
                slotColors[i].x = slots[i]->color.r;
                slotColors[i].y = slots[i]->color.g;
                slotColors[i].z = slots[i]->color.b;
                if (ImGui::ColorPicker3("Color", (float*) &slotColors[i], ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs)) {
                    ofFloatColor tempColor;

                    tempColor = ofFloatColor(slotColors[i].x, slotColors[i].y, slotColors[i].z);
                    //ofLogNotice("color changed to " + ofToString(tempColor));
                    slots[i]->color = tempColor;
                }
                ImGui::PopItemWidth();

                // ------- Image -----                    
            } else if (slotTypes[i] == 1) {
                ImGui::PushItemWidth(250);

                if (ImGui::BeginMenu("load Image")) {
                    //go through and print out all the paths
                    for (unsigned int j = 0; j < imageDir.size(); j++) {
                        if (ImGui::MenuItem(imageDir.getPath(j).c_str())) {
                            slots[i]->loadImage(imageDir.getPath(j));
                        }
                    }
                    ImGui::EndMenu();
                }

                //                string tempstring = "LOADED: " + ofToString(slots[i]->imagePath);
                //                ImGui::Text(tempstring.c_str());

                //ofFbo tempFbo = slots[i]->getContent();
                // ------- Video -----                    
            } else if (slotTypes[i] == 2) {
                ImGui::PushItemWidth(250);

                if (ImGui::BeginMenu("load Video")) {
                    //go through and print out all the paths
                    for (unsigned int j = 0; j < videoDir.size(); j++) {
                        if (ImGui::MenuItem(videoDir.getPath(j).c_str())) {
                            slots[i]->loadVideo(videoDir.getPath(j));
                        }
                    }
                    ImGui::EndMenu();

                }
                ImGui::PopItemWidth();
                bool tempState = videoPaused[i];
                if (ImGui::Checkbox("paused", &tempState)) {
                    videoPaused[i] = tempState;
                    slots[i]->setVideoPaused(videoPaused[i]);
                }
                ImGui::SameLine();
                bool tempState2 = videoLooping[i];
                if (ImGui::Checkbox("loop", &tempState2)) {
                    videoLooping[i] = tempState2;
                    if (videoLooping[i]) {
                        slots[i]->setVideoLoopState(OF_LOOP_NORMAL);
                    } else {
                        slots[i]->setVideoLoopState(OF_LOOP_NONE);
                    }
                }
                ImGui::SameLine();
				if (ImGui::Button("speed to 1")) {
					videoSpeed[i] = 1.0;
					slots[i]->setVideoSpeed(videoSpeed[i]);
				}
				ImGui::SameLine();
                // TODO: check if there is a flash, find better approach in restartVideo() if yes
                if (ImGui::Button("rewind")) {
                    slots[i]->restartVideo();
                }
                ImGui::PushItemWidth(350);

				float tempPos = slots[i]->getVideoPosition()*100;
                if (ImGui::SliderFloat("scrub", &tempPos, 0, 100)) {
                    slots[i]->setVideoPosition(tempPos);
                }
				if (ImGui::SliderFloat("speed", &videoSpeed[i], 0, 30)) {
					slots[i]->setVideoSpeed(videoSpeed[i]);
				}
				ImGui::PopItemWidth();

                // ImGui::Image()
            }

			ImGui::PushItemWidth(350);
			int tempFadeTime = slots[i]->fadeTimeForSlot;				
			if (ImGui::SliderInt("fade time", &tempFadeTime, 0, 3600)) {
				slots[i]->fadeTimeForSlot = tempFadeTime;
			}
			ImGui::PopItemWidth();

			ImGui::PushItemWidth(90);
			float tempBrightness = slots[i] ->brightness;
			float tempContrast = slots[i]->contrast;
			float tempSaturation = slots[i]->saturation;
			if (ImGui::SliderFloat("br", &tempBrightness, 0, 3.0)) {
				slots[i]->brightness = tempBrightness;
			}
			ImGui::SameLine();
			if (ImGui::SliderFloat("co", &tempContrast, 0, 3.0)) {
				slots[i]->contrast = tempContrast;
			}
			ImGui::SameLine();
			if (ImGui::SliderFloat("sa", &tempSaturation, 0, 3.0)) {
				slots[i]->saturation = tempSaturation;
			}
			ImGui::SameLine();
			if (ImGui::Button("->0")) {
				slots[i]->brightness = 1.0;
				slots[i]->contrast = 1.0;
				slots[i]->saturation = 1.0;
			}
				
				
			ImGui::PopItemWidth();

			string tempstring = "(loaded: " + ofToString(slots[i]->getInfo()) + ")";
            ImGui::Text(tempstring.c_str());

            ImGui::PopID();
            ImGui::NextColumn();
            ImGui::PushID(1000 + i);



            ImGui::RadioButton("Color", &targetSlotTypes[i], 0);
            ImGui::SameLine();
            ImGui::RadioButton("Image", &targetSlotTypes[i], 1);
            ImGui::SameLine();
            ImGui::RadioButton("Video", &targetSlotTypes[i], 2);

            if (targetSlotTypes[i] != slots[i]->getTargetSlotType()) {
                ofLogNotice("Slot " + ofToString(i + 1) + " set To type " + ofToString(targetSlotTypes[i]));
                slots[i]->setTargetSlotType(targetSlotTypes[i]);
            }

            // ------- Target Color -----
            if (targetSlotTypes[i] == 0) {
                ImGui::PushItemWidth(150);

                if (ImGui::ColorPicker3("Color", (float*) &targetSlotColors[i], ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs)) {
                    ofFloatColor tempColor;

                    tempColor = ofFloatColor(targetSlotColors[i].x, targetSlotColors[i].y, targetSlotColors[i].z);
                    //ofLogNotice("color changed to " + ofToString(tempColor));
                    slots[i]->targetColor = tempColor;
                }
                ImGui::PopItemWidth();

                // ------- Target Image -----                    
            } else if (targetSlotTypes[i] == 1) {

                if (ImGui::BeginMenu("load target Image")) {
                    //go through and print out all the paths
                    for (unsigned int j = 0; j < imageDir.size(); j++) {
                        if (ImGui::MenuItem(imageDir.getPath(j).c_str())) {
                            slots[i]->loadTargetImage(imageDir.getPath(j));
                        }
                    }
                    ImGui::EndMenu();
                }
                //                string tempstring = "TARGET loaded: " + ofToString(slots[i]->targetImagePath);
                //                ImGui::Text(tempstring.c_str());


                // ------- Target Video -----                    

            } else if (targetSlotTypes[i] == 2) {

                if (ImGui::BeginMenu("load target Video")) {
                    //go through and print out all the paths
                    for (unsigned int j = 0; j < videoDir.size(); j++) {
                        if (ImGui::MenuItem(videoDir.getPath(j).c_str())) {
                            slots[i]->loadTargetVideo(videoDir.getPath(j));
                        }
                    }
                    ImGui::EndMenu();

                }
				bool tempRewindOnTransition = slots[i]->rewindOnTransition;
				if (ImGui::Checkbox("rewind on Transiition", &tempRewindOnTransition)) {
					slots[i]->rewindOnTransition = tempRewindOnTransition;
				}

            }

            tempstring = "TARGET: " + ofToString(slots[i]->getTargetInfo());
            ImGui::Text(tempstring.c_str());

            // change To button
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);

            if (slots[i]->targetAlpha < 255) {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(1.0f, 0.8f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(1.0f, 0.8f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(1.0f, 0.8f, 0.8f));
                ImGui::Button("<-");
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.9, 0.2, 0, 1), "- Transition!");
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.4f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.4f, 0.7f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.4f, 0.8f, 0.8f));
                if (ImGui::Button("<-")) {
                    slots[i]->startTransitionForSlot();
                }
            }

            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar(1);

            ImGui::PopID();
            ImGui::Columns(1);
            ImGui::TreePop();
        } else {
            ImGui::SameLine();
            string contentOfSlot = "";

            if (slots[i]) {

                contentOfSlot += " | " + slots[i]->getInfo();
            }


            ImGui::TextColored(ImVec4(0.9, 0.9, 0.9, 1), contentOfSlot.c_str());
        }
        ImGui::Separator();

    }

    ImGui::End();

}


// -----------------------------------

void ofApp::addScreen(ofPoint position, int width, int height, int pointsX, int pointsY, int pixelsPerGridDivision) {
    ofFbo tempFbo;
    tempFbo.allocate(width, height, GL_RGB);
    contentFbos.push_back(tempFbo);
    screenManager.addScreen(position, width, height, pointsX, pointsY, pixelsPerGridDivision);

    ofImage tempMask;
    tempMask.load(settings.getStringValue("deafultMask"));

    masks.push_back(tempMask);
    maskPaths.push_back(settings.getStringValue("deafultMask"));

    slotNumberForScreen.push_back(0);
    targetSlotNumberForScreen.push_back(0);
    targetSlotNumberForScreenUnApproved.push_back(0);
    targetSlotNumberAlphaForScreen.push_back(0);
    fadeTimeForScreen.push_back(500);
    fadeTimerForScreen.push_back(0);

    for (unsigned int i = 0; i < masks.size(); i++) {
        masks[i].setImageType(OF_IMAGE_COLOR_ALPHA);
        masks[i].getTexture().setSwizzle(GL_TEXTURE_SWIZZLE_A, GL_RED);
    }
}

// -----------------------------------

void ofApp::removeScreen(int tempScreenNumber) {

    screenManager.removeScreen(tempScreenNumber);
    // TODO: check position. Remove screen on the middle, screen(s?) after can't be moved any more

    contentFbos.erase(contentFbos.begin() + (tempScreenNumber - 1));
    masks.erase(masks.begin() + (tempScreenNumber - 1));
    maskPaths.erase(maskPaths.begin() + (tempScreenNumber - 1));
    slotNumberForScreen.erase(slotNumberForScreen.begin() + (tempScreenNumber - 1));
    targetSlotNumberForScreen.erase(targetSlotNumberForScreen.begin() + (tempScreenNumber - 1));
    targetSlotNumberForScreenUnApproved.erase(targetSlotNumberForScreenUnApproved.begin() + (tempScreenNumber - 1));
    targetSlotNumberAlphaForScreen.erase(targetSlotNumberAlphaForScreen.begin() + (tempScreenNumber - 1));
    fadeTimeForScreen.erase(fadeTimeForScreen.begin() + (tempScreenNumber - 1));

}

// -----------------------------------

void ofApp::addSlot() {

    oeSlot* slot = new oeSlot();
    slotTypes.push_back(1);
    targetSlotTypes.push_back(1);
    slot->init(settings.getIntValue("contentWidth"), settings.getIntValue("contentHeight"));
    slot->targetAlpha = 255;
    slot->fadeTimeForSlot = 1200;
    slots.push_back(slot);
    slotColors.push_back(startSlotColor);
    targetSlotColors.push_back(startSlotColor);
    videoPaused.push_back(false);
    videoLooping.push_back(true);
    videoSpeed.push_back(1.0);
    targetVideoPaused.push_back(false);
    targetVideoLooping.push_back(true);
    targetVideoSpeed.push_back(1.0);

}


//--------------------------------------------------------------

void ofApp::keyPressed(int key) {

    if (bezier.getActive()) {
        // if bezier mangager (mask generation) is NOT active: default mode
    } else {
        switch (key) {

            case 'i':
                screenManager.info();
                break;

            case OF_KEY_LEFT:
            case OF_KEY_RIGHT:
            case OF_KEY_UP:
            case OF_KEY_DOWN:
                ofLogNotice("actualScreen is " + ofToString(screenManager.actualScreen));
                ofLogNotice("key in ofApp");
                if (screenManager.actualScreen > 0) {

                    screenManager.keyPressed(key);
                }
                break;
        }
    }
}


//--------------------------------------------------------------

void ofApp::resetFlashMessages() {

    sceneMessage = "";
}


//--------------------------------------------------------------

void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------

void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------

void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------

void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------

void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------

void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------

void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------

void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------

void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------

void ofApp::dragEvent(ofDragInfo dragInfo) {

}


