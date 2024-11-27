#include "GLWindow.h"


GLWindow::GLWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "NNV", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	//not necessary
	//glfwSetCursorPos(window, (WIDTH / 2), (HEIGHT / 2));

	//window bude mat ulozeny pointer na tuto classu a mozeme ho potom zavolat v Callback metodach
	glfwSetWindowUserPointer(window, this);

	//aby sa render updatnul pocas rescalovania okna
	glfwSetWindowSizeCallback(window, window_size_callback_static);

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
}

GLWindow::~GLWindow()
{
	std::cout << "Destroyed Window" << std::endl;
	glfwDestroyWindow(window);
	glfwTerminate();
}

void GLWindow::InitCamera(Camera* camera, FrameBuffer* frameBuffer)
{
	m_camera = camera;
	m_frameBuffer = frameBuffer;
}

void GLWindow::window_size_callback_static(GLFWwindow* window, int width, int height)
{
	GLWindow* that = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
	that->window_size_callback(window, width, height);
}

void GLWindow::window_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	m_frameBuffer->RescaleFrameBuffer(width, height);
}