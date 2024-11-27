#pragma once
#include "FrameBuffer.h"
#include "Camera.h"
#include "ModelData.h"
#include <fstream>
#include <sstream>
#include <iostream>

class Renderer {
public:
	Renderer(FrameBuffer* fb, Camera* cam);
	void loadNN(ModelData model);
	void renderScene();
	GLuint getFrameTexture() { return m_frameBuffer->getFrameTexture(); }
private:
	GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);
	GLuint compileShader(GLenum type, const std::string& source);
	std::string loadShaderSource(const char* filePath);
	void useShaderProgram();
	void RenderToScreen();
	void setupFullscreenQuad();

	float test = 0;
	FrameBuffer *m_frameBuffer;
	Camera *m_camera;

	int nOfVerticies;
	GLuint shaderProgram;
	GLuint screenShaderProgram;
	GLuint VAO;
	GLuint VBO;

	//for debuging purposes
	GLuint quadVAO, quadVBO, quadEBO;
};