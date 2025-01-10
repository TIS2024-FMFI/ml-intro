#include "Node.h"

GLuint Node::shaderProgram = 0;
GLuint Node::nodeVAO = 0;
GLuint Node::nodeVBO = 0;
std::vector<float> Node::nodeData;
bool Node::initialized = false;
int Node::vertCount = 0;

Node::Node(vec3 position, float activation) : pos(position), activation(activation) {
    Node::InitializeBuffers();
    Node::AddNode(*this);
    Label(position, activation);
}