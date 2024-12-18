#include "Renderer.h"




Renderer::Renderer(FrameBuffer *fb, Camera *cam)
	: m_frameBuffer(fb), m_camera(cam) 
{
	setupFullscreenQuad();
	shaderProgram = createShaderProgram("resources/shaders/pointShader.vert", "resources/shaders/pointShader.frag");
	edgeShaderProgram = createShaderProgram("resources/shaders/edgeShader.vert", "resources/shaders/edgeShader.frag");
	screenShaderProgram = createShaderProgram("resources/shaders/screenShader.vert", "resources/shaders/screenShader.frag");

	glGenVertexArrays(1, &vertVAO);
	glGenBuffers(1, &vertVBO);

	glGenVertexArrays(1, &edgeVAO);
	glGenBuffers(1, &edgeVBO);

	glPointSize(25.0f);
	glLineWidth(5.0f);

	loadNN(ModelData("examples/test/data.json"));
}

void Renderer::loadNN(ModelData model) {
	layers = model.getActivations();
	weights = model.getWeights();

	float layerSpacing = 2.0f, nodeSpacing = 0.5f;

	vector<vec3> lastLayer = {};
	for (int i = 0; i < layers.size(); ++i) {
		vector<vec3> currentLayer = {};
		for (size_t j = 0; j < layers[i].size(); ++j) {
			vec3 nodePos = vec3((i - 1) * layerSpacing, j * nodeSpacing - layers[i].size() * nodeSpacing / 2.0f, 0.0f);
			nodes.emplace_back(nodePos);
			currentLayer.emplace_back(nodePos);
			if (i > 0) {
				for (size_t k = 0; k < layers[i - 1].size(); ++k) {
					edges.emplace_back(lastLayer[k], currentLayer[j]);
				}
			}
		}
		lastLayer = currentLayer;
	}

	glBindVertexArray(vertVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vertVBO);
	glBufferData(GL_ARRAY_BUFFER, nodes.size() * sizeof(vec3), nodes.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	vector<vec3> edgeVertices;
	for (const auto& edge : edges) {
		edgeVertices.push_back(edge.first);
		edgeVertices.push_back(edge.second);
	}

	glBindVertexArray(edgeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, edgeVBO);
	glBufferData(GL_ARRAY_BUFFER, edgeVertices.size() * sizeof(vec3), edgeVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Renderer::renderScene() {
	test += 0.002;
	m_camera->SetCameraView(vec3(sin(test) * 5., .0, cos(test) * 5.), vec3(.0));
	m_camera->UpdateProjMatrix();


	m_frameBuffer->Bind();


	useShaderProgram();

	m_frameBuffer->Unbind();

	RenderToScreen();
}

void Renderer::useShaderProgram() {
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	glDisable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(edgeShaderProgram);

	GLint viewLoc = glGetUniformLocation(edgeShaderProgram, "view");
	GLint projLoc = glGetUniformLocation(edgeShaderProgram, "projection");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(m_camera->GetViewMatrix()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(m_camera->GetProjMatrix()));

	glBindVertexArray(edgeVAO);
	for (size_t i = 0; i < edges.size(); ++i) {
		vec4 color = weightToColor(getNthWeight(i, weights));
		glUniform4fv(glGetUniformLocation(edgeShaderProgram, "color"), 1, value_ptr(color));
		glDrawArrays(GL_LINES, i * 2, 2);
	}
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	glUseProgram(shaderProgram);
	glEnable(GL_PROGRAM_POINT_SIZE);

	viewLoc = glGetUniformLocation(shaderProgram, "view");
	projLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(m_camera->GetViewMatrix()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(m_camera->GetProjMatrix()));
	glUniform1f(glGetUniformLocation(shaderProgram, "pointSize"), 200.f);

	glBindVertexArray(vertVAO);
	for (size_t i = 0; i < nodes.size(); ++i) {
		vec4 color = activationToColor(getNthActivation(i, layers));
		glUniform4fv(glGetUniformLocation(shaderProgram, "color"), 1, value_ptr(color));
		glDrawArrays(GL_POINTS, i, 1);
	}
	glDisable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(0);


	for (size_t i = 0; i < edges.size(); ++i) {
		vec3 labelPos = glm::mix(edges[i].first, edges[i].second, 0.25);
		label.drawLabel(labelPos, getNthWeight(i, weights));
	}
	vec4 offset = m_camera->GetViewMatrix()* vec4(0., 0., 0.01, 0.);
	for (size_t i = 0; i < nodes.size(); ++i) {
		vec3 labelPos = nodes[i];
		labelPos.z += offset.z;
		label.drawLabel(labelPos, getNthActivation(i, layers));
	}
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