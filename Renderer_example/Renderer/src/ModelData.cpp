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