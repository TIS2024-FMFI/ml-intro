#include "Edge.h"

GLuint Edge::shaderProgram = 0;
GLuint Edge::edgeVAO = 0;
GLuint Edge::edgeVBO = 0;
std::vector<float> Edge::edgeData;
bool Edge::initialized = false;
int Edge::vertCount = 0;

Edge::Edge(vec3 posA, vec3 posB, float w) : posA(posA), posB(posB), weight(w) {
    Edge::InitializeBuffers();
    Edge::AddEdge(*this);

    vec3 p = mix(posA, posB, 0.25);
    Label(p, weight);
}