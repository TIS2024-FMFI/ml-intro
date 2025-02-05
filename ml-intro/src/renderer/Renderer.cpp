#include "Renderer.h"

Renderer::Renderer() {
	glPointSize(25.0f);
	glLineWidth(1.0f);

	loadNN({}, {});
}

void Renderer::Init(FrameBuffer* fb, Camera* cam) {
	frameBuffer = fb;
	camera = cam;
	initialized = true;
}


void Renderer::loadNN(vector<Eigen::MatrixXd> activations,  vector<Eigen::MatrixXd> weights) {
	generatePositions(activations);
	createVertices();
	createEdges();
	updateNN(activations, weights);

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



void Renderer::generatePositions(vector<Eigen::MatrixXd> activations) {
	float layerSpacing = 2.0f, nodeSpacing = 1.0f;
	if (squareRender) { layerSpacing = 5; nodeSpacing = 0.5f; }
	int nOfLayers = activations.size();
	vertPos.clear();

	for (int layer = 0; layer < nOfLayers; layer++) {
		float x = (layer - (nOfLayers-1) * .5f) * layerSpacing;
		vertPos.push_back({});
		int n = activations[layer].size();
		int square = sqrt(n);
		for (int i = 0; i < n; i++) {
			float y = (squareRender ? (i % square) - (square-1) * .5f : i - (n-1)*.5f) * nodeSpacing;
			float z = (squareRender ? (i / square) - (square-1) * .5f : 0) * nodeSpacing;
			vertPos.back().push_back(vec3(x, y, z));
		}
	}
}

void Renderer::createVertices() {
	Node::InitializeBuffers();
	Node::ClearNodes();
	for (auto layer : vertPos) {
		for (vec3 vertex : layer) {
			Node::Node(vertex);
		}
	}
}

void Renderer::createEdges() {
	Edge::InitializeBuffers();
	Edge::ClearEdges();
	for (int layer = 1; layer < vertPos.size(); layer++) {
		for (auto current : vertPos[layer]) {
			for (auto prev : vertPos[layer - 1]) {
				Edge(prev, current);
			}
		}
	}
}

void Renderer::setupLabels(vector<Eigen::MatrixXd> activations, vector<Eigen::MatrixXd> weights) {
	Label::InitializeBuffers();
	Label::ClearLabels();
	Label::UploadData();
	renderText = false;

	vector<float> data = unzipVec(activations);
	int i = 0;
	labels.clear();
	for (int layer = 0; layer < vertPos.size(); layer++) {
		for (vec3 pos : vertPos[layer]) {
			labels.push_back(Label(pos, data[i++]));
		}
	}

	data = unzipVec(weights);
	i = 0;
	for (int layer = 1; layer < vertPos.size(); layer++) {
		for (auto current : vertPos[layer]) {
			for (auto prev : vertPos[layer - 1]) {
				labels.push_back(Label(mix(prev, current, 0.25f), data[i++]));
			}
		}
	}
}

void Renderer::renderLabels() {
	if (!renderText) return;
	if (Label::isVertexCountZero()) {
		for (Label l : labels) {
			Label::AddLabel(l); //this one is expensive
		}
		Label::UploadData();
	}
}

void Renderer::updateNN(vector<Eigen::MatrixXd> activations, vector<Eigen::MatrixXd> weights) {
	Node::UpdateValues(unzipVec(activations));
	Edge::UpdateValues(unzipVec(weights));
	setupLabels(activations, weights);
	renderLabels();
	uploadToBuffers();
}

vector<float> Renderer::unzipVec(vector<Eigen::MatrixXd> weights) {
	vector<float> out;
	for (auto&& l : weights) {
		out.insert(out.end(), l.reshaped().begin(), l.reshaped().end());
	}
	return out;
}

void Renderer::uploadToBuffers() {
	Node::UploadData();
	Edge::UploadData();
	if (renderText) Label::UploadData();
}