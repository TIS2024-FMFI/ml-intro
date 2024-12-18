#include "ModelData.h"


ModelData::ModelData(const char* fileName) {
    jsonToStruct(fileName);
}

void ModelData::jsonToStruct(const string& filename) {
    ifstream inputFile(filename);

    json jsonData;
    inputFile >> jsonData;

    data = jsonData.get<NN_DATA>();
}

vector<vec3> ModelData::getVerticiesPositions() {
    vec3 center = vec3(.0, .0, .0);
    int nOfLayers = numberOfLayers();
    float length = 10.;
    vec3 anchor = vec3(length / 2., .0, .0);

    vector<vec3> out;
    for (int x = 0; x < nOfLayers; x++) {
        for (uint8_t y = 0; y < layerSize(x); y++) {
            out.push_back(anchor + vec3(0, y - layerSize(x) / 2, 0));
        }
        anchor.x -= length / (nOfLayers - 1);
    }

    return out;
}

int ModelData::layerStart(int layer) {
    int out = 0;
    for (int i = 0; i < layer; out += layerSize(i++));
    return out;
}

vector<tuple<vec3, vec3, double, double>> ModelData::getEdges() {
    vector<tuple<vec3, vec3, double, double>> edges;
    vector<vec3> vertices = getVerticiesPositions();

    for (size_t layer = 1; layer < data.weights.size() + 1; ++layer) {
        const auto& layerWeights = data.weights[layer - 1];
        int prevLayerStartIndex = layerStart(layer - 1);
        int currentLayerStartIndex = layerStart(layer);

        for (size_t dest = 0; dest < layerWeights.size(); ++dest) {
            for (size_t src = 0; src < layerWeights[dest].size(); ++src) {
                int startIndex = src + prevLayerStartIndex;
                int endIndex = dest + currentLayerStartIndex;

                if (startIndex >= vertices.size() || endIndex >= vertices.size()) {
                    std::cerr << "Index out of range: startIndex = " << startIndex
                        << ", endIndex = " << endIndex << std::endl;
                    continue;
                }

                glm::vec3 start = vertices[startIndex];
                glm::vec3 end = vertices[endIndex];
                double weight = layerWeights[dest][src];
                double activation = 0.5; //todo
                edges.emplace_back(start, end, weight, activation);
            }
        }
    }

    return edges;
}

