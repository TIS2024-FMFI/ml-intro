#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "ShaderPrograms.h"

using namespace glm;
using namespace std;

class Node {
public:
    vec3 pos;
    float activation;
    float isInputLayerNode;

    Node(vec3 position, float isInputLayerNode);

    static vec3* negCol;
    static vec3* posCol;
    static float PointSize;

    static void InitializeBuffers();
    static void AddNode(const Node& node);
    static void UpdateValues(vector<float>);
    static void UploadData();
    static void RenderNodes(Camera* camera);
    static void ClearNodes();

private:
    static GLuint shaderProgram;
    static GLuint nodeVAO, nodeVBO;
    static std::vector<float> nodeData;
    static bool initialized;
    static int vertCount;
};