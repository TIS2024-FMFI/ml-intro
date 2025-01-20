#include "Renderer.h"

Renderer::Renderer() {
	glPointSize(25.0f);
	glLineWidth(5.0f);

	loadNN({}, {});
}

void Renderer::Init(FrameBuffer* fb, Camera* cam) {
	frameBuffer = fb;
	camera = cam;
	initialized = true;
}


void Renderer::loadNN(vector<Eigen::MatrixXd> activations,  vector<Eigen::MatrixXd> weights) {
	Edge::ClearEdges();
	Node::ClearNodes();

	/*
	float layerSpacing = 2.0f, nodeSpacing = 1.0f;


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
	*/
}


void Renderer::renderScene() {
	if (!initialized) return;

	camera->UpdateProjMatrix();

	frameBuffer->Bind();

	Edge::RenderEdges(camera);
	Node::RenderNodes(camera);

	if (renderText) {
		Label::RenderLabels(camera);
	}

	frameBuffer->Unbind();
}