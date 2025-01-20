#pragma once
#include "FrameBuffer.h"
#include "Camera.h"
#include "Label.h"
#include "Node.h"
#include "Edge.h"
#include "ShaderPrograms.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <Eigen/Dense>


using namespace std;
using namespace glm;

class Renderer {
public:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	static Renderer& getInstance() {
		static Renderer instance;
		return instance;
	}

	void Init(FrameBuffer* fb, Camera* cam);
	void loadNN(vector<Eigen::MatrixXd> activations, vector<Eigen::MatrixXd> weights);
	void renderScene();
	GLuint getFrameTexture() { return frameBuffer->getFrameTexture(); }

	void SquareRender() { squareRender = true; };
	void PlaneRender() { squareRender = false; };

	void setText(bool doRender) { renderText = doRender; };
	bool isEnabled() { return renderText; }
private:
	Renderer();
	~Renderer() {}


	FrameBuffer* frameBuffer;
	Camera* camera;

	bool initialized = false;
	bool renderText = false;
	bool squareRender = false;

	//temp
	vector<pair<vec3, vec3>> edges;
	vector<vec3> nodes;

};
