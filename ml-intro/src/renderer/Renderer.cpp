#include "Renderer.h"

bool Renderer::renderText = false;
bool Renderer::squareRender = true;

Renderer::Renderer(FrameBuffer* fb, Camera* cam)
	: m_frameBuffer(fb), m_camera(cam)
{
	glGenVertexArrays(1, &edgeVAO);
	glGenBuffers(1, &edgeVBO);

	glGenVertexArrays(1, &vertVAO);
	glGenBuffers(1, &vertVBO);

	glPointSize(25.0f);
	glLineWidth(5.0f);

	loadNN();
}


void Renderer::loadNN(/*todo: send the network data here*/) {
	layers = {{-0.78f, 0.42f, 0.52f, -0.44f, 0.12f, -0.93f, -0.023f, 0.60f, 0.56f},
			   {-1.f, -.5f, .5f, -1.f},
			   {0.f} };
	weights = { {-0.71f, 0.42f, -0.22f, 0.02f, 0.24f, -0.18f, 0.24f, -0.20f, 0.52f}, 
				{-0.18f, -0.16f, 0.80f, -0.58f, 0.72f, -0.60f, -0.99f, -0.24f, 0.28f}, 
				{0.04f, -0.66f, 0.34f, -0.35f, -0.28f, -0.34f, -0.30f, 0.74f, -0.72f}, 
				{0.04f, -0.59f, 0.29f, -0.26f, -0.55f, 0.68f, 0.32f, -0.13f, -0.53f},
	
				{0.2, 0.4, 0.8, 1} };;
	/*
	layers = { {0.2f, -0.8f}, { 0.5f, -0.3f, 0.9f }, { 0.7f } };
	weights = { {1, -1}, {0.5, -0.75}, {0, -0.5}, {0.25, 0.5, -0.25} };;
	*/
	float layerSpacing = 2.0f, nodeSpacing = 1.0f;

	Edge::ClearEdges();
	Node::ClearNodes();

	int n = 0, p = 0;
	vector<vec3> lastLayer = {};
	for (int i = 0; i < layers.size(); ++i) {
		vector<vec3> currentLayer = {};
		for (size_t j = 0; j < layers[i].size(); ++j) {
			float z = 0.0f;
			float y = j * nodeSpacing - layers[i].size() * nodeSpacing / 2.0f;
			if (squareRender) {
				float sqrt_f = sqrtf(layers[i].size());
				float offset = (sqrt_f-1) * nodeSpacing / 2.0f;

				z = (int)(j / sqrt_f) * nodeSpacing - offset;
				y = (j % (int)sqrt_f) * nodeSpacing - offset;
			}

			vec3 nodePos = vec3((i - 1) * layerSpacing, y, z);
			Node(nodePos, getNthActivation(n++, layers));

			currentLayer.emplace_back(nodePos);
			if (i > 0) {
				for (size_t k = 0; k < layers[i - 1].size(); ++k) {
					Edge(lastLayer[k], currentLayer[j], getNthWeight(p++, weights));
				}
			}
		}
		lastLayer = currentLayer;
	}

	Node::UploadData();
	Edge::UploadData();
}


void Renderer::renderScene() {
	test += 0.001;

	m_camera->SetCameraView(vec3(sin(test) * 5., sin(test*3.14159265)*3., cos(test) * 5.), vec3(.0));
	m_camera->UpdateProjMatrix();


	m_frameBuffer->Bind();


	useShaderProgram();

	m_frameBuffer->Unbind();
}

void Renderer::useShaderProgram() {

	Edge::RenderEdges(m_camera);
	Node::RenderNodes(m_camera);

	if (renderText) {
		Label::RenderLabels(m_camera);
	}
}