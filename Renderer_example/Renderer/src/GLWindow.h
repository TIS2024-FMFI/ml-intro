#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//kvoli statickemu callbacku potrebujeme mat v hlavnom okne framebuffer a Cameru . . . inac by okno nereagovalo pocas nasich inputov
#include "Camera.h"
#include "FrameBuffer.h"


class GLWindow
{
public:
	GLWindow();
	~GLWindow();

	float WIDTH = 1200.0f;
	float HEIGHT = 720.0f;
	GLFWwindow* window;

	void InitCamera(Camera* camera, FrameBuffer* frameBuffer);

	static void window_size_callback_static(GLFWwindow* window, int width, int height);
	void window_size_callback(GLFWwindow* window, int width, int height);
private:

	Camera* m_camera;
	FrameBuffer* m_frameBuffer;
};