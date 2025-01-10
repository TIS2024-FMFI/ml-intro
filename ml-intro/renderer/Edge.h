#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

using namespace glm;

class Edge {
public:
    vec3 posA;
    vec3 posB;
    float weight;
    static GLuint shaderProgram;

    Edge(vec3 posA, vec3 posB, float w);

    static void InitializeBuffers();
    static void AddEdge(const Edge& edge);
    static void UploadData();
    static void RenderEdges(/*ToDo*/);
    static void ClearEdges();

private:
    static GLuint edgeVAO, edgeVBO;
    static std::vector<float> edgeData;
    static bool initialized;
    static int vertCount;
};
