#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

class FrameBuffer
{
public:
	FrameBuffer(float width, float height);
	~FrameBuffer();
	GLuint getFrameTexture() { return texture; };
	void RescaleFrameBuffer(float width, float height);
	void Bind() const;
	void Unbind() const;

	glm::vec3 bcg = glm::vec3(.7, .75, .8);
private:
	GLuint fbo;
	GLuint texture;
	GLuint rbo;
};
