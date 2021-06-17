#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key) {}
	void keyReleased(int key) {}
	void mouseMoved(int x, int y) {}
	void mouseDragged(int x, int y, int button) {}
	void mousePressed(int x, int y, int button) {}
	void mouseReleased(int x, int y, int button) {}
	void mouseEntered(int x, int y) {}
	void mouseExited(int x, int y) {}
	void windowResized(int w, int h) {}
	void dragEvent(ofDragInfo dragInfo) {}
	void gotMessage(ofMessage msg) {}

	ofxBox2d box2d;
	float radius;
	vector<shared_ptr<ofxBox2dCircle>> circle_list;
	vector<vector<glm::vec2>> log_list;
	vector<float> life_list;
	vector<ofColor> color_list;

	ofImage image;
	ofFbo fbo;
};