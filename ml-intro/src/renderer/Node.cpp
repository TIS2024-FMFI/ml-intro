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

void Node::InitializeBuffers() {
    if (initialized) return;

    glGenVertexArrays(1, &nodeVAO);
    glGenBuffers(1, &nodeVBO);

    glBindVertexArray(nodeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, nodeVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float))); // activation
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Node::shaderProgram = ShaderPrograms::getInstance().getNodeShaderProgramId();

    Label::InitializeBuffers();

    initialized = true;
}

void Node::AddNode(const Node& node) {

    nodeData.push_back(node.pos.x);
    nodeData.push_back(node.pos.y);
    nodeData.push_back(node.pos.z);
    nodeData.push_back(node.activation);

    vertCount++;
}

void Node::ClearNodes() {
    nodeData.clear();
    vertCount = 0;
    Label::ClearLabels();
}

void Node::UploadData() {
    glBindBuffer(GL_ARRAY_BUFFER, nodeVBO);
    glBufferData(GL_ARRAY_BUFFER, nodeData.size() * sizeof(float), nodeData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    Label::UploadData();
}

void Node::RenderNodes(Camera* cam) {

    if (!initialized) return;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, value_ptr(cam->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, value_ptr(cam->GetProjMatrix()));
    glUniform1f(glGetUniformLocation(shaderProgram, "pointSize"), 400.0f);

    glBindVertexArray(nodeVAO);
    glDrawArrays(GL_POINTS, 0, vertCount);
    glBindVertexArray(0);

    glDisable(GL_PROGRAM_POINT_SIZE);
    glDisable(GL_POINT_SPRITE);
}