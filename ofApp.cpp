#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);

	this->box2d.init();
	this->box2d.setGravity(0, 50);
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();

	this->radius = 15;

	this->image.loadImage("images/lenna.png");
	this->image.resize(720, 720);
	this->fbo.allocate(720, 720);
}

//--------------------------------------------------------------
void ofApp::update() {

	vector<ofColor> base_color = { ofColor(255, 255, 0), ofColor(255, 0, 255), ofColor(0, 255, 255) };

	if (ofGetFrameNum() % 1 == 0) {

		auto circle = make_shared<ofxBox2dCircle>();
		circle->setPhysics(0.5, 0.83, 0.05);
		circle->setup(this->box2d.getWorld(), ofRandom(150, ofGetWidth() - 150), this->radius, this->radius);
		this->circle_list.push_back(circle);

		vector<glm::vec2> log;
		this->log_list.push_back(log);
		this->life_list.push_back(80);
		this->color_list.push_back(base_color[(int)ofRandom(base_color.size())]);
	}

	this->box2d.update();

	for (int i = this->circle_list.size() - 1; i > -1; i--) {

		this->log_list[i].push_back(glm::vec2(this->circle_list[i]->getPosition().x, this->circle_list[i]->getPosition().y));
		while (this->log_list[i].size() > 20) {

			this->log_list[i].erase(this->log_list[i].begin());
		}

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->circle_list.erase(this->circle_list.begin() + i);
			this->log_list.erase(this->log_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}
	}

	this->fbo.begin();
	ofClear(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	int head_size = this->radius;
	for (int i = 0; i < this->circle_list.size(); i++) {

		if (this->log_list[i].size() < 2) continue;

		if (this->life_list[i] < 15) {
			
			ofSetColor(this->color_list[i], ofMap(this->life_list[i], 0, 15, 0, 255));
		}
		else {

			ofSetColor(this->color_list[i]);
		}

		ofMesh mesh;
		vector<glm::vec3> right, left, frame;
		glm::vec3 last_location;
		float last_theta;

		for (int k = 0; k < this->log_list[i].size() - 1; k++) {

			auto location = glm::vec3(this->log_list[i][k], 0);
			auto next = glm::vec3(this->log_list[i][k + 1], 0);

			auto direction = next - location;
			auto theta = atan2(direction.y, direction.x);

			right.push_back(location + glm::vec3(ofMap(k, 0, this->log_list[i].size(), 0.1, head_size) * cos(theta + PI * 0.5), ofMap(k, 0, this->log_list[i].size(), 0.1, head_size) * sin(theta + PI * 0.5), 0));
			left.push_back(location + glm::vec3(ofMap(k, 0, this->log_list[i].size(), 0.1, head_size) * cos(theta - PI * 0.5), ofMap(k, 0, this->log_list[i].size(), 0.1, head_size) * sin(theta - PI * 0.5), 0));

			last_location = location;
			last_theta = theta;
		}

		for (int k = 0; k < right.size(); k++) {

			mesh.addVertex(left[k]);
			mesh.addVertex(right[k]);
		}

		for (int k = 0; k < mesh.getNumVertices() - 2; k += 2) {

			mesh.addIndex(k + 0); mesh.addIndex(k + 1); mesh.addIndex(k + 3);
			mesh.addIndex(k + 0); mesh.addIndex(k + 2); mesh.addIndex(k + 3);
		}

		auto tmp_head_size = ofMap(log_list[i].size() - 2, 0, log_list[i].size(), 0.1, head_size);
		mesh.addVertex(last_location);

		int index = mesh.getNumVertices();
		for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

			mesh.addVertex(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
		}

		for (int i = index; i < mesh.getNumVertices() - 1; i++) {

			mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
		}

		mesh.draw();
	}

	ofDisableBlendMode();
	this->fbo.end();
	
	this->image.getTextureReference().setAlphaMask(this->fbo.getTexture());
	this->image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255);
	this->image.draw(0, 0);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}