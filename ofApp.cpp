#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(0);

	ofEnableDepthTest();
	ofSetLineWidth(1);

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->line.clear();

	auto noise_param = glm::vec3(ofRandom(360), ofRandom(360), ofRandom(360));

	for (int radius = 50; radius <= 300; radius += 5) {

		auto rotation = glm::vec3(
			ofMap(ofNoise(noise_param.x, radius * 0.005 - ofGetFrameNum() * 0.03), 0, 1, -30, 30),
			ofMap(ofNoise(noise_param.y, radius * 0.005 - ofGetFrameNum() * 0.03), 0, 1, -30, 30),
			ofMap(ofNoise(noise_param.z, radius * 0.005 - ofGetFrameNum() * 0.03), 0, 1, -30, 30));
	
		auto rotation_x = glm::rotate(glm::mat4(), rotation.x * (float)DEG_TO_RAD, glm::vec3(1, 0, 0));
		auto rotation_y = glm::rotate(glm::mat4(), rotation.y * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
		auto rotation_z = glm::rotate(glm::mat4(), rotation.z * (float)DEG_TO_RAD, glm::vec3(0, 0, 1));

		int start_index = this->line.getNumVertices();
		int index = start_index;
		for (int deg = 0; deg < 360; deg += 1) {

			auto vertex = glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0);

			if (radius % 100 != 0) {

				vertex = glm::vec4(vertex, 0) * rotation_x * rotation_y;
			}

			this->face.addVertex(vertex);
			this->line.addVertex(vertex);

			if (deg > 0) {

				this->line.addIndex(index - 1);
				this->line.addIndex(index);
			}

			index++;
		}

		this->line.addIndex(this->line.getNumVertices() - 1);
		this->line.addIndex(start_index);
	}

	int index_span = 1;
	for (int index = 0; index < this->face.getNumVertices() - 361; index += index_span) {

		this->face.addIndex(index); this->face.addIndex(index + 1); this->face.addIndex(index + 361);
		this->face.addIndex(index); this->face.addIndex(index + 361); this->face.addIndex(index + 360);
	}

	index_span = 30;
	for (int index = 0; index < this->line.getNumVertices() - 360; index += index_span) {

		this->line.addIndex(index);
		this->line.addIndex(index + 360);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateZ(ofGetFrameNum());

	ofSetColor(0);
	this->face.draw();

	ofSetColor(255);
	this->line.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}