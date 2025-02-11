#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


class MyWindow {
public:
	MyWindow(int width, int height) : width(width), height(height) 
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);

		window = glfwCreateWindow(width, height, "NNV", NULL, NULL);
		if (window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}

		glfwSetWindowUserPointer(window, this);

		//glfwSetWindowSizeCallback(window, window_size_callback_static);

		glfwMakeContextCurrent(window);
		gladLoadGL();

		glViewport(0, 0, width, height);
	}

	~MyWindow()
	{
		std::cout << "Window Closed" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	GLFWwindow *window;
	int width;
	int height;

};