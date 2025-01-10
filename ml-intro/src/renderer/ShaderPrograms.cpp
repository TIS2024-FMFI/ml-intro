#include "ShaderPrograms.h"


ShaderPrograms::ShaderPrograms() {
	NodeShaderProgram = createShaderProgram("resources/shaders/testShader.vert", "resources/shaders/testShader.frag");
	/*todo: other shaders*/
}

ShaderPrograms::~ShaderPrograms() {
	glDeleteProgram(NodeShaderProgram);
}


std::string ShaderPrograms::loadShaderSource(const char* filePath) {
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


GLuint ShaderPrograms::compileShader(GLenum type, const std::string& source) {
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

GLuint ShaderPrograms::createShaderProgram(const char* vertexPath, const char* fragmentPath) {
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