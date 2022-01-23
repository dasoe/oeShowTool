#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofAppGLFWWindow window;
	window.setMultiDisplayFullscreen(true);
	//window.setVerticalSync(true);
	ofSetupOpenGL(&window, 1920, 1200, OF_FULLSCREEN); 			// <-------- setup the GL context  


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN OF_GAMEMODE
	// pass in width and height too:
	ofRunApp( new ofApp());

}
