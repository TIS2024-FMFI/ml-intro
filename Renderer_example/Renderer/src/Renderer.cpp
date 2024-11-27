#include "Renderer.h"




Renderer::Renderer(FrameBuffer *fb, Camera *cam)
	: m_frameBuffer(fb), m_camera(cam) 
{
	setupFullscreenQuad();
	loadNN(ModelData("examples/test/data.json"));
	shaderProgram = createShaderProgram("resources/shaders/pointShader.vert", "resources/shaders/pointShader.frag");
	screenShaderProgram = createShaderProgram("resources/shaders/screenShader.vert", "resources/shaders/screenShader.frag");
}

void Renderer::loadNN(ModelData model) {
	nOfVerticies = model.numberOfVerticies();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nOfVerticies * sizeof(vec3), &model.getVerticiesPositions()[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::renderScene() {
	test += 0.002;
	m_camera->SetCameraView(vec3(sin(test) * 20., .0, cos(test) * 20.), vec3(.0));
	m_camera->UpdateProjMatrix();


	m_frameBuffer->Bind();

	useShaderProgram();

	m_frameBuffer->Unbind();

	RenderToScreen();
}

void Renderer::useShaderProgram() {
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);


	glUseProgram(shaderProgram);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, value_ptr(m_camera->GetViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, value_ptr(m_camera->GetProjMatrix()));
	glUniform1f(glGetUniformLocation(shaderProgram, "pointSize"), 500.f);
	glUniform3f(glGetUniformLocation(shaderProgram, "color"), 0.5f, 0.7f, 1.0f);


	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, nOfVerticies);
	glBindVertexArray(0);
}

std::string Renderer::loadShaderSource(const char* filePath) {
	std::ifstream shaderFile(filePath);
	std::stringstream shaderStream;

	if (shaderFile) {
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
	}
	else {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filePath << std::endl;
	}

	return shaderStream.str();
}

GLuint Renderer::compileShader(GLenum type, const std::string& source) {
	GLuint shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	// Check for compilation errors
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
}

GLuint Renderer::createShaderProgram(const char* vertexPath, const char* fragmentPath) {
	std::string vertexSource = loadShaderSource(vertexPath);
	std::string fragmentSource = loadShaderSource(fragmentPath);

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}



void Renderer::RenderToScreen() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(screenShaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_frameBuffer->getFrameTexture());
	glUniform1i(glGetUniformLocation(shaderProgram, "screenTexture"), 0);

	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::setupFullscreenQuad() {
	float quadVertices[] = {
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
	};

	unsigned int quadIndices[] = {
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadEBO);

	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

}