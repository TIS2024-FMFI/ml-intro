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

using namespace std;
using namespace glm;

class Renderer {
public:
	Renderer(FrameBuffer* fb, Camera* cam);
	void loadNN(/*todo*/);
	void renderScene();
	GLuint getFrameTexture() { return m_frameBuffer->getFrameTexture(); }

	void testUpdate() {
		for (int y = 0; y < layers.size(); y++) {
			for (int x = 0; x < layers[y].size(); x++) {
				layers[y][x] = -layers[y][x];
			}
		}

		for (int y = 0; y < weights.size(); y++) {
			for (int x = 0; x < weights[y].size(); x++) {
				weights[y][x] = -weights[y][x];
			}
		}
	}

	static void SquareRender() { squareRender = true; };
	static void PlaneRender() { squareRender = false; };

	static void EnableText() { renderText = true; };
	static void DisableText() { renderText = false; };

private:
	void useShaderProgram();

	float getNthActivation(int n, vector<vector<float>> layers) {
		int currentId = 0;
		int currentLayer = 0;
		for (; currentId + layers[currentLayer].size() <= n; currentId += layers[currentLayer++].size());
		return layers[currentLayer][n - currentId];
	}

	float getNthWeight(int n, vector<vector<float>> weights) {
		int currentId = 0;
		int currentLayer = 0;
		for (; currentId + weights[currentLayer].size() <= n; currentId += weights[currentLayer++].size());
		return weights[currentLayer][n - currentId];
	}


	float test = 0;
	FrameBuffer* m_frameBuffer;
	Camera* m_camera;

	int nOfVerticies;

	vector<pair<vec3, vec3>> edges;
	vector<vec3> nodes;

	//for debuging purposes
	vector<vector<float>> layers;
	vector<vector<float>> weights;

	GLuint edgeVAO, edgeVBO;
	GLuint vertVAO, vertVBO;

	static bool renderText;
	static bool squareRender;
};
