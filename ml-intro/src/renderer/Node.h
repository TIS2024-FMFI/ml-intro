#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Label.h"
#include "ShaderPrograms.h"

using namespace glm;

class Node {
public:
    vec3 pos;
    float activation;

    Node(vec3 position, float activation);

    static void InitializeBuffers();
    static void AddNode(const Node& node);
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
#pragma once
