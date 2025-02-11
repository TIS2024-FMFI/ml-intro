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
	void updateNN(vector<Eigen::MatrixXd> activations, vector<Eigen::MatrixXd> weights);
	void renderScene();
	GLuint getFrameTexture() { return frameBuffer->getFrameTexture(); }

	void SquareRender() { squareRender = true; };
	void PlaneRender() { squareRender = false; };
	bool isSquareRender() { return squareRender; }

	void setText(bool doRender) { renderText = doRender; };
	bool isEnabled() { return renderText; }
	void renderLabels();

	vec3 negative = vec3(1, 0, 0);
	vec3 positive = vec3(0, 1, 0);
	float layerSpacing = 2.0f;
	float nodeSpacing = 1.0f;
	bool squareRender = false;

private:
	Renderer();
	~Renderer() {}

	void generatePositions(vector<Eigen::MatrixXd>);
	void createVertices();
	void createEdges();
	void setupLabels(vector<Eigen::MatrixXd>, vector<Eigen::MatrixXd>);
	void uploadToBuffers();
	vector<float> unzipVec(vector<Eigen::MatrixXd>);


	FrameBuffer* frameBuffer;
	Camera* camera;

	bool initialized = false;
	bool renderText = false;

	//temp
	//vector<pair<vec3, vec3>> edges;
	//vector<vec3> nodes;

	vector<vector<vec3>> vertPos;
	vector<Label> labels;

};
