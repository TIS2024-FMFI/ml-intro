#pragma once
#include "FrameBuffer.h"
#include "Camera.h"
#include "ModelData.h"
#include "Label.h"
#include <fstream>
#include <sstream>
#include <iostream>

class Renderer {
public:
	Renderer(FrameBuffer* fb, Camera* cam);
	void loadNN(ModelData model);
	void renderScene();
	GLuint getFrameTexture() { return m_frameBuffer->getFrameTexture(); }

	void updateWeights(vector<vector<float>> newWeights) { weights = newWeights; }
	void updateActivations(vector<vector<float>> newActivations) { layers = newActivations; }

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

private:
	GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);
	GLuint compileShader(GLenum type, const std::string& source);
	std::string loadShaderSource(const char* filePath);
	void useShaderProgram();
	void RenderToScreen();
	void setupFullscreenQuad();

	vec4 activationToColor(float activation) {
		return vec4(clamp(-activation, 0.0f, 1.0f), clamp(activation, 0.0f, 1.0f), .0f, 1.0f);
	}

	vec4 weightToColor(float weight) {
		return vec4(clamp(-weight, 0.0f, 1.0f), clamp(weight, 0.0f, 1.0f), .0f, 1.0f);
	}

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
	FrameBuffer *m_frameBuffer;
	Camera *m_camera;

	int nOfVerticies;
	GLuint shaderProgram;
	GLuint edgeShaderProgram;
	GLuint screenShaderProgram;
	GLuint basicShaderProgram;
	GLuint pointVAO, pointVBO;

	//GLuint edgeVAO, edgeVBO;
	vector<pair<vec3, vec3>> edges;
	vector<vec3> nodes;

	Label label = Label(m_camera);

	//for debuging purposes
	GLuint quadVAO, quadVBO, quadEBO;
	vector<vector<float>> layers;
	vector<vector<float>> weights;

	GLuint vertVBO, vertVAO;
	GLuint edgeVAO, edgeVBO;
};