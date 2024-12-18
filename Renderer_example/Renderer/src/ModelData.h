#pragma once
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <json.hpp>
#include <glm/glm.hpp>

using namespace std;
using namespace nlohmann;
using namespace glm;


struct NN_DATA {
	vector<int> layers;
	vector<vector<vector<double>>> weights;
};

class ModelData
{
public:
	ModelData(const char* fileName);
	int numberOfLayers() { return data.layers.size(); }
	int layerSize(uint8_t layerId) { return data.layers[layerId]; }
	vector<vector<double>> getWeightMatrixForLayer(uint8_t layerId) { return data.weights[layerId]; }
	vector<double> getWeightsForNode(uint8_t layerId, uint32_t nodeId) { return getWeightMatrixForLayer(layerId)[nodeId]; }
	int numberOfVerticies() {
		int sum = 0;
		for_each(data.layers.begin(), data.layers.end(), [&](uint8_t n) { sum += n; });
		return sum;
	}
	vector<vec3> getVerticiesPositions();
	vector<tuple<vec3, vec3, double, double>> getEdges(); // vecA, vecB, weight, activation 



	vector<vector<float>> getActivations() { return { {0.2f, -0.8f}, { 0.5f, -0.3f, 0.9f }, { 0.7f } }; }
	vector<vector<float>> getWeights() { return { {1, -1}, {0.5, -0.75}, {0, -0.5}, {0.25, 0.5, -0.25} }; }

private:
	void jsonToStruct(const string& fileName);
	int layerStart(int layer);
	NN_DATA data;
};

inline void from_json(const nlohmann::json& j, NN_DATA& data) {
	data.layers = j.at("layers").get<vector<int>>();
	data.weights = j.at("weights").get<vector<vector<vector<double>>>>();
}